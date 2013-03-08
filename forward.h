#ifndef __BitmapIndex__forward__
#define __BitmapIndex__forward__

#include <memory>
#include <utility>
#include <string>
#include <vector>
#include <unordered_map>
//#include <map>

class Bitmap;
typedef std::shared_ptr<Bitmap> BitmapPtr;

class BitmapManager;
typedef std::shared_ptr<BitmapManager> BitmapManagerPtr;

class PartitionManager;
typedef std::shared_ptr<PartitionManager> PartitionManagerPtr;

typedef std::unordered_map<wchar_t, BitmapPtr> CharBitmapMap;
//typedef std::map<char, BitmapPtr> CharBitmapMap;

typedef std::unordered_map<wchar_t, BitmapManagerPtr> PartitionMap;

typedef std::pair<const uint32_t, const uint32_t> RangePair;

class SourceHashMap;
typedef std::shared_ptr<SourceHashMap> SourceHashMapPtr;

typedef std::shared_ptr<std::vector<std::string>> StringVecPtr;
typedef std::shared_ptr<std::vector<uint32_t>> UInt32VecPtr;
typedef std::shared_ptr<std::vector<uint64_t>> UIntVec64Ptr;

typedef std::shared_ptr<std::vector<wchar_t>> WCharVecPtr;

#endif /* defined(__BitmapIndex__forward__) */
