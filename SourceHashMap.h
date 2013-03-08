//
//  SourceHashMap.h
//  BitmapIndex
//
//  Created on 09.02.13.
//  Copyright (c) 2013 Christoph Schaefer. All rights reserved.
//

#ifndef __BitmapIndex__SourceHashMap__
#define __BitmapIndex__SourceHashMap__

#include <iostream>
#include <unordered_map>
#include "forward.h"

class SourceHashMap {
public:
    SourceHashMap();
    virtual ~SourceHashMap(){}
    void add(const std::string& source, const uint32_t& rowNr);
    void flush(const uint32_t& rowNr);
    const RangePair getRangeOfSource(const std::string& source) const;
    void writeToDisc(const std::string& path);
    void readFromDisc(const std::string& path);
private:
    std::shared_ptr<std::unordered_map<std::string, RangePair>> _sourceHashMapPtr;
    std::string _currentSource;
    uint32_t _firstOccurrenceOfCurrentSource;
    
};
#endif /* defined(__BitmapIndex__SourceHashMap__) */
