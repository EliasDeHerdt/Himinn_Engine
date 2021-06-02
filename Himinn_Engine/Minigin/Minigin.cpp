#include "MiniginPCH.h"
#include "Minigin.h"
#include <chrono>
#include <thread>
#include <vld.h>
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <SDL.h>
#include "GameObject.h"
#include "Scene.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "GameTime.h"
#include "audio.h"
#include "TextComponent.h"
#include "ImageComponent.h"
#include "FPSComponent.h"
#include "LivesComponent.h"
#include "ScoreComponent.h"
#include "PlayerComponent.h"
#include "SubjectComponent.h"

#include "PlayerObserver.h"
#include "AudioLibrary.h"
#include "SoundServiceLocator.h"
#include "SoundLogger.h"
#include "SDLSoundSytem.h"
#include "Command.h"

using namespace std;
using namespace std::chrono;

void Himinn::Minigin::Initialize()
{
	// Enables usage off Simple_SDL audio
	_putenv("SDL_AUDIODRIVER=DirectSound");
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}
	if (SDL_Init(SDL_INIT_AUDIO) != 0)
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	//Initializes simple audio
	initAudio();
	
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
	int lives = 3;
	SDL_Color color = SDL_Color{ 0, 255, 0 };
	shared_ptr<Font> font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	
	// Observers
	std::shared_ptr<PlayerObserver> pPlayerOneObserver{make_shared<PlayerObserver>()};
	std::shared_ptr<PlayerObserver> pPlayerTwoObserver{make_shared<PlayerObserver>()};
	
	// Background
	auto go = std::make_shared<GameObject>();
	go->AddComponent<ImageComponent>(make_shared<ImageComponent>(go, "background.jpg"));
	scene.Add(go);

	// Logo
	go = std::make_shared<GameObject>();
	go->AddComponent<ImageComponent>(make_shared<ImageComponent>(go, "logo.png"));
	go->SetPosition(216, 180);
	scene.Add(go);

	// FPS
	go = std::make_shared<GameObject>();
	go->AddComponent<FPSComponent>(make_shared<FPSComponent>(go, font, color));
	go->SetPosition(0, 0);
	scene.Add(go);

	// Title
	color = SDL_Color{ 255, 0, 0 };
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	
	go = std::make_shared<GameObject>();
	go->AddComponent<TextComponent>(make_shared<TextComponent>(go, "Programming 4 Assignment", font, color));
	go->SetPosition(80, 20);
	scene.Add(go);

	// Player 1
	// Lives Component
	color = SDL_Color{ 255, 125, 0 };
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);
	
	go = std::make_shared<GameObject>();
	go->AddComponent<LivesComponent>(make_shared<LivesComponent>(go, lives, font, color));
	go->SetPosition(10, 390);
	scene.Add(go);

	pPlayerOneObserver->SetLivesComponent(go->GetComponent<LivesComponent>());

	// Score Component
	go = std::make_shared<GameObject>();
	go->AddComponent<ScoreComponent>(make_shared<ScoreComponent>(go, font, color));
	go->SetPosition(10, 410);
	scene.Add(go);

	pPlayerOneObserver->SetScoreComponent(go->GetComponent<ScoreComponent>());

	// Player Component
	auto player1 = std::make_shared<GameObject>();
	player1->AddComponent<SubjectComponent>(make_shared<SubjectComponent>(player1));
	player1->GetComponent<SubjectComponent>().lock()->AddObserver(pPlayerOneObserver);
	player1->AddComponent<PlayerComponent>(make_shared<PlayerComponent>(player1, lives));
	player1->AddComponent<ImageComponent>(make_shared<ImageComponent>(player1, "QBert.png"));
	player1->SetPosition(150, 390);
	scene.Add(player1);
	
	// Player 2
	// Lives Component
	color = SDL_Color{ 255, 0, 255 };

	go = std::make_shared<GameObject>();
	go->AddComponent<LivesComponent>(make_shared<LivesComponent>(go, lives, font, color));
	go->SetPosition(10, 430);
	scene.Add(go);

	pPlayerTwoObserver->SetLivesComponent(go->GetComponent<LivesComponent>());

	// Score Component
	go = std::make_shared<GameObject>();
	go->AddComponent<ScoreComponent>(make_shared<ScoreComponent>(go, font, color));
	go->SetPosition(10, 450);
	scene.Add(go);

	pPlayerTwoObserver->SetScoreComponent(go->GetComponent<ScoreComponent>());
	
	// Player Component
	auto player2 = std::make_shared<GameObject>();
	player2->AddComponent<SubjectComponent>(make_shared<SubjectComponent>(player2));
	player2->GetComponent<SubjectComponent>().lock()->AddObserver(pPlayerTwoObserver);
	player2->AddComponent<PlayerComponent>(make_shared<PlayerComponent>(player2, lives));
	player2->AddComponent<ImageComponent>(make_shared<ImageComponent>(player2, "Coily.png"));
	player2->SetPosition(160, 430);
	scene.Add(player2);

	// Input
	InputManager& inputManager = InputManager::GetInstance();
	
	// Player 1 Commands
	inputManager.AddCommand("PlayerOneDies", new ObjectDiesCommand(player1));
	inputManager.AddCommand("PlayerOneJumpPlatform", new GainScoreCommand(player1, ScoreGain::ColorChange));
	inputManager.AddCommand("PlayerOneKillCoily", new GainScoreCommand(player1, ScoreGain::DefeatCoily));

	// Player 2 Commands
	inputManager.AddCommand("PlayerTwoDies", new ObjectDiesCommand(player2));
	inputManager.AddCommand("PlayerTwoJumpPlatform", new GainScoreCommand(player2, ScoreGain::ColorChange));
	inputManager.AddCommand("PlayerTwoKillCoily", new GainScoreCommand(player2, ScoreGain::DefeatCoily));
	
	// Binds
	inputManager.BindButtonInput(0, VK_PAD_A, "PlayerOneJumpPlatform");
	inputManager.BindButtonInput(0, VK_PAD_X, "PlayerOneKillCoily");
	inputManager.BindButtonInput(0, VK_PAD_B, "PlayerOneDies");

	// For now, player 2 is also on controller 1, but 2nd controller works
	inputManager.BindButtonInput(0, VK_PAD_DPAD_DOWN, "PlayerTwoJumpPlatform");
	inputManager.BindButtonInput(0, VK_PAD_DPAD_LEFT, "PlayerTwoKillCoily");
	inputManager.BindButtonInput(0, VK_PAD_DPAD_RIGHT, "PlayerTwoDies");

	// SoundService
	SoundServiceLocator::RegisterSoundSystem(new SoundLogger(new SDLSoundSytem()));
	//SoundServiceLocator::RegisterSoundSystem(new SoundLogger(new SDLSoundSytem(), true));

	SoundServiceLocator::GetSoundSystem()->SetVolume(80);
	
	// Audio
	AudioLibrary::GetInstance().AddAudioFile("../QBert/Resources/qbert/door1.wav");
	AudioLibrary::GetInstance().AddAudioFile("../QBert/Resources/qbert/door2.wav");
}

void Himinn::Minigin::Cleanup()
{
	Renderer::GetInstance().Destroy();
	delete SoundServiceLocator::GetSoundSystem();
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
			Himinn::GameTime::GetInstance().SetDeltaTime(dt);
			
			//Make last time the current time (while loop will keep going)
			//Add difference in time to last check to lag
			lastTime = currentTime;
			lag += dt;

			//If we may continue, start updating frames
			//If lag is higher than allowed delay per update, keep doing updates until we've caught up
			//This update will be the FixedUpdate a.k.a. the physics updates
			doContinue = input.ProcessInput();
			while (lag >= MsPerUpdate)
			{
				sceneManager.FixedUpdate();
				lag -= MsPerUpdate;
			}
			//Do the normal Update and following this the LateUpdate
			//The LateUpdate will handle everything that can only happen correctly after the normal Update
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
