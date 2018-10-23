#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qdebug.h"
#include<string.h>
#include <iostream>
#include <sstream>
#include<math.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    filedialog(new QFileDialog(this)),
    filedialog2(new QFileDialog(this)),
    player(new QMediaPlayer(this))
{
    filedialog->setNameFilter("Sound(*.wav *.mp3)");
    filedialog->setFileMode(QFileDialog::ExistingFile);
    ui->setupUi(this);
    player->connect(player,SIGNAL(stateChanged(QMediaPlayer::State)),this,SLOT(on_player_stateChanged(QMediaPlayer::State)));
}

MainWindow::~MainWindow()
{
    delete player;
    delete ui;
}

void MainWindow::on_browse_clicked()
{

    QString a="browse";
    qDebug()<<a+"clicked";
    filedialog->exec();
    QStringList files=filedialog->selectedFiles();
    path=files[0];
    tem=path.mid(0,path.lastIndexOf("/")+1);
    qDebug()<<files[0];
    ui->path->setText(path);

}


void MainWindow::on_load_clicked()
{
    QString a="load";
    qDebug()<<a+"clicked";
    af.load(path.toStdString());
    ui->textEdit->setText(getInfo());

}

void MainWindow::on_play_clicked()
{
    if(ui->play->text()=="Play"){
        player->setMedia(NULL);
        QString tempPath=tem;
        tempPath.append("/temp.wav");
        af.save(tempPath.toStdString());
        player->setMedia(QMediaContent(QUrl::fromLocalFile(tempPath)));
        player->play();
        ui->play->setText("Pause");
        showInfo("play");
    }
    else if(ui->play->text()=="Pause"){
        player->pause();
        ui->play->setText("Resume");
    }
    else if(ui->play->text()=="Resume"){
        player->play();
        ui->play->setText("Pause");
    }
}

void MainWindow::on_stop_clicked()
{
    player->stop();
    ui->play->setText("Play");
    showInfo("stop");
}

void MainWindow::on_player_stateChanged(QMediaPlayer::State state){
    if(state==QMediaPlayer::StoppedState){
        ui->play->setText("Play");
    }
}


void MainWindow::on_save_clicked()
{
//    QString savePath=filedialog2->getExistingDirectory(this,"请选择保存路径...","./");
//    savePath.append("/temp.wav");
    QString savePath=filedialog2->getSaveFileName(this,"请选择保存路径...","./",tr("Images (*.wav *.mp3)"));
    af.save(savePath.toStdString());
    showInfo("save");
}

void MainWindow::on_actionreverse_triggered()
{
        AudioFile<double>::AudioBuffer buffer;
        buffer.resize (2);
        int numSamples = af.getNumSamplesPerChannel();
        buffer[0].resize (numSamples);
        buffer[1].resize (numSamples);
        for (int i = 0; i < numSamples; i++)
        {
            buffer[0][i]=af.samples[0][numSamples-1-i];
            buffer[1][i]=af.samples[1][numSamples-1-i];
        }
        af.setAudioBuffer(buffer);
        showInfo("reverse");
}

void MainWindow::on_actionfade_triggered()
{
    double end = af.getLengthInSeconds();
    double start = end>=2?end-2:end/2;
    QJsonValue jv = readParam("start");
    if(!jv.isUndefined()&&jv.isDouble()){
        start = jv.toDouble();
    }
    jv = readParam("end");
    if(!jv.isUndefined()&&jv.isDouble()){
        end=jv.toDouble();
    }
    int samplerate=af.getSampleRate();
    for(int i=0;i<af.getNumChannels();i++){
        for(int j=start*samplerate;j<end*samplerate;j++){
            af.samples[i][j]*=(end*samplerate-j)/(end*samplerate-start*samplerate);
        }
    }

    showInfo("fade");
}
void MainWindow::on_actionresample_triggered()
{
    double res = 1;
    QJsonValue jv = readParam("resample");
    if(!jv.isUndefined()&&jv.isDouble()){
        res = jv.toDouble();
    }
    AudioFile<double>::AudioBuffer buffer;
    buffer.resize (2);
    int numSamples = af.getNumSamplesPerChannel();
    for(int i=0;i<af.getNumChannels();i++){
        buffer[i].resize(ceil(res*numSamples));
        for(int j=0;j<numSamples;j++){
            buffer[i][ceil(j*res)]=af.samples[i][j];
        }
    }

    af.setAudioBuffer(buffer);
    showInfo("resample");
}

QJsonValue MainWindow::readParam(QString name){
    name=name.toLower();
    QString str = ui->lineEdit_2->text();
    QJsonDocument jd = QJsonDocument::fromJson(str.toUtf8());
    return jd.object().value(name);
}

void MainWindow::on_actionvulomn_triggered()
{
    double res = 1;
    QJsonValue jv = readParam("volumn");
    if(!jv.isUndefined()&&jv.isDouble()){
        res = jv.toDouble();
    }
    AudioFile<double>::AudioBuffer buffer;
    buffer.resize (2);
    int numSamples = af.getNumSamplesPerChannel();
    for(int i=0;i<af.getNumChannels();i++){
        buffer[i].resize(numSamples);
        for(int j=0;j<numSamples;j++){
            buffer[i][j]=af.samples[i][j]*res;
        }
    }

    af.setAudioBuffer(buffer);
    if(res>1){showInfo("vulomnUP");
    }
    else showInfo("vulomnDOWN");
}

