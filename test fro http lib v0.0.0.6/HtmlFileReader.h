#pragma once
#include "FileReader.h"
#include <vector>
/*
* TODO:
* implemnt joning to file css and js connected to the html file.
* implement writing to file. multi threaded?
* implement getting file as route
*/

namespace http{
	class HtmlFileReader :public http::FileReader
	{

	public:
		HtmlFileReader() = default;
		inline HtmlFileReader(const std::string& fname) :FileReader(fname) {};
		bool connectFiles()noexcept;
		bool connectFile(const std::string& fileName)noexcept;
		bool writeToFile(const std::string& htmlMarkup,bool head)noexcept;
		


		/// <summary>
		/// this function removes new lines from file and connects all files 
		/// to the current string to svae on size of packets and number of packets
		/// WARNING: if this stpe is not done before running the server this will slow the server and will be uselles
		/// DONT BE STUPID!!!
		/// </summary>
		void optimizeFileForSending()noexcept;
	protected:
		struct ConnectedFileInfo
		{
			enum {Css=0,Script} typeOfConnectedFile;
			std::string filename;
			std::pair<size_t, size_t> startAndEndLocations;
			bool hasClosingPair;
		};
	private:
		
		std::vector<ConnectedFileInfo> getConnectedFilesInfo()const;
		bool DoesFileExist(const std::string& fname)const;
		std::string getFileHtml(const ConnectedFileInfo& finfo)const;
		
		
	};
}


