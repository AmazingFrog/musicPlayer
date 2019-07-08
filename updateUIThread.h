#ifndef UPDATEUITHREAD_H
#define UPDATEUITHREAD_H
#include <QObject>
#include <QThread>
#include "play.h"

class UpdateUIThread : public QThread{
    Q_OBJECT
public:
    UpdateUIThread();
    UpdateUIThread(Play* p):play(p){}
    void quit();
    void run();
private:
    bool quitFlag = false;
    Play* play = 0;
signals:
    void sendAlreadPlay_S(int s);
    void songFinish();
};

#endif // UPDATEUITHREAD_H
