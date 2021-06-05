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

Game::Game()
	: Minigin()
	, m_ManagerObserver(std::make_shared<ManagerObserver>())
	, m_ManagerObject(std::make_shared<Himinn::GameObject>())
	, m_LevelManagerComponent(std::make_shared<LevelManagerComponent>(m_ManagerObject))
	, m_PlayerManagerComponent(std::make_shared<PlayerManagerComponent>(m_ManagerObject))
{
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
	m_PlayerManagerComponent->SetGameMode(GameMode::Coop);
	m_LevelManagerComponent->AddLevel("Level1", "../Data/QBert/LevelSettings/LevelSettings_Level1.txt");
	m_LevelManagerComponent->AddLevel("Level2", "../Data/QBert/LevelSettings/LevelSettings_Level2.txt");
	m_LevelManagerComponent->AddLevel("Level3", "../Data/QBert/LevelSettings/LevelSettings_Level3.txt");
	
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

void Game::LoadLevel1()
{
	//auto levelObject{ make_shared<Himinn::GameObject>() };
	//auto levelComp{ make_shared<LevelManagerComponent>(levelObject, scene, "Level2", "../Data/QBert/LevelSettings/LevelSettings_Level1.txt" ) };
	//levelObject->AddComponent<LevelManagerComponent>(levelComp);
	//
	//m_PlayerManagerObject->SetupManagerForLevel(levelObject, levelComp->GetGrid());
	//levelObject->AddComponent<PlayerManagerComponent>(m_PlayerManagerObject);
	//
	//scene.Add(levelObject);
	//m_Levels.push_back(levelObject);
	//// Player Component
	//auto player1 = std::make_shared<Himinn::GameObject>();
	//player1->AddComponent<Himinn::SubjectComponent>(make_shared<Himinn::SubjectComponent>(player1));
	//player1->GetComponent<Himinn::SubjectComponent>().lock()->AddObserver(pPlayerOneObserver);
	//player1->AddComponent<CharacterComponent>(make_shared<CharacterComponent>(player1, gridComp, lives));
	//player1->AddComponent<Himinn::ImageComponent>(make_shared<Himinn::ImageComponent>(player1, "QBert/Characters/Character_QBert.png"));
	//scene.Add(player1);
}

void Game::LoadLevel2()
{
	/*auto& scene = Himinn::SceneManager::GetInstance().CreateScene("Level2");
	
	auto levelObject{ make_shared<Himinn::GameObject>() };
	auto levelComp{ make_shared<LevelManagerComponent>(levelObject, scene, "Level3", "../Data/QBert/LevelSettings/LevelSettings_Level1.txt") };
	levelObject->AddComponent<LevelManagerComponent>(levelComp);
	
	scene.Add(levelObject);
	m_Levels.push_back(levelObject);*/
}

void Game::LoadLevel3()
{
	/*auto& scene = Himinn::SceneManager::GetInstance().CreateScene("Level3");
	
	auto levelObject{ make_shared<Himinn::GameObject>() };
	auto levelComp{ make_shared<LevelManagerComponent>(levelObject, scene, "", "../Data/QBert/LevelSettings/LevelSettings_Level1.txt") };
	levelObject->AddComponent<LevelManagerComponent>(levelComp);
	
	scene.Add(levelObject);
	m_Levels.push_back(levelObject);*/
}
