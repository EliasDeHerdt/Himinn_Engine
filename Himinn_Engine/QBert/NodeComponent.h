#pragma once
#include <memory>
#include <string>
#include <vector>

#include "DataTypes.h"
#include "Component.h"

namespace Himinn {
	class ImageComponent;
	class SubjectComponent;
}

class NodeComponent : public Himinn::Component
{
public:
	NodeComponent(const std::weak_ptr<Himinn::GameObject>& owner, const std::vector<std::string>& textures, Himinn::IVector2 playerOffset = {0, 0}, unsigned int startLevel = 0, bool cycleLevels = false);
	virtual ~NodeComponent() override = default;
	NodeComponent(const NodeComponent& other) = delete;
	NodeComponent& operator=(const NodeComponent& other) = delete;
	NodeComponent(NodeComponent&& other) = delete;
	NodeComponent& operator=(NodeComponent&& other) = delete;

	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void OnAddedToObject() override;

	const Himinn::IVector2& GetPlayerOffset() const;
	void SetPlayerOffset(Himinn::IVector2 position);

	void IncrementNodeLevel();
	void SetNodeLevel(unsigned int level);
	void SetNeighbor(std::weak_ptr<NodeComponent> nodeComponent, Himinn::QBertDirection direction);

private:
	bool m_CycleLevels;
	unsigned int m_NodeLevel;
	Himinn::IVector2 m_PlayerOffset;
	std::vector<std::string> m_TexturePaths;

	std::weak_ptr<NodeComponent> m_pTopLeftNeighbor;
	std::weak_ptr<NodeComponent> m_pTopRightNeighbor;
	std::weak_ptr<NodeComponent> m_pBottomLeftNeighbor;
	std::weak_ptr<NodeComponent> m_pBottomRightNeighbor;
	
	// Components
	std::weak_ptr<Himinn::ImageComponent> m_pImageComponent;
	std::weak_ptr<Himinn::SubjectComponent> m_pSubjectComponent;

	void SetTexture();
};

