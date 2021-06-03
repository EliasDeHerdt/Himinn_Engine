#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "DataTypes.h"

Himinn::GameObject::~GameObject() = default;

void Himinn::GameObject::FixedUpdate()
{
	for (shared_ptr<Component> comp : m_Components)
		comp->FixedUpdate();
}

void Himinn::GameObject::Update()
{
	for (shared_ptr<Component> comp : m_Components)
		comp->Update();
}

void Himinn::GameObject::LateUpdate()
{
	for (shared_ptr<Component> comp : m_Components)
		comp->LateUpdate();
}

void Himinn::GameObject::Render() const
{
	for (shared_ptr<Component> comp : m_Components)
		comp->Render();
}

void Himinn::GameObject::MarkForDestruction()
{
	m_Destroy = true;
}

bool Himinn::GameObject::ShouldBeDestroyed() const
{
	return m_Destroy;
}

void Himinn::GameObject::SetPosition(float x, float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
}

void Himinn::GameObject::SetPosition(IVector2 position)
{
	m_Transform.SetPosition((float)position.x, (float)position.y, 0.0f);
}

const Himinn::Transform& Himinn::GameObject::GetTransform() const
{
	return m_Transform;
}
