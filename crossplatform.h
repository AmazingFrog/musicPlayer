#ifndef CROSSPLATFORM_H
#define CROSSPLATFORM_H
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#ifdef OS_LINUX
#define readFileNameFromDir readFileNameFromDir_Linux
#define removeDir removeDir_Linux
extern std::vector<std::string> readFileNameFromDir_Linux(const std::string&);
extern void removeDir_Linux(const std::string& dir);
#else
#define readFileNameFromDir readFileNameFromDir_Win
#define removeDir removeDir_Win
extern std::vector<std::string> readFileNameFromDir_Win(const std::string&);
extern void removeDir_Win(const std::string& dir);
#endif // OS_LINUX
#endif // CROSSPLATFORM_H
