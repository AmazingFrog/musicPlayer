#include "crossplatform.h"
#include <cstdio>
#include <cstdlib>
#ifdef OS_LINUX
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
std::vector<std::string> readFileNameFromDir_Linux(const std::string& dir){
    std::vector<std::string> fileName;
    struct stat sb;
    DIR* dirPoint;
    struct dirent* file;
    stat(dir.c_str(),&sb);
    if((sb.st_mode&S_IFMT) != S_IFDIR){
        std::cout << dir << " is not dir" << std::endl;
        return fileName;
    }
    dirPoint = opendir(dir.c_str());
    while((file=readdir(dirPoint)) != 0){
        if(file->d_name[0] == '.' || file->d_type == DT_DIR){
            continue;
        }
        fileName.push_back(file->d_name);
    }
    return fileName;
}
void removeDir_Linux(const std::string& dir)
{
    char cmd[256] = {0};
    sprintf(cmd,"rm -rf %s",dir.c_str());
    system(cmd);
    return;
}
#else
#include <io.h>
std::vector<std::string> readFileNameFromDir_Win(const std::string& dir){
    long long handle = 0;
    std::vector<std::string> fileName;
    std::string d;
    struct _finddata_t fileinfo;
    d = dir;
    if(*(d.rbegin()) != '\\'){
        d.append("\\*");
    }
    else{
        d.push_back('*');
    }
    handle = _findfirst(d.c_str(), &fileinfo);
    if (handle == -1) {
        std::cout << "error" << std::endl;
		return fileName;
    }
    do {
        if (fileinfo.name[0] == '.' || fileinfo.attrib&_A_SUBDIR){
            continue;
        }
        fileName.push_back(fileinfo.name);
    } while (_findnext(handle, &fileinfo) == 0);
    _findclose(handle);
    return fileName;
}
void removeDir_Win(const std::string& dir)
{
    char cmd[256] = {0};
    sprintf(cmd,"rd /s /q %s",dir.c_str());
    system(cmd);
    return;
}
#endif // OS_LINUX
