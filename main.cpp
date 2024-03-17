#include "GameLayer.h"
#include <Engine3D/Core/Application.h>
#include <Engine3D/Core/EntryPoint.h>

class Game : public  Engine3D::Application{
public:
	Game(Engine3D::ApplicationCommandLineArgs args) : Application("Game", args) {
		pushLayer(new GameLayer());
	}
};


Engine3D::Application* Engine3D::CreateApplication(ApplicationCommandLineArgs args){
	return new Game(args);
}

