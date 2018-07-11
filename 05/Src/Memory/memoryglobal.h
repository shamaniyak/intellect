#ifndef MEMORYGLOBAL_H
#define MEMORYGLOBAL_H

#if defined(BUILD_MEMORY_LIB)
    #define MEMORY_EXPORT Q_DECL_EXPORT
#else
    #define MEMORY_EXPORT
#endif

#endif // MEMORYGLOBAL_H
