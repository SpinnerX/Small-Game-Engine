#pragma once
#include <GameEngine/Core/GameEngine.h>

struct ParticleProps{
	glm::vec2 pos;
	glm::vec2 velocity, velocityVariation;
	glm::vec4 colorBegin, colorEnd;
	float sizeBegin, sizeEnd, sizeVariation;
	float lifetime = 1.0f;
};

class ParticleSystem{
public:
	ParticleSystem();

	void emit(const ParticleProps& particleProps);

	void onUpdate(RendererEngine::Timestep ts);

	void onRender();

private:
	struct Particle{
		glm::vec2 pos;
		glm::vec2 velocity;
		glm::vec4 colorBegin, colorEnd;
		float rotation = 0.0f;
		float sizeBegin, sizeEnd;

		float lifeTime = 1.0f;
		float lifeRemain = 0.0f;
		bool isActive = false;
	};

	std::vector<Particle> _particlesPool;
	uint32_t _particlePoolIndex = 0;
};
