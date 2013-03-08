/*
 * TripletImporter.h
 *
 *  Created on: 13.02.2013
 *      Author: Christoph Schaefer
 */

#ifndef __BitmapIndex__TripletImporter__
#define __BitmapIndex__TripletImporter__

#include "forward.h"

class TripletImporter {
public:
	TripletImporter(const PartitionManagerPtr& partitionManager);
	virtual ~TripletImporter(){};
	void startImport(const std::string& path);
private:
	PartitionManagerPtr _partitionManager;
};

#endif /* __BitmapIndex__TripletImporter__ */
