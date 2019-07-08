#ifndef CROSSPLATFORM_H
#define CROSSPLATFORM_H
#include <QtGlobal>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
/**
 * read all file name into namelist from "dirConfig" file and set nameMapDir and msicDir
 * @param [out] nameMapDir(std::vector<unsigned int>)
 * @param [out] musicDir(std::vector<std::string>)
 * @return nameList(std::vector<std::string>)
 */
#ifdef Q_OS_LINUX
#define readFileNameFromDir readFileNameFromDir_Linux
extern std::vector<std::string> readFileNameFromDir_Linux(std::vector<unsigned int>&,std::vector<std::string>&);
#else
#ifdef Q_OS_WIN
#define readFileNameFromDir readFileNameFromDir_Win

extern std::vector<std::string> readFileNameFromDir_Win(std::vector<unsigned int>&,std::vector<std::string>&);
#endif // Q_OS_WIN
#endif // Q_OS_LINUX
#endif // CROSSPLATFORM_H
