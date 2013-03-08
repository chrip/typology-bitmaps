//
//  BitmapManager.cpp
//  BitmapIndex
//
//  Created on 08.02.13.
//  Copyright (c) 2013 Christoph Schaefer. All rights reserved.
//

#include "BitmapManager.h"
#include "Bitmap.h"
#include "SourceHashMap.h"
#include "util.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

BitmapManager::BitmapManager() :
    _rowCount(0),
    _targetPtr(std::make_shared<std::vector<std::string>>()),
    _scorePtr(std::make_shared<std::vector<float>>()),
    _sourceHashMapPtr(std::make_shared<SourceHashMap>())
{
    for (uint16_t i=0; i < MAX_TARGET_LENGTH; ++i) {
        for(const wchar_t& c : ALPHABET){
            _targetCharBitmapMap[c] = std::make_shared<Bitmap>(c);
        }
        _targetCharPositionBitmaps.push_back(_targetCharBitmapMap);
    }
}

void BitmapManager::addSourceTargetScore(const std::string& source, const std::string& target, float score)
{
	std::wstring wTarget = util::toWString(target);
	if(isInAlphabet(wTarget)){
		try {
			_sourceHashMapPtr->add(source, _rowCount);
			_scorePtr->push_back(score);
			_targetPtr->push_back(target);
			addTarget(wTarget);
			_rowCount++;
		} catch (const char* s) {
			std::cout << s << std::endl;
			exit(1);
		}
		catch(...) {
			std::cout << "Something unexpected happend in BitmapManager::addSourceTargetScore :(" << std::endl;
			exit(1);
		}
	}
}

void BitmapManager::flushSourceTargetScore(){
	_sourceHashMapPtr->flush(_rowCount);
}

void BitmapManager::addTarget(const std::wstring& target){
	uint16_t targetSize = target.size();
	for (int i=0; i < MAX_TARGET_LENGTH; ++i) {
		wchar_t targetChar;
		if(i < targetSize) {
			targetChar = target[i];
		}
		else {
			targetChar = '\t'; // this char must not be in the alphabet
		}
		for(auto& bitmap : _targetCharPositionBitmaps[i]){
			bitmap.second->addRow(targetChar);
		}
	}
}

bool BitmapManager::isInAlphabet(const std::wstring& target){
	auto iter = target.begin();
	auto targetEnd = target.end();
	for (uint16_t i=0; i < MAX_TARGET_LENGTH && iter != targetEnd; ++i, ++iter) {
		if(!isInAlphabet(*iter)) {
			return false;
		}
	}
	return true;
}

bool BitmapManager::isInAlphabet(const wchar_t& c){
	bool charFound = false;
	for(const wchar_t& a : ALPHABET) {
		if(c == a) {
			charFound = true;
			break;
		}
	}
	return charFound;
}

