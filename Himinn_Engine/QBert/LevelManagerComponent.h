#pragma once
#include <string>
#include <memory>
#include <vector>


#include "Component.h"

namespace Himinn {
	class SubjectComponent;
	class Scene;
	class GameObject;
}

class GridObserver;
class GridComponent;
class LevelManagerComponent final : public Himinn::Component
{
public:
	LevelManagerComponent(const std::weak_ptr<Himinn::GameObject>& owner);
	~LevelManagerComponent() override = default;
	LevelManagerComponent(const LevelManagerComponent& other) = delete;
	LevelManagerComponent& operator=(const LevelManagerComponent& other) = delete;
	LevelManagerComponent(LevelManagerComponent&& other) = delete;
	LevelManagerComponent& operator=(LevelManagerComponent&& other) = delete;

	void FixedUpdate() override;
	void Update() override;
	void LateUpdate() override;
	void Render() override;
	void OnAddedToObject() override;

	void StartGame();
	void LevelCompleted();
	void AddLevel(std::string name, std::string settingsPath);
	std::weak_ptr<Himinn::Scene> GetLevel(int level);
	std::weak_ptr<Himinn::Scene> GetLevel(std::string name);
	std::weak_ptr<GridComponent> GetGrid(int i);
	void SetActiveLevel(unsigned level);
	void SetActiveLevel(std::string name);
	void CleanUp();

private:
	unsigned m_ActiveLevel;
	std::shared_ptr<GridObserver> m_pGridObserver;
	std::vector<std::weak_ptr<Himinn::Scene>> m_LevelScenes;
	std::vector<std::shared_ptr<GridComponent>> m_LevelGrids;

	std::weak_ptr<Himinn::SubjectComponent> m_SubjectComponent;

	void TransferLevel();
	void NotifyObserver();
};
