#ifndef DDAGENT_GLOBAL_H
#define DDAGENT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(DDAGENT_LIBRARY)
#  define DDAGENTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define DDAGENTSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // DDAGENT_GLOBAL_H
