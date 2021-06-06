#include "NodeComponent.h"

#include <iostream>


#include "ControllerComponent.h"
#include "EnemyComponent.h"
#include "GameObject.h"
#include "ImageComponent.h"
#include "NodeObserver.h"
#include "SubjectComponent.h"
#include "QBertComponent.h"

class SlickAndSamComponent;

NodeComponent::NodeComponent(const std::weak_ptr<Himinn::GameObject>& owner, const std::vector<std::string>& textures, Himinn::IVector2 playerOffset, unsigned int startLevel, bool cycleLevels)
	: Component(owner)
	, m_CycleLevels(cycleLevels)
	, m_PlayerOffset(playerOffset)
	, m_NodeLevel(0)
	, m_TexturePaths(textures)
	, m_QBerts()
	, m_Enemies()
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
			GrantScore();
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
			GrantScore();
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

void NodeComponent::AddGameObject(std::weak_ptr<Himinn::GameObject> object)
{
	if (object.expired())
		return;

	if (object.lock()->GetComponent<QBertComponent>().expired())
	{
		if (std::find_if(m_Enemies.begin(), m_Enemies.end(), [object](std::weak_ptr<Himinn::GameObject> rhs)
			{
				return object.lock() == rhs.lock();
			}) == m_Enemies.end())
		{
			m_Enemies.push_back(object);
		}
	}
	else
	{
		if (std::find_if(m_QBerts.begin(), m_QBerts.end(), [object](std::weak_ptr<Himinn::GameObject> rhs)
		{
				return object.lock() == rhs.lock();
		}) == m_QBerts.end())
		{
			m_QBerts.push_back(object);
		}
	}

	if (!m_QBerts.empty()
		&& !m_Enemies.empty())
		CheckOverlaps();
}

void NodeComponent::RemoveGameObject(std::weak_ptr<Himinn::GameObject> object)
{
	m_QBerts.erase(std::remove_if(m_QBerts.begin(), m_QBerts.end(), [object](std::weak_ptr<Himinn::GameObject> rhs)
		{
			return (object.lock() == rhs.lock());
		}), m_QBerts.end());
	
	m_Enemies.erase(std::remove_if(m_Enemies.begin(), m_Enemies.end(), [object](std::weak_ptr<Himinn::GameObject> rhs)
		{
			return (object.lock() == rhs.lock());
		}), m_Enemies.end());
}

void NodeComponent::SetNodeLevel(unsigned level)
{
	if (level >= 0
		&& level < m_TexturePaths.size())
	{
		if (!m_pSubjectComponent.expired())
		{
			if (level == m_TexturePaths.size() - 1)
				m_pSubjectComponent.lock()->Notify({}, (unsigned)NodeObserverEvent::NodeReady);

			if (m_NodeLevel == m_TexturePaths.size() - 1
				&& level != m_NodeLevel)
				m_pSubjectComponent.lock()->Notify({}, (unsigned)NodeObserverEvent::NodeNotReady);
		}
		
		m_NodeLevel = level;
		SetTexture();
	}
}

void NodeComponent::SetTexture()
{
	if (!m_pImageComponent.lock())
		return;

	m_pImageComponent.lock()->SetTexture(m_TexturePaths.at(m_NodeLevel));
}

void NodeComponent::CheckOverlaps()
{
	for (auto qbert : m_QBerts)
	{
		if (qbert.expired())
			continue;
		
		for (auto enemy : m_Enemies)
		{
			if (enemy.expired())
				continue;
			
			auto enemyComp = enemy.lock()->GetComponent<EnemyComponent>();
			if (!enemyComp.expired())
			{
				if (qbert.lock()->GetTransform().GetPosition() == enemy.lock()->GetTransform().GetPosition())
					enemyComp.lock()->OnOverlap(qbert);
			}
		}
	}
}

void NodeComponent::GrantScore()
{
	if (m_QBerts.empty())
		return;
	
	auto player = m_QBerts.back();
	if (player.expired())
		return;

	auto controllerComp = player.lock()->GetComponent<ControllerComponent>();
	if (controllerComp.expired())
		return;

	controllerComp.lock()->GainScore(25);
}
