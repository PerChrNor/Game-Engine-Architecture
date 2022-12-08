#include "particlesystem.h"
#include "resourcemanager.h"
#include "gltypes.h"

ParticleSystem::ParticleSystem()
{
    mParticlePool.resize(1000);
}
ParticleSystem::~ParticleSystem()
{

}

void ParticleSystem::ParticleUpdate(float mTime)
{
    for (auto &mParticle : mParticlePool) //goes through particle pool
    {
        if (!mParticle.Active) //continues if inactive
        {
            continue;
        }
        if (mParticle.mLifeRemaining <= 0.0f) //if particle is dead, set active to false
        {
            mParticle.Active = false;
            continue;
        }
        mParticle.mLifeRemaining -= mTime; //subtract life remaining from timer
        mParticle.mPos += mParticle.mVelocity * mTime; //increase posision by velocity * timer
        mParticle.mRotation += 0.01f * mTime; //increase rotation
    }
}

void ParticleSystem::Emitter()
{
    particleComponent &mParticle = mParticlePool[mPoolIndex]; //ref to particle
    mParticle.Active = true; //set to true
    mParticle.mPos = mEmitter.mPos; //set posision
    srand(time(NULL));
    float rng = rand()%2+0;//randomized float
    mParticle.mRotation = rng; //randomize rotation

    //Velocity
    mParticle.mVelocity = mEmitter.mVelocity; //set velocity
    mParticle.mVelocity.setX(mEmitter.mVelocityAlt.x() *rng-0.5f); //set x,
    mParticle.mVelocity.setY(mEmitter.mVelocityAlt.y() *rng-0.5f); //set y,
    mParticle.mVelocity.setZ(mEmitter.mVelocityAlt.z() *rng-0.5f); //set z,

    mParticle.mLifeSpan = mEmitter.mLifeSpan; //sett lifespan
    mParticle.mLifeRemaining = mEmitter.mLifeSpan; //set remaining lifespan
    mParticle.mSizeBegin = mEmitter.mSizeBegin + mEmitter.mSizeAlt *rng-0.5f; //set start size
    mParticle.mSizeEnd = mEmitter.mSizeEnd; //set final size

    mPoolIndex = --mPoolIndex % mParticlePool.size(); //set up poolindex for next particle
}


void ParticleSystem::init()
{
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadvertices), quadvertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(float), 0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof( float ),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    //Second buffer - holds the indices:
    glGenBuffers(1, &mEAB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEAB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadindicies), quadindicies, GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void ParticleSystem::draw()
{
    glBindVertexArray( mVAO );
    ResourceManager::getInstance().mShaderProgram[PARTICLESHADER]->use(mTransformationMatrix, nullptr);
    glDrawElements(GL_TRIANGLES, sizeof(quadindicies), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
