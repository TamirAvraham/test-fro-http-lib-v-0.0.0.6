#pragma once
#include<string>
#include <fstream>
#include <sstream> 

/*
* TODO:
* implement better chaching.
* think of more features
*/

namespace http {
	class FileReader
	{
	public:
		
		FileReader(const std::string& filename,bool refreshOnRead=false);
		~FileReader();
		std::string getFileAsString();
		size_t getSizeOfFile();
		void refreshFile();
	protected:
		std::string _fileName;
		std::string _currentFileReadOut;
		bool _refreshOnRead;

		std::string readfile(const std::string& fname)const;
	};
}


