#ifndef GLERROR_H
#define GLERROR_H

#define DEBUG 0

void _check_gl_error(const char *file, int line);


#if DEBUG 
///
/// Usage
/// [... some opengl calls]
/// glCheckError();
///
#define check_gl_error() _check_gl_error(__FILE__,__LINE__)
#else
#define check_gl_error()
#endif
#endif // GLERROR_H
