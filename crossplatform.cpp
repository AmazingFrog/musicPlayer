#include "crossplatform.h"
#ifdef Q_OS_LINUX
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
std::vector<std::string> readFileNameFromDir_Linux(std::vector<unsigned int>& nameMapDir,std::vector<std::string>& musicDir){
    std::vector<std::string> musicName;
    struct stat sb;
    std::ifstream fin("dirConfig");
    std::string dir;
    DIR* dirPoint;
    struct dirent* file;
    unsigned int dirSubscript = 0;
    while(fin.good()){
        fin >> dir;
        if(*(dir.rbegin()) != '/'){
            dir.push_back('/');
        }
        stat(dir.c_str(),&sb);
        if((sb.st_mode&S_IFMT) != S_IFDIR){
            std::cout << dir << "is not dir" << std::endl;
            continue;
        }
        musicDir.push_back(dir);
        dirPoint = opendir(dir.c_str());
        while((file=readdir(dirPoint)) != 0){
            if(file->d_name[0] == '.' || file->d_type == DT_DIR){
                continue;
            }
            musicName.push_back(file->d_name);
            nameMapDir.push_back(dirSubscript);
        }
        ++dirSubscript;
    }
    return musicName;
}
#else
#ifdef Q_OS_WIN
#include <io.h>
std::vector<std::string> readFileNameFromDir_Win(std::vector<unsigned int>& nameMapDir,std::vector<std::string>& musicDir){
    long handle = 0;
    unsigned int dirSubscript = 0;
    std::vector<std::string> musicName;
    std::string dir;
    std::ifstream fin("dirConfig");
    while(fin.good()){
        fin >> dir;
        if(*(dir.rbegin()) != '\\'){
            dir.push_back("\\*");
        }
        else{
            dir.push_back('*');
        }
        handle = _findfirst(dir.c_str(), &fileinfo);
        if (handle == -1) {
            std::cout << "error" << std::endl;
            continue;
        }
        do {
            if (fileinfo.name[0] == '.' || fileinfo.attrib&_A_SUBDIR){
                continue;
            }
            musicName.push_back(fileinfo.name);
            nameMapDir.push_back(dirSubscript);
        } while (_findnext(handle, &fileinfo) == 0);
        ++dirSubscript;
        _findclose(handle);
    }
    return musicName;
}
#endif // Q_OS_WIN
#endif //Q_OS_LINUX
