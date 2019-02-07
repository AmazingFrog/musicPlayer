#include "play.h"
Play::Play(){
    this->Init();
}
Play::Play(const unsigned int bufferNumber){
    this->bufferNum = (bufferNumber>BUFFER_MAX)?(BUFFER_MAX):(bufferNumber);
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
    alGenBuffers(this->bufferNum,this->buffer);
    this->playState = PLAY_STATE_STOP;
    this->playMusicThread = new std::thread(std::mem_fun(&Play::f),this);
    this->playMusicThread->detach();
}
Play::~Play(){
    alDeleteSources(1,&(this->source));
    alDeleteBuffers(this->bufferNum,this->buffer);
    alcMakeContextCurrent(0);
    alcDestroyContext(this->context);
    alcCloseDevice(this->dev);
    delete this->playMusicThread;
}
void Play::play(){
    this->notifyCommandCome.lock();
    this->command = PLAYER_COMMAND_PLAY;
    this->commandCome = true;
    this->notifyCommandCome.unlock();
}
void Play::pause(){
    this->notifyCommandCome.lock();
    this->command = PLAYER_COMMAND_PAUSE;
    this->commandCome = true;
    this->notifyCommandCome.unlock();
}
void Play::resume(){
    if(this->playState == PLAY_STATE_STOP){
        return;
    }
    this->notifyCommandCome.lock();
    this->command = PLAYER_COMMAND_RESUME;
    this->commandCome = true;
    this->notifyCommandCome.unlock();
}
void Play::stop(){
    this->notifyCommandCome.lock();
    this->command = PLAYER_COMMAND_STOP;
    this->commandCome = true;
    this->notifyCommandCome.unlock();
}
void Play::jumpTo(const unsigned long jumpTo_S){
    if(jumpTo_S > this->musicTime_S){
        return;
    }
    this->notifyCommandCome.lock();
    this->commandValuePtr = new unsigned long;
    *(unsigned long*)(this->commandValuePtr) = jumpTo_S;
    this->command = PLAYER_COMMAND_JUMP;
    this->commandCome = true;
    this->notifyCommandCome.unlock();
}
void Play::quit(){
    this->notifyCommandCome.lock();
    this->command = PLAYER_COMMAND_QUIT;
    this->commandCome = true;
    this->notifyCommandCome.unlock();
    while(this->commandCome);
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
    return this->playState;
}
void Play::f(){
    ALint processed = 0;
    ALint lastProcessed = 1;
    int alreadyPlay_S = 0;
    while(1){
        //如果有命令
        if(this->commandCome){
            this->notifyCommandCome.lock();
            //解析命令
            switch(this->command){
            case PLAYER_COMMAND_PLAY:
                this->soundDataSubscript = 0;
                this->alreadyPlayByte = 0;
                //首次填充buffer
                for (unsigned int i = 0; i < this->bufferNum; ++i){
                    alBufferData(this->buffer[i],this->format,this->soundData+this->soundDataSubscript,this->perSecondByte,this->freq);
                    this->soundDataSubscript += this->perSecondByte;
                }
                alSourceQueueBuffers(this->source,this->bufferNum,this->buffer);
                alSourcePlay(this->source);
                this->playState = PLAY_STATE_PLAYING;
                break;
            case PLAYER_COMMAND_JUMP:
                this->jump();
                this->playState = PLAY_STATE_PLAYING;
                break;
            case PLAYER_COMMAND_PAUSE:
                alSourceStop(this->source);
                this->playState = PLAY_STATE_PAUSE;
                break;
            case PLAYER_COMMAND_RESUME:
                this->playState = PLAY_STATE_PLAYING;
                alSourcePlay(this->source);
                break;
            case PLAYER_COMMAND_STOP:
                alSourceStop(this->source);
                alSourcei(this->source,AL_BUFFER,0);
                this->playState = PLAY_STATE_STOP;
                break;
            case PLAYER_COMMAND_QUIT:
                alSourceStop(this->source);
                alSourcei(this->source,AL_BUFFER,0);
                this->playState = PLAY_STATE_STOP;
                this->notifyCommandCome.unlock();
                this->commandCome = false;
                return;
            default:
                break;
            }
            this->command = PLAYER_COMMAND_NO_COMMAND;
            this->commandCome = false;
            this->notifyCommandCome.unlock();
        }
        //只有在播放的时候才更新buffer
        if(this->playState == PLAY_STATE_PLAYING){
            //获取空闲队列个数
            alGetSourceiv(this->source,AL_BUFFERS_PROCESSED,&processed);
            if((processed == 1 && alreadyPlay_S+this->bufferNum <= this->musicTime_S) || processed > lastProcessed){
                emit this->sendAlreadyPlay_S(this->alreadyPlayByte/this->perSecondByte);
                this->alreadyPlayByte += this->perSecondByte;
                lastProcessed = processed;
                ++alreadyPlay_S;
            }
            //剩余音频数据大于一秒且有空闲缓冲区
            if(0 < processed && (this->soundDataSubscript+this->perSecondByte <= this->soundDataByteLength)){
                this->bufferMutex.lock();
                //更新每个buffer
                while(processed--){
                    if(this->soundDataSubscript+this->perSecondByte > this->soundDataByteLength){
                        break;
                    }
                    ALuint bid;
                    alSourceUnqueueBuffers(this->source,1,&bid);
                    alBufferData(bid,this->format,this->soundData+this->soundDataSubscript,this->perSecondByte,this->freq);
                    this->soundDataSubscript += this->perSecondByte;
                    alSourceQueueBuffers(this->source,1,&bid);
                }
                this->bufferMutex.unlock();
            }
            //buffer全部空闲且剩余音频数据不够一秒
            else if(processed == (int)this->bufferNum){
                this->playState = PLAY_STATE_STOP;
                this->alreadyPlayByte = 0;
                alSourceStop(this->source);
                alSourceUnqueueBuffers(this->source,this->bufferNum,this->buffer);
                alSourcei(this->source,AL_BUFFER,0);
                emit this->songFinish();
            }
        }
    }
}
void Play::jump(){
    unsigned long jumpTo_S = *(unsigned long*)(this->commandValuePtr);
    delete (unsigned long*)(this->commandValuePtr);
    this->commandValuePtr = 0;
    if(jumpTo_S > this->musicTime_S){
        return;
    }
    alSourceStop(this->source);
    alSourcei(this->source,AL_BUFFER,0);
    this->alreadyPlayByte = jumpTo_S * this->perSecondByte;
    this->soundDataSubscript = this->alreadyPlayByte;
    this->bufferMutex.lock();
    for(unsigned int i=0;i<this->bufferNum&&(this->soundDataSubscript+this->perSecondByte < this->soundDataByteLength);++i){
        alBufferData(this->buffer[i],this->format,this->soundData+this->soundDataSubscript,this->perSecondByte,this->freq);
        this->soundDataByteLength += this->perSecondByte;
    }
    alSourceQueueBuffers(this->source,this->bufferNum,this->buffer);
    this->bufferMutex.unlock();
    alSourcePlay(this->source);
}
unsigned long Play::getMusicTime_S(){
    return this->musicTime_S;
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
