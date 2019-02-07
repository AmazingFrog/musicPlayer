#ifndef CROSSPLATFORM_H
#define CROSSPLATFORM_H


#ifdef __linux__
#define corss_stat(path,sb) stat(path,sb)
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#else
#define cross_stat _stat
#endif //__linux__


#endif // CROSSPLATFORM_H
