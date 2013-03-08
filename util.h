/*
 * util.h
 *
 *  Created on: 14.02.2013
 *      Author: Christoph Schaefer
 */

#ifndef __BitmapIndex__util__
#define __BitmapIndex__util__

#include <sys/time.h>
#include <sstream>
#include <string>
#include <cwchar>
#include "forward.h"
#include <dirent.h>
#include <string.h>
#include <algorithm>

typedef unsigned long long timestamp_t;

class util {
public:

	static timestamp_t getTimestamp()
	{
	  struct timeval now;
	  gettimeofday (&now, NULL);
	  return  now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
	}
	static std::wstring toWString(const std::string& utf8){
		std::mbstate_t state = std::mbstate_t();
		const char* utf8Chars = utf8.c_str();
		size_t len = std::mbsrtowcs(NULL, &utf8Chars, 0, &state);
		if(len != size_t(-1)) {
			std::vector<wchar_t> wstr = std::vector<wchar_t>(len+1);
			std::mbsrtowcs(&wstr[0], &utf8Chars, len+1, &state);
			return std::wstring(&wstr[0], len);
		}
		else {
			return std::wstring();
		}
	}
	static wchar_t firstCharToWChar(const std::string& utf8) {
		wchar_t result;
		mbrtowc(&result, &utf8[0], MB_CUR_MAX, NULL);
		return result;
	}
	static std::string wCharToMultiByte(const wchar_t& source) {
		char buffer[MB_CUR_MAX];
		int length = wctomb(buffer, source);
		return std::string(buffer, length);
	}

	static StringVecPtr getAllItemsInDir(const std::string& path) {
	  DIR *dp;
	  struct dirent *entry;
	  dp = opendir(path.c_str());
	  if (dp == NULL) {
	    std::cout << "Path \"" << path << "\" does not exist or could not be read." << std::endl;
	    throw;
	  }
	  StringVecPtr filenames = std::make_shared<std::vector<std::string>>();
	  while ((entry = readdir(dp))){
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
			continue;
		}
		filenames->push_back(entry->d_name);
	  }
	  closedir(dp);
	  std::sort(filenames->begin(), filenames->end());
	  return filenames;
	}

};

#endif /* __BitmapIndex__util__ */
