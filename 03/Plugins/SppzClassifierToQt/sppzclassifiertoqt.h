#ifndef SPPZCLASSIFIERTOQT_H
#define SPPZCLASSIFIERTOQT_H

#include "sppzclassifiertoqt_global.h"
#include <Interfaces/isppzclassifierplugin.h>

class IChar;
class IClassType;
class IClassifier;
class ICharGroup;
class ICharSection;

//
class CharWrapper : public QObject
{
  Q_OBJECT

public:
  explicit CharWrapper(QObject *parent);

  void setC(IChar *c);
  IChar *c() const;

public slots:
  QString name() const;
  int classId() const;
  bool isGroup() const;
  unsigned int charCode() const;
  bool readOnly() const;
  bool computed() const;
  bool canFlyMission() const;
  bool canSituation() const;

private:
  IChar* c_ = 0;
};

//
class CharGroupWrapper : public QObject
{
  Q_OBJECT

public:
  explicit CharGroupWrapper(QObject *parent);

  ICharGroup *cg() const;
  void setCg(ICharGroup *cg);

public slots:
  int id() const;             // код группы
  QString name() const;  // Название группы характеристик
  // Выдает первую характеристику в списке
  QObject * getFirstChar();
  // Выдает следующую характеристику
  QObject * getNextChar();
  // Выдает кол-во хар-к
  int getCharCount();

private:
  ICharGroup *cg_ = 0;
  CharWrapper *currentChar_ = 0;
};

//
class CharSectionWrapper : public QObject
{
  Q_OBJECT

public:
  explicit CharSectionWrapper(QObject *parent);

  ICharSection *cs() const;
  void setCs(ICharSection *cs);

public slots:
  int getCount();
  QObject * getFirst();
  QObject * getNext();

private:
  ICharSection *cs_ = 0;
  CharGroupWrapper *current_ = 0;
};

//
class ClassWrapper : public QObject
{
  Q_OBJECT

public:
  explicit ClassWrapper(QObject *parent);

  void setC(IClassType *c);
  IClassType *c() const;

public slots:
  int id() const;
  QString name() const;
  // харктеристики класса
  QObject * charSection();
  // групповые харктеристики
  QObject * groupSection();

private:
  IClassType* c_ = 0;
  CharSectionWrapper *chars_ = 0;
  CharSectionWrapper *gChars_ = 0;
};

//
class SPPZCLASSIFIERTOQTSHARED_EXPORT SppzClassifierToQt : public QObject, public ISppzClassifierPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID ISppzClassifierPlugin_iid)
  Q_INTERFACES(ISppzClassifierPlugin)

public:
  explicit SppzClassifierToQt(QObject *parent=0);
  ~SppzClassifierToQt();

  // IPlugin interface
public:
  virtual int getVersion() override
  {
    return 1;
  }

  void initLibrary();

public slots:

  int getCharCount();
  // Выдает первую
  QObject* getFirstChar();
  // Выдает следующую
  QObject* getNextChar();

  QObject *getCurrentChar() const {return currentChar_;}

  int getClassCount();
  // Выдает первый класс
  QObject* getFirstClass();
  // Выдает слоедующий класс
  QObject* getNextClass();

private:
  IClassifier *c_ = 0;
  CharWrapper *currentChar_ = 0;
  ClassWrapper *currentClass_ = 0;

};

#endif // SPPZCLASSIFIERTOQT_H
