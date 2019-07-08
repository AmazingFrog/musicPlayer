#ifndef __PLAY_H__
#define __PLAY_H__

#include <mutex>
#include <atomic>
#include <thread>
#include <cstdlib>
#include <cstring>
#include <QObject>
#include <iostream>
#include <unistd.h>
#include <exception>
#include "AL/al.h"
#include "AL/alc.h"
#include "AL/alext.h"

#define PLAY_STATE_PLAYING        0x0101
#define PLAY_STATE_PAUSE          0x0102
#define PLAY_STATE_STOP           0x0103
#define PLAY_STATE_FINISH         0x0104

extern int getSampleByteLength(ALenum format);

struct PlayInfo{
    unsigned long musicByteLength = 0;
    unsigned long musicTime_S = 0;
    unsigned long freq = 0;
    ALenum format = 0;
    unsigned int sampleByteLength = 0;
    PlayInfo(){}
    PlayInfo(unsigned long musicLength,unsigned long musicTime,unsigned long fr,ALenum fo):
        musicByteLength(musicLength),
        musicTime_S(musicTime),
        freq(fr),
        format(fo),
        sampleByteLength(getSampleByteLength(this->format)){}
};

class Play:public QObject{
    Q_OBJECT

private:
    //音频相关参数
    int playState;
    unsigned int bufferNum = 8;
    unsigned long perSecondByte = 0;
    unsigned long alreadyPlayByte = 0;
    unsigned long soundDataByteLength = 0;
    unsigned long soundDataSubscript = 0;
    char* soundData = 0;
    unsigned long musicTime_S = 0;

    //openal相关参数
    ALCdevice* dev;
    ALCcontext* context;
    ALuint source;
    ALuint buffer;
    ALenum format;
    ALsizei freq;

    void Init();
public:
    Play();
    ~Play();
    int getState();

    void play();
    void pause();
    void resume();
    void jumpTo(const unsigned long jumpTo_S);
    void quit();
    void stop();

    void setBuffer(void* data,ALsizei len,ALenum fo,ALsizei fr,unsigned long ti);
    void setBuffer(void* data,const PlayInfo& playInfo);

    unsigned long getMusicTime_S();
    int getAlreadyPlay_S();
    //发送信号
signals:
    void songFinish();
};
#endif //__PLAY_H__
