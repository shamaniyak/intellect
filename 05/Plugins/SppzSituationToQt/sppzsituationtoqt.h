#ifndef SPPZSITUATIONTOQT_H
#define SPPZSITUATIONTOQT_H

#include "sppzsituationtoqt_global.h"
#include <Interfaces/isppzsituationplugin.h>
#include <QVariant>

class ISituationIntf;
class ISituationObjList;
class IFigureIntf;
class IFigureProperties;

//  PrpertiesWrapper

class PropertiesWrapper : public QObject
{
  Q_OBJECT

public:
  explicit PropertiesWrapper(QObject *parent);

  IFigureProperties *props() const;
  void setProps(IFigureProperties *props);

public slots:
  int getCount() const;
  QString getName(int i) const;
  QVariant getVal(int i) const;

private:
  IFigureProperties *props_ = nullptr;
};

//  ObjectWrapper

class ObjectWrapper : public QObject
{
  Q_OBJECT

public:
  explicit ObjectWrapper(QObject *parent);

  IFigureIntf *obj() const;
  void setObj(IFigureIntf *obj);

public slots:
  QString getName() const;
  int getClassId() const;
  double getPos(double &b, double &l) const;
  double getB() const;
  double getL() const;
  double getH() const;

  QObject *getProperties();

private:
  IFigureIntf *obj_ = nullptr;
  PropertiesWrapper *props_ = nullptr;

};

//  SituationObjListWrapper

class SituationObjListWrapper : public QObject
{
  Q_OBJECT

public:
  explicit SituationObjListWrapper(QObject *parent);

  ISituationObjList *list() const;
  void setList(ISituationObjList *list);

public slots:
  int getCount();
  QObject * getFirst();
  QObject * getNext();

private:
  ISituationObjList *list_ = 0;
  ObjectWrapper *currentObj_ = 0;

};

//  SppzSituationToQt

class SPPZSITUATIONTOQTSHARED_EXPORT SppzSituationToQt : public QObject, public ISppzSituationPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID ISppzSituationPlugin_iid)
  Q_INTERFACES(ISppzSituationPlugin)

public:
  explicit SppzSituationToQt(QObject *parent=0);
  ~SppzSituationToQt();

  // ISppzSituationPlugin interface
public:
  virtual int getVersion()
  {
    return 1;
  }

public slots:
  QObject * getObjects();

private:
  ISituationIntf *sit_ = nullptr;
  SituationObjListWrapper *objList_ = nullptr;

  void initLibrary();
};

#endif // SPPZSITUATIONTOQT_H
