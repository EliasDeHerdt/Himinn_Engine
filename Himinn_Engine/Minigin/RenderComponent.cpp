#include "MiniginPCH.h"
#include "RenderComponent.h"

#include "Renderer.h"
#include "GameObject.h"

Himinn::RenderComponent::RenderComponent(const std::weak_ptr<GameObject>& owner)
	: Component(owner)
{
}

void Himinn::RenderComponent::FixedUpdate()
{
}

void Himinn::RenderComponent::Update()
{
}

void Himinn::RenderComponent::LateUpdate()
{
}

void Himinn::RenderComponent::Render()
{
	if (m_Texture != nullptr)
	{
		const auto pos = m_Owner.lock()->GetTransform().GetPosition();
		Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
	}
}

void Himinn::RenderComponent::SetTexture(const shared_ptr<Texture2D>& texture)
{
	m_Texture = texture;
}
