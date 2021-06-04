#include "PlayerManager.h"

#include <windows.h>
#include "CharacterComponent.h"
#include "Commands.h"
#include "GameObject.h"
#include "ImageComponent.h"
#include "GridComponent.h"
#include "InputManager.h"
#include "Scene.h"
#include "SceneManager.h"

PlayerManager::PlayerManager()
	: m_GameMode(GameMode::SinglePlayer)
	, m_PlayersInitialized(false)
	, m_pGridComponent()
	, m_Players()
{
	
}

void PlayerManager::SetGameMode(GameMode gameMode)
{
	m_GameMode = gameMode;
}

void PlayerManager::MovePlayersToSpawns()
{
	for (auto player : m_Players)
	{
		auto characterComp = player.first->GetComponent<CharacterComponent>();

		characterComp.lock()->MoveToSpawn();
	}
}

void PlayerManager::SetupManagerForLevel(std::weak_ptr<GridComponent> grid)
{
	m_pGridComponent = grid;
	if (!m_PlayersInitialized)
	{
		switch (m_GameMode) {
		case GameMode::SinglePlayer:
		{
			m_MaxPlayers = 1;
			auto player1{ make_shared<Himinn::GameObject>() };
			player1->AddComponent<CharacterComponent>(make_shared<CharacterComponent>(player1, m_pGridComponent, 3));
			player1->AddComponent<Himinn::ImageComponent>(make_shared<Himinn::ImageComponent>(player1, "QBert/Characters/Character_QBert.png"));
				
			AddPlayer(player1, { VK_PAD_Y, VK_PAD_B , VK_PAD_X , VK_PAD_A , 0, true });
			break;
		}
		case GameMode::Coop:
		{
			m_MaxPlayers = 2;

			// Player 1
			auto player{ make_shared<Himinn::GameObject>() };
			auto characterComp{ make_shared<CharacterComponent>(player, m_pGridComponent, 3) };
			player->AddComponent<CharacterComponent>(characterComp);
			player->AddComponent<Himinn::ImageComponent>(make_shared<Himinn::ImageComponent>(player, "QBert/Characters/Character_QBert.png"));
			characterComp->SetGridSpawnPosition(m_pGridComponent.lock()->GetLeftPeakPosition());
				
			AddPlayer(player, { VK_PAD_Y, VK_PAD_B , VK_PAD_X , VK_PAD_A , 0, true });

			// Player 2
			player = make_shared<Himinn::GameObject>();
			characterComp = make_shared<CharacterComponent>(player, m_pGridComponent, 3);
			player->AddComponent<CharacterComponent>(characterComp);
			player->AddComponent<Himinn::ImageComponent>(make_shared<Himinn::ImageComponent>(player, "QBert/Characters/Character_QBert2.png"));
			characterComp->SetGridSpawnPosition(m_pGridComponent.lock()->GetRightPeakPosition());
				
			AddPlayer(player, { VK_PAD_Y, VK_PAD_B , VK_PAD_X , VK_PAD_A , 1, true });
			break;
		}	
		case GameMode::Versus:
			m_MaxPlayers = 2;
			
			break;
		default:break;
		}

		m_PlayersInitialized = true;
	}
	
	for (auto player : m_Players)
	{
		auto characterComp = player.first->GetComponent<CharacterComponent>();
		
		characterComp.lock()->SetGrid(m_pGridComponent);
		characterComp.lock()->MoveToSpawn();

		Himinn::SceneManager::GetInstance().GetActiveScene().lock()->Add(player.first);
	}
}

void PlayerManager::AddPlayer(std::shared_ptr<Himinn::GameObject>& player, PlayerControls controls)
{
	if (!player->GetComponent<CharacterComponent>().lock())
		return;

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
