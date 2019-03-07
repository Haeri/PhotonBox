#ifndef MEM_DEBUG_H
#define MEM_DEBUG_H

#include "crtdbg.h"

#ifdef PB_MEM_DEBUG
#define DEBUG_NEW   new( _NORMAL_BLOCK, __FILE__, __LINE__)
#else
#define DEBUG_NEW
#endif

#endif // MEM_DEBUG_H