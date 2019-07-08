#include "updateUIThread.h"
#include <iostream>
UpdateUIThread::UpdateUIThread(){
}
void UpdateUIThread::quit(){
    this->quitFlag = true;
}
void UpdateUIThread::run(){
    int lastPlay = 0;
    int nowPlay = 0;
    int state = 0;
    while(!this->quitFlag){
        state = this->play->getState();
        if(state == PLAY_STATE_PLAYING){
            nowPlay = this->play->getAlreadyPlay_S();
            if(nowPlay != lastPlay){
                emit this->sendAlreadPlay_S(nowPlay);
                lastPlay = nowPlay;
            }
        }
        else if(state == PLAY_STATE_FINISH){
            emit this->songFinish();
            return;
        }
    }
    this->quitFlag = false;
}
