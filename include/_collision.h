#ifndef _COLLISION_H
#define _COLLISION_H

#include <_common.h>

class _collision
{
    public:
        _collision();
        virtual ~_collision();

        bool isLinearCol(vec3,vec3);
        bool isRadialCol(vec2,vec2,float,float,float);//positions x,y , radius A,B, threshold
        bool isSphereCol(vec3,vec3,float,float,float);//positions x,y , radius A,B, threshold
        bool isPlanoCol(vec2,vec2);
        bool isCubicCol(vec3,vec3,vec3,vec3);


    protected:

    private:
};

#endif // _COLLISION_H
