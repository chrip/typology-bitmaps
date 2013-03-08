//
//  SourceHashMap.cpp
//  BitmapIndex
//
//  Created on 09.02.13.
//  Copyright (c) 2013 Christoph Schaefer. All rights reserved.
//

#include "SourceHashMap.h"
#include <stdexcept>
#include <fstream>
#include <sstream>


SourceHashMap::SourceHashMap() :
_sourceHashMapPtr(std::make_shared<std::unordered_map<std::string, RangePair>>()),
_currentSource(""),
_firstOccurrenceOfCurrentSource(0)
{
}


void SourceHashMap::add(const std::string& source, const uint32_t& rowNr)
{
    if(source != _currentSource) {
        if(rowNr != 0) {
            RangePair fromToPair(_firstOccurrenceOfCurrentSource, rowNr);
            std::pair<std::string, RangePair> sourceRange(_currentSource, fromToPair);
            _sourceHashMapPtr->insert(sourceRange);
        }
        _currentSource = source;
        _firstOccurrenceOfCurrentSource = rowNr;
    }
    
}

void SourceHashMap::flush(const uint32_t& rowNr)
{
            RangePair fromToPair(_firstOccurrenceOfCurrentSource, rowNr);
            std::pair<const std::string, RangePair> sourceRange(_currentSource, fromToPair);
            _sourceHashMapPtr->insert(sourceRange);
}

const RangePair SourceHashMap::getRangeOfSource(const std::string& source) const {
    try {
        return _sourceHashMapPtr->at(source);
    }
    catch (std::out_of_range& oor) {
        return RangePair(0,0);
    }
    return RangePair(0,0);
}

void SourceHashMap::writeToDisc(const std::string& path) {
    std::ofstream ofs;
    ofs.open(path + "source_range", std::ios::out);
    auto iterEnd = _sourceHashMapPtr->end();
    auto iter = _sourceHashMapPtr->begin();
    for (; iter != iterEnd; ++iter){
        ofs << iter->first << '\t' << iter->second.first << '\t' << iter->second.second << std::endl;
    }
    ofs.close();
}

void SourceHashMap::readFromDisc(const std::string& path) {
    std::ifstream ifs;
    ifs.open(path + "source_range", std::ios::in);
    std::string source;
    uint32_t from, to;
    for(std::string line; getline(ifs, line);) {
		std::istringstream ss(line);
		ss >> source >> from >> to;
		RangePair fromToPair(from, to);
		std::pair<std::string, RangePair> sourceRange(source, fromToPair);
		_sourceHashMapPtr->insert(sourceRange);
	}
    ifs.close();
}

