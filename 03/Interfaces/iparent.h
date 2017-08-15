#ifndef IPARENT_H
#define IPARENT_H

class IParent
{
  virtual ~IParent() {}

  IParent * getParent() = 0;
};

#endif // IPARENT_H
