#include "HtmlFileReader.h"
#define NOT_FOUND_IN_FILE(loc_to_cheack) loc_to_cheack==std::string::npos
#define n_f(v) NOT_FOUND_IN_FILE(v)
#define str(content) std::string(content)
bool http::HtmlFileReader::connectFiles() noexcept
{
    /*for (auto fInfo : getConnectedFilesInfo()) {
        std::cout 
            << "file name: " << fInfo.filename 
            << " type: " << (!fInfo.typeOfConnectedFile ? "css" : "script") 
            << " start in file=" << fInfo.startAndEndLocations.first 
            << " end in file=" << fInfo.startAndEndLocations.second
            << '\n';
    }*/
    size_t diffrence = 0;
    for (ConnectedFileInfo info:getConnectedFilesInfo())
    {
        if (not DoesFileExist(info.filename))
        {
            return false;
        }
        std::string content = getFileHtml(info);
        size_t sizeOfBraket = (info.startAndEndLocations.second - info.startAndEndLocations.first);
        _currentFileReadOut.replace(info.startAndEndLocations.first+diffrence,sizeOfBraket , content);
        diffrence += content.size() - sizeOfBraket;
    }
    return true;
}



void http::HtmlFileReader::optimizeFileForSending() noexcept
{
    connectFiles();

}

std::vector<http::HtmlFileReader::ConnectedFileInfo> http::HtmlFileReader::getConnectedFilesInfo() const
{
    std::vector<ConnectedFileInfo> ret;
    size_t startOfBracket=0;
    size_t endOfBracket;
    bool hasClosingBracket=true;
    //getting css names
    do
    {
        hasClosingBracket = true;
        //finding the bracket
        startOfBracket = _currentFileReadOut.find("<link ", startOfBracket);
        if (n_f(startOfBracket)) break;

        endOfBracket = _currentFileReadOut.find("</link>", startOfBracket);
        if (n_f(endOfBracket)) {
            hasClosingBracket = false;
            endOfBracket = _currentFileReadOut.find('>', startOfBracket);
            if (n_f(endOfBracket)) { break; }
            endOfBracket += 1;
        }
        
        //getting the content fo the braket
        std::string markupContent = _currentFileReadOut.substr(startOfBracket, endOfBracket);
        //cheacking is css file
        if (n_f(markupContent.find("rel=\"stylesheet\""))) continue;
        //getting the name out of the file

        size_t startOfFileName = markupContent.find("href=\"")+6;
        if (n_f(startOfFileName)) continue;
        size_t endOfFileName = markupContent.find("\"", startOfFileName);
        if (n_f(endOfFileName)) continue;
        
        //inserting the name into ret
        ret.push_back({ ConnectedFileInfo::Css,markupContent.substr(startOfFileName,(endOfFileName - startOfFileName)),{startOfBracket,endOfBracket},hasClosingBracket });
        startOfBracket = endOfBracket;

    } while (1);
    startOfBracket = 0;
    
    //getting names of script files
    do
    {
        hasClosingBracket = true;
        //finding the bracket
        startOfBracket = _currentFileReadOut.find("<script ", startOfBracket);
        if (n_f(startOfBracket)) break;
        endOfBracket = _currentFileReadOut.find("</script>", startOfBracket);
        if (n_f(endOfBracket)) {
            hasClosingBracket = false;
            endOfBracket = _currentFileReadOut.find('>', startOfBracket);
            if (n_f(endOfBracket)) { break; }
            endOfBracket+=1;
        }
        //getting the content fo the braket
        std::string markupContent = _currentFileReadOut.substr(startOfBracket, endOfBracket);
        //getting the name out of the file

        size_t startOfFileName = markupContent.find("src=\"")+5;
        if (n_f(startOfFileName)) continue;
        size_t endOfFileName = markupContent.find("\"", startOfFileName);
        if (n_f(endOfFileName)) continue;
        //inserting the name into ret
        ret.push_back({ ConnectedFileInfo::Script,markupContent.substr(startOfFileName,(endOfFileName - startOfFileName)),{startOfBracket,endOfBracket},hasClosingBracket });
        startOfBracket = endOfBracket;

    } while (1);

    
    return ret;
}

bool http::HtmlFileReader::DoesFileExist(const std::string& fname) const
{
    std::ifstream file(fname);
    return file.is_open();
}

std::string http::HtmlFileReader::getFileHtml(const ConnectedFileInfo& finfo) const
{
    std::string ret;
    switch (finfo.typeOfConnectedFile)
    {
    case finfo.Css:
        ret = "<style>\n";
        ret += readfile(finfo.filename);
        ret += "</style>";
        
        break;
    case finfo.Script:
        ret = "<script>\n";
        ret += readfile(finfo.filename);
        if (!finfo.hasClosingPair)
        {
            ret += "</script>";
        }
        
        break;
    }
    return ret;
}

