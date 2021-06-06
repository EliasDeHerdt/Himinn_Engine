#include "UIComponent.h"

UIComponent::UIComponent(const std::weak_ptr<Himinn::GameObject>& owner)
	: Component(owner)
{
}

void UIComponent::FixedUpdate()
{
}

void UIComponent::Update()
{
}

void UIComponent::LateUpdate()
{
}

void UIComponent::Render()
{
}

void UIComponent::OnAddedToObject()
{
}
