#include "linearBuffer.h"
void LinearBuffer::push(void* data,unsigned long len){
    if(!(this->length % this->perBufferByteSize)){
        if(!(this->useBufferNumber < this->bufferNumber)){
            this->buffer.push_back(new char[this->perBufferByteSize]);
            ++this->useBufferNumber;
        }
    }
    unsigned int subscript = this->length / this->perBufferByteSize;
    unsigned long end = this->length % this->perBufferByteSize;
    char* pdata = (char*)data;
    if(end + len <= this->perBufferByteSize){
        memcpy(this->buffer[subscript]+end,pdata,len);
        this->length += len;
    }
    else{
        memcpy(this->buffer[subscript]+end,pdata,this->perBufferByteSize-end);
        pdata += (this->perBufferByteSize - end);
        len -= (this->perBufferByteSize - end);
        this->length += (this->perBufferByteSize - end);
        while(len > 0){
            char* t = new char[this->perBufferByteSize];
            unsigned int copyLen = (len<=this->perBufferByteSize)?(len):(this->perBufferByteSize);
            memcpy(t,pdata,copyLen);
            pdata += copyLen;
            this->buffer.push_back(t);
            len -= copyLen;
            this->length += copyLen;
            ++(this->bufferNumber);
            ++(this->useBufferNumber);
        }
    }
}
unsigned long LinearBuffer::getLength(){
    return this->length;
}
void* LinearBuffer::getData(){
    if(this->ret != 0){
        delete[] this->ret;
    }
    this->ret = new char[this->length];
    unsigned long i = 0;
    auto j=this->buffer.begin();
    if(this->length <= this->perBufferByteSize){
        memcpy(this->ret,*j,this->length);
        return (void*)this->ret;
    }
    for(unsigned int k=0;k<this->useBufferNumber-1;++k,++j,i+=this->perBufferByteSize){
        memcpy(this->ret+i,*j,this->perBufferByteSize);
    }
    memcpy(this->ret+i,*j,this->length-i);
    return (void*)this->ret;
}
LinearBuffer::~LinearBuffer(){
    if(this->ret != 0){
        delete[] this->ret;
        this->ret = 0;
    }
    for(auto i=this->buffer.begin();i!=this->buffer.end();++i){
        delete[] *i;
    }
}
void LinearBuffer::clear(){
    this->length = 0;
    this->useBufferNumber = 0;
}
