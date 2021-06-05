#pragma once
#include <memory>
#include <string>
#include <vector>

#include "DataTypes.h"
#include "Component.h"

namespace Himinn {
	class SubjectComponent;
	class Scene;
}

class NodeObserver;
class GridComponent : public Himinn::Component
{
public:
	GridComponent(const std::weak_ptr<Himinn::GameObject>& owner, std::weak_ptr<Himinn::Scene> scene, const std::string& filePath);
	virtual ~GridComponent() override = default;
	GridComponent(const GridComponent& other) = delete;
	GridComponent& operator=(const GridComponent& other) = delete;
	GridComponent(GridComponent&& other) = delete;
	GridComponent& operator=(GridComponent&& other) = delete;

	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void OnAddedToObject() override;

	void ReadGridSettingsFile(const std::string& filePath);
	void UpgradeNode(int layer, int number);
	void UpdateNodeCompletion(bool nodeReady);
	void CheckLevelFinished();
	
	Himinn::IVector2 GetLeftPeakPosition();
	Himinn::IVector2 GetRightPeakPosition();
	Himinn::IVector2 GetNodeCharacterPosition(int layer, int number);
	std::weak_ptr<Himinn::GameObject> GetNode(int layer, int number) const;
	bool CheckForLift(int layer, int number);
private:
	// Node settings
	bool m_NodeCanCycle;
	unsigned int m_NodeStartLevel;
	unsigned int m_NodeLayerAmount;
	Himinn::IVector2 m_NodePlayerOffset;
	std::vector<std::string> m_NodeTexturePaths;

	// Lift settings
	Himinn::IVector2 m_NodeLiftLayers;
	std::string m_NodeLiftTexture;
	std::vector<std::shared_ptr<Himinn::GameObject>> m_pLifts;
	
	// Grid
	std::weak_ptr<Himinn::Scene> m_Scene;
	Himinn::IVector2 m_NodeOffset;
	unsigned int m_NodesCompleted;
	std::shared_ptr<NodeObserver> m_pNodeObserver;
	std::weak_ptr<Himinn::SubjectComponent> m_pSubjectComponent;
	std::vector<std::shared_ptr<Himinn::GameObject>> m_pNodes;

	void GenerateGrid();
	int GetListIndex(int layer, int number) const;
};
