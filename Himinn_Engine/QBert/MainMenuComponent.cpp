#include "MainMenuComponent.h"


#include "EnemyManagerComponent.h"
#include "imgui_impl_opengl2.h"
#include "imgui_impl_sdl.h"
#include "Renderer.h"
#include "LevelManagerComponent.h"
#include "PlayerManagerComponent.h"

MainMenuComponent::MainMenuComponent(const std::weak_ptr<Himinn::GameObject>& owner, const std::weak_ptr<LevelManagerComponent>& levelManager, const std::weak_ptr<EnemyManagerComponent>& enemyManager, const std::weak_ptr<PlayerManagerComponent>& playerManager)
	: Component(owner)
	, m_pLevelManagerComponent(levelManager)
	, m_pEnemyManagerComponent(enemyManager)
	, m_pPlayerManagerComponent(playerManager)
{
}

void MainMenuComponent::FixedUpdate()
{
}

void MainMenuComponent::Update()
{
}

void MainMenuComponent::LateUpdate()
{
}

void MainMenuComponent::Render()
{
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(Himinn::Renderer::GetInstance().GetWindow());

	ImVec2 buttonSize = { 120, 20 };
	ImGui::NewFrame(); // creates new debug window
	ImGui::Begin("Select Mode", &m_ShowHud);
	if (ImGui::Button("Single player", buttonSize))
	{
		SetGameMode(GameMode::SinglePlayer);
		CreateLevels();
	}
	if (ImGui::Button("Co-op", buttonSize))
	{
		SetGameMode(GameMode::Coop);
		CreateLevels();
	}
	if (ImGui::Button("Versus", buttonSize))
	{
		if (!m_pEnemyManagerComponent.expired())
			m_pEnemyManagerComponent.lock()->SetVersusMode(true);
		SetGameMode(GameMode::Versus);
		CreateLevels();
	}
	if (ImGui::Button("How To Play", buttonSize))
		m_ShowTutorial = !m_ShowTutorial;
	ImGui::End();

	if (m_ShowTutorial)
	{
		ImGui::Begin("How To Play", &m_ShowHud);
		ImGui::Text("QBert (Player 1):");
		ImGui::Text("Y: Move Top Left");
		ImGui::Text("B: Move Top Right");
		ImGui::Text("A: Move Bottom Left");
		ImGui::Text("X: Move Bottom Right");
		ImGui::Text("QBert2 or Coily (Player 2):");
		ImGui::Text("E: Move Top Left");
		ImGui::Text("R: Move Top Right");
		ImGui::Text("D: Move Bottom Left");
		ImGui::Text("F: Move Bottom Right");
		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}

void MainMenuComponent::OnAddedToObject()
{
}

void MainMenuComponent::SetGameMode(GameMode gamemode)
{
	if (m_pPlayerManagerComponent.expired())
		return;

	m_pPlayerManagerComponent.lock()->SetGameMode(gamemode);
}

void MainMenuComponent::CreateLevels()
{
	if (m_pLevelManagerComponent.expired())
		return;

	m_pLevelManagerComponent.lock()->AddLevel("Level1", "../Data/QBert/LevelSettings/LevelSettings_Level1.txt");
	//m_pLevelManagerComponent.lock()->AddLevel("Level2", "../Data/QBert/LevelSettings/LevelSettings_Level2.txt");
	//m_pLevelManagerComponent.lock()->AddLevel("Level3", "../Data/QBert/LevelSettings/LevelSettings_Level3.txt");

	m_pLevelManagerComponent.lock()->StartGame();
}
