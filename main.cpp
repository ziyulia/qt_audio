#include "mainwindow.h"
#include <QApplication>
#include<AudioFile.h>
#include<QMediaPlayer>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
//    AudioFile<double> audioFile;
//    audioFile.load ("E:/three/digitalpractice/audio/stereo.wav");
//    AudioFile<double> audioFile1;
//    audioFile1=audioFile;
//    audioFile1.setNumChannels(1);//变为单通道
//    audioFile1.save("E:/three/digitalpractice/audio/stereo_single.wav");
//    AudioFile<double>::AudioBuffer buffer;
//    buffer.resize (2);
//    int channel = 0;
//    int numSamples = audioFile.getNumSamplesPerChannel();
//    buffer[0].resize (numSamples);
//    buffer[1].resize (numSamples);
//    for (int i = 0; i < numSamples; i++)
//    {
//        buffer[0][i]=audioFile.samples[channel][i];
//        buffer[1][i]=0;
//    }
//    audioFile.setAudioBuffer(buffer);
//    audioFile.save("E:/three/digitalpractice/audio/stereo_singleequals0.wav");
//    QMediaPlayer player;
//    player.setMedia(QMediaContent(QUrl::fromLocalFile("E:/three/digitalpractice/audio/stereo.wav")));
//    player.play();
    return a.exec();
}
