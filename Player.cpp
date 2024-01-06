#include "Player.h"
#include "Core/KeyCodes.h"
#include "Core/Timestep.h"
#include "imgui.h"
#include <glm/gtc/matrix_transform.hpp>
using namespace RendererEngine;


Player::Player(){
	// smoke particles
	_smokeProps.pos = {0.f, 0.f};
	/* _smokeProps.colorBegin = {8.0f, 8.0f, 8.0f, 1.0f}; */
	_smokeProps.colorBegin = {0.8f, 0.8f, 0.8f, 1.0f};
	_smokeProps.colorEnd = {0.6f, 0.6f, 0.6f, 1.0f};
	_smokeProps.sizeBegin = 0.35f, _smokeProps.sizeEnd = 0.0f, _smokeProps.sizeVariation=0.15f;
	_smokeProps.lifetime = 4.0f;
	_smokeProps.velocity = {-2.0f, 0.0f}, _smokeProps.velocityVariation = {4.0f, 2.0f};

	// Flames
	_engineParticles.pos = {0.f, 0.f};
	/* _smokeProps.colorBegin = {8.0f, 8.0f, 8.0f, 1.0f}; */
	_engineParticles.colorBegin = {254/255.0f, 109/255.0f, 41/ 255.0f, 1.0f};
	_engineParticles.colorEnd = {254 / 255.0f, 212/255.0f, 123/255.0f, 1.0f};
	_engineParticles.sizeBegin = 0.35f, _engineParticles.sizeEnd = 0.0f, _engineParticles.sizeVariation=0.3f;
	_engineParticles.lifetime = 4.0f;
	_engineParticles.velocity = {-2.0f, 0.0f}, _engineParticles.velocityVariation = {4.0f, 2.0f};
}

void Player::loadAssets(){
	_shipTexture = Texture2D::Create("assets/textures/Ship.png");
}

void Player::onUpdate(RendererEngine::Timestep ts){
	_time += ts;

	// This is simply just a very basic way of handling physics mechanics
	// Just have velocity and gravity.
	if(InputPoll::isKeyPressed(RENDER_KEY_SPACE)){
		_velocity.y += _enginePower;

		if(_velocity.y < 0.0f){
			_velocity.y += _enginePower * 2.0f;
		}

		// Adding flames
		glm::vec2 emissionPoint = {0.0f, -0.6f};
		float rotation = glm::radians(getRotation());
		glm::vec4 rotated = glm::rotate(glm::mat4(1.0f), rotation, {0.f, 0.0f, 1.0f}) * glm::vec4(emissionPoint, 0.0f, 1.0f);
		_engineParticles.pos = _position + glm::vec2{rotated.x, rotated.y};
		_engineParticles.velocity.y = -_velocity.y * 0.2f - 0.2f;
		_particlesSystem.emit(_engineParticles);
	}
	else{
		_velocity.y -= _gravity;
	}
	
	_velocity.y = glm::clamp(_velocity.y, -20.0f, 20.0f);
	_position += _velocity * (float)ts;

	// Utilizing Particle System here
	
	if(_time > _smokeNextEmitTime){
		_smokeProps.pos = _position;
		_particlesSystem.emit(_smokeProps);
		_smokeNextEmitTime += 0.1f;
	}

	_particlesSystem.onUpdate(ts);
}

void Player::onRender(){
	Renderer2D::drawRotatedQuad({_position.x, _position.y, 0.5f}, {1.0f, 1.3f}, glm::radians(getRotation()), _shipTexture);
	_particlesSystem.onRender();
}

void Player::onImguiRender(){
	ImGui::DragFloat("Engine Power", &_enginePower, 0.1f);
	ImGui::DragFloat("Gravity", &_gravity, 0.1f);
}
void Player::reset(){
	_position = {-10.0f, 0.0f};
	_velocity = {5.0f, 0.0f};
}
