#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

Himinn::GameObject::~GameObject() = default;

bool Himinn::GameObject::AddComponent(shared_ptr<Component> component)
{
	for (shared_ptr<Component> comp : m_Components) {

		if (typeid(*comp) == typeid(*component))
			return false;
	}
	m_Components.push_back(component);
	return true;
}

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
	if (m_ShouldRender) {

		for (shared_ptr<Component> comp : m_Components)
			comp->Render(m_Transform);
	}
}

void Himinn::GameObject::SetPosition(float x, float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
}

const Himinn::Transform& Himinn::GameObject::GetTransform() const
{
	return m_Transform;
}
