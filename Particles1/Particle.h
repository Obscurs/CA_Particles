#pragma once
#include <glm\glm.hpp>
#include "Plane.h"
#include "Triangle.h"
#include "Sphere.h"

//#define PARTICLE_SYS_ENABLED
#define HAIR_ENABLED
const float SPACING = 0.5;
class Particle
{
public:
#if defined HAIR_ENABLED
	const float SK_STRUCTURAL = 100;
	const float SD_STRUCTURAL = 30;
	const float SK_SHEAR = 30;
	const float SD_SHEAR = 10;
	const float SK_FLEX = 30;
	const float SD_FLEX = 10;

	float totalTime = 0;
#endif
	const float GRAVITY = -9.8f;
	enum class UpdateMethod : std::int8_t { EulerOrig, EulerSemi, Verlet };

	Particle();
	Particle(const float& x, const float& y, const float& z);
	void Init();
	//Particle(glm::vec3 pos, glm::vec3 vel, float bouncing = 1.0f, bool fixed = false, int lifetime = -1, glm::vec3 force = glm::vec3(0, 0, 0));
	~Particle();
	//setters
	void setPosition(const float& x, const float& y, const float& z);
	void setPosition(glm::vec3 pos);
	void setPreviousPosition(const float& x, const float& y, const float& z);
	void setPreviousPosition(glm::vec3 pos);
	void setVelocity(const float& x, const float& y, const float& z);
	void setVelocity(glm::vec3 vel);
	void setForce(const float& x, const float& y, const float& z);
	void setForce(glm::vec3 force);
	void setBouncing(float bouncing);
	void setLifetime(float lifetime);
	void setFixed(bool fixed);
	void ProcessKeyboard(int dir, float dt);
#if defined HAIR_ENABLED
	void setP1u(Particle* p);
	void setP2d(Particle* p);
	void setP3l(Particle* p);
	void setP4r(Particle* p);

	void setBendu(Particle* p);
	void setBendd(Particle* p);
	void setBendl(Particle* p);
	void setBendr(Particle* p);

	void setShear_ul(Particle* p);
	void setShear_ur(Particle* p);
	void setShear_dl(Particle* p);
	void setShear_dr(Particle* p);

#endif

	//getters
	glm::vec3 getCurrentPosition();
	glm::vec3 getPreviousPosition();
	glm::vec3 getForce();
	glm::vec3 getVelocity();
	float getBouncing();
	float getLifetime();
	bool isFixed();

	//other
	void addForce(glm::vec3 force);
	void addForce(const float& x, const float& y, const float& z);
	void updateParticle(const float& dt, UpdateMethod method = UpdateMethod::EulerOrig);
	bool collisionParticlePlane(Plane &p);
	void correctCollisionParticlePlain(Plane &p);
	bool collisionParticleTriangle(Triangle& p);
	void correctCollisionParticleTriangle(Triangle& p);
	bool collisionParticleSphere(Sphere& p);
	void correctCollisionParticleSphere(Sphere& p);
	void draw(Shader* ourShader, bool wireframed);
	glm::vec3 m_velocity;

private:
	void updateForce(const float& dt);
	glm::vec3 m_currentPosition;
	glm::vec3 m_previousPosition;
	glm::vec3 m_previousPreviousPosition;
	glm::vec3 m_force;
	glm::vec3 m_correctedPrev;
	bool m_hasColided;
	bool m_firstVerlet;
	float m_mass;
	Sphere m_sphere;
	float m_bouncing;
	float m_lifetime;
	bool  m_fixed;



#if defined HAIR_ENABLED
	Particle* m_par1u;
	Particle* m_par2d;
	Particle* m_par3l;
	Particle* m_par4r;

	Particle* m_bendu;
	Particle* m_bendd;
	Particle* m_bendl;
	Particle* m_bendr;

	Particle* m_shear_ul;
	Particle* m_shear_ur;
	Particle* m_shear_dl;
	Particle* m_shear_dr;
#endif
};

