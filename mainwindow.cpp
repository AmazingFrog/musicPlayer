#ifndef OS_LINUX
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif //_MSC_VER
#endif //OS_LINUX
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "util.h"
#include <iostream>
#include <QTextCodec>

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
    this->show_d = ui->show;
    //进度条控件
    this->progressBar->setMaximum(0);
    this->progressBar->setSingleStep(1);
    connect(this->play,SIGNAL(sendAlreadyPlay_S(unsigned long)),this,SLOT(getAlreadyPlay_S(unsigned long)));
    //读取歌曲信息
    this->initMusicDirInfo();
    connect(this->play,SIGNAL(songFinish()),this,SLOT(nextSong()));
    this->musicLength = static_cast<unsigned int>(this->musicName.size());
}

MainWindow::~MainWindow(){
    delete ui;
    this->play->quit();
    delete this->play;
    delete this->listModel;
}

void MainWindow::initMusicDirInfo(){
#ifndef OS_LINUX
    QTextCodec* codec = QTextCodec::codecForName("GBK");
#endif
    std::ifstream fin("dirConfig");
    std::string dir;
    QStringList stringList;
    unsigned int dirSubscript = 0;
    while(fin.good()){
        fin >> dir;    
        std::vector<std::string> musicName = readFileNameFromDir(dir);
        this->musicDir.push_back(dir);
        for(auto i=musicName.begin();i!=musicName.end();++i){
            this->nameMapDir.push_back(dirSubscript);
#ifndef OS_LINUX
            stringList << codec->toUnicode(i->c_str());
#else
            stringList << QString::fromStdString(*i);
#endif    
        }
        this->listModel = new QStringListModel(stringList);
        this->listLayout->setModel(this->listModel);
        ++dirSubscript;
    }
}

void MainWindow::on_playSong_clicked(){
    MusicInfo mInfo;
    PlayInfo pInfo;
    switch(this->play->getState()){
    case PLAY_STATE_PAUSE:
        this->play->resume();
        this->playBtn->setIcon(this->pauseIcon);
        break;
    case PLAY_STATE_PLAYING:
        this->play->pause();
        this->playBtn->setIcon(this->playIcon);
        break;
    case PLAY_STATE_STOP:
        this->musicData = (char*)this->decode.readMusic((this->musicDir[this->nameMapDir[0]]+this->musicName[0]).c_str(),&mInfo);
        pInfo = MusicInfoCoverToPlayInfo(mInfo);
        this->play->setBuffer(this->musicData,pInfo);
        this->progressBar->setMaximum(this->play->getMusicTime_S());
        this->totalTime->setText(QString::fromStdString(timeToString(this->play->getMusicTime_S())));
        this->play->play();
        this->playing->setText(QString::fromStdString(this->musicName[0]));
        this->playBtn->setIcon(this->pauseIcon);
    default:
        break;
    }
}
void MainWindow::on_progressBar_sliderReleased(){
    this->show_d->setText(QString::fromStdString(timeToString(this->progressBar->value())));
    this->play->jumpTo(this->progressBar->value());
}
void MainWindow::on_progressBar_valueChanged(int value){
    this->alreadyPlay->setText(QString("%1").arg(value));
}
void MainWindow::getAlreadyPlay_S(unsigned long s){
    this->progressBar->setValue(s);
    this->alreadyPlay->setText(QString::fromStdString(timeToString(s)));
}
void MainWindow::on_list_doubleClicked(const QModelIndex &index){
    int r = index.row();
    MusicInfo mInfo;
    PlayInfo pInfo;
    std::string name(this->musicDir[this->nameMapDir[r]]+index.data().toString().toStdString());
    this->play->stop();
    this->musicData = (char*)this->decode.readMusic(name.c_str(),&mInfo);
    pInfo = MusicInfoCoverToPlayInfo(mInfo);
    this->play->setBuffer(this->musicData,pInfo);
    this->progressBar->setMaximum(this->play->getMusicTime_S());
    this->totalTime->setText(QString::fromStdString(timeToString(this->play->getMusicTime_S())));
    this->play->play();
    this->musicPlayList.push_back(r);
    this->playing->setText(index.data().toString());
    this->playBtn->setIcon(this->pauseIcon);
}
void MainWindow::on_playMode_clicked(){
    this->playMode = nextPlayMode(this->playMode);
    this->playModeBtn->setText(tr(PLAY_MODE_NAME[this->playMode]));
}
void MainWindow::nextSong(){
    int now = this->getNextSongSubscript();
    MusicInfo mInfo;
    PlayInfo pInfo;
    std::string name(this->musicDir[this->nameMapDir[now]]+this->musicName[now]);
    this->play->stop();
    this->musicData = (char*)this->decode.readMusic(name.c_str(),&mInfo);
    pInfo = MusicInfoCoverToPlayInfo(mInfo);
    this->play->setBuffer(this->musicData,pInfo);
    this->progressBar->setMaximum(this->play->getMusicTime_S());
    this->totalTime->setText(QString::fromStdString(timeToString(this->play->getMusicTime_S())));
    this->play->play();
    this->musicPlayList.push_back(now);
    this->playing->setText(QString::fromStdString(this->musicName[now]));
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
