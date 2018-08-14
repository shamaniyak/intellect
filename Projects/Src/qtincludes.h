#ifndef QTINCLUDES_H
#define QTINCLUDES_H

#if defined __cplusplus
/* Add C++ includes here */

#ifdef QT_CORE_LIB
#include <QtCore>
#endif

#ifdef QT_GUI_LIB
# include <qtgui>
#endif

#ifdef QT_NETWORK_LIB
# include <qtnetwork>
#endif

#ifdef QT_XML_LIB
# include <qtxml>
#endif

#ifdef QT_SQL_LIB
# include <qtsql>
#endif

#ifdef QT_SCRIPT_LIB
#include <QtScript>
#endif

#ifdef QT_QUICK_LIB
#include <QtQuick>
#endif

#endif

#endif // QTINCLUDES_H
