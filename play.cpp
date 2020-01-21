#include "play.h"
Play::Play(){
    this->Init();
}
void Play::Init(){
    this->dev = alcOpenDevice(0);
    if (!(this->dev)){
        throw std::exception();
    }
    this->context = alcCreateContext(this->dev,0);
    if (!alcMakeContextCurrent(this->context)){
        throw std::exception();
    }
    ALfloat listenerOri[] ={0.0f,0.0f,1.0f,0.0f,1.0f,0.0f};
    alListener3f(AL_POSITION,0,0,1.0f);
    alListener3f(AL_VELOCITY,0,0,0);
    alListenerfv(AL_ORIENTATION,listenerOri);
    alGenSources(1,&(this->source));
    alSource3f(this->source,AL_POSITION,0,0,0);
    alSource3f(this->source,AL_VELOCITY,0,0,0);
    alSpeedOfSound(1.0f);
    alSourcei(this->source,AL_LOOPING,AL_FALSE);
    alGenBuffers(1,&(this->buffer));
    this->playState = PLAY_STATE_STOP;
}
Play::~Play(){
    alDeleteSources(1,&(this->source));
    alDeleteBuffers(1,&(this->buffer));
    alcMakeContextCurrent(0);
    alcDestroyContext(this->context);
    alcCloseDevice(this->dev);
}
void Play::play(){
    alBufferData(this->buffer,this->format,this->soundData,this->soundDataByteLength,this->freq);
    alSourcei(this->source,AL_BUFFER,this->buffer);
    alSourcePlay(this->source);
    this->playState = PLAY_STATE_PLAYING;
}
void Play::pause(){
    alSourcePause(this->buffer);
    this->playState = PLAY_STATE_PAUSE;
}
void Play::resume(){
    alSourcePlay(this->source);
    this->playState = PLAY_STATE_PLAYING;
}
void Play::jumpTo(const unsigned long jumpTo_S){
    if(jumpTo_S > this->musicTime_S){
        return;
    }
    alSourceStop(this->source);
    this->alreadyPlayByte = jumpTo_S * this->perSecondByte;
    alSourcei(this->source,AL_BUFFER,0);
    alBufferData(this->buffer,this->format,this->soundData+this->alreadyPlayByte,this->soundDataByteLength-this->alreadyPlayByte,this->freq);
    alSourcei(this->source,AL_BUFFER,this->buffer);
    alSourcePlay(this->source);
}
void Play::stop(){
    alSourceStop(this->source);
    alSourcei(this->source,AL_BUFFER,0);
    this->playState = PLAY_STATE_STOP;
}
void Play::quit(){
    this->stop();
}
void Play::setBuffer(void *data,ALsizei len,ALenum fo,ALsizei fr,unsigned long ti){
    int sampleByteLength = getSampleByteLength(fo);
    this->format = fo;
    this->freq = fr;
    this->soundDataByteLength = len;
    this->musicTime_S = ti;
    this->perSecondByte = this->soundDataByteLength / this->musicTime_S;
    this->perSecondByte += (sampleByteLength-(this->soundDataByteLength/this->musicTime_S)%sampleByteLength);
    this->soundData = (char *)data;
}
void Play::setBuffer(void* data,const PlayInfo& playInfo){
    this->format = playInfo.format;
    this->freq = playInfo.freq;
    this->soundDataByteLength = playInfo.musicByteLength;
    this->musicTime_S = playInfo.musicTime_S;
    this->perSecondByte = this->soundDataByteLength / this->musicTime_S;
    this->perSecondByte += (playInfo.sampleByteLength-(this->soundDataByteLength/this->musicTime_S)%playInfo.sampleByteLength);
    this->soundData = (char *)data;
}
int Play::getState(){
    int a = -1;
    alGetSourcei(this->source,AL_SOURCE_STATE,&a);
    if(a == AL_STOPPED){
        alSourcei(this->source,AL_BUFFER,0);
        return PLAY_STATE_FINISH;
    }
    return this->playState;
}

unsigned long Play::getMusicTime_S(){
    return this->musicTime_S;
}
unsigned long Play::getAlreadyPlay_S(){
    int ret = 0;
    alGetSourcei(this->source,AL_SEC_OFFSET,&ret);
    return ret;
}

int getSampleByteLength(ALenum format){
    int sampleByteLength = 0;
    switch (format){
        case AL_FORMAT_STEREO_DOUBLE_EXT:
            sampleByteLength = 16;
            break;
        case AL_FORMAT_MONO_DOUBLE_EXT:
        case AL_FORMAT_STEREO_FLOAT32:
            sampleByteLength = 8;
            break;
        case AL_FORMAT_MONO_FLOAT32:
        case AL_FORMAT_STEREO16:
            sampleByteLength = 4;
            break;
        case AL_FORMAT_MONO16:
        case AL_FORMAT_STEREO8:
            sampleByteLength = 2;
            break;
        case AL_FORMAT_MONO8:
            sampleByteLength = 1;
            break;
        default:
            sampleByteLength = 4;
            break;
    }
    return sampleByteLength;
}
