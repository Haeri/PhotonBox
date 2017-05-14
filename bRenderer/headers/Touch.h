#ifndef B_TOUCH_H
#define B_TOUCH_H

#include <map>

struct Touch
{
    GLfloat startPositionX;
    GLfloat startPositionY;
    GLfloat lastPositionX;
    GLfloat lastPositionY;
    GLfloat currentPositionX;
    GLfloat currentPositionY;
};

/* Typedefs */
typedef std::map< int, Touch > TouchMap;

#endif /* defined(B_TOUCH_H) */
