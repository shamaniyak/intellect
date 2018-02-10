#include "sppzclassifiertoqt.h"

#include <iclassifier.h>

#include <QLibrary>

//  CharWrapper

CharWrapper::CharWrapper(QObject *parent) : QObject(parent)
{

}

void CharWrapper::setC(IChar *c)
{
  c_ = c;
}

IChar *CharWrapper::c() const
{
  return c_;
}

QString CharWrapper::name() const
{
  QString s;
  if(c_)
  {

    //char name[256] = {0};
    //c_->name1(name, 256);
    s = QString::fromLocal8Bit(c_->name());
  }
  return s;
}

int CharWrapper::classId() const
{
  if(c_)
    return c_->classId();
  return -1;
}

bool CharWrapper::isGroup() const
{
  if(c_)
    return c_->isGroup();
  return false;
}

unsigned int CharWrapper::charCode() const
{
  if(c_)
    return c_->charCode();
  return 0;
}

bool CharWrapper::readOnly() const
{
  if(c_)
    return c_->readOnly();
  return false;
}

bool CharWrapper::computed() const
{
  if(c_)
    return c_->computed();
  return false;
}

bool CharWrapper::canFlyMission() const
{
  if(c_)
    return c_->canFlyMission();
  return false;
}

bool CharWrapper::canSituation() const
{
  if(c_)
    return c_->canSituation();
  return false;
}

//  CharGroupWrapper

CharGroupWrapper::CharGroupWrapper(QObject *parent) : QObject(parent)
  ,currentChar_(new CharWrapper(this))
{

}

ICharGroup *CharGroupWrapper::cg() const
{
  return cg_;
}

void CharGroupWrapper::setCg(ICharGroup *cg)
{
  cg_ = cg;
}

int CharGroupWrapper::id() const
{
  if(cg_)
    return cg_->id();
  return -1;
}

QString CharGroupWrapper::name() const
{
  QString s;
  if(cg_)
  {
    s = QString::fromLocal8Bit( cg_->name() );
  }
  return s;
}

QObject *CharGroupWrapper::getFirstChar()
{
  if(cg_)
    currentChar_->setC( cg_->GetFirstChar() );

  if(currentChar_->c())
    return currentChar_;
  return nullptr;
}

QObject *CharGroupWrapper::getNextChar()
{
  if(cg_)
    currentChar_->setC( cg_->GetNextChar( currentChar_->c() ) );

  if(currentChar_->c())
    return currentChar_;
  return nullptr;
}

int CharGroupWrapper::getCharCount()
{
  if(cg_)
    return cg_->GetCharCount();
  return 0;
}

//  CharSectionWrapper

CharSectionWrapper::CharSectionWrapper(QObject *parent) : QObject(parent),
  current_(new CharGroupWrapper(this))
{

}

ICharSection *CharSectionWrapper::cs() const
{
  return cs_;
}

void CharSectionWrapper::setCs(ICharSection *cs)
{
  cs_ = cs;
}

int CharSectionWrapper::getCount()
{
  if(cs_)
    return cs_->GetCount();
  return 0;
}

QObject *CharSectionWrapper::getFirst()
{
  if(cs_)
    current_->setCg( cs_->GetFirst() );

  if(current_->cg())
    return current_;
  return nullptr;
}

QObject *CharSectionWrapper::getNext()
{
  if(cs_)
    current_->setCg( cs_->GetNext( current_->cg() ) );

  if(current_->cg())
    return current_;
  return nullptr;
}

//  ClassWrapper

ClassWrapper::ClassWrapper(QObject *parent) : QObject(parent),
  chars_(new CharSectionWrapper(this)),
  gChars_(new CharSectionWrapper(this))
{

}

void ClassWrapper::setC(IClassType *c)
{
  c_ = c;
}

IClassType *ClassWrapper::c() const
{
  return c_;
}

int ClassWrapper::id() const
{
  if(c_)
    return c_->id();
  return -1;
}

QString ClassWrapper::name() const
{
  QString s;
  if(c_)
  {
    const char *name = c_->name();
    s = QString::fromLocal8Bit( name );
  }
  return s;
}

QObject *ClassWrapper::charSection()
{
  if(c_)
    chars_->setCs( c_->charSection() );

  if(chars_->cs())
    return chars_;
  return nullptr;
}

QObject *ClassWrapper::groupSection()
{
  if(c_)
    gChars_->setCs(c_->groupSection());

  if(gChars_->cs())
    return gChars_;
  return nullptr;
}

//  SppzClassifierToQt

SppzClassifierToQt::SppzClassifierToQt(QObject *parent) : QObject(parent),
  currentChar_(new CharWrapper(this)),
  currentClass_(new ClassWrapper(this))
{
  this->setObjectName("SppzClassifier");

  initLibrary();
}

SppzClassifierToQt::~SppzClassifierToQt()
{
  if(c_)
    c_->release();
  c_ = nullptr;
}

void SppzClassifierToQt::initLibrary()
{
  if(c_)
    return;

  QLibrary *lib = new QLibrary("d:\\Work\\sppz\\Exe\\classifier", this);
  if(lib->load())
  {
    typedef IClassifier * (*FunCreateClassifier)();
    FunCreateClassifier fun = (FunCreateClassifier)lib->resolve("_CreateClassifier");
    if(fun)
      c_ = dynamic_cast<IClassifier*>( fun() );
  }
}

int SppzClassifierToQt::getCharCount()
{
  if(c_)
    return(c_->GetCharCount());
  return 0;
}

QObject *SppzClassifierToQt::getFirstChar()
{
  if(c_)
  {
    auto ch = c_->GetFirstChar();
    if(ch)
    {
      currentChar_->setC( ch );
      return currentChar_;
    }
  }
  return nullptr;
}

QObject *SppzClassifierToQt::getNextChar()
{
  if(c_)
  {
    auto ch = currentChar_->c();
    ch = c_->GetNextChar( ch );
    if(ch)
    {
      currentChar_->setC( ch );
      return currentChar_;
    }
  }
  return nullptr;
}

int SppzClassifierToQt::getClassCount()
{
  if(c_)
    return c_->GetClassCount();
  return 0;
}

QObject *SppzClassifierToQt::getFirstClass()
{
  if(c_)
  {
    auto cl = c_->GetFirstClass();
    if(cl)
    {
      currentClass_->setC( cl );
      return currentClass_;
    }
  }

  return nullptr;
}

QObject *SppzClassifierToQt::getNextClass()
{
  if(c_)
  {
    auto cl = currentClass_->c();
    cl = c_->GetNextClass( cl );
    if(cl)
    {
      currentClass_->setC( cl );
      return currentClass_;
    }
  }

  return nullptr;
}
