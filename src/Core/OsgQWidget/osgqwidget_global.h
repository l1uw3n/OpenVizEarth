#ifndef OSGQWIDGET_GLOBAL_H
#define OSGQWIDGET_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef OSGQWIDGET_LIB
# define OSGQWIDGET_EXPORT Q_DECL_EXPORT
#else
# define OSGQWIDGET_EXPORT Q_DECL_IMPORT
#endif

#endif // OSGQWIDGET_GLOBAL_H
