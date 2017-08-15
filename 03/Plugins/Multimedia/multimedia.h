#ifndef MULTIMEDIA_H
#define MULTIMEDIA_H

#include "multimedia_global.h"
#include <imultimedia.h>

#include <QWidget>
#include <QVideoWidget>

class QMediaPlayer;

class VideoPlayer : public QVideoWidget
{
  Q_OBJECT

public:
  explicit VideoPlayer(QWidget *parent = 0);

public slots:
  void open(const QString &fileName);

private:
  QMediaPlayer *mp_ = 0;
};

class MULTIMEDIASHARED_EXPORT Multimedia : public QObject, public IMultimediaPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID IMultimediaPlugin_iid)
  Q_INTERFACES(IMultimediaPlugin)

public:
  explicit Multimedia(QObject *parent=0);
  ~Multimedia();

  // IMultimediaPlugin interface
public:
  virtual int getVersion() override
  {
    return 1;
  }

public slots:
  QObject *createSound(const QString &fileName);
  QObject *createMediaPlayer(const QString &fileName);
  QWidget *createVideoWidget();
  QWidget *createVideoPlayer();
};

#endif // MULTIMEDIA_H
