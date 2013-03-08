//
//  PartitionManager.cpp
//
//  Created on 17.02.13.
//  Copyright (c) 2013 Christoph Schaefer. All rights reserved.
//

#include "PartitionManager.h"
#include "BitmapManager.h"
#include <sys/stat.h>
#include "util.h"
#include <stdexcept>

PartitionManager::PartitionManager(const std::string& partitionPath) :
_partitionPath(partitionPath),
_lastAddedPartitionKey(),
_lastAddedBitmapManager(std::make_shared<BitmapManager>()),
_isFirstAddSourceTargetScore(true)
{}

void PartitionManager::addSourceTargetScore(const std::string& source,
                          const std::string& target,
                          float score) {

	wchar_t partitionName = util::firstCharToWChar(source);
	if(_lastAddedBitmapManager->isInAlphabet(partitionName)){
		if(_isFirstAddSourceTargetScore){
			_lastAddedPartitionKey = partitionName;
			_isFirstAddSourceTargetScore = false;
		}
		else if(partitionName != _lastAddedPartitionKey){
			writeSubPartitionToDisc(partitionName);
			_lastAddedPartitionKey = partitionName;
			_lastAddedBitmapManager = std::make_shared<BitmapManager>();
		}

		_lastAddedBitmapManager->addSourceTargetScore(source, target, score);
	}
}

void PartitionManager::flushSourceTargetScore() {
	writeSubPartitionToDisc(_lastAddedPartitionKey);
}

StringVecPtr PartitionManager::getTargets(const std::string& source, const std::string& target) {
	wchar_t c = util::firstCharToWChar(source);
	try {
		return _partitions.at(c)->getTargets(source, util::toWString(target));
	} catch (const std::out_of_range& oor) {
		return std::make_shared<std::vector<std::string>>();
	}
}

void PartitionManager::writeSubPartitionToDisc(const wchar_t& partitionName) {
	_lastAddedBitmapManager->flushSourceTargetScore();
	std::string subPartitionPath = _partitionPath + std::to_string(static_cast<uint32_t>(_lastAddedPartitionKey));
	if(mkdir(subPartitionPath.c_str(), S_IRWXU | S_IRWXG | S_IRWXO) == 0) {
		_lastAddedBitmapManager->writeToDisc(subPartitionPath + "/");
		return;
	}
	// perhaps the main partition folder does not exists
	else if(mkdir(_partitionPath.c_str(), S_IRWXU | S_IRWXG | S_IRWXO) == 0) {
		// second try
		if(mkdir(subPartitionPath.c_str(), S_IRWXU | S_IRWXG | S_IRWXO) == 0) {
			_lastAddedBitmapManager->writeToDisc(subPartitionPath + "/");
			return;
		}
	}
	std::cout << "ERROR while creating partition dir " << subPartitionPath << std::endl;
	exit(1);

}

void PartitionManager::loadPartitions() {
	StringVecPtr partitionNames = util::getAllItemsInDir(_partitionPath);
	for(auto& name : *partitionNames) {
		BitmapManagerPtr partitionManager = std::make_shared<BitmapManager>();

		// the partitions are save under the interger corresponding to the wchar_t
		// because same chars like "/" are forbidden in dir names
		// therefore we have to cast from string to int to wchar_t
		wchar_t partitionName = atoi(name.c_str());

		std::cout << "reading partition " << name << " (" << util::wCharToMultiByte(partitionName) << ")" << std::endl;
		partitionManager->readFromDisc(_partitionPath + name + "/");

		_partitions[partitionName] = partitionManager;
	  }
}
