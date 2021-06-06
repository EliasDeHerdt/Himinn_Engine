#include "PlayerManagerComponent.h"

#include <iostream>
#include <SDL_pixels.h>
#include <windows.h>

#include "CharacterComponent.h"
#include "Commands.h"
#include "Font.h"
#include "GameObject.h"
#include "ImageComponent.h"
#include "GridComponent.h"
#include "InputManager.h"
#include "LivesComponent.h"
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
		auto characterComp = player.first->GetComponent<CharacterComponent>();

		characterComp.lock()->MoveToSpawn();
	}
}

void PlayerManagerComponent::SetupManagerForLevel(std::weak_ptr<Himinn::Scene> scene, std::weak_ptr<GridComponent> grid)
{
	if (grid.expired()
		|| m_Owner.expired())
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
			player->GetComponent<CharacterComponent>().lock()->SetGridSpawnPosition(m_pGridComponent.lock()->GetLeftPeakPosition());
			
			// Player 2
			player = make_shared<Himinn::GameObject>();
			AddPlayer(player, "QBert/Characters/Character_QBert2.png", { VK_PAD_DPAD_UP, VK_PAD_DPAD_RIGHT , VK_PAD_DPAD_LEFT , VK_PAD_DPAD_DOWN , 0, true });

			player->GetComponent<CharacterComponent>().lock()->SetGridSpawnPosition(m_pGridComponent.lock()->GetRightPeakPosition());
			//AddPlayer(player, { VK_PAD_Y, VK_PAD_B , VK_PAD_X , VK_PAD_A , 1, true });
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

	for (int i = 0; i < m_Players.size(); ++i)
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
		auto characterComp = m_Players.at(i).first->GetComponent<CharacterComponent>();
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
		go->AddComponent<Himinn::ScoreComponent>(make_shared<Himinn::ScoreComponent>(go, font, color));
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
}

void PlayerManagerComponent::PlayerDied()
{
	for (int i = 0; i < m_Players.size(); ++i)
	{
		auto qbertComp = m_Players.at(i).first->GetComponent<QBertComponent>();
		if (qbertComp.expired())
			continue;
		
		if (qbertComp.lock()->GetLives() == 0)
		{
			m_Players.at(i).first->MarkForDestruction();
		}
	}
}

void PlayerManagerComponent::AddPlayer(std::shared_ptr<Himinn::GameObject>& player, std::string texturePath, PlayerControls controls)
{
	if ( m_Players.size() == m_MaxPlayers)
		return;
	
	auto subjectComp = std::make_shared<Himinn::SubjectComponent>(player);
	player->AddComponent<Himinn::SubjectComponent>(subjectComp);

	auto observer = std::make_shared<PlayerObserver>();
	subjectComp->AddObserver(observer);

	auto qbertComp = std::make_shared<QBertComponent>(player, m_LivesPerPlayer, texturePath);
	player->AddComponent<QBertComponent>(qbertComp);
	
	auto characterComp = std::make_shared<CharacterComponent>(player, m_pGridComponent);
	player->AddComponent<CharacterComponent>(characterComp);
	
	m_PlayerObservers.push_back(observer);
	m_Players.push_back(make_pair(player, controls));
	
	Himinn::InputManager& inputManager = Himinn::InputManager::GetInstance();
	inputManager.AddCommand("Player" + std::to_string(m_Players.size()) +  "MoveTopLeft", new MoveCommand(player, Himinn::QBertDirection::TopLeft));
	inputManager.AddCommand("Player" + std::to_string(m_Players.size()) + "TopRight", new MoveCommand(player, Himinn::QBertDirection::TopRight));
	inputManager.AddCommand("Player" + std::to_string(m_Players.size()) + "BottomLeft", new MoveCommand(player, Himinn::QBertDirection::BottomLeft));
	inputManager.AddCommand("Player" + std::to_string(m_Players.size()) + "BottomRight", new MoveCommand(player, Himinn::QBertDirection::BottomRight));
	
	if (controls.controller)
	{
		inputManager.BindButtonInput(controls.playerNumber, controls.topLeft, "Player" + std::to_string(m_Players.size()) + "MoveTopLeft");
		inputManager.BindButtonInput(controls.playerNumber, controls.topRight, "Player" + std::to_string(m_Players.size()) + "TopRight");
		inputManager.BindButtonInput(controls.playerNumber, controls.bottomLeft, "Player" + std::to_string(m_Players.size()) + "BottomLeft");
		inputManager.BindButtonInput(controls.playerNumber, controls.bottomRight, "Player" + std::to_string(m_Players.size()) + "BottomRight");
	}
}
