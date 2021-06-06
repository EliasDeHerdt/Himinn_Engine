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
#include "LevelManagerComponent.h"

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
#include "PlayerManagerComponent.h"
#include "LevelManagerComponent.h"
#include "ManagerObserver.h"
#include "SlickAndSamComponent.h"
#include "UggAndWrongwayComponent.h"

Game::Game()
	: Minigin()
	, m_ManagerObserver(std::make_shared<ManagerObserver>())
	, m_ManagerObject(std::make_shared<Himinn::GameObject>())
	, m_LevelManagerComponent(std::make_shared<LevelManagerComponent>(m_ManagerObject))
	, m_PlayerManagerComponent(std::make_shared<PlayerManagerComponent>(m_ManagerObject))
{
	srand(unsigned(time(nullptr)));
	
	m_ManagerObserver->SetLevelManager(m_LevelManagerComponent);
	m_ManagerObserver->SetPlayerManager(m_PlayerManagerComponent);

	auto subjectComp = std::make_shared<Himinn::SubjectComponent>(m_ManagerObject);
	subjectComp->AddObserver(m_ManagerObserver);
	m_ManagerObject->AddComponent<Himinn::SubjectComponent>(subjectComp);
	m_ManagerObject->AddComponent<LevelManagerComponent>(m_LevelManagerComponent);
	m_ManagerObject->AddComponent<PlayerManagerComponent>(m_PlayerManagerComponent);
}

void Game::LoadGame()
{
	//m_PlayerManagerComponent->SetGameMode(GameMode::Coop);
	m_LevelManagerComponent->AddLevel("Level1", "../Data/QBert/LevelSettings/LevelSettings_Level1.txt");
	m_LevelManagerComponent->AddLevel("Level2", "../Data/QBert/LevelSettings/LevelSettings_Level2.txt");
	m_LevelManagerComponent->AddLevel("Level3", "../Data/QBert/LevelSettings/LevelSettings_Level3.txt");

	auto go = std::make_shared<Himinn::GameObject>();
	auto ssComp = std::make_shared<SlickAndSamComponent>(go, m_LevelManagerComponent->GetGrid(0), 0.8f);
	go->AddComponent<SlickAndSamComponent>(ssComp);
	m_LevelManagerComponent->GetLevel("Level1").lock()->Add(go);

	go = std::make_shared<Himinn::GameObject>();
	auto uwComp = std::make_shared<UggAndWrongwayComponent>(go, m_LevelManagerComponent->GetGrid(0), 0.8f);
	go->AddComponent<UggAndWrongwayComponent>(uwComp);
	m_LevelManagerComponent->GetLevel("Level1").lock()->Add(go);
	
	ssComp->Spawn();
	uwComp->Spawn();
	
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