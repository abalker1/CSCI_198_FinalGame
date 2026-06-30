#include "_collision.h"

_collision::_collision()
{
    //ctor
}

_collision::~_collision()
{
    //dtor
}

bool _collision::isLinearCol(vec3, vec3)
{
    // No implementation, unused anyway
}

// Returns true if the circle at p1 with radius r1 overlaps in excess of threshold with the circle at p2 with radius r2
bool _collision::isRadialCol(vec2 p1, vec2 p2, float r1, float r2, float threshold)
{
   return(sqrt(pow((p1.x-p2.x),2)+pow((p1.y-p2.y),2))-(r1+r2)< threshold);

}

// Returns true if the sphere at p1 with radius r1 overlaps in excess of threshold with the sphere at p2 with radius r2
bool _collision::isSphereCol(vec3 p1, vec3 p2, float r1, float r2, float threshold)
{
  return(sqrt(pow((p1.x-p2.x),2)+pow((p1.y-p2.y),2)+pow((p1.z-p2.z),2))-(r1+r2)< threshold);
}

bool _collision::isPlanoCol(vec2, vec2)
{
    // No implementation, unused anyway
}

// Returns true if the bounding box centered at p1 with half-dimensions h1 collides with the bounding box centered at p2 with half-dimensions h2.
bool _collision::isCubicCol(vec3 p1, vec3 p2, vec3 h1, vec3 h2)
{
    // Calculate absolute difference between 3D coordinate positions
    vec3 delta{fabs(p1.x - p2.x), fabs(p1.y - p2.y), fabs(p1.z - p2.z)};

    // Define the allowances that determine a collision (as is this is strict, no extra tolerance)
    vec3 allowance{h1.x + h2.x, h1.y + h2.y, h1.z + h2.z};

    // If the differences are less than or equal to the allowances, things are colliding
    return delta.x <= allowance.x && delta.y <= allowance.y && delta.z <= allowance.z;
}
