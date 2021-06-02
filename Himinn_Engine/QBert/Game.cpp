#include "Game.h"

#include <windows.h>
#include <vld.h>

#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Scene.h"
#include "DataTypes.h"

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
#include "GridComponent.h"

void Game::LoadGame() const
{
	auto& scene = Himinn::SceneManager::GetInstance().CreateScene("Demo");
	int lives = 3;
	SDL_Color color = SDL_Color{ 0, 255, 0 };
	std::shared_ptr<Himinn::Font> font = Himinn::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	// Observers
	std::shared_ptr<Himinn::PlayerObserver> pPlayerOneObserver{std::make_shared<Himinn::PlayerObserver>() };
	std::shared_ptr<Himinn::PlayerObserver> pPlayerTwoObserver{std::make_shared<Himinn::PlayerObserver>() };

	// Background
	auto go = std::make_shared<Himinn::GameObject>();
	go->AddComponent<Himinn::ImageComponent>(make_shared<Himinn::ImageComponent>(go, "background.jpg"));
	scene.Add(go);

	// Logo
	go = std::make_shared<Himinn::GameObject>();
	go->AddComponent<Himinn::ImageComponent>(make_shared<Himinn::ImageComponent>(go, "logo.png"));
	go->SetPosition(216, 180);
	scene.Add(go);

	// Node Test
	go = std::make_shared<Himinn::GameObject>();
	go->AddComponent<GridComponent>(make_shared<GridComponent>(go, scene, "./../Data/QBert/LevelSettings/LevelSettings_Level1.txt"));
	scene.Add(go);

	// FPS
	go = std::make_shared<Himinn::GameObject>();
	go->AddComponent<Himinn::FPSComponent>(make_shared<Himinn::FPSComponent>(go, font, color));
	go->SetPosition(0, 0);
	scene.Add(go);

	// Title
	color = SDL_Color{ 255, 0, 0 };
	font = Himinn::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	go = std::make_shared<Himinn::GameObject>();
	go->AddComponent<Himinn::TextComponent>(make_shared<Himinn::TextComponent>(go, "Programming 4 Assignment", font, color));
	go->SetPosition(80, 20);
	scene.Add(go);

	// Player 1
	// Lives Component
	color = SDL_Color{ 255, 125, 0 };
	font = Himinn::ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);

	go = std::make_shared<Himinn::GameObject>();
	go->AddComponent<Himinn::LivesComponent>(make_shared<Himinn::LivesComponent>(go, lives, font, color));
	go->SetPosition(10, 390);
	scene.Add(go);

	pPlayerOneObserver->SetLivesComponent(go->GetComponent<Himinn::LivesComponent>());

	// Score Component
	go = std::make_shared<Himinn::GameObject>();
	go->AddComponent<Himinn::ScoreComponent>(make_shared<Himinn::ScoreComponent>(go, font, color));
	go->SetPosition(10, 410);
	scene.Add(go);

	pPlayerOneObserver->SetScoreComponent(go->GetComponent<Himinn::ScoreComponent>());

	// Player Component
	auto player1 = std::make_shared<Himinn::GameObject>();
	player1->AddComponent<Himinn::SubjectComponent>(make_shared<Himinn::SubjectComponent>(player1));
	player1->GetComponent<Himinn::SubjectComponent>().lock()->AddObserver(pPlayerOneObserver);
	player1->AddComponent<Himinn::PlayerComponent>(make_shared<Himinn::PlayerComponent>(player1, lives));
	player1->AddComponent<Himinn::ImageComponent>(make_shared<Himinn::ImageComponent>(player1, "QBert.png"));
	player1->SetPosition(150, 390);
	scene.Add(player1);

	// Player 2
	// Lives Component
	color = SDL_Color{ 255, 0, 255 };

	go = std::make_shared<Himinn::GameObject>();
	go->AddComponent<Himinn::LivesComponent>(make_shared<Himinn::LivesComponent>(go, lives, font, color));
	go->SetPosition(10, 430);
	scene.Add(go);

	pPlayerTwoObserver->SetLivesComponent(go->GetComponent<Himinn::LivesComponent>());

	// Score Component
	go = std::make_shared<Himinn::GameObject>();
	go->AddComponent<Himinn::ScoreComponent>(make_shared<Himinn::ScoreComponent>(go, font, color));
	go->SetPosition(10, 450);
	scene.Add(go);

	pPlayerTwoObserver->SetScoreComponent(go->GetComponent<Himinn::ScoreComponent>());

	// Player Component
	auto player2 = std::make_shared<Himinn::GameObject>();
	player2->AddComponent<Himinn::SubjectComponent>(make_shared<Himinn::SubjectComponent>(player2));
	player2->GetComponent<Himinn::SubjectComponent>().lock()->AddObserver(pPlayerTwoObserver);
	player2->AddComponent<Himinn::PlayerComponent>(make_shared<Himinn::PlayerComponent>(player2, lives));
	player2->AddComponent<Himinn::ImageComponent>(make_shared<Himinn::ImageComponent>(player2, "Coily.png"));
	player2->SetPosition(160, 430);
	scene.Add(player2);

	// Input
	Himinn::InputManager& inputManager = Himinn::InputManager::GetInstance();

	// Player 1 Commands
	inputManager.AddCommand("PlayerOneDies", new Himinn::ObjectDiesCommand(player1));
	inputManager.AddCommand("PlayerOneJumpPlatform", new Himinn::GainScoreCommand(player1, Himinn::ScoreGain::ColorChange));
	inputManager.AddCommand("PlayerOneKillCoily", new Himinn::GainScoreCommand(player1, Himinn::ScoreGain::DefeatCoily));

	// Player 2 Commands
	inputManager.AddCommand("PlayerTwoDies", new Himinn::ObjectDiesCommand(player2));
	inputManager.AddCommand("PlayerTwoJumpPlatform", new Himinn::GainScoreCommand(player2, Himinn::ScoreGain::ColorChange));
	inputManager.AddCommand("PlayerTwoKillCoily", new Himinn::GainScoreCommand(player2, Himinn::ScoreGain::DefeatCoily));

	// Binds
	inputManager.BindButtonInput(0, VK_PAD_A, "PlayerOneJumpPlatform");
	inputManager.BindButtonInput(0, VK_PAD_X, "PlayerOneKillCoily");
	inputManager.BindButtonInput(0, VK_PAD_B, "PlayerOneDies");

	// For now, player 2 is also on controller 1, but 2nd controller works
	inputManager.BindButtonInput(0, VK_PAD_DPAD_DOWN, "PlayerTwoJumpPlatform");
	inputManager.BindButtonInput(0, VK_PAD_DPAD_LEFT, "PlayerTwoKillCoily");
	inputManager.BindButtonInput(0, VK_PAD_DPAD_RIGHT, "PlayerTwoDies");

	// SoundService
	Himinn::SoundServiceLocator::RegisterSoundSystem(new Himinn::SoundLogger(new Himinn::SDLSoundSytem()));
	//SoundServiceLocator::RegisterSoundSystem(new SoundLogger(new SDLSoundSytem(), true));

	Himinn::SoundServiceLocator::GetSoundSystem()->SetVolume(80);

	// Audio
	Himinn::AudioLibrary::GetInstance().AddAudioFile("../QBert/Resources/qbert/door1.wav");
	Himinn::AudioLibrary::GetInstance().AddAudioFile("../QBert/Resources/qbert/door2.wav");
}

void Game::Cleanup()
{
	Minigin::Cleanup();
}
