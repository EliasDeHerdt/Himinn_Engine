#include "MiniginPCH.h"
#include "ImageComponent.h"

#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"

Himinn::ImageComponent::ImageComponent(const string& filename)
	: m_RenderComponent{}
{
	m_RenderComponent.SetTexture(ResourceManager::GetInstance().LoadTexture(filename));
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

void Himinn::ImageComponent::Render(const Transform& transform)
{
	m_RenderComponent.Render(transform);
}

void Himinn::ImageComponent::SetTexture(const string& filename)
{
	m_RenderComponent.SetTexture(ResourceManager::GetInstance().LoadTexture(filename));
}
