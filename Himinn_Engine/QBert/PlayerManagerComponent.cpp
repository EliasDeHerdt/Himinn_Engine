#include "PlayerManagerComponent.h"

#include <iostream>
#include <SDL_pixels.h>
#include <windows.h>

#include "ControllerComponent.h"
#include "Commands.h"
#include "Font.h"
#include "GameObject.h"
#include "ImageComponent.h"
#include "GridComponent.h"
#include "InputManager.h"
#include "LivesComponent.h"
#include "ManagerObserver.h"
#include "PlayerObserver.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "ScoreComponent.h"
#include "SubjectComponent.h"
#include "QBertComponent.h"

PlayerManagerComponent::PlayerManagerComponent(const std::weak_ptr<Himinn::GameObject>& owner)
	: Component(owner)
	, m_GameMode(GameMode::SinglePlayer)
	, m_MaxPlayers()
	, m_LivesPerPlayer(3)
	, m_PlayersInitialized(false)
	, m_Players()
	, m_pGridComponent() 
{
}

void PlayerManagerComponent::FixedUpdate()
{
}

void PlayerManagerComponent::Update()
{
}

void PlayerManagerComponent::LateUpdate()
{
}

void PlayerManagerComponent::Render()
{
}

void PlayerManagerComponent::OnAddedToObject()
{
	if (m_Owner.expired())
		return;
	
	auto subjectComp = m_Owner.lock()->GetComponent<Himinn::SubjectComponent>();
	if (subjectComp.expired())
		std::cout << "PlayerManagerComponent: No SubjectComponent was present, no observations will be made.\n";
	else
		m_SubjectComponent = subjectComp;
}

void PlayerManagerComponent::SetGameMode(GameMode gameMode)
{
	m_GameMode = gameMode;
}

void PlayerManagerComponent::MovePlayersToSpawns()
{
	for (auto player : m_Players)
	{
		auto characterComp = player.first->GetComponent<ControllerComponent>();

		characterComp.lock()->MoveToSpawn();
	}
}

void PlayerManagerComponent::SetupManagerForLevel(std::weak_ptr<Himinn::Scene> scene, std::weak_ptr<GridComponent> grid)
{
	if (m_Owner.expired()
		|| grid.expired())
		return;

	m_pGridComponent = grid;
	
	if (!m_PlayersInitialized)
	{
		// Player 1
		m_MaxPlayers = 1;
		auto player{ make_shared<Himinn::GameObject>() };
		AddPlayer(player, "QBert/Characters/Character_QBert.png", { VK_PAD_Y, VK_PAD_B , VK_PAD_X , VK_PAD_A , 0, true });
		
		switch (m_GameMode) {
		case GameMode::Coop:
		{
			m_MaxPlayers = 2;

			// Player 1 change spawn
			player->GetComponent<ControllerComponent>().lock()->SetGridSpawnPosition(m_pGridComponent.lock()->GetLeftPeakPosition());
			
			// Player 2
			player = make_shared<Himinn::GameObject>();
			AddPlayer(player, "QBert/Characters/Character_QBert2.png", { SDLK_e, SDLK_r , SDLK_d , SDLK_f , 1, false });
			player->GetComponent<ControllerComponent>().lock()->SetGridSpawnPosition(m_pGridComponent.lock()->GetRightPeakPosition());
			break;
		}	
		case GameMode::Versus:
			m_MaxPlayers = 2;
			
			break;
		default:break;
		}

		m_PlayersInitialized = true;
	}

	if (scene.expired())
		return;

	SDL_Color color = SDL_Color{ 255, 125, 0 };
	std::shared_ptr<Himinn::Font> font = Himinn::ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);

	for (int i = 0; i < (int)m_Players.size(); ++i)
	{
		if (i > 0)
		{
			switch (m_GameMode)
			{
			case GameMode::Coop:
				color = SDL_Color{ 0, 0, 255 };
				break;
			case GameMode::Versus:
				color = SDL_Color{ 125, 0, 255 };
				break;
			default: break;
			}
		}

		auto qbertComp = m_Players.at(i).first->GetComponent<QBertComponent>();
		auto characterComp = m_Players.at(i).first->GetComponent<ControllerComponent>();
		if (qbertComp.expired()
			|| characterComp.expired())
			continue;
		
		// Lives Component
		auto go = std::make_shared<Himinn::GameObject>();
		go->AddComponent<Himinn::LivesComponent>(make_shared<Himinn::LivesComponent>(go, qbertComp.lock()->GetLives(), font, color));
		go->SetPosition(10, float(20 + (40 * i)));
		scene.lock()->Add(go);
		
		m_PlayerObservers.at(i)->SetLivesComponent(go->GetComponent<Himinn::LivesComponent>());

		// Score Component
		go = std::make_shared<Himinn::GameObject>();
		go->AddComponent<Himinn::ScoreComponent>(make_shared<Himinn::ScoreComponent>(go, qbertComp.lock()->GetScore(), font, color));
		go->SetPosition(10, float(40 +( 40 * i)));
		scene.lock()->Add(go);
		
		m_PlayerObservers.at(i)->SetScoreComponent(go->GetComponent<Himinn::ScoreComponent>());

		m_PlayerObservers.at(i)->SetPlayerManangerComponent(m_Owner.lock()->GetComponent<PlayerManagerComponent>());

		if (qbertComp.lock()->GetLives() == 0)
			continue;
		
		characterComp.lock()->SetGrid(m_pGridComponent);
		characterComp.lock()->MoveToSpawn();

		scene.lock()->Add(m_Players.at(i).first);
	}

	if (m_SubjectComponent.expired())
		return;

	Himinn::EventInfo info{};
	for (auto player : m_Players)
		info.pointerInfo.push_back(player.first->GetComponent<ControllerComponent>());
	
	m_SubjectComponent.lock()->Notify(info, (unsigned)ManagerObserverEvent::PassingPlayers);
}

