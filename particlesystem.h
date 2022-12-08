#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <QOpenGLFunctions_4_1_Core>
#include "components.h"
#include "Shaders/particleshader.h"
#include "camera.h"
#include "constants.h"
#include "visualobject.h"
#include <memory>
///
///Code writen by candidate: 8003
///
class ParticleShader;
struct ParticleEmitter;
class ParticleSystem : public VisualObject
{
public:
    ///
    /// \brief ParticleSystem, the constructor resizes the particle pool to be 1000.
    ///
    ParticleSystem();
    ~ParticleSystem();
    ///
    /// \brief ParticleUpdate, updates the particles every frame.
    /// It goes through particle pool and sees if a particle is active or not,
    /// if not active: continue.
    /// if particle is dead, set active to false.
    /// Subtract life remaining from mTime.
    /// Increase posision by velocity * mTime
    /// Increase rotation by 0.01f * mTime
    /// \param mTime, suppose to be a timer variable, but isn't at the moment.
    ///
    void ParticleUpdate(float mTime);
    ///
    /// \brief Emitter, Emits 1 particle by using the ParticleEmitter component.
    /// It reuses an existing particle from the memory.
    /// finds next index from the particlepool and makes a refrence to a particle.
    /// make that particle active and sets posision, rotation, velocity, size, and lifespan.
    ///
    void Emitter();
    ///
    /// \brief init function, initialze the quadvertices and particlesystem.
    ///
   void init();
   ///
   /// \brief draw function, draws the particles and shader.
   ///
   void draw();


   particleComponent mParticle;
   ParticleEmitter mEmitter;
    
private:   

   ///
   /// \brief particleShader, shared pointer to particleshader class.
   ///
    std::shared_ptr<ParticleShader> particleShader;
    ///
    /// \brief mParticlePool, creates a pool of particles from the particle component.
    /// So that we can set it aside and reuse the array
    ///
    std::vector<particleComponent> mParticlePool;

    uint32_t mPoolIndex{999}; //pool of next active particle index

    static constexpr float quadvertices[] //large array to hold vertices for the particles
    {
        // Position
        -1.f, -1.f, 0.f,
         1.f, -1.f, 0.f,
         1.f,  1.f, 0.f,
        -1.f,  1.f, 0.f
    };

    static constexpr float quadindicies[] //large array to hold indicies for the particles
    {
        0, 1, 2, 2, 3, 0
    };
};

#endif // PARTICLESYSTEM_H
