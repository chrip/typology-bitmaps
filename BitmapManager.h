//
//  BitmapManager.h
//  BitmapIndex
//
//  Created on 08.02.13.
//  Copyright (c) 2013 Christoph Schaefer. All rights reserved.
//

#ifndef __BitmapIndex__BitmapManager__
#define __BitmapIndex__BitmapManager__

#include <iostream>
#include "forward.h"
class BitmapManager {

public:
    BitmapManager();
    virtual ~BitmapManager(){};
    void addSourceTargetScore(const std::string& source,
                              const std::string& target,
                              float score);
    void flushSourceTargetScore();
    StringVecPtr getTargets(const std::string& source, const std::wstring& target);
    uint64_t getRowCount() { return _rowCount; }
    void writeToDisc(const std::string& path);
    void readFromDisc(const std::string& path);
    bool isInAlphabet(const std::wstring& target);
    bool isInAlphabet(const wchar_t& c);

private:

    PartitionMap _sourceCharPartitionMap;
    CharBitmapMap _targetCharBitmapMap;
    std::vector<CharBitmapMap> _targetCharPositionBitmaps;
    uint64_t _rowCount;
    StringVecPtr _targetPtr;
    std::shared_ptr<std::vector<float>> _scorePtr;
    SourceHashMapPtr _sourceHashMapPtr;
    BitmapPtr _resultBitmap;
    const uint16_t MAX_TARGET_LENGTH = 5;
    const std::wstring ALPHABET = L"0123456789AÄBCDEFGHIJKLMNOÖPQRSTUÜVWXYZaäbcdefghijklmnoöpqrsßtuüvwxyz,.-:?!;'";
    
    void addTarget(const std::wstring& target);
};
#endif /* defined(__BitmapIndex__BitmapManager__) */
