#include "MiniginPCH.h"
#include "ImageComponent.h"

#include "ResourceManager.h"
#include "GameObject.h"
#include "Renderer.h"

Himinn::ImageComponent::ImageComponent(const std::weak_ptr<GameObject>& owner, const string& filename)
	: Component(owner)
	, m_RenderComponent{}
{
	if (!owner.lock()->AddComponent<RenderComponent>(make_shared<RenderComponent>(owner)))
		std::cout << "ImageComponent: A RenderComponent was already present, so no new one was added\n";
	m_RenderComponent = owner.lock()->GetComponent<RenderComponent>();
	if (!m_RenderComponent.expired())
		m_RenderComponent.lock()->SetTexture(ResourceManager::GetInstance().LoadTexture(filename));
}

void Himinn::ImageComponent::FixedUpdate()
{
}

void Himinn::ImageComponent::Update()
{
}

void Himinn::ImageComponent::LateUpdate()
{
}

void Himinn::ImageComponent::Render()
{
}

void Himinn::ImageComponent::SetTexture(const string& filename) const
{
	m_RenderComponent.lock()->SetTexture(ResourceManager::GetInstance().LoadTexture(filename));
}