QString MainWindow::getInfo() const{
    stringstream ss;
    ss<<"--------------------------------------"<<endl
     <<"|Num Channels: "<<af.getNumChannels()<<endl
    <<"|Num Samples Per Channel: "<<af.getNumSamplesPerChannel()<<endl
    <<"|Sample Rate: "<<af.getSampleRate()<<endl
    <<"|Bit Depth: "<<af.getBitDepth()<<endl
    <<"|Length in Seconds: "<<af.getLengthInSeconds()<<endl
    <<"--------------------------------------"<<endl;
    return QString(ss.str().c_str());
}



void MainWindow::showInfo(QString a){
    QString b;
    b=ui->textEdit->toPlainText();  //拿到已经存在的字符
    b.append("\n");                 //添加新的字符
    b.append(a);
    b.append("\n");
    b.append("--------------------------------------");
    ui->textEdit->setText(b);       //显示
}



void MainWindow::on_actionola_triggered()
{ int N=af.getSampleRate()/10;
  int Sa=N/2,Ss;
  double rate=1;
  QJsonValue jv = readParam("rate");
  if(!jv.isUndefined()&&jv.isDouble()){
      rate = jv.toDouble();
  }
  Ss=Sa/rate;
  AudioFile<double>::AudioBuffer buffer;
  buffer.resize (2);
  int numSamples = af.getNumSamplesPerChannel();
  int window_num=(numSamples-N)/(N-Sa)+1;
  double D[N];
  for(int i=0;i<af.getNumChannels();i++){
      buffer[i].resize((window_num-1)*(N-Ss)+N);
      for(int m=0;m<window_num;m++){
          for(int j=0;j<N;j++){
              D[j]=0.5-0.5*cos(2*3.14*j/N);
              D[j]=af.samples[i][j+m*(N-Sa)]*D[j];
              buffer[i][j+m*(N-Ss)]=buffer[i][j+m*(N-Ss)]+D[j];
          }

      }

  }
   af.setAudioBuffer(buffer);

   showInfo("OverLap and Add with Rate"+QString::number(rate));
}


void MainWindow::on_actiondtw_triggered()
{
    QString p;
    QJsonValue jv = readParam("path");
    if(!jv.isUndefined()&&jv.isString()){
        p = jv.toString();
    }
    qDebug()<<p;
    AudioFile<double> af2;
    af2.load(p.toStdString()); //第二段音频
    double start=0,end=af.getLengthInSeconds(),min;
    int len=af2.getNumSamplesPerChannel();double D[len],D2[len];
    int Sa=len/2;
    int numSamples = af.getNumSamplesPerChannel();
    int window_num=(numSamples-len)/(len-Sa)+1;
     qDebug()<<QString::number(window_num);
    for(int m=0;m<window_num;m++){
            for(int j=0;j<len;j++){
                D[j]=af.samples[0][j+m*(len-Sa)];
                D2[j]=af2.samples[0][j];
                }
            double m1=dtw(D,D2,len,len); //返回dtw结果
            if(m==0){
                min=m1;
                start=0;
                end=(start+len)/af.getSampleRate();
            }
            else{
                if(m1<min){
                    min=m1;
                    double l=len-Sa,n=af.getSampleRate();
                    start=m*l/n;
                    end=start+len/n;
                }

            }

        }
    QString sstart=QString::number(start,'g',4);
    QString send=QString::number(end,'g',4);
    showInfo("start:"+sstart+"  \n"+"end:"+send);

}
double MainWindow::dtw(double s1[],double s2[],int w,int h){
        double **mat;  //c++不能定义动态数组
        mat=new double*[h];

    for(int i=0;i<2205;i++){
        mat[i]=new double[w];
        for(int j=0;j<w;j++){
            double dist =abs(s1[j]-s2[i]);
            if(i==0&&j==0){mat[i][j]=dist;}
            else if(i!=0&&j==0){mat[i][j]=dist+mat[i-1][j]; }
            else if(i==0&&j!=0){mat[i][j]=dist+mat[i][j-1]; }
            else{
                mat[i][j]=dist+min(min(mat[i-1][j],mat[i][j-1]),mat[i-1][j-1]);
            }
        }
    }
    double m;
    double pathsum=0,pathsig=1;
    int i=h-1;
    int j=w-1;
    while(j>=1||i>=1){
         if(i>0&&j>0){
           m=min(min(mat[i-1][j],mat[i-1][j-1]),mat[i][j-1]);
           if(m==mat[i-1][j]){
               pathsum+=m;
               i--;
               }
           else if(m==mat[i-1][j-1]){
            pathsum+=m;
            i--;
            j--;
            }
           else{
            pathsum+=m;
            j--;}
         }
         else if(i==0){
            pathsum+=mat[0][j-1];
            j--;
         }
         else{/*j==0*/
            pathsum+=mat[i-1][0];
             i--;
         }
   }
     for(int i=0;i<h;i++)
        delete []mat[i];
       delete []mat;
   return pathsum;
}
