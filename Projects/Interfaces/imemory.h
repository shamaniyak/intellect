#ifndef IMEMORY_H
#define IMEMORY_H

class IMemory
{
  virtual ~IMemory() {}

  IMemory * getParent() = 0;
};

#endif // IMEMORY_H
