#include "_particles.h"



_particles::_particles()
{
    //ctor
}

_particles::~_particles()
{
    this->clear();
}

// Creates a new particle container instance and adds it to the vector of particle container instances
void _particles::createParticles(PType type, vec3 pos)
{
    switch (type) {

        case EXPLOSION:
            instances.push_back(new explosion(pos));
            instances.back()->init();
            break;

        default:
            break;

    }
}

// Updates all of the particle container instances
void _particles::update(float dt)
{
    for (auto* i : instances) {
        i->update(dt);
    }
    cull(); // Cull excess particle container instances
}

// Draws all of the particle container instances
void _particles::draw()
{
    for (auto* i : instances) {
        i->draw();
    }
}

// Erases all of the particle container instances and clears the particle instance vector
void _particles::clear()
{
    for (auto* i : instances) {
        delete i;
    }
    instances.clear();
}

void _particles::cull()
{
    // If we hit the limit, start deleting the oldest particle container instances
    if (instances.size() > limit) {
        delete instances.front();
        instances.pop_front();
    }
}

