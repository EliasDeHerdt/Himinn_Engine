#pragma once
#include <vector>

#include "ControllableComponent.h"
#include "DataTypes.h"
#include "EnemyComponent.h"

class ControllerComponent;

namespace Himinn {
	class ImageComponent;
}

class GridComponent;
class CoilyComponent final : public EnemyComponent, public ControllableComponent
{
public:
	CoilyComponent(const std::weak_ptr<Himinn::GameObject>& owner, const std::weak_ptr<GridComponent>& grid, const std::vector<std::weak_ptr<ControllerComponent>>& players, float moveDelay);
	virtual ~CoilyComponent() override = default;
	CoilyComponent(const CoilyComponent& other) = delete;
	CoilyComponent& operator=(const CoilyComponent& other) = delete;
	CoilyComponent(CoilyComponent&& other) = delete;
	CoilyComponent& operator=(CoilyComponent&& other) = delete;

	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void OnAddedToObject() override;

	virtual void OnDeath() override;
	virtual void OnScore(int amount) override;
	virtual void OnOverlap(std::weak_ptr<Himinn::GameObject> other) override;

	void Spawn();
	bool SetControllerComponent(std::weak_ptr<ControllerComponent> controllerComp);

private:
	bool m_Snake;
	bool m_IsControlled;

	int m_TargetID;
	Himinn::IVector2 m_Target;
	const std::vector<std::weak_ptr<ControllerComponent>> m_Players;

	std::weak_ptr<ControllerComponent> m_pControllerComponent;
	
	void Move();
	void CheckForHatch();
	bool UpdateTarget();
};
