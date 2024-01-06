#include "GameLayer.h"
#include <GameEngine/Core/EntryPoint.h>

/*
 *
 * One Hour Game
 * Building a basic flappy bird Game
 *
 * 1.) Physics simulator system
 * 2.) Particle system
 * 3.) Basic Collision Detection
 *	- Detection of collision with triangles and rectangles.
 *	- Basically we can check intersection for inside the triangle.
 * 4.) Post-processing effects
 * 5.) Camera following player
 * 6.) UI
 * 7.) Audio/Audio processing
 *
 * Things that wont do
 * - No Audio
 * 
 *
 *
 *
*/

class GameApplication : public RendererEngine::Application{
public:
	GameApplication() : Application(){
		pushLayer(new GameLayer());
	}

	~GameApplication(){}

private:
};


RendererEngine::Application* RendererEngine::CreateApplication(){
	return new GameApplication();
}



