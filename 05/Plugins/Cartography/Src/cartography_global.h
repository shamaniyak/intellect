#ifndef CARTOGRAPHY_GLOBAL_H
#define CARTOGRAPHY_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CARTOGRAPHY_LIBRARY)
#  define CARTOGRAPHYSHARED_EXPORT Q_DECL_EXPORT
#else
#  define CARTOGRAPHYSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // CARTOGRAPHY_GLOBAL_H
