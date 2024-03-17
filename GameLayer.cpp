#include "GameLayer.h"
#include <Engine3D/Core/Application.h>
#include <Engine3D/Renderer2D/RenderCommand.h>
#include <Engine3D/Renderer2D/Renderer2D.h>
#include "Random.h"
using namespace Engine3D;

GameLayer::GameLayer() : Layer("Game Layer"){
	auto& window = Application::Get().GetWindow();
	createCamera(window.GetWidth(), window.GetHeight());
	Random::init();
}

void GameLayer::onAttach(){
	level.init();
	ImGuiIO io = ImGui::GetIO();
	font = io.Fonts->AddFontFromFileTTF("assets/OpenSans-Regular.ttf", 120.0f);
}

void GameLayer::onDetach(){}

void GameLayer::onUpdate(Timestep ts){
	time += ts;

	if((int)(time * 10.0f) % 8 > 4)
		blink = !blink;

	if(InputPoll::isKeyPressed(KeyCode::Escape))
		Application::Get().close();
	else if(InputPoll::isKeyPressed(KeyCode::A))
		state = GameState::GameOver;

	switch (state) {
		case GameState::MainMenu:
			break;
		case GameState::GameOver:
			Application::Get().close();
			break;
		case GameState::Play:
			level.onUpdate(ts);
			const auto& playerPos = level.getPlayer().getPosition();
			camera->setPosition({playerPos.x, playerPos.y, 0.f}); // basically what allows us to move the camera direction along with player.
			break;
	}

	RendererCommand::setClearColor({ 0.2, 0.2f, 0.2f, 1 }); // Essentially how to set the background
	RendererCommand::clear();
	
	Renderer2D::beginScene(*camera);
	level.onRender();
	Renderer2D::endScene();
}


void GameLayer::onImguiRender(){
	// UI Stuff
	switch (state) {
		case GameState::Play:
			{
				uint32_t score = level.getPlayer().getScore();
				std::string scoreStr = std::string("Score: ") + std::to_string(score);
				/* ImGui::GetForegroundDrawList()->AddText(ImGui::GetWindowPos(), 0xffffffff, scoreStr.c_str()); */
				ImGui::GetForegroundDrawList()->AddText(font, 48.0f, ImGui::GetWindowPos(), 0xffffffff, scoreStr.c_str());
			}
			break;
		case GameState::MainMenu:
			{
				auto pos = ImGui::GetWindowPos();
				auto width = Application::Get().GetWindow().GetWidth();
				auto height = Application::Get().GetWindow().GetHeight();
				pos.x += width * 0.5f - 300.0f;
				pos.y += 50.0f;
				if(blink)
					ImGui::GetForegroundDrawList()->AddText(font, 120.0f, pos, 0xffffffff, "Click to Play!");
				}
			break;
		case GameState::GameOver:
			break;
	}
}


void GameLayer::onEvent(Event& event){
	Engine3D::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Engine3D::WindowResizeEvent>(bind_function(this, &GameLayer::onWindowResize));
	dispatcher.Dispatch<Engine3D::MouseButtonPressedEvent>(bind_function(this, &GameLayer::onMouseButtonPressed));
}

bool GameLayer::onMouseButtonPressed(MouseButtonPressedEvent& event){
	state = GameState::Play;
	return false;
}

bool GameLayer::onWindowResize(WindowResizeEvent& event){
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

	camera = CreateScope<Engine3D::OrthographicCamera>(left, right, bottom, top);
}
