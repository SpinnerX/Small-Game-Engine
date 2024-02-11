#pragma once
#include <GameEngine/Core/Layer.h>
#include "Level.h"
#include <imgui/imgui.h>

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
	void createCamera(uint32_t w, uint32_t h);

private:
	RendererEngine::Scope<RendererEngine::OrthographicCamera> camera;
	Level level;
	ImFont* font;

	enum class GameState {
		Play=0,
		MainMenu=1,
		GameOver=2
	};

	GameState state = GameState::MainMenu;
	float time = 0.0f;
	float blink = false;
};
