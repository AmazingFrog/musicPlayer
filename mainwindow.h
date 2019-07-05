#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QSlider>
#include <QIcon>
#include <QLabel>
#include <QListView>
#include <vector>
#include <string>
#include <fstream>
#include <QStringList>
#include <QStringListModel>
#include <cstdlib>
#include <ctime>

#include "play.h"
#include "decode.h"
//#include "crossplatform.h"


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

public slots:
    void getAlreadyPlay_S(unsigned long s);
    void nextSong();

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
    //解码器,播放器
    Decode decode;
    Play *play;
    char* musicData;
    //歌曲
    unsigned int musicLength;
//    std::vector<std::string> musicName;
//    std::vector<std::string> musicDir;
    QStringList musicName;
    QStringList musicDir;
    std::vector<unsigned int> nameMapDir;
    std::vector<int> musicPlayList;
    QStringListModel* listModel;
    void initMusicDirInfo();
    int getNextSongSubscript();
    
    QStringList readFileNameFromDir(const QString& dir);
};

#endif // MAINWINDOW_H
