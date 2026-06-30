#include "_fallAssignmentMgr.h"

_fallAssignmentMgr::_fallAssignmentMgr()
{
    srand(static_cast<unsigned int>(time(nullptr)));    // Seed random number generator
    nextSpawn = 3.0f; // Time until first spawn
    this->texture = new _texture();
}

_fallAssignmentMgr::~_fallAssignmentMgr()
{
    delete texture;
}

// Honestly this is an abomination, I should have just left this in scene but oh well
void _fallAssignmentMgr::spawnAssignment(vector<_fallAssignment>&  assignments, vec3 pos)
{
    // Get dimensions for new assignment being created
    float w = 6.0f;
    float h = 7.8f;
    float d = 0.1f;

    // Create the new obstacle and add it to the assignments container
    float maxDistance = 40.0f;
    float maxX = pos.x + maxDistance;
    float maxZ = pos.z + maxDistance;

    _fallAssignment a(frand(-maxX, maxX), 80.0f, frand(-maxZ, maxZ), w, h, d, speed);
    assignments.push_back(a);

    // Get time for next assignment spawn
    nextSpawn = frand(1.6f, 3.2);
}

void _fallAssignmentMgr::drawAssignments(vector<_fallAssignment>& assignments)
{
    // Return early if there are no assignments to be drawn
    if (assignments.empty()) {
        return;
    }

    // Push attributes so we can tinker with stuff without nuking the rest of the draw functions
    glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_TEXTURE_BIT);
    glDisable(GL_LIGHTING);

    // Bind the assignment texture
    texture->bind();

    // Draw each assignment in the vector
    for(auto &a : assignments) {
        a.draw();
    }

    // Restore the attribute matrix
    glPopAttrib();
}

void _fallAssignmentMgr::loadTexture(const char* texturePath)
{
    texture->load(texturePath);
}
