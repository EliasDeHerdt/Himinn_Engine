#pragma once
#include <memory>

#include "Component.h"

namespace Himinn {
	class GameObject;
}

class EnemyComponent : public Himinn::Component
{
public:
	EnemyComponent(const std::weak_ptr<Himinn::GameObject>& owner) : Component(owner) {};
	virtual ~EnemyComponent() override = default;
	EnemyComponent(const EnemyComponent& other) = delete;
	EnemyComponent& operator=(const EnemyComponent& other) = delete;
	EnemyComponent(EnemyComponent&& other) = delete;
	EnemyComponent& operator=(EnemyComponent&& other) = delete;

	virtual void FixedUpdate() override = 0;
	virtual void Update() override = 0;
	virtual void LateUpdate() override = 0;
	virtual void Render() override = 0;
	virtual void OnAddedToObject() override = 0;

	virtual void OnOverlap() = 0;
};

