#ifndef _FALLASSIGNMENTMGR_H
#define _FALLASSIGNMENTMGR_H

#include <_common.h>
#include <_fallAssignment.h>
#include <_texture.h>


class _fallAssignmentMgr
{
    public:
        _fallAssignmentMgr();
        virtual ~_fallAssignmentMgr();

        float nextSpawn;    // Time until the next spawn
        float speed = 15;   // Speed of the falling assignments

        void spawnAssignment(vector<_fallAssignment>& assignments, vec3 pos);  // "Spawn" a new assignment into an assignment vector
        void drawAssignments(vector<_fallAssignment>& assignments);            // Draw all of the assignment in an assignment vector

        _texture *texture = nullptr;                // For applying textures to the falling assignments
        void loadTexture(const char* texturePath);  // Function to load the texture to be used for the assignments

    protected:

    private:
};

#endif // _FALLASSIGNMENTMGR_H
