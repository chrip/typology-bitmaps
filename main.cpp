//
//  main.cpp
//  BitmapIndex
//
//  Created on 08.02.13.
//  Copyright (c) 2013 Christoph Schaefer. All rights reserved.
//

#include <iostream>
#include "PartitionManager.h"
#include "TripletImporter.h"
#include "Server.h"
#include "util.h"
#include "forward.h"
#include <clocale>

int main(int argc, const char * argv[])
{
	// this program needs a lot of file handles. Therefore you have to increase your ulimit -n to a high number.
	// To do so, insert at the end of the file /etc/security/limits.conf the following two lines:
	//* soft  nofile  100000
	//* hard  nofile  100000

    // important to read german utf-8 files
    std::setlocale(LC_ALL, "de_DE.UTF-8");

    // where the bitmap files will be saved
    std::string partitionDir = "/home/typology/partitions/";
    // Import dir where the triples "source target score" like "Hund Katze 0.23e-3" can be found
    // The files have to be sorted on the column "source" and inside the same source descending on "score".
    // All sources starting with the same character have to be in the same file. 
    //std::string importDir = "/home/typology/1sorted/";
    std::string importDir = "";
    
	PartitionManagerPtr partitionManager(std::make_shared<PartitionManager>(partitionDir));

	if(importDir != "") {
		// import raw data and save bitmaps on disc
		TripletImporter importer(partitionManager);
		timestamp_t tStart = util::getTimestamp();
		importer.startImport(importDir);
		timestamp_t tEnd = util::getTimestamp();
		std::cout << "import finished after " << (tEnd-tStart)/1000000.0 << " sec" << std::endl;
	}
	else {
		// load the bitmaps from disc
		timestamp_t tStart = util::getTimestamp();
		partitionManager->loadPartitions();
		timestamp_t tEnd = util::getTimestamp();
		std::cout << "loading partitions finished after " << (tEnd-tStart)/1000000.0 << " sec" << std::endl;
		Server server(partitionManager, 8081);
		server.communicate();
	}

    std::cout << "exit main" << std::endl;
    return 0;
}

