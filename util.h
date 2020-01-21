#ifndef __UTIL_H__
#define __UTIL_H__
#include <string>
#include <cstdio>
#include "play.h"
#include "decode.h"

PlayInfo MusicInfoCoverToPlayInfo(const MusicInfo& info);
std::string timeToString(int time);

#endif //__UTIL_H__
