#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <ctime>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include <QDir>
#include <QIcon>
#include <QLabel>
#include <QTimer>
#include <QSlider>
#include <QListView>
#include <QFileInfo>
#include <QTextCodec>
#include <QTextStream>
#include <QStringList>
#include <QMainWindow>
#include <QPushButton>
#include <QStringListModel>

#include "util.h"
#include "play.h"
#include "decode.h"

#define PLAY_MODE_MUNBER        3
#define PLAY_MODE_RANDOM        0
#define PLAY_MODE_ORDER         1
#define PLAY_MODE_SINGLE        2
#define nextPlayMode(a)         ((((a)+1)-PLAY_MODE_MUNBER)?((a)+1):0)
static const char* const PLAY_MODE_NAME[] = {"随机","顺序","单曲"};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_playSong_clicked();
    void on_progressBar_sliderReleased();
    void on_progressBar_valueChanged(int value);
    void on_list_doubleClicked(const QModelIndex &index);
    void on_playMode_clicked();
    void on_nextSong_clicked();
    void on_lastSong_clicked();

private:
    //ui
    Ui::MainWindow *ui;
    QPushButton* playBtn;
    QPushButton* nextSongBtn;
    QPushButton* lastSongBtn;
    QPushButton* playModeBtn;
    QSlider* progressBar;
    QListView* listLayout;
    QLabel* playing;
    //播放模式
    int playMode;
    //按钮图标
    QIcon playIcon;
    QIcon pauseIcon;
    //播放进度
    QLabel* alreadyPlay;
    QLabel* totalTime;
    QLabel* show_d;
    //解码器,播放器,ui更新线程
    Decode decode;
    Play *play;
//    UpdateUIThread* updateUI;
    char* musicData;
    //查询歌曲已经播放时间的定时器
    QTimer queryAlreadyPlayTime_s;
    //歌曲
    unsigned int musicLength;
    QStringList musicName;
    QStringList musicDir;
    std::vector<unsigned int> nameMapDir;
    std::vector<int> musicPlayList;
    QStringListModel* listModel;
    void initMusicDirInfo();
    int getNextSongSubscript();
    
    QStringList readFileNameFromDir(const QString& dir);
    void playMusicName(int subscript);
};

#endif // MAINWINDOW_H
