#pragma once
#include "ParticleSystem.h"
#include <GameEngine/Core/GameEngine.h>

class Player{
public:
	Player();
	
	void loadAssets();
	
	void onUpdate(RendererEngine::Timestep ts);
	void onRender();

	void onImguiRender();

	void reset();

	float getRotation() { return _velocity.y * 4.0f - 90.0f; }
	const glm::vec2& getPosition() const { return _position; }

	uint32_t getScore() const { return (uint32_t)(_position.x + 10.0f) / 10.0f; }

private:
	glm::vec2 _position = {-10.f, 0.f}; // player position
	glm::vec2 _velocity = {5.f, 0.f};
	
	float _enginePower = 0.5f;
	float _gravity = 0.8f;
	float _time = 0.0f;
	float _smokeInterval = 0.4f;
	float _smokeNextEmitTime = _smokeInterval;

	RendererEngine::Ref<RendererEngine::Texture2D> _shipTexture;

	ParticleProps _smokeProps, _engineParticles; // Particle Props that is smoke, and for flames
	ParticleSystem _particlesSystem;
};
