#pragma once
#include "Component.h"

class EndMenuComponent final : public Himinn::Component
{
public:
	EndMenuComponent(const std::weak_ptr<Himinn::GameObject>& owner);
	~EndMenuComponent() override = default;
	EndMenuComponent(const EndMenuComponent& other) = delete;
	EndMenuComponent& operator=(const EndMenuComponent& other) = delete;
	EndMenuComponent(EndMenuComponent&& other) = delete;
	EndMenuComponent& operator=(EndMenuComponent&& other) = delete;

	void FixedUpdate() override;
	void Update() override;
	void LateUpdate() override;
	void Render() override;
	void OnAddedToObject() override;

private:
	bool m_ShowHud = true;
};
