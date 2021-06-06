#pragma once
#include "Component.h"

class ControllableComponent : public virtual Himinn::Component
{
public:
	ControllableComponent(bool canUpdateNodes = false) : m_CanUpdateNodes(canUpdateNodes) {};
	virtual ~ControllableComponent() override = default;
	ControllableComponent(const ControllableComponent& other) = delete;
	ControllableComponent& operator=(const ControllableComponent& other) = delete;
	ControllableComponent(ControllableComponent&& other) = delete;
	ControllableComponent& operator=(ControllableComponent&& other) = delete;

	virtual void FixedUpdate() override = 0;
	virtual void Update() override = 0;
	virtual void LateUpdate() override = 0;
	virtual void Render() override = 0;
	virtual void OnAddedToObject() override = 0;

	virtual void OnDeath() = 0;
	virtual void OnScore(int amount) = 0;

	bool CanUpdateNodes() const { return m_CanUpdateNodes; }
	
protected:
	bool m_CanUpdateNodes;
};
