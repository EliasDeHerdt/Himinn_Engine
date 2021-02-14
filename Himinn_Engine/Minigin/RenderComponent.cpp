#include "MiniginPCH.h"
#include "RenderComponent.h"

#include "Renderer.h"

Himinn::RenderComponent::RenderComponent()
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

void Himinn::RenderComponent::Render(const Transform& transform)
{
	if (m_Texture != nullptr)
	{
		const auto pos = transform.GetPosition();
		Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
	}
}

void Himinn::RenderComponent::SetTexture(const shared_ptr<Texture2D>& texture)
{
	m_Texture = texture;
}
