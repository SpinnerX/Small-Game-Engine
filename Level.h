#pragma once
#include <glm/glm.hpp>
#include "Player.h"
#include <vector>

struct Pillar{

	glm::vec3 topPosition = {0.f, 10.0f, 0.f};
	glm::vec2 topScale = {5.f, 15.0f};
	
	glm::vec3 bottomPosition = {15.0f, 10.0f, 0.0f};
	glm::vec2 bottomScale = {15.0f, 15.0f};
};

class Level{
public:
	Level();
		
	void init();

	void onUpdate(RendererEngine::Timestep ts);

	void onRender();

	void onImguiRender();

	Player& getPlayer() { return player; }

private:
	void CreatePillar(int index, float offset);
	bool collisionTest();
	void gameOver();

private:
	Player player;
	std::vector<Pillar> pillars;
	std::vector<glm::vec2> points;
	float pillarTarget = 30.0f;
	int pillarIndex = 0;
	glm::vec3 pillarHSV = {0.0f, 0.8f, 0.8f};
	RendererEngine::Ref<RendererEngine::Texture2D> triangleTexture;
};
