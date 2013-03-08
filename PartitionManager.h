
//  Created on 17.02.13.
//  Copyright (c) 2013 Christoph Schaefer. All rights reserved.
//

#ifndef __BitmapIndex__PartitionManager__
#define __BitmapIndex__PartitionManager__

#include <iostream>
#include "forward.h"

class PartitionManager {

public:
    PartitionManager(const std::string& partitionPath);
    void addSourceTargetScore(const std::string& source,
                              const std::string& target,
                              float score);
    void flushSourceTargetScore();
    StringVecPtr getTargets(const std::string& source, const std::string& target);
    void loadPartitions();
    
private:
    void writeSubPartitionToDisc(const wchar_t& partitionName);
    PartitionMap _partitions;
    std::string _partitionPath;
    wchar_t _lastAddedPartitionKey;
    BitmapManagerPtr _lastAddedBitmapManager;
    bool _isFirstAddSourceTargetScore;
};

#endif /* defined(__BitmapIndex__PartitionManager__) */
