#include "multimedia.h"
#include <QSound>
#include <QMediaPlayer>

VideoPlayer::VideoPlayer(QWidget *parent) : QVideoWidget(parent)
{
  mp_ = new QMediaPlayer(this);
  mp_->setVideoOutput(this);
}

void VideoPlayer::open(const QString &fileName)
{
  mp_->setMedia(QMediaContent(QUrl::fromLocalFile(fileName)));
  mp_->play();
}

//  Multimedia

Multimedia::Multimedia(QObject *parent) : QObject(parent)
{
  this->setObjectName("Multimedia");
}

Multimedia::~Multimedia()
{

}

QObject *Multimedia::createSound(const QString &fileName)
{
  return( new QSound(fileName, this) );
}

QObject *Multimedia::createMediaPlayer(const QString &fileName)
{
  auto mp = new QMediaPlayer(this);
  mp->setMedia(QMediaContent(QUrl::fromLocalFile(fileName)));
  return( mp );
}

QWidget *Multimedia::createVideoWidget()
{
  auto vw = new QVideoWidget();
  return( vw );
}

QWidget *Multimedia::createVideoPlayer()
{
  return( new VideoPlayer() );
}
