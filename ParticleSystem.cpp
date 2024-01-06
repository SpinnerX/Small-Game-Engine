#include "ParticleSystem.h"
#include "Renderer/Renderer2D.h"
#include "Random.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

ParticleSystem::ParticleSystem(){
	_particlesPool.resize(1000);
}

void ParticleSystem::emit(const ParticleProps& particleProps){
	Particle& _particle = _particlesPool[_particlePoolIndex];
	_particle.isActive = true;
	_particle.pos = particleProps.pos;
	_particle.rotation = Random::Float() * 2.0f * glm::pi<float>();


	// velocity
	_particle.velocity = particleProps.velocity;
	_particle.velocity.x += particleProps.velocity.x * (Random::Float() - 0.5f);
	_particle.velocity.y += particleProps.velocity.y * (Random::Float() - 0.5f);

	// colors
	_particle.colorBegin = particleProps.colorBegin;
	_particle.colorEnd = particleProps.colorEnd;

	_particle.lifeTime = particleProps.lifetime;
	_particle.lifeRemain = particleProps.lifetime;

	_particle.sizeBegin = particleProps.sizeBegin + particleProps.sizeVariation * (Random::Float() * 0.5f);
	_particle.sizeEnd = particleProps.sizeEnd;


	_particlePoolIndex = ++_particlePoolIndex % _particlesPool.size();
}

void ParticleSystem::onUpdate(RendererEngine::Timestep ts){
	for(auto& particle : _particlesPool){
		if(!particle.isActive) continue;
		
		
		if(particle.lifeRemain <= 0.0f){
			particle.isActive = false;
			continue;
		}

		if(particle.lifeRemain <= 0.0f){
			particle.isActive = false;
			continue;
		}

		particle.lifeRemain -= ts;
		particle.pos += particle.velocity * (float)ts;
		particle.rotation += 0.01f  * ts;
	}
}

void ParticleSystem::onRender(){
	for(auto& particle : _particlesPool){
		if(!particle.isActive) continue;

		float life = particle.lifeRemain / particle.lifeTime;
		glm::vec4 color = glm::lerp(particle.colorEnd, particle.colorBegin, life);
		color.a = color.a * life;

		float size = glm::lerp(particle.sizeEnd, particle.sizeBegin, life);
		RendererEngine::Renderer2D::drawRotatedQuad(particle.pos, {size, size}, particle.rotation, color);
	}
}

