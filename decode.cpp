#include "decode.h"
Decode::Decode(){}
Decode::~Decode(){
    this->avCodec = 0;
    this->b.clear();
    if (this->audioStreamIdx != -1){
        this->audioStreamIdx = -1;
    }
    if (this->avCodecContext != 0){
        avcodec_free_context(&(this->avCodecContext));
        this->avCodecContext = 0;
    }
    if (this->avFrame != 0){
        av_frame_free(&(this->avFrame));
        this->avFrame = 0;
    }
    if (this->avPacket != 0){
        av_packet_unref(this->avPacket);
        this->avPacket = 0;
    }
    if (this->avFormatContext != 0){
        avformat_free_context(this->avFormatContext);
        this->avFormatContext = 0;
    }
}
void *Decode::readMusic(const char *file,MusicInfo *info){
    this->passValue = (void *)info;
    this->musicFile = file;
    std::thread t(this->f,this);
    t.join();
    return this->data;
}
void Decode::f(Decode *const _this){
    _this->avPacket = av_packet_alloc();
    _this->avFrame = av_frame_alloc();
    avformat_open_input(&_this->avFormatContext,_this->musicFile,0,0);
    assert(_this->avFormatContext != 0 && "open input error");
    avformat_find_stream_info(_this->avFormatContext,0);
    for (unsigned int i = 0; i < _this->avFormatContext->nb_streams; ++i){
        if (_this->avFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO){
            _this->audioStreamIdx = i;
            break;
        }
    }
    _this->avCodec = avcodec_find_decoder(_this->avFormatContext->streams[_this->audioStreamIdx]->codecpar->codec_id);
    _this->avCodecContext = avcodec_alloc_context3(_this->avCodec);
    avcodec_open2(_this->avCodecContext,_this->avCodec,0);
    //解码
    unsigned long musicLen = 0;
    while (av_read_frame(_this->avFormatContext,_this->avPacket) == 0){
        if (_this->avPacket->stream_index == _this->audioStreamIdx){
            int ret;
            ret = avcodec_send_packet(_this->avCodecContext,_this->avPacket);
            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF){
                break;
            }
            else if (ret < 0){
                printf("send packet error\n");
                break;
            }
            while (1){
                ret = avcodec_receive_frame(_this->avCodecContext,_this->avFrame);
                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF){
                    break;
                }
                else if (ret < 0){
                    printf("Error during decoding\n");
                    return;
                }
                unsigned int data_size = av_get_bytes_per_sample(_this->avCodecContext->sample_fmt);
                for (int i = 0; i < _this->avFrame->nb_samples; ++i){
                    for (int ch = 0; ch < _this->avCodecContext->channels; ++ch){
                        _this->b.push(_this->avFrame->data[ch] + data_size * i,data_size);
                        musicLen += data_size;
                    }
                }
            }
        }
    }
    _this->avPacket->data = 0;
    _this->avPacket->size = 0;
    avcodec_send_packet(_this->avCodecContext,_this->avPacket);
//    printf("alread decode %luB\n",_this->b.getLength());
    //设置返回值
    MusicInfo *p = (MusicInfo *)_this->passValue;
    p->format = _this->avCodecContext->sample_fmt;
    p->freq = _this->avCodecContext->sample_rate;
    p->musicByteLength = musicLen;
    p->musicTime_S = _this->avFormatContext->duration / AV_TIME_BASE;
    _this->data = _this->b.getData();
    //清理
    avcodec_free_context(&(_this->avCodecContext));
    avformat_free_context(_this->avFormatContext);
    _this->b.clear();
    _this->avCodecContext = 0;
    _this->avCodec = 0;
    _this->audioStreamIdx = -1;
    _this->avFormatContext = 0;
    //释放锁
}
