#ifndef _PARTICLES_H
#define _PARTICLES_H

#include <_common.h>

#define MAX_PARTICLES 2000

// Simple struct for a single particle
// Instead of defining a whole new class we can just define this here to be used so _particles can manage all sets of particles in a scene
struct particle {

    // Particles only need to know their position + life when created - We'll determine the rest in a different container based on what family the particle is in (e.g. explosion)
    particle() { this->life = 0; }
    virtual ~particle() { /* dtor */ }

    vec3 pos = {0, 0, 0};
    vec3 vel = {0, 0, 0};
    col3 col = {0, 0, 0};
    float life = 0;
};

//Create an abstract-ish container of particles
struct container {

    // Every particle container will need a source position
    container(vec3 pos) : pos(pos) {}
    virtual ~container() {}

    const vec3 pos = {0, 0, 0};
    const int dtScaleF = 150;    // We need an additional scale factor to offset how tiny dt is when passed in the update function

    deque<particle> particles{MAX_PARTICLES};

    virtual void init() { /* Does nothing */ }
    virtual void update(float dt) { /* Does nothing */ }
    virtual void draw() { /* Does nothing */ }
};

// Structure which represents an explosion - Basically just a specialized container for particles which will implement behavior to represent an "explosion" of particles
struct explosion : public container {

    // The passed position will be the "source" of the explosion
    explosion(vec3 pos) : container(pos) { /* Add specific functionality here if needed */ }
    virtual ~explosion() {}

    // Initialize each particle in the particle container
    virtual void init() {
        for (auto& p : particles) {
            p.pos = this->pos;
            p.life = 1.0f;

            // Set some values necessary for determining the velocity vector of the particle to mimic explosions
            float theta = (rand()/(float)RAND_MAX) * 2.0f * PI;
            float phi = (rand()/(float)RAND_MAX) * PI;
            float speed = (rand()/(float)RAND_MAX) * 0.1f + 0.05f;

            // Set each particle's velocity
            p.vel = {
                speed * sin(phi) * cos(theta),
                speed * sin(phi) * sin(theta),
                speed * cos(phi)
            };

            // Set each particle's color
            float bw = either(1.0f, 0.0f, 0.75f); // should return 1.0f 75% of the time, and 0.0f 25% of the time
            p.col = {
                bw,
                bw,
                bw
            };
        }
    }

    // Update each particle's position based on it's velocity as set in the init() function
    virtual void update(float dt){
        for (auto& p : particles) {
            if (p.life > 0.0f) {
                p.pos.x += p.vel.x * dt * dtScaleF;
                p.pos.y += p.vel.y * dt * dtScaleF;
                p.pos.z += p.vel.z * dt * dtScaleF;
            }
            // Decrement the life of the particle so it eventually dies
            p.life -= 0.003f;
        }
    }

    // Draw all of the particles in the particle container
    virtual void draw(){
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        for (auto& p : particles) {
            // Set the point size to change how big each particle is, then set the drawing mode and draw each particle with it's own properties
            glPointSize(3);
            glBegin(GL_POINTS);
            if(p.life > 0) {
                glColor4f(p.col.r, p.col.g, p.col.b, p.life);
                glVertex3f(p.pos.x, p.pos.y, p.pos.z);
            }
            glEnd();
        }
        glEnable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
    }
};

// List available types of particle containers here (I'm only implementing explosions for now)
enum PType {EXPLOSION}; // We could also add like smoke or gibs or whatever other type of particles, so that we can uniquely determine particle behavior based on container type

class _particles
{
    public:
        _particles();
        virtual ~_particles();

        int limit = 10; // This is the maximum number of particle container instances that are allowed to exist at the same time

        deque<container*> instances;

        void createParticles(PType type, vec3 pos);
        void update(float dt);
        void draw();

        void clear();
        void cull();


    protected:

    private:
};

#endif // _PARTICLES_H
