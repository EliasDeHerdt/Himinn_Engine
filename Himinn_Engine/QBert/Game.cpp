#include "Game.h"

#include <windows.h>
#include <vld.h>

#include "InputManager.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "Scene.h"
#include "LevelManagerComponent.h"
#include "SubjectComponent.h"
#include "AudioLibrary.h"
#include "SoundServiceLocator.h"
#include "SoundLogger.h"
#include "SDLSoundSytem.h"
#include "PlayerManagerComponent.h"
#include "EnemyManagerComponent.h"
#include "imgui.h"
#include "ManagerObserver.h"
#include "SlickAndSamComponent.h"

Game::Game()
	: Minigin()
	, m_ManagerObserver(std::make_shared<ManagerObserver>())
	, m_ManagerObject(std::make_shared<Himinn::GameObject>())
	, m_LevelManagerComponent(std::make_shared<LevelManagerComponent>(m_ManagerObject))
	, m_EnemyManagerComponent(std::make_shared<EnemyManagerComponent>(m_ManagerObject))
	, m_PlayerManagerComponent(std::make_shared<PlayerManagerComponent>(m_ManagerObject))
{
	srand(unsigned(time(nullptr)));
	
	m_ManagerObserver->SetLevelManager(m_LevelManagerComponent);
	m_ManagerObserver->SetEnemyManager(m_EnemyManagerComponent);
	m_ManagerObserver->SetPlayerManager(m_PlayerManagerComponent);

	auto subjectComp = std::make_shared<Himinn::SubjectComponent>(m_ManagerObject);
	subjectComp->AddObserver(m_ManagerObserver);
	m_ManagerObject->AddComponent<Himinn::SubjectComponent>(subjectComp);
	m_ManagerObject->AddComponent<LevelManagerComponent>(m_LevelManagerComponent);
	m_ManagerObject->AddComponent<EnemyManagerComponent>(m_EnemyManagerComponent);
	m_ManagerObject->AddComponent<PlayerManagerComponent>(m_PlayerManagerComponent);
}

void Game::LoadGame()
{
	//m_PlayerManagerComponent->SetGameMode(GameMode::Coop);
	m_LevelManagerComponent->AddLevel("Level1", "../Data/QBert/LevelSettings/LevelSettings_Level1.txt");
	m_LevelManagerComponent->AddLevel("Level2", "../Data/QBert/LevelSettings/LevelSettings_Level2.txt");
	m_LevelManagerComponent->AddLevel("Level3", "../Data/QBert/LevelSettings/LevelSettings_Level3.txt");

	/*Himinn::InputManager& inputManager = Himinn::InputManager::GetInstance();
	inputManager.AddCommand("SnekMoveTopLeft", new MoveCommand(go, Himinn::QBertDirection::TopLeft));
	inputManager.AddCommand("SnekTopRight", new MoveCommand(go, Himinn::QBertDirection::TopRight));
	inputManager.AddCommand("SnekBottomLeft", new MoveCommand(go, Himinn::QBertDirection::BottomLeft));
	inputManager.AddCommand("SnekBottomRight", new MoveCommand(go, Himinn::QBertDirection::BottomRight));
	
	inputManager.BindButtonInput(0, VK_PAD_DPAD_UP, "SnekMoveTopLeft");
	inputManager.BindButtonInput(0, VK_PAD_DPAD_RIGHT, "SnekTopRight");
	inputManager.BindButtonInput(0, VK_PAD_DPAD_LEFT, "SnekBottomLeft");
	inputManager.BindButtonInput(0, VK_PAD_DPAD_DOWN, "SnekBottomRight");*/
	
	// SoundService
	Himinn::SoundServiceLocator::RegisterSoundSystem(new Himinn::SoundLogger(new Himinn::SDLSoundSytem()));
	//SoundServiceLocator::RegisterSoundSystem(new SoundLogger(new SDLSoundSytem(), true));

	Himinn::SoundServiceLocator::GetSoundSystem()->SetVolume(80);

	// Audio
	Himinn::AudioLibrary::GetInstance().AddAudioFile("../Data/QBert/Audio/Elevator.wav");

	m_LevelManagerComponent->StartGame();
}

void Game::Cleanup()
{
	Minigin::Cleanup();
}
