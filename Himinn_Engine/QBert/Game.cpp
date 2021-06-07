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
#include "EndMenuComponent.h"
#include "SoundServiceLocator.h"
#include "SoundLogger.h"
#include "SDLSoundSytem.h"
#include "PlayerManagerComponent.h"
#include "EnemyManagerComponent.h"
#include "imgui.h"
#include "ManagerObserver.h"
#include "MainMenuComponent.h"

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
	Himinn::InputManager::GetInstance().SetAmountOfPlayers(2);
	auto mainMenu = Himinn::SceneManager::GetInstance().CreateScene("MainMenu");

	auto go = std::make_shared<Himinn::GameObject>();
	auto mainMenuComp = std::make_shared<MainMenuComponent>(go, m_LevelManagerComponent, m_EnemyManagerComponent, m_PlayerManagerComponent);
	go->AddComponent<MainMenuComponent>(mainMenuComp);
	mainMenu.lock()->Add(go);

	auto endScreen = Himinn::SceneManager::GetInstance().CreateScene("EndScreen");

	go = std::make_shared<Himinn::GameObject>();
	auto endMenuComp = std::make_shared<EndMenuComponent>(go);
	go->AddComponent<EndMenuComponent>(endMenuComp);
	endScreen.lock()->Add(go);
	
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
