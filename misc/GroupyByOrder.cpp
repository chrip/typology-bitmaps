//============================================================================
// Name        : GroupyByOrder.cpp
// Author      : Christoph Schaefer
// Version     : 0.1
// Description : A preprocessing step that groups and orders the typology raw data.
//	source, target, score = SELECT source, target, score FROM typology GROUP BY source, target, score ORDER BY score DESC;
//
//	example input file n.txt:
//	source	target	score
//	======================================
//	nach	dem	#0.1168345592119594
//	nach	der	#0.0830497442807186
//	nicht	mehr	#0.0934728339431037
//	nur	noch	#0.09872978261098073
//	nicht	nur	#0.044924458692597445
//	nach	einem	#0.026614109756463417
//	noch	nicht	#0.057654759469661754
//	nicht	in	#0.029437663388999596
//	nahm	er	#0.2309793972511082
//	nach	einer	#8.233841086867023E-5

// example output file n.txt:
//	source	target	score
//	======================================
//	nach	dem	0.1168345592119594
//	nach	der	0.0830497442807186
//	nach	einem	0.026614109756463417
//	nach	einer	8.233841086867023E-5
//	nahm	er	0.2309793972511082
//	nicht	mehr	0.0934728339431037
//	nicht	nur	0.044924458692597445
//	nicht	in	0.029437663388999596
//	noch	nicht	0.057654759469661754
//	nur	noch	0.09872978261098073

//============================================================================

#include <iostream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <fstream>
#include <dirent.h>
#include <string.h>
#include <tuple>
#include <vector>
#include <algorithm>
#include <sys/stat.h>
#include <wchar.h>


bool compareTwoRows(std::tuple<std::string,std::string,float> rowA, std::tuple<std::string,std::string,float> rowB){
    return ( (std::get<0>(rowA)<std::get<0>(rowB)) || ((std::get<0>(rowA)==std::get<0>(rowB))&&(std::get<2>(rowA)>std::get<2>(rowB))) );
};

int main() {
	std::setlocale(LC_ALL, "de_DE.UTF-8"); // important for utf-8 multibyte chars
	const std::string path = "/home/typology/1/";
	const std::string path2 = "/home/typology/1sorted/";
	 struct dirent *entry;
	  DIR *dp;

	  dp = opendir(path.c_str());
	  if (dp == NULL) {
	    std::cout << "Path \"" << path << "\" does not exist or could not be read." << std::endl;
	    return 1;
	  }

	  std::string source;
	  std::string target;
	  float score;
	  std::vector<std::string> vSource;
	  std::vector<std::string> vTarget;
	  std::vector<float> vScore;
	  std::tuple<int,char> foo;
	  std::vector<std::tuple<std::string,std::string,float>>* vt = new std::vector<std::tuple<std::string,std::string,float>>();
	  std::vector<std::string> filenames;

	  while ((entry = readdir(dp))){
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
			continue;
		}
		filenames.push_back(entry->d_name);
	  }
	  closedir(dp);
	  std::sort(filenames.begin(), filenames.end());
	  wchar_t lastChar;
	  wchar_t currentChar;
	  std::string lastfName;

	  mbrtowc(&lastChar, &filenames[0][0], MB_CUR_MAX, NULL); // Convert multibyte sequence (utf-8) to wide character
	  lastfName = filenames[0];
	  for(auto iter = filenames.begin(); iter != filenames.end(); ++iter) {
		  mbrtowc(&currentChar, &((*iter)[0]), 4, NULL);
		  		if(currentChar != lastChar) {
		  			std::sort(vt->begin(),vt->end(),&compareTwoRows);
		  			std::ofstream myfile ((path2 + lastfName).c_str() );
		  			if (myfile.is_open())
		  			{
		  				for (auto i : *vt){
		  					myfile << std::get<0>(i) << '\t' << std::get<1>(i) << '\t' << std::get<2>(i) << '\n';
		  				}
		  			  myfile.close();
		  			}
		  			vt->clear();
		  			lastChar = currentChar;
		  			lastfName = *iter;
		  		}
	    std::ifstream file((path + *iter).c_str());
	    std::cout << "reading file : " << path + *iter << std::endl;
	    for(std::string line; getline(file, line);) {
			std::istringstream ss(line);
			ss >> source >> target;
			ss.ignore(2);
			ss >> score;
			vt->push_back(std::make_tuple (source, target, score));
	    }

  		if(iter == filenames.end() - 1) {
  			std::sort(vt->begin(),vt->end(),&compareTwoRows);
  			std::ofstream myfile ((path2 + lastfName).c_str() );
  			if (myfile.is_open())
  			{
  				for (auto i : *vt){
  					myfile << std::get<0>(i) << '\t' << std::get<1>(i) << '\t' << std::get<2>(i) << '\n';
  				}
  			  myfile.close();
  			}
  		}
	  }
	  delete vt;
	  return 0;
}
