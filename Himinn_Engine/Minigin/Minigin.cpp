#include "MiniginPCH.h"
#include "Minigin.h"
#include <chrono>
#include <thread>
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <SDL.h>
#include "GameObject.h"
#include "Scene.h"

#include "Time.h"
#include "TextComponent.h"
#include "ImageComponent.h"
#include "FPSComponent.h"

using namespace std;
using namespace std::chrono;

void Himinn::Minigin::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_Window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (m_Window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(m_Window);
}

/**
 * Code constructing the scene world starts here
 */
void Himinn::Minigin::LoadGame() const
{
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");

	auto imgComp = make_shared<ImageComponent>("background.jpg");
	auto go = std::make_shared<GameObject>();
	go->AddComponent(imgComp);
	scene.Add(go);

	imgComp = make_shared<ImageComponent>("logo.png");
	go = std::make_shared<GameObject>();
	go->SetPosition(216, 180);
	go->AddComponent(imgComp);
	scene.Add(go);

	auto color = SDL_Color{ 0, 255, 0 };
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	auto FPSComp = make_shared<FPSComponent>(font, color);
	go = std::make_shared<GameObject>();
	go->SetPosition(0, 0);
	go->AddComponent(FPSComp);
	scene.Add(go);

	color = SDL_Color{ 255, 0, 0 };
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto txtComp = make_shared<TextComponent>("Programming 4 Assignment", font, color);
	go = std::make_shared<GameObject>();
	go->SetPosition(80, 20);
	go->AddComponent(txtComp);
	scene.Add(go);
}

void Himinn::Minigin::Cleanup()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
}

void Himinn::Minigin::Run()
{
	Initialize();

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");

	LoadGame();

	{
		auto& renderer = Renderer::GetInstance();
		auto& sceneManager = SceneManager::GetInstance();
		auto& input = InputManager::GetInstance();

		bool doContinue = true;
		auto lastTime = high_resolution_clock::now();
		float lag = 0.0f;
		while (doContinue)
		{
			//Calculate and set DeltaTime
			const auto currentTime = high_resolution_clock::now();
			float dt = duration<float>(currentTime - lastTime).count();
			Himinn::Time::GetInstance().SetDeltaTime(dt);
			
			//Make last time the current time (while loop will keep going)
			//Add difference in time to last check to lag
			lastTime = currentTime;
			lag += dt;

			//If we may continue, start updating frames
			//If lag is higher than allowed delay per update, keep doing updates untill we've cought up
			//This update will be the FixedUpdate a.k.a. the physics updates
			doContinue = input.ProcessInput();
			while (lag >= MsPerUpdate)
			{
				sceneManager.FixedUpdate();
				lag -= MsPerUpdate;
			}
			//Do the normal Update and following this the LateUpdate
			//The LateUpdate will handle everything that can only happen correctly afther the normal Update
			sceneManager.Update();
			sceneManager.LateUpdate();

			//Render everything
			renderer.Render();
			
			//auto sleepTime = duration_cast<duration<float>>(currentTime + milliseconds(MsPerUpdate) - high_resolution_clock::now());
			//this_thread::sleep_for(sleepTime);
		}
	}

	Cleanup();
}