void PlayerManagerComponent::PlayerDied()
{
	int playersAlive{ (int)m_Players.size() };
	for (int i = 0; i < (int)m_Players.size(); ++i)
	{
		auto qbertComp = m_Players.at(i).first->GetComponent<QBertComponent>();
		if (qbertComp.expired())
			continue;
		
		if (qbertComp.lock()->GetLives() == 0)
		{
			--playersAlive;
			m_Players.at(i).first->MarkForDestruction();
		}
	}

	if (m_SubjectComponent.expired())
		return;

	m_SubjectComponent.lock()->Notify({}, (unsigned)ManagerObserverEvent::ClearEnemies);
	if (playersAlive == 0)
		m_SubjectComponent.lock()->Notify({}, (unsigned)ManagerObserverEvent::GameOver);

}

void PlayerManagerComponent::CleanUp()
{
	m_Players.clear();
	m_PlayersInitialized = false;
}

void PlayerManagerComponent::AddPlayer(std::shared_ptr<Himinn::GameObject>& player, std::string texturePath, PlayerControls controls)
{
	if (m_MaxPlayers == (int)m_Players.size() )
		return;
	
	auto subjectComp = std::make_shared<Himinn::SubjectComponent>(player);
	player->AddComponent<Himinn::SubjectComponent>(subjectComp);

	auto observer = std::make_shared<PlayerObserver>();
	subjectComp->AddObserver(observer);
	
	auto qbertComp = std::make_shared<QBertComponent>(player, m_LivesPerPlayer, texturePath);
	player->AddComponent<QBertComponent>(qbertComp);

	auto controllerComp = std::make_shared<ControllerComponent>(player, m_pGridComponent);
	player->AddComponent<ControllerComponent>(controllerComp);
	
	m_PlayerObservers.push_back(observer);
	m_Players.push_back(make_pair(player, controls));
	
	Himinn::InputManager& inputManager = Himinn::InputManager::GetInstance();
	inputManager.AddCommand("Player" + std::to_string(m_Players.size()) +  "MoveTopLeft", std::make_shared<MoveCommand>(player, Himinn::QBertDirection::TopLeft));
	inputManager.AddCommand("Player" + std::to_string(m_Players.size()) + "TopRight", std::make_shared<MoveCommand>(player, Himinn::QBertDirection::TopRight));
	inputManager.AddCommand("Player" + std::to_string(m_Players.size()) + "BottomLeft", std::make_shared<MoveCommand>(player, Himinn::QBertDirection::BottomLeft));
	inputManager.AddCommand("Player" + std::to_string(m_Players.size()) + "BottomRight", std::make_shared<MoveCommand>(player, Himinn::QBertDirection::BottomRight));

	if (controls.controller)
	{
		inputManager.BindButtonInput(controls.playerNumber, controls.topLeft, "Player" + std::to_string(m_Players.size()) + "MoveTopLeft");
		inputManager.BindButtonInput(controls.playerNumber, controls.topRight, "Player" + std::to_string(m_Players.size()) + "TopRight");
		inputManager.BindButtonInput(controls.playerNumber, controls.bottomLeft, "Player" + std::to_string(m_Players.size()) + "BottomLeft");
		inputManager.BindButtonInput(controls.playerNumber, controls.bottomRight, "Player" + std::to_string(m_Players.size()) + "BottomRight");
	}
	else
	{
		inputManager.BindButtonInput(controls.topLeft, "Player" + std::to_string(m_Players.size()) + "MoveTopLeft");
		inputManager.BindButtonInput(controls.topRight, "Player" + std::to_string(m_Players.size()) + "TopRight");
		inputManager.BindButtonInput(controls.bottomLeft, "Player" + std::to_string(m_Players.size()) + "BottomLeft");
		inputManager.BindButtonInput(controls.bottomRight, "Player" + std::to_string(m_Players.size()) + "BottomRight");
	}

}
