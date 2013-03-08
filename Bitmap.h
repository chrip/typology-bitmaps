//
//  Bitmap.h
//  BitmapIndex
//
//  Created on 09.02.13.
//  Copyright (c) 2013 Christoph Schaefer. All rights reserved.
//

#ifndef __BitmapIndex__Bitmap__
#define __BitmapIndex__Bitmap__

#include <iostream>
#include <vector>
#include "forward.h"

class Bitmap {
public:
    Bitmap(wchar_t c);
    Bitmap(wchar_t c, const uint64_t& size);
    virtual ~Bitmap();
    void addRow(const wchar_t& c);
    UInt32VecPtr getTargetRows(const RangePair& range);
    void AND(const BitmapPtr& bitmap, const RangePair& range);
    void AND(const BitmapPtr& result, const BitmapPtr& bitmap, const RangePair& range);
    static const uint16_t MAX_RESULT_SIZE = 10;
    void writeToDisc(const std::string& path);
    void readFromDisc(const std::string& path);
private:
    // the char this bitmap stands for
    wchar_t _character;

    uint64_t _rowCount;
    UIntVec64Ptr _bitVec; // vector used for import
    uint64_t* _bitArray; // mmapped array of uint64_t (the bitmap)
    int _bitmapFileDescriptor;
    uint64_t _totalPopCount;
    void setBitAtPosition(uint64_t& number, const uint16_t& position);
};
#endif /* defined(__BitmapIndex__Bitmap__) */
