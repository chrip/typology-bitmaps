//
//  Bitmap.cpp
//  BitmapIndex
//
//  Created on 09.02.13.
//  Copyright (c) 2013 Christoph Schaefer. All rights reserved.
//

#include "Bitmap.h"
#include "math.h"
#include <fstream>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>


Bitmap::Bitmap(wchar_t c) :
    _character(c),
    _rowCount(0),
    _bitVec(std::make_shared<std::vector<uint64_t>>()),
    _bitArray(NULL),
    _bitmapFileDescriptor(-1),
    _totalPopCount(0)
{    
};

Bitmap::Bitmap(wchar_t c, const uint64_t& size) :
    _character(c),
    _rowCount(size),
    _bitVec(),
    _bitArray(new uint64_t[size/64 + 1]),
    _bitmapFileDescriptor(-1),
    _totalPopCount(0)
{
};

Bitmap::~Bitmap() {
	if(_bitArray) {
		if(_bitmapFileDescriptor != -1) {
			close(_bitmapFileDescriptor);
		}
		else {
			delete[] _bitArray;
		}
	}
};

void Bitmap::addRow(const wchar_t& c){
	uint16_t currentOffset = _rowCount % 64;
    if (currentOffset == 0) {
        _bitVec->push_back(uint64_t(0));
    }
    if (c != _character) {
        // this bitmap stands for an other character
        // the correponding bit remains at 0
    }
    else {
        _totalPopCount++;
        setBitAtPosition(_bitVec->at(_rowCount/64), currentOffset);
    }
    _rowCount++;
};

void Bitmap::setBitAtPosition(uint64_t& number, const uint16_t& position) {
    number |= uint64_t(1) << position;
};

UInt32VecPtr Bitmap::getTargetRows(const RangePair& range){
    uint64_t blockNumber = range.first / 64; // 64 rows in one block, one block is a uint64
    uint64_t* rowIter = _bitArray + blockNumber;
    uint64_t* rowIterEnd = _bitArray + (range.second / 64);
    
    UInt32VecPtr result = std::make_shared<std::vector<uint32_t>>();
    uint16_t resultSize = 0;

    //TODO flip bitorder
    for ( ; rowIter <= rowIterEnd; ++rowIter, ++blockNumber) {
        uint64_t bits = *rowIter;
        // http://stackoverflow.com/questions/14086854/get-an-array-of-the-bit-positions-within-a-64-bit-integer
        for (; bits != 0; resultSize++, bits &= (bits - 1L))
        {
            uint64_t row = blockNumber * 64 + log2(bits & ~(bits-1L));
            if(row >= range.first && row < range.second) {
                result->push_back(row);
                if(resultSize == MAX_RESULT_SIZE) {
                    return result;
                }
            }
        }
    }
    return result;
};

void Bitmap::AND(const BitmapPtr& bitmap, const RangePair& range){
	uint64_t* rhsIter = bitmap->_bitArray + range.first/64;
	uint64_t* resultIter = _bitArray + range.first/64;
	uint64_t* resultIterEnd = _bitArray + (range.second/64) + 1;
    
    for(; resultIter != resultIterEnd; ++resultIter, ++rhsIter){
        *resultIter &= *rhsIter;
        //std::cout << "resultIter: " << *resultIter << ", rhsIter: " << bitmap->_character << *rhsIter << std::endl;
    }
}

void Bitmap::AND(const BitmapPtr& lhsBitmap, const BitmapPtr& rhsBitmap, const RangePair& range) {
	uint64_t* lhsIter = lhsBitmap->_bitArray + range.first/64;
	uint64_t* rhsIter = rhsBitmap->_bitArray + range.first/64;

	uint64_t* resultIter = _bitArray + range.first/64;
	uint64_t* resultIterEnd =_bitArray + (range.second/64) + 1;

    for(; resultIter != resultIterEnd; ++lhsIter, ++rhsIter, ++resultIter){
        *resultIter = *lhsIter & *rhsIter;
        //std::cout << "resultIter: " << *resultIter << ", lhs: " << lhsBitmap->_character << *lhsIter << ", rhs: " << rhsBitmap->_character << *rhsIter << std::endl;
    }
}

void Bitmap::writeToDisc(const std::string& path) {
    if(_totalPopCount) {
        std::ofstream ofs;
        // hash the _character because some chars like '/' are not allowed in filenames
        ofs.open(path + "_" + std::to_string(static_cast<uint32_t>(_character)), std::ios::out | std::ios::binary);        
        uint64_t vectorSize = _bitVec->size();
        ofs.write(reinterpret_cast<char*>(&vectorSize), sizeof(uint64_t));
        ofs.write(reinterpret_cast<char*>(&_bitVec->at(0)), vectorSize * sizeof(uint64_t));
        ofs.close();

    }
}

void Bitmap::readFromDisc(const std::string& path){
	_bitmapFileDescriptor = open((path  + "_" + std::to_string(static_cast<uint32_t>(_character))).c_str(), O_RDONLY);
    if(_bitmapFileDescriptor > 0) {
    	uint64_t vectorSize;
		read(_bitmapFileDescriptor, &vectorSize, sizeof(uint64_t));
		_rowCount = vectorSize * 64;
		size_t fileSizeInBytes = vectorSize * sizeof(uint64_t) + sizeof(uint64_t);
		_bitArray = reinterpret_cast<uint64_t*>(mmap(0, fileSizeInBytes, PROT_READ, MAP_SHARED, _bitmapFileDescriptor, 0));
		if(_bitArray == MAP_FAILED) {
			std::cout << "ERROR mmapping file " << path  << "_" << static_cast<uint32_t>(_character) << std::endl;
			exit(1);
		}
		_bitArray++; // jump over the "header" which contains the file size
    }
    else {
    	_rowCount = 0;
    }

}




