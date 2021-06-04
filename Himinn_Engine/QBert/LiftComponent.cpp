#include "LiftComponent.h"

#include <iostream>
#include "GameObject.h"
#include "ImageComponent.h"
#include "SoundServiceLocator.h"

LiftComponent::LiftComponent(const weak_ptr<Himinn::GameObject>& owner, const std::string& texture, Himinn::IVector2 gridPosition, Himinn::IVector2 playerOffset)
	: Component(owner)
	, m_TexturePaths(texture)
	, m_PlayerOffset(playerOffset)
	, m_GridPosition(gridPosition)
	, m_pImageComponent()
{
	if (!owner.lock()->AddComponent<Himinn::ImageComponent>(make_shared<Himinn::ImageComponent>(owner, texture)))
		std::cout << "LiftComponent: A ImageComponent was already present, so no new one was added.\n";
	m_pImageComponent = owner.lock()->GetComponent<Himinn::ImageComponent>();
}

void LiftComponent::FixedUpdate()
{
}

void LiftComponent::Update()
{
}

void LiftComponent::LateUpdate()
{
}

void LiftComponent::Render()
{
}

const Himinn::IVector2& LiftComponent::GetGridPosition() const
{
	return m_GridPosition;
}

const Himinn::IVector2& LiftComponent::GetPlayerOffset() const
{
	return m_PlayerOffset;
}

void LiftComponent::SetPlayerOffset(Himinn::IVector2 offset)
{
	m_PlayerOffset = offset;
}

void LiftComponent::Activate()
{
	Himinn::SoundServiceLocator::GetSoundSystem()->QueueSound(0, 100);
	m_Owner.lock()->MarkForDestruction();
}
