#include "MiniginPCH.h"
#include "InputComponent.h"

#include "InputManager.h"

Himinn::InputComponent::InputComponent(const std::weak_ptr<GameObject>& owner, bool useController)
	: Component(owner)
	, m_UseController{useController}
{
}

void Himinn::InputComponent::FixedUpdate()
{
}

void Himinn::InputComponent::Update()
{
	InputManager::GetInstance().HandleInput(*m_Owner.lock(), m_UseController);
}

void Himinn::InputComponent::LateUpdate()
{
}

void Himinn::InputComponent::Render()
{
}
