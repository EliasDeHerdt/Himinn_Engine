#pragma once
#include "Component.h"

class EndMenuComponent : public Himinn::Component
{
public:
	EndMenuComponent(const std::weak_ptr<Himinn::GameObject>& owner);
	virtual ~EndMenuComponent() override = default;
	EndMenuComponent(const EndMenuComponent& other) = delete;
	EndMenuComponent& operator=(const EndMenuComponent& other) = delete;
	EndMenuComponent(EndMenuComponent&& other) = delete;
	EndMenuComponent& operator=(EndMenuComponent&& other) = delete;

	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void OnAddedToObject() override;

private:
	bool m_ShowHud = true;
};
