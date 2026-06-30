#ifndef _FALLASSIGNMENT_H
#define _FALLASSIGNMENT_H

#include <_common.h>

class _fallAssignment
{
    public:
        _fallAssignment(float x, float y, float z,
                        float w, float h, float d,
                        float s);
        virtual ~_fallAssignment();

        float x, y, z;  // Pos
        float w, h, d;  // Dim
        float s;        // Speed

        bool graded;

        void update(float dt);
        void draw() const;


    protected:

    private:
};

#endif // _FALLASSIGNMENT_H
