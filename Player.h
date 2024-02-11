#pragma once
#include <GameEngine/Core/Timestep.h>
#include <GameEngine/Renderer/Texture.h>
#include "ParticleSystem.h"

class Player{
public:
	Player();

	void loadAssets();

	void onUpdate(RendererEngine::Timestep ts);

	void onRender();

	void onImguiRender();

	void reset();

	float getRotation() { return velocity.y * 4.0f - 90.0f; }

	const glm::vec2& getPosition() const { return pos; }

	uint32_t getScore() const { return (uint32_t)(pos.x + 10.0f) / 10.0f; }

private:
	glm::vec2 pos = {-10.0f, 0.f};
	glm::vec2 velocity = {5.f, 0.f};
	
	float enginePower = 0.5f;
	float gravity = 0.8f;
	float time = 0.0f;
	float smokeInterval = 0.4f;
	float smokeNextEmitTime = smokeInterval;

	RendererEngine::Ref<RendererEngine::Texture2D> shipTexture;

	ParticleProps smokeProps, engineParticles; // Particle Props that is smoke, and for flames
	ParticleSystem particlesSystem;
};

