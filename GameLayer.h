#pragma once
#include <GameEngine/Core/GameEngine.h>
#include "Events/MouseEvent.h"
#include "Level.h"
#include "imgui.h"


class GameLayer : public RendererEngine::Layer{
public:
	GameLayer();
	virtual void onAttach() override;
	virtual void onDetach() override;

	virtual void onUpdate(RendererEngine::Timestep ts) override;

	virtual void onImguiRender() override;

	virtual void onEvent(RendererEngine::Event& event) override;
	
	bool onMouseButtonPressed(RendererEngine::MouseButtonPressedEvent& event);
	bool onWindowResize(RendererEngine::WindowResizeEvent& event);
	
private:
	
	// Creating the Camera
	void createCamera(uint32_t w, uint32_t h);


private:
	RendererEngine::Scope<RendererEngine::OrthographicCamera> _camera;
	Level _level;
	ImFont* _font;


	// Different game states
	
	enum class GameState{
		Play = 0, MainMenu = 1, GameOver = 2
	};

	GameState state = GameState::MainMenu;
	float _time = 0.0f;
	bool _blink = false;
};
