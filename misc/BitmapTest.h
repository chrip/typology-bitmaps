//
//  BitmapTest.h
//  typo
//
//  Created by Schaef on 16.02.13.
//  Copyright (c) 2013 Christoph Schaefer. All rights reserved.
//

#ifndef __BitmapIndex__BitmapTest__
#define __BitmapIndex__BitmapTest__

#include "../Bitmap.h"
#include "../forward.h"

class BitmapTest {

public:
    static void test() {
        testAddAndGetRows();
    }
    
    static void testAddAndGetRows() {
        Bitmap bitmap('m');
        UInt32VecPtr expectedResult = std::make_shared<std::vector<uint32_t>>();
        
        bitmap.addRow('m'); // m adds a 1
        expectedResult->push_back(0);
        bitmap.addRow('m');
        expectedResult->push_back(1);
        bitmap.addRow('f'); // f adds a 0
        bitmap.addRow('f');
        bitmap.addRow('m');
        expectedResult->push_back(4);
        bitmap.addRow('f');
        bitmap.addRow('f');
        bitmap.addRow('m');
        expectedResult->push_back(7);
        
        UInt32VecPtr result = bitmap.getTargetRows(RangePair(0,8));
        if (*result != *expectedResult) {
            throw "wrong result in Bitmap::getTargetRows";
        }
        
        result = bitmap.getTargetRows(RangePair(1,8));
        if(result->size() != 3 || result->at(0) != 1 || result->at(1) != 4 || result->at(2) != 7) {
            throw "wrong result in Bitmap::getTargetRows";
        }
        
        result = bitmap.getTargetRows(RangePair(4,5));
        if(result->size() != 1 || result->at(0) != 4) {
            throw "wrong result in Bitmap::getTargetRows";
        }
        for(int i = 8; i < 123; ++i){
            bitmap.addRow('M'); // add 0s
        }
        
        bitmap.addRow('m'); // add 1
        expectedResult->push_back(123);
        
        for(int i = 124; i < 200; ++i){
            bitmap.addRow('M'); // add more 0s
        }
        bitmap.addRow('m'); // add 1
        expectedResult->push_back(200);
        
        result = bitmap.getTargetRows(RangePair(0,201));
        if (*result != *expectedResult) {
            throw "wrong result in Bitmap::getTargetRows";
        }

    }
};

#endif /* defined(__BitmapIndex__BitmapTest__) */
