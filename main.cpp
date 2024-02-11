#include "GameLayer.h"
#include <GameEngine/Core/Application.h>
#include <GameEngine/Core/EntryPoint.h>

class Game : public  RendererEngine::Application{
public:
	Game(RendererEngine::ApplicationCommandLineArgs args) : Application("Game", args) {
		pushLayer(new GameLayer());
	}
};


RendererEngine::Application* RendererEngine::CreateApplication(ApplicationCommandLineArgs args){
	return new Game(args);
}

