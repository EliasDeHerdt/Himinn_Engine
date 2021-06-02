#include "MiniginPCH.h"
#include "RenderComponent.h"

#include <SDL_render.h>


#include "Renderer.h"
#include "GameObject.h"
#include "Texture2D.h"

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

Himinn::IVector2 Himinn::RenderComponent::GetTextureDimensions() const
{
	int width{}, height{};
	SDL_QueryTexture(m_Texture->GetSDLTexture(), 0, 0, &width, &height);
	return IVector2{ width, height };
}
