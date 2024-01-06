#include "GameLayer.h"
#include "Core/Application.h"
#include "Core/core.h"
#include "Events/ApplicationEvent.h"
#include "Events/Event.h"
#include "Events/MouseEvent.h"
#include "Random.h"
#include "Renderer/OrthographicCamera.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer2D.h"
#include "imgui.h"
#include <GameEngine/Core/core.h>
using namespace RendererEngine;

GameLayer::GameLayer() : Layer("Game Layer") {
	auto& window = Application::Get().GetWindow();
	createCamera(window.getWidth(), window.getHeight());
	Random::init();
}

void GameLayer::onAttach(){
	_level.init();
	ImGuiIO io = ImGui::GetIO();
	_font = io.Fonts->AddFontFromFileTTF("assets/OneHourGameAssets/OpenSans-Regular.ttf", 120.0f);
}

void GameLayer::onDetach(){}

void GameLayer::onUpdate(RendererEngine::Timestep ts) {
	_time += ts;

	if((int)(_time * 10.0f) % 8 > 4)
		_blink = !_blink;

	switch (state) {
		case GameState::Play:
			_level.onUpdate(ts);
			const auto& playerPos = _level.getPlayer().getPosition();
			_camera->setPosition({playerPos.x, playerPos.y, 0.f}); // basically what allows us to move the camera direction along with player.
			break;
	}

	RendererCommand::setClearColor({ 0.2, 0.2f, 0.2f, 1 }); // Essentially how to set the background
	RendererCommand::clear();
	
	Renderer2D::beginScene(*_camera);
	_level.onRender();
	Renderer2D::endScene();
}

void GameLayer::onImguiRender(){
	// UI Stuff
	switch (state) {
		case GameState::Play:
			{
				uint32_t score = _level.getPlayer().getScore();
				std::string scoreStr = std::string("Score: ") + std::to_string(score);
				/* ImGui::GetForegroundDrawList()->AddText(ImGui::GetWindowPos(), 0xffffffff, scoreStr.c_str()); */
				ImGui::GetForegroundDrawList()->AddText(_font, 48.0f, ImGui::GetWindowPos(), 0xffffffff, scoreStr.c_str());
			}
			break;
		case GameState::MainMenu:
			{
				auto pos = ImGui::GetWindowPos();
				auto width = Application::Get().GetWindow().getWidth();
				auto height = Application::Get().GetWindow().getHeight();
				pos.x += width * 0.5f - 300.0f;
				pos.y += 50.0f;
				if(_blink)
					ImGui::GetForegroundDrawList()->AddText(_font, 120.0f, pos, 0xffffffff, "Click to Play!");
				}
			break;
		case GameState::GameOver:
			break;
	}

}

void GameLayer::onEvent(RendererEngine::Event& event){
	RendererEngine::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<RendererEngine::WindowResizeEvent>(bind_function(this, &GameLayer::onWindowResize));
	dispatcher.Dispatch<RendererEngine::MouseButtonPressedEvent>(bind_function(this, &GameLayer::onMouseButtonPressed));
}

bool GameLayer::onMouseButtonPressed(MouseButtonPressedEvent& event){
	state = GameState::Play;
	return false;	
}

bool GameLayer::onWindowResize(RendererEngine::WindowResizeEvent& event){
	createCamera(event.GetWidth(), event.GetHeight());
	return false;
}

void GameLayer::createCamera(uint32_t w, uint32_t h){
	coreLogInfo("Created Camera!");
	float aspectRatio = (float)w / (float)h; // Create aspect ratio so the camera doesn't stretch
	float cameraWidth = 8.0f;
	float bottom = -cameraWidth;
	float top  = cameraWidth;
	float left = bottom * aspectRatio;
	float right = top * aspectRatio;

	_camera = CreateScope<RendererEngine::OrthographicCamera>(left, right, bottom, top);
}

