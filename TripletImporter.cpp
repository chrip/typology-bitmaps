/*
 * TripletImporter.cpp
 *
 *  Created on: 13.02.2013
 *      Author: Christoph Schaefer
 */

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <fstream>
#include <dirent.h>
#include <string.h>

#include "TripletImporter.h"
#include "PartitionManager.h"
#include "util.h"

TripletImporter::TripletImporter(const PartitionManagerPtr& partitionManager) :
_partitionManager(partitionManager)
{
}

void TripletImporter::startImport(const std::string& path){
  std::string source;
  std::string target;
  float score;
  uint64_t linesSeen(0);
  StringVecPtr filenames = util::getAllItemsInDir(path);
  for(auto& filename : *filenames) {
    std::ifstream file (path + filename);
    std::cout << "reading file : " << path + filename << std::endl;
    for(std::string line; getline(file, line);) {
    	++linesSeen;
		std::istringstream ss(line);
		ss >> source >> target >> score;
		_partitionManager->addSourceTargetScore(source, target, score);
    }
    //std::cout << "rows: " << _partitionManager->getRowCount() << std::endl;
  }
  _partitionManager->flushSourceTargetScore();

//  std::cout << "Successfully " << _partitionManager->getRowCount() << " rows imported, skiped "
//		  << linesSeen - _partitionManager->getRowCount() << " rows." << std::endl;
}