StringVecPtr BitmapManager::getTargets(const std::string& source, const std::wstring& target) {
	//timestamp_t tStartGetRange = util::getTimestamp();
	RangePair range = _sourceHashMapPtr->getRangeOfSource(source);
	//timestamp_t tEndGetRange = util::getTimestamp();

	//timestamp_t tStartGetRows = util::getTimestamp();
	//timestamp_t tEndGetRows;
	//timestamp_t tStartGetStrings;
	//timestamp_t tEndGetStrings;

    StringVecPtr result = std::make_shared<std::vector<std::string>>();
    if(range.second == 0) {
        // source not found, return an empty vector
    	//tEndGetRows = util::getTimestamp();
    	//tStartGetStrings = util::getTimestamp();
    	//tEndGetStrings = util::getTimestamp();
        return result;
    }
    size_t targetLength = target.size();


    try {
//        if (true) {
//        	tEndGetRows = util::getTimestamp();
//            uint16_t resultSize = 0;
//            tStartGetStrings = util::getTimestamp();
//            for( uint32_t row = range.first; resultSize < Bitmap::MAX_RESULT_SIZE && row < range.second; ++row) {
//            	if((*_targetPtr)[row].compare(0, targetLength, target) == 0) {
//                result->push_back( _targetPtr->at(row));
//                resultSize++;
//            	}
//            }
		if (targetLength == 0) {
			//tEndGetRows = util::getTimestamp();
			uint16_t resultSize = 0;
			//tStartGetStrings = util::getTimestamp();
			for( uint32_t row = range.first; resultSize < Bitmap::MAX_RESULT_SIZE && row < range.second; ++row) {

				result->push_back( (*_targetPtr)[row]);
				resultSize++;

			}
			//tEndGetStrings = util::getTimestamp();
        }
        else if (targetLength == 1){
            auto vec = _targetCharPositionBitmaps[0].at(target[0])->getTargetRows(range);
            //tEndGetRows = util::getTimestamp();
            //tStartGetStrings = util::getTimestamp();
            for (auto& i : *vec){
                result->push_back((*_targetPtr)[i]);
            }
            //tEndGetStrings = util::getTimestamp();
        }
        else {
            _resultBitmap->AND(_targetCharPositionBitmaps[0].at(target[0]), _targetCharPositionBitmaps[1].at(target[1]), range);
            for(size_t charIter = 2; charIter < targetLength && charIter < MAX_TARGET_LENGTH; ++charIter){
                _resultBitmap->AND(_targetCharPositionBitmaps[charIter].at(target[charIter]), range);
            }
            //tEndGetRows = util::getTimestamp();
            //tStartGetStrings = util::getTimestamp();
            auto vec = _resultBitmap->getTargetRows(range);
            for (auto& i : *vec){
                result->push_back((*_targetPtr)[i]);
            }
            //tEndGetStrings = util::getTimestamp();
        }
    } catch (const std::out_of_range& oor) {
        // A bitmap for a given target char was not found.
        // This happens if the corresponding bitmap is plain zero.
    }
    
    //result->push_back("getRange: " + std::to_string((tEndGetRange-tStartGetRange)/1000.0));
    //result->push_back("range: " + std::to_string(range.second - range.first));
    //result->push_back("getRows: " + std::to_string((tEndGetRows-tStartGetRows)/1000.0));
    //result->push_back("getStrings: " + std::to_string((tEndGetStrings-tStartGetStrings)/1000.0));
    return result;
}

void BitmapManager::writeToDisc(const std::string& path) {
    _sourceHashMapPtr->writeToDisc(path);
    std::ofstream ofs;
    ofs.open(path + "target_score", std::ios::out);
    auto targetEnd = _targetPtr->end();
    auto targetIter = _targetPtr->begin();
    auto scoreIter = _scorePtr->begin();
    for (; targetIter != targetEnd; ++targetIter, ++scoreIter){
        ofs << *targetIter << '\t' << *scoreIter << std::endl;
    }
    ofs.close();
    
    
    for (uint16_t i=0; i < MAX_TARGET_LENGTH; ++i) {
        for(auto& targetCharBitmap : _targetCharPositionBitmaps[i]) {
            targetCharBitmap.second->writeToDisc(path + std::to_string(i));
        }
    }
}

void BitmapManager::readFromDisc(const std::string& path) {
    _sourceHashMapPtr->readFromDisc(path);
    std::ifstream ifs;
    ifs.open(path + "target_score", std::ios::in);
    std::string target;
    float score;
    _rowCount = 0;
    for(std::string line; getline(ifs, line);) {
    		std::istringstream ss(line);
    		ss >> target >> score;
    		_scorePtr->push_back(score);
    		_targetPtr->push_back(target);
    		_rowCount++;
        }
    ifs.close();

    // initialize the result bitmap
    _resultBitmap = std::make_shared<Bitmap>(L'R', _rowCount);

    for (uint16_t i=0; i < MAX_TARGET_LENGTH; ++i) {
        for(auto& targetCharBitmap : _targetCharPositionBitmaps[i]) {
        	std::string filePath = path + std::to_string(i);
            targetCharBitmap.second->readFromDisc(path + std::to_string(i));
        }
    }
}



