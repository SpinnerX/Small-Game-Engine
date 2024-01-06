#include "Level.h"
#include "Core/Timestep.h"
#include "Color.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/Texture.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Random.h"
#include "imgui.h"
using namespace RendererEngine;

// HSV to RGB gives us that glowing effect on our triangles
static glm::vec4 HSVtoRGB(const glm::vec3& hsv) {
	int H = (int)(hsv.x * 360.0f);
	double S = hsv.y;
	double V = hsv.z;

	double C = S * V;
	double X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
	double m = V - C;
	double Rs, Gs, Bs;

	if (H >= 0 && H < 60) {
		Rs = C;
		Gs = X;
		Bs = 0;
	}
	else if (H >= 60 && H < 120) {
		Rs = X;
		Gs = C;
		Bs = 0;
	}
	else if (H >= 120 && H < 180) {
		Rs = 0;
		Gs = C;
		Bs = X;
	}
	else if (H >= 180 && H < 240) {
		Rs = 0;
		Gs = X;
		Bs = C;
	}
	else if (H >= 240 && H < 300) {
		Rs = X;
		Gs = 0;
		Bs = C;
	}
	else {
		Rs = C;
		Gs = 0;
		Bs = X;
	}

	return { (Rs + m), (Gs + m), (Bs + m), 1.0f };
}

static bool PointInTri(const glm::vec2& p, glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2){
	float s = p0.y * p2.x - p0.x * p2.y + (p2.y - p0.y) * p.x + (p0.x - p2.x) * p.y;
	float t = p0.x * p1.y - p0.y * p1.x + (p0.y - p1.y) * p.x + (p1.x - p0.x) * p.y;

	if ((s < 0) != (t < 0))
		return false;

	float A = -p1.y * p2.x + p0.y * (p2.x - p1.x) + p0.x * (p1.y - p2.y) + p1.x * p2.y;

	return A < 0 ?
		(s <= 0 && s + t >= A) :
		(s >= 0 && s + t <= A);
}

Level::Level(){

}

void Level::init(){
	_triangleTexture = Texture2D::Create("assets/textures/Triangle.png");
	_player.loadAssets();

	_pillars.resize(5);
	for(int i = 0; i < 5; i++){
		CreatePillar(i, i * 10.0f);
	}
}

void Level::onUpdate(RendererEngine::Timestep ts){
	_player.onUpdate(ts);

	if(collisionTest()){
		gameOver();
		return;
	}

	_pillarHSV.x += 0.1f * ts;
	if(_pillarHSV.x > 1.0f){
		_pillarHSV.x = 0.0f;
	}


	if(_player.getPosition().x > _pillarTarget){
		CreatePillar(_pillarIndex, _pillarTarget + 20.0f);
		_pillarIndex = ++_pillarIndex % _pillars.size();
		_pillarTarget += 10.0f;
	}
}

void Level::onRender(){
	const auto& playerPos = _player.getPosition();
	glm::vec4 color = HSVtoRGB(_pillarHSV);
	
	// Draw background
	Renderer2D::drawQuad({ playerPos.x, 0.0f, -0.8f }, { 50.0f, 50.0f }, { 0.3f, 0.3f, 0.3f, 1.0f });

	// Floor and Ceiling
	Renderer2D::drawQuad({playerPos.x, 34.0f}, {50.0f, 50.0f}, color);
	Renderer2D::drawQuad({playerPos.x, -34.0f}, {50.0f, 50.0f}, color);
	
	// Rendering the pillars
	for(auto& p : _pillars){
		Renderer2D::drawRotatedQuad(p.topPosition, p.topScale, glm::radians(180.0f), _triangleTexture, 1.0f, color);
		Renderer2D::drawQuad(p.bottomPosition, p.bottomScale, _triangleTexture, 1.0, color);
	}

	_player.onRender();
	_points.clear();
}

void Level::onImguiRender(){
	_player.onImguiRender();
}

void Level::CreatePillar(int index, float offset){
	Pillar& pillar = _pillars[index];
	pillar.topPosition.x = offset;
	pillar.bottomPosition.x = offset;

	// Prevent issues in how the pillars distance from each other and rendering. When being created.
	pillar.topPosition.z = index * 0.1f - 0.5f;
	pillar.bottomPosition.z = index * 0.1f - 0.5f + 0.5f;
	
	float center = Random::Float() * 35.0f - 17.5f;
	float gap = 2.0f + Random::Float() * 5.0f;

	pillar.topPosition.y = 10.0f - ((10.0f - center) * 0.2f) + gap * 0.5f;
	pillar.bottomPosition.y = -10.0f - ((-10.0f - center) * 0.2f) - gap * 0.5f;
}

bool Level::collisionTest(){
	
	glm::vec4 playerVertices[4] = {
		{-0.5f, -0.5f, 0.0f, 1.0f},
		{0.5f, -0.5f, 0.0f, 1.0f},
		{0.5f,  0.5f, 0.0f, 1.0f},
		{-0.5f,  0.5f, 0.0f, 1.0f}
	};
	
	auto& pos = _player.getPosition();
	glm::vec4 playerTransformedVertices[4];
	
	for(int i = 0; i < 4; i++){
		playerTransformedVertices[i] = glm::translate(glm::mat4(1.0f), {pos.x, pos.y, 0.0f}) 
									   * glm::rotate(glm::mat4(1.0f), glm::radians(_player.getRotation()), {0.0f, 0.0f, 1.0f})
									   * glm::scale(glm::mat4(1.0f), {1.0f, 1.3f, 1.0f}) 
									   * playerVertices[i];
	
	}
	
	// This is to match the triangles texture image (where each corner is 10% from the texture edge)
	glm::vec4 pillarVertices[4] = {
		{-0.5f + 0.1f, -0.5f + 0.1f, 0.0f, 1.0f},
		{0.5f - 0.1f, -0.5f + 0.1f, 0.0f, 1.0f},
		{0.5 + 0.0f, 0.5f - 0.1f, 0.0f, 1.0f},
	};
	
	for(auto& p : _pillars){
		glm::vec2 tri[3];

		// Top pillars
		for(int i = 0; i < 3; i++){
			tri[i] = glm::translate(glm::mat4(1.0f), {p.topPosition.x, p.topPosition.y, 0.0f}) 
									   * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), {0.0f, 0.0f, 1.0f})
									   * glm::scale(glm::mat4(1.0f), {p.topScale.x, p.topScale.y, 1.0f})
									   * pillarVertices[i];	
		}

		for(auto& vert : playerTransformedVertices){
			if(PointInTri({vert.x, vert.y}, tri[0], tri[1], tri[2])){
				return true;
			}
		}
		
		// Bottom Pillars
		for(int i = 0; i < 3; i++){
			tri[i] = glm::translate(glm::mat4(1.0f), {p.bottomPosition.x, p.bottomPosition.y, 0.0f}) 
									   * glm::scale(glm::mat4(1.0f), {p.bottomScale.x, p.bottomScale.y, 1.0f})
									   * pillarVertices[i];
		}

		for(auto& vert : playerTransformedVertices){
			if(PointInTri({vert.x, vert.y}, tri[0], tri[1], tri[2])){
				return true;
			}
		}
	}
	return false;
}

void Level::gameOver(){
	_player.reset();
	_pillarTarget = 30.0f;
	_pillarIndex = 0.0f;

	for(int i = 0; i < 5; i++){
		CreatePillar(i, i * 10.0f);
	}
}
