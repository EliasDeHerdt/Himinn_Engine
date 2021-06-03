#pragma once
#include <memory>
#include <string>
#include <vector>

#include "DataTypes.h"
#include "Component.h"

namespace Himinn {
	class Scene;
}

class GridComponent : public Himinn::Component
{
public:
	GridComponent(const std::weak_ptr<Himinn::GameObject>& owner, Himinn::Scene& scene, const std::string& filePath);
	virtual ~GridComponent() override = default;
	GridComponent(const GridComponent& other) = delete;
	GridComponent& operator=(const GridComponent& other) = delete;
	GridComponent(GridComponent&& other) = delete;
	GridComponent& operator=(GridComponent&& other) = delete;

	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

	void ReadGridSettingsFile(const std::string& filePath);
	Himinn::IVector2 GetNodeCharacterPosition(int layer, int number) const;
	std::weak_ptr<Himinn::GameObject> GetNode(int layer, int number) const;

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
	
	// Grid
	Himinn::Scene& m_Scene;
	Himinn::IVector2 m_NodeOffset;
	std::vector<std::shared_ptr<Himinn::GameObject>> m_pGameObjects;

	void GenerateGrid();
	int GetListIndex(int layer, int number) const;
};
