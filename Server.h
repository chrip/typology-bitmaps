/*
 * Server.h
 *
 *  Created on: 13.02.2013
 *      Author: Christoph Schaefer
 */

#ifndef __BitmapIndex__Server__
#define __BitmapIndex__Server__

#include "forward.h"

class Server {
public:
	Server(const PartitionManagerPtr& partitionManager, int portNr);
	virtual ~Server();
	void communicate();
private:
    int _sockFd;
    int _newSockFd;
    PartitionManagerPtr _partitionManager;
};

#endif /* __BitmapIndex__Server__ */
