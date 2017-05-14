#ifndef B_OS_DETECT_H
#define B_OS_DETECT_H


#ifdef _WIN32
/* Windows 32-bit and 64-bit */
#define B_OS_DESKTOP
#elif __APPLE__
#include "TargetConditionals.h"
#if TARGET_IPHONE_SIMULATOR
/* iOS Simulator*/
#define B_OS_IOS
#elif TARGET_OS_IPHONE
/* iOS */
#define B_OS_IOS
#elif TARGET_OS_MAC
/* Mac OSX */
#define B_OS_DESKTOP
#else
/* Probably unsupported */
#define B_OS_DESKTOP
#endif
#elif __linux
/* Linux */
#define B_OS_DESKTOP
#else
/* Probably unsupported */
#define B_OS_DESKTOP
#endif

#endif /* defined(B_OS_DETECT_H) */
