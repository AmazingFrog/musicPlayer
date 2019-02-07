#ifndef __BUFFER_H__
#define __BUFFER_H__
#include <vector>
#include <cstring>
#include <iostream>
class LinearBuffer{
private:
    unsigned int bufferNumber = 0;
    unsigned int useBufferNumber = 0;
    unsigned long length = 0;
    unsigned long perBufferByteSize = 10 << 20;//10M
    std::vector<char*> buffer;
    char* ret = 0;
public:
    LinearBuffer(){}
    LinearBuffer(unsigned long bufferSize):perBufferByteSize(bufferSize){}
    ~LinearBuffer();
    void* getData();
    void clear();
    unsigned long getLength();
    void push(void* data,unsigned long len);
};

#endif // __BUFFER_H__
