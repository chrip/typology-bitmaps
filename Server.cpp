/*
 * Server.cpp
 *
 *  Created on: 13.02.2013
 *      Author: Christoph Schaefer
 *      example from http://www.linuxhowtos.org/C_C++/socket.htm
 */

#include "Server.h"
#include "PartitionManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <sstream>
#include "util.h"

Server::Server(const PartitionManagerPtr& partitionManager, int portNr) :
	_sockFd(0),
	_newSockFd(0),
	_partitionManager(partitionManager)
{

    struct sockaddr_in serv_addr;
    _sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (_sockFd < 0) {
       std::cout << "ERROR opening socket" << std::endl;
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portNr);
    if (bind(_sockFd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "ERROR on binding" << std::endl;
        return;
    }

    listen(_sockFd,5);
    std::cout << "Listening on port " << portNr << std::endl;

}

void Server::communicate() {
    socklen_t clilen;
    char buffer[64];
	struct sockaddr_in cli_addr;
	clilen = sizeof(cli_addr);
    ssize_t n; /* byte count or error */
	_newSockFd = accept(_sockFd,
				(struct sockaddr *) &cli_addr,
				&clilen);
	if (_newSockFd < 0) {
		std::cout << "ERROR on accept" << std::endl;
	}
    while(1) {
    	memset(buffer, 0, 64); // clear buffer
        std::string source;
        std::string target;
		n = read(_newSockFd,buffer,63);
		if (n < 0) {
			std::cout << "ERROR reading from socket" << std::endl;
			break;
		}
		std::string b(buffer);
		std::istringstream ss(b);
		ss >> source >> target;
		timestamp_t tStart = util::getTimestamp();
		StringVecPtr result = _partitionManager->getTargets(source, target);
		timestamp_t tEnd = util::getTimestamp();
		std::string s("{\"results\": [");
		size_t size = result->size();
		for(size_t i = 0; i < size; ++i) {
			s += (i)?",\"":"\"";
			s += result->at(i);
			s += "\"";
		}
		s += "], \"time_in_ms\": \"";
		s += std::to_string((tEnd-tStart)/1000.0);
		s += "\"}\n";
		n = write(_newSockFd, s.c_str(), s.size());
		if (n < 0) {
			std::cout << "ERROR writing to socket" << std::endl;
			break;
		}
    }
}

Server::~Server() {
    close(_newSockFd);
    close(_sockFd);
}
