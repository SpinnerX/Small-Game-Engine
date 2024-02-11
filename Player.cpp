#include "Player.h"
#include <imgui/imgui.h>
#include <GameEngine/Renderer/Renderer2D.h>
#include <glm/gtc/matrix_transform.hpp>
using namespace RendererEngine;

Player::Player(){
	// smoke particles
	smokeProps.pos = {0.f, 0.f};
	/* _smokeProps.colorBegin = {8.0f, 8.0f, 8.0f, 1.0f}; */
	smokeProps.colorBegin = {0.8f, 0.8f, 0.8f, 1.0f};
	smokeProps.colorEnd = {0.6f, 0.6f, 0.6f, 1.0f};
	smokeProps.sizeBegin = 0.35f, smokeProps.sizeEnd = 0.0f, smokeProps.sizeVariation=0.15f;
	smokeProps.lifetime = 4.0f;
	smokeProps.velocity = {-2.0f, 0.0f}, smokeProps.velocityVariation = {4.0f, 2.0f};

	// Flames
	engineParticles.pos = {0.f, 0.f};
	/* _smokeProps.colorBegin = {8.0f, 8.0f, 8.0f, 1.0f}; */
	engineParticles.colorBegin = {254/255.0f, 109/255.0f, 41/ 255.0f, 1.0f};
	engineParticles.colorEnd = {254 / 255.0f, 212/255.0f, 123/255.0f, 1.0f};
	engineParticles.sizeBegin = 0.35f, engineParticles.sizeEnd = 0.0f, engineParticles.sizeVariation=0.3f;
	engineParticles.lifetime = 4.0f;
	engineParticles.velocity = {-2.0f, 0.0f}, engineParticles.velocityVariation = {4.0f, 2.0f};
}

void Player::loadAssets(){
	shipTexture = Texture2D::Create("assets/textures/Ship.png");
}

void Player::onUpdate(RendererEngine::Timestep ts){
	time += ts;

	// This is simply just a very basic way of handling physics mechanics
	// Just have velocity and gravity.
	if(InputPoll::isKeyPressed(RENDER_KEY_SPACE)){
		velocity.y += enginePower;

		if(velocity.y < 0.0f){
			velocity.y += enginePower * 2.0f;
		}

		// Adding flames
		glm::vec2 emissionPoint = {0.0f, -0.6f};
		float rotation = glm::radians(getRotation());
		glm::vec4 rotated = glm::rotate(glm::mat4(1.0f), rotation, {0.f, 0.0f, 1.0f}) * glm::vec4(emissionPoint, 0.0f, 1.0f);
		engineParticles.pos = pos + glm::vec2{rotated.x, rotated.y};
		engineParticles.velocity.y = -velocity.y * 0.2f - 0.2f;
		particlesSystem.emit(engineParticles);
	}
	else{
		velocity.y -= gravity;
	}
	
	velocity.y = glm::clamp(velocity.y, -20.0f, 20.0f);
	pos += velocity * (float)ts;


	// Utilizing Particle System here
	
	if(time > smokeNextEmitTime){
		smokeProps.pos = pos;
		particlesSystem.emit(smokeProps);
		smokeNextEmitTime += 0.1f;
	}

	particlesSystem.onUpdate(ts);
}

void Player::onRender(){
	RendererEngine::Renderer2D::drawRotatedQuad({pos.x, pos.y, 0.5f}, {1.0f, 1.3f}, glm::radians(getRotation()), shipTexture);
	particlesSystem.onRender();
}

void Player::onImguiRender(){
	ImGui::DragFloat("Engine Power", &enginePower, 0.1f);
	ImGui::DragFloat("Gravity", &gravity, 0.1f);
}

void Player::reset(){
	pos = {-10.0f, 0.0f};
	velocity = {5.0f, 0.0f};
}
