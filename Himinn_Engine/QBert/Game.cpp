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
#include "CharacterComponent.h"
#include "SubjectComponent.h"

#include "PlayerObserver.h"
#include "AudioLibrary.h"
#include "SoundServiceLocator.h"
#include "SoundLogger.h"
#include "SDLSoundSytem.h"
#include "Command.h"
#include "Commands.h"
#include "GridComponent.h"

void Game::LoadGame() const
{
	auto& scene = Himinn::SceneManager::GetInstance().CreateScene("QBertGame");
	int lives = 3;
	SDL_Color color = SDL_Color{ 0, 255, 0 };
	std::shared_ptr<Himinn::Font> font = Himinn::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	// Observers
	std::shared_ptr<Himinn::PlayerObserver> pPlayerOneObserver{ make_shared<Himinn::PlayerObserver>() };
	
	// Background
	auto go = std::make_shared<Himinn::GameObject>();
	go->AddComponent<Himinn::ImageComponent>(make_shared<Himinn::ImageComponent>(go, "background.jpg"));
	scene.Add(go);

	// Node Test
	go = std::make_shared<Himinn::GameObject>();
	auto gridComp = make_shared<GridComponent>(go, scene, "../Data/QBert/LevelSettings/LevelSettings_Level1.txt");
	go->AddComponent<GridComponent>(gridComp);
	scene.Add(go);

	// FPS
	go = std::make_shared<Himinn::GameObject>();
	go->AddComponent<Himinn::FPSComponent>(make_shared<Himinn::FPSComponent>(go, font, color));
	go->SetPosition(0, 0);
	scene.Add(go);

	// Player 1
	// Lives Component
	color = SDL_Color{ 255, 125, 0 };
	font = Himinn::ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);

	go = std::make_shared<Himinn::GameObject>();
	go->AddComponent<Himinn::LivesComponent>(make_shared<Himinn::LivesComponent>(go, lives, font, color));
	go->SetPosition(10, 20);
	scene.Add(go);

	pPlayerOneObserver->SetLivesComponent(go->GetComponent<Himinn::LivesComponent>());
	
	// Score Component
	go = std::make_shared<Himinn::GameObject>();
	go->AddComponent<Himinn::ScoreComponent>(make_shared<Himinn::ScoreComponent>(go, font, color));
	go->SetPosition(10, 40);
	scene.Add(go);
	
	pPlayerOneObserver->SetScoreComponent(go->GetComponent<Himinn::ScoreComponent>());
	
	// Player Component
	auto player1 = std::make_shared<Himinn::GameObject>();
	player1->AddComponent<Himinn::SubjectComponent>(make_shared<Himinn::SubjectComponent>(player1));
	player1->GetComponent<Himinn::SubjectComponent>().lock()->AddObserver(pPlayerOneObserver);
	player1->AddComponent<CharacterComponent>(make_shared<CharacterComponent>(player1, gridComp, lives));
	player1->AddComponent<Himinn::ImageComponent>(make_shared<Himinn::ImageComponent>(player1, "QBert/Characters/Character_QBert.png"));
	scene.Add(player1);

	// Input
	Himinn::InputManager& inputManager = Himinn::InputManager::GetInstance();

	// Player 1 Commands
	inputManager.AddCommand("PlayerMoveTopLeft", new MoveCommand(player1, Himinn::QBertDirection::TopLeft));
	inputManager.AddCommand("PlayerMoveTopRight", new MoveCommand(player1, Himinn::QBertDirection::TopRight));
	inputManager.AddCommand("PlayerMoveBottomLeft", new MoveCommand(player1, Himinn::QBertDirection::BottomLeft));
	inputManager.AddCommand("PlayerMoveBottomRight", new MoveCommand(player1, Himinn::QBertDirection::BottomRight));
	
	inputManager.BindButtonInput(0, VK_PAD_Y, "PlayerMoveTopLeft");
	inputManager.BindButtonInput(0, VK_PAD_B, "PlayerMoveTopRight");
	inputManager.BindButtonInput(0, VK_PAD_X, "PlayerMoveBottomLeft");
	inputManager.BindButtonInput(0, VK_PAD_A, "PlayerMoveBottomRight");

	// SoundService
	Himinn::SoundServiceLocator::RegisterSoundSystem(new Himinn::SoundLogger(new Himinn::SDLSoundSytem()));
	//SoundServiceLocator::RegisterSoundSystem(new SoundLogger(new SDLSoundSytem(), true));

	Himinn::SoundServiceLocator::GetSoundSystem()->SetVolume(80);

	// Audio
	Himinn::AudioLibrary::GetInstance().AddAudioFile("../Data/QBert/Audio/Elevator.wav");
}

void Game::Cleanup()
{
	Minigin::Cleanup();
}
