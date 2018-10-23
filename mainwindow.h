#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<AudioFile.h>
#include<QMediaPlayer>
#include<QFileDialog>
#include<QDir>
#include<QDebug>
#include <iostream>
#include <sstream>
#include <string>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>
using namespace std;
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
    void on_load_clicked();

    void on_browse_clicked();

    void on_play_clicked();

    void on_player_stateChanged(QMediaPlayer::State);

    void on_stop_clicked();

    void on_save_clicked();

    void on_actionreverse_triggered();

    QString getInfo() const;
    void showInfo(QString);

    void on_actionfade_triggered();

    void on_actionvulomn_triggered();
    QJsonValue readParam(QString name);

    void on_actionresample_triggered();

    void on_actionola_triggered();
    double dtw(double s1[],double s2[],int w,int h);
    double distance(double p1,double p2);

    void on_actiondtw_triggered();

private:
    Ui::MainWindow *ui;
    QFileDialog *filedialog;
    QFileDialog *filedialog2;
    QMediaPlayer *player;
    QString path,tem;
    AudioFile<double> af;

};

#endif // MAINWINDOW_H
