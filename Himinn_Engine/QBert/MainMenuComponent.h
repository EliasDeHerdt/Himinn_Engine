#pragma once
#include "Component.h"

enum class GameMode;
class LevelManagerComponent;
class EnemyManagerComponent;
class PlayerManagerComponent;
class MainMenuComponent : public Himinn::Component
{
public:
	MainMenuComponent(const std::weak_ptr<Himinn::GameObject>& owner, const std::weak_ptr<LevelManagerComponent>& levelManager, const std::weak_ptr<EnemyManagerComponent>& enemyManager, const std::weak_ptr<PlayerManagerComponent>& playerManager);
	virtual ~MainMenuComponent() override = default;
	MainMenuComponent(const MainMenuComponent& other) = delete;
	MainMenuComponent& operator=(const MainMenuComponent& other) = delete;
	MainMenuComponent(MainMenuComponent&& other) = delete;
	MainMenuComponent& operator=(MainMenuComponent&& other) = delete;

	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void OnAddedToObject() override;

	void SetGameMode(GameMode gamemode);
	void CreateLevels();

private:
	bool m_ShowHud = true;
	bool m_ShowTutorial = false;

	std::weak_ptr<LevelManagerComponent> m_pLevelManagerComponent;
	std::weak_ptr<EnemyManagerComponent> m_pEnemyManagerComponent;
	std::weak_ptr<PlayerManagerComponent> m_pPlayerManagerComponent;
};

