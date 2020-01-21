#ifndef OS_LINUX
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif //_MSC_VER
#endif //OS_LINUX
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWindow){
    ui->setupUi(this);

    this->play = new Play();
    this->playMode = PLAY_MODE_RANDOM;
    this->musicLength = 0;
    //初始化控件
    this->playBtn = ui->playSong;
    this->nextSongBtn = ui->nextSong;
    this->lastSongBtn = ui->lastSong;
    this->progressBar = ui->progressBar;
    this->listLayout = ui->list;
    this->playing = ui->playing;
    this->playModeBtn = ui->playMode;
    this->playModeBtn->setText(tr(PLAY_MODE_NAME[this->playMode]));
    //播放图标控件
    this->playIcon = QIcon(":/res/playerIcon/res/play.png");
    this->pauseIcon = QIcon(":/res/playerIcon/res/pause.png");
    this->playBtn->setIcon(this->playIcon);
    //时间进度控件
    this->totalTime = ui->totalTIme_S;
    this->alreadyPlay = ui->alreadyPlay_S;
    //进度条控件
    this->progressBar->setMaximum(0);
    this->progressBar->setSingleStep(1);
    //读取歌曲信息
    this->initMusicDirInfo();
    this->musicLength = this->musicName.size();
    //定时器设置
    queryAlreadyPlayTime_s.setInterval(1000);
    queryAlreadyPlayTime_s.setSingleShot(false);
    connect(&(this->queryAlreadyPlayTime_s),&QTimer::timeout,[=](){
        unsigned long alreadyPlayTime = this->play->getAlreadyPlay_S();
        if(alreadyPlayTime >= this->play->getMusicTime_S()){
            on_nextSong_clicked();
            return;
        }
        ui->progressBar->setValue(alreadyPlayTime);
    });
}
MainWindow::~MainWindow(){
    delete ui;
    this->play->quit();
    delete this->play;
    delete this->listModel;
}
void MainWindow::initMusicDirInfo(){
    unsigned int dirSubscript = 0;
    QFile configFile("dirConfig");
    QString dir;
    if(configFile.open(QFile::ReadOnly)){
        QTextStream dirInputStream(&configFile);
        while(!dirInputStream.atEnd()){
            dirInputStream >> dir;
            if(dir.isEmpty()){
                continue;
            }
            QStringList musicNameInDir = readFileNameFromDir(dir);
            for(auto i=musicNameInDir.begin();i!=musicNameInDir.end();++i){
                this->nameMapDir.push_back(dirSubscript);
                this->musicName << *i;
            }
#ifdef Q_OS_LINUX
            if(!dir.endsWith("/")){
                dir.push_back('/');
            }
#else
            if(!dir.endsWith("\\")){
                dir.push_back('\\');
            }
#endif
            this->musicDir << dir;
        }
        this->listModel = new QStringListModel(this->musicName);
        this->listLayout->setModel(this->listModel);
        ++dirSubscript;
    }
    this->listModel = new QStringListModel(this->musicName);
    this->listLayout->setModel(this->listModel);
}
void MainWindow::on_playSong_clicked(){
    switch(this->play->getState()){
    case PLAY_STATE_PAUSE:
        this->queryAlreadyPlayTime_s.start();
        this->play->resume();
        this->playBtn->setIcon(this->pauseIcon);
        break;
    case PLAY_STATE_PLAYING:
        this->queryAlreadyPlayTime_s.start();
        this->play->pause();
        this->playBtn->setIcon(this->playIcon);
        break;
    case PLAY_STATE_STOP:
        this->queryAlreadyPlayTime_s.stop();
        this->playMusicName(0);
        break;
    default:
        break;
    }
}
void MainWindow::on_progressBar_sliderReleased(){
    this->play->jumpTo(this->progressBar->value());
    this->queryAlreadyPlayTime_s.start();
}
void MainWindow::on_progressBar_valueChanged(int value){
    ui->alreadyPlay_S->setText(QString("%1:%2").arg(value/60).arg(value%60));
}
void MainWindow::on_list_doubleClicked(const QModelIndex &index){
    int r = index.row();
    this->playMusicName(r);
    this->queryAlreadyPlayTime_s.start();
}
void MainWindow::on_playMode_clicked(){
    this->playMode = nextPlayMode(this->playMode);
    this->playModeBtn->setText(tr(PLAY_MODE_NAME[this->playMode]));
}
int MainWindow::getNextSongSubscript(){
    int next = 0;
    switch(this->playMode){
    case PLAY_MODE_ORDER:
        next = *(this->musicPlayList.rbegin()) + 1;
        next %= this->musicLength;
        break;
    case PLAY_MODE_RANDOM:
        srand(time(0));
        do{
            next = rand()%this->musicLength;
        }
        while(next == *(this->musicPlayList.rbegin()));
        break;
    case PLAY_MODE_SINGLE:
        next = *(this->musicPlayList.rbegin());
        break;
    }
    return next;
}
void MainWindow::playMusicName(int subscript){
    MusicInfo mInfo;
    PlayInfo pInfo;
    QString a = this->musicDir[this->nameMapDir[subscript]]+this->musicName[subscript];
    std::string name(a.toStdString());
    this->play->stop();
    this->musicData = (char*)this->decode.readMusic(name.c_str(),&mInfo);
    pInfo = MusicInfoCoverToPlayInfo(mInfo);
    this->play->setBuffer(this->musicData,pInfo);
    this->progressBar->setMaximum(this->play->getMusicTime_S());
    this->totalTime->setText(QString::fromStdString(timeToString(this->play->getMusicTime_S())));
    this->musicPlayList.push_back(subscript);
    this->playing->setText(this->musicName[subscript]);
    this->playBtn->setIcon(this->pauseIcon);
    this->play->play();
}
void MainWindow::on_nextSong_clicked(){
    int next = this->getNextSongSubscript();
    this->playMusicName(next);
}
void MainWindow::on_lastSong_clicked(){
    auto i = this->musicPlayList.rbegin();
    if(i+1 != this->musicPlayList.rend()){
        this->playMusicName(*(i+1));
    }
}
QStringList MainWindow::readFileNameFromDir(const QString &dirName){
    QStringList ret;
    QFileInfo dirInfo(dirName);
    if(dirInfo.isDir()){
        QDir dir(dirName);
        QFileInfoList allMusicFileInfo = dir.entryInfoList(QDir::Files|QDir::Readable);
        for(auto i=allMusicFileInfo.begin();i!=allMusicFileInfo.end();++i){
            ret << i->fileName();
        }
    }
    return ret;
}
