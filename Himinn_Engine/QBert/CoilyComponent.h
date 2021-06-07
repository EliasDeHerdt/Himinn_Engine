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
	~CoilyComponent() override = default;
	CoilyComponent(const CoilyComponent& other) = delete;
	CoilyComponent& operator=(const CoilyComponent& other) = delete;
	CoilyComponent(CoilyComponent&& other) = delete;
	CoilyComponent& operator=(CoilyComponent&& other) = delete;

	void FixedUpdate() override;
	void Update() override;
	void LateUpdate() override;
	void Render() override;
	void OnAddedToObject() override;

	void OnDeath() override;
	void OnScore(int amount) override;
	void OnOverlap(std::weak_ptr<Himinn::GameObject> other) override;

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
