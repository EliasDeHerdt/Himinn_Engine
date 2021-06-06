#pragma once
#include "Component.h"

class UIComponent : public Himinn::Component
{
public:
	UIComponent(const std::weak_ptr<Himinn::GameObject>& owner);
	virtual ~UIComponent() override = default;
	UIComponent(const UIComponent& other) = delete;
	UIComponent& operator=(const UIComponent& other) = delete;
	UIComponent(UIComponent&& other) = delete;
	UIComponent& operator=(UIComponent&& other) = delete;

	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void OnAddedToObject() override;

private:
	
};

