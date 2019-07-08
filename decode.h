#ifndef __DECODE_H__
#define __DECODE_H__

#include <mutex>
#include <thread>
#include <atomic>
#include <memory>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>

#include "linearBuffer.h"

#ifdef __cplusplus
extern "C"{
#endif
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#ifdef __cplusplus
}
#endif

struct MusicInfo{
    long format = 0;
    unsigned int freq = 0;
    unsigned long musicByteLength = 0;
    unsigned int musicTime_S = 0;

    MusicInfo(){}
    MusicInfo(long fo,unsigned int fr,unsigned long len,unsigned int musicTime):
        format(fo),
        freq(fr),
        musicByteLength(len),
        musicTime_S(musicTime){}
};

class Decode{
private:
    //解码相关
    const char* musicFile = 0;
    int audioStreamIdx = -1;
    AVFrame* avFrame = 0;
    AVCodecContext* avCodecContext = 0;
    AVFormatContext* avFormatContext = 0;
    AVPacket* avPacket = 0;
    AVCodec* avCodec = 0;
    LinearBuffer b;
    //由f设置
    void* data = 0;

    //多线程相关
    void* passValue = 0;

    static void f(Decode* const _this);
public:
    Decode();
    ~Decode();
    /**
     * 返回pcm数据
     * @param 填充musicInfo
    */
    void* readMusic(const char* file,MusicInfo* musicInfo);
};
#endif // __DECODE_H__
