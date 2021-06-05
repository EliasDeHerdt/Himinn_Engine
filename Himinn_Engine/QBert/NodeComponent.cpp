#include "NodeComponent.h"

#include <iostream>
#include "GameObject.h"
#include "ImageComponent.h"
#include "NodeObserver.h"
#include "SubjectComponent.h"

NodeComponent::NodeComponent(const std::weak_ptr<Himinn::GameObject>& owner, const std::vector<std::string>& textures, Himinn::IVector2 playerOffset, unsigned int startLevel, bool cycleLevels)
	: Component(owner)
	, m_CycleLevels(cycleLevels)
	, m_PlayerOffset(playerOffset)
	, m_NodeLevel(0)
	, m_TexturePaths(textures)
	, m_pImageComponent()
	, m_pSubjectComponent()
{
	std::string path{};
	if (!textures.empty())
	{
		if (startLevel < textures.size())
			m_NodeLevel = startLevel;
		
		path = textures[m_NodeLevel];
	}
	
	if (!owner.lock()->AddComponent<Himinn::ImageComponent>(make_shared<Himinn::ImageComponent>(owner, path)))
		std::cout << "NodeComponent: A ImageComponent was already present, so no new one was added.\n";
	m_pImageComponent = owner.lock()->GetComponent<Himinn::ImageComponent>();

	
}

void NodeComponent::FixedUpdate()
{
}

void NodeComponent::Update()
{
}

void NodeComponent::LateUpdate()
{
}

void NodeComponent::Render()
{
}

void NodeComponent::OnAddedToObject()
{
	auto subjectComp{ m_Owner.lock()->GetComponent<Himinn::SubjectComponent>() };
	if (subjectComp.expired())
		std::cout << "NodeComponent: No SubjectComponent was present, no observations will be made.\n";
	else
		m_pSubjectComponent = subjectComp;
}

const Himinn::IVector2& NodeComponent::GetPlayerOffset() const
{
	return m_PlayerOffset;
}

void NodeComponent::SetPlayerOffset(Himinn::IVector2 position)
{
	m_PlayerOffset = position;
}

void NodeComponent::IncrementNodeLevel()
{
	if (m_CycleLevels)
	{
		if (m_NodeLevel + 1 < m_TexturePaths.size())
		{
			++m_NodeLevel;
			if (m_NodeLevel == m_TexturePaths.size() - 1
				&& !m_pSubjectComponent.expired())
				m_pSubjectComponent.lock()->Notify({}, (unsigned)NodeObserverEvent::NodeReady);
		}
		else
		{
			--m_NodeLevel;
			if (!m_pSubjectComponent.expired())
				m_pSubjectComponent.lock()->Notify({}, (unsigned)NodeObserverEvent::NodeNotReady);
		}
		SetTexture();
	}
	else
	{
		if (m_NodeLevel + 1 < m_TexturePaths.size())
		{
			++m_NodeLevel;
			SetTexture();
			if (m_NodeLevel == m_TexturePaths.size() - 1
				&& !m_pSubjectComponent.expired())
				m_pSubjectComponent.lock()->Notify({}, (unsigned)NodeObserverEvent::NodeReady);
		}
	}
}

void NodeComponent::SetNeighbor(std::weak_ptr<NodeComponent> nodeComponent, Himinn::QBertDirection direction)
{
	switch (direction)
	{
		case Himinn::QBertDirection::TopLeft:
			m_pTopLeftNeighbor = nodeComponent;
			break;
		case Himinn::QBertDirection::TopRight:
			m_pTopRightNeighbor = nodeComponent;
			break;
		case Himinn::QBertDirection::BottomLeft:
			m_pBottomLeftNeighbor = nodeComponent;
			break;
		case Himinn::QBertDirection::BottomRight:
			m_pBottomRightNeighbor = nodeComponent;
			break;
	}
}

void NodeComponent::SetNodeLevel(unsigned level)
{
	if (level >= 0
		&& level < m_TexturePaths.size())
	{
		m_NodeLevel = level;
		SetTexture();

		if (m_NodeLevel == m_TexturePaths.size() - 1
			&& !m_pSubjectComponent.expired())
			m_pSubjectComponent.lock()->Notify({}, (unsigned)NodeObserverEvent::NodeReady);
	}
}

void NodeComponent::SetTexture()
{
	if (!m_pImageComponent.lock())
		return;

	m_pImageComponent.lock()->SetTexture(m_TexturePaths.at(m_NodeLevel));
}