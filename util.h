#ifndef __UTIL_H__
#define __UTIL_H__
#include <string>
#include <cstdio>
#include "decode.h"
#include "play.h"

PlayInfo MusicInfoCoverToPlayInfo(const MusicInfo& info){
    ALenum format;
    switch (info.format)
    {
        //double 双声道 立体声
        case AV_SAMPLE_FMT_DBLP:
            format = AL_FORMAT_STEREO_DOUBLE_EXT;
            break;
        //double 单声道
        case AV_SAMPLE_FMT_DBL:
            format = AL_FORMAT_MONO_DOUBLE_EXT;
            break;
        //float 双声道 立体声
        case AV_SAMPLE_FMT_FLTP:
            format = AL_FORMAT_STEREO_FLOAT32;
            break;
        //float 单声道
        case AV_SAMPLE_FMT_FLT:
            format = AL_FORMAT_MONO_FLOAT32;
            break;
        //16bit 双声道 立体声
        case AV_SAMPLE_FMT_S16P:
            format = AL_FORMAT_STEREO16;
            break;
        //16bit 单声道
        case AV_SAMPLE_FMT_S16:
            format = AL_FORMAT_MONO16;
            break;
        //8bit 双声道 立体声
        case AV_SAMPLE_FMT_U8P:
            format = AL_FORMAT_STEREO8;
            break;
        //8bit 单声道
        case AV_SAMPLE_FMT_U8:
            format = AL_FORMAT_MONO8;
            break;
        default:
            format = AL_FORMAT_STEREO16;
            break;
    }
    return PlayInfo(info.musicByteLength,info.musicTime_S,info.freq,format);
}
std::string timeToString(unsigned long time){
    char t[10] = {0};
    sprintf(t,"%lu:%lu",time/60,time%60);
    return std::string(t);
}
#endif //__UTIL_H__
