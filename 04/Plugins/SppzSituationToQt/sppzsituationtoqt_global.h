#ifndef SPPZSITUATIONTOQT_GLOBAL_H
#define SPPZSITUATIONTOQT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SPPZSITUATIONTOQT_LIBRARY)
#  define SPPZSITUATIONTOQTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SPPZSITUATIONTOQTSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SPPZSITUATIONTOQT_GLOBAL_H