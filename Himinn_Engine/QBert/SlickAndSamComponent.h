#pragma once
#include "EnemyComponent.h"

#include <memory>
#include "DataTypes.h"

namespace Himinn {
	class ImageComponent;
}

class GridComponent;
class SlickAndSamComponent : public EnemyComponent
{
public:
	SlickAndSamComponent(const std::weak_ptr<Himinn::GameObject>& owner, const std::weak_ptr<GridComponent>& grid, float moveDelay);
	virtual ~SlickAndSamComponent() override = default;
	SlickAndSamComponent(const SlickAndSamComponent& other) = delete;
	SlickAndSamComponent& operator=(const SlickAndSamComponent& other) = delete;
	SlickAndSamComponent(SlickAndSamComponent&& other) = delete;
	SlickAndSamComponent& operator=(SlickAndSamComponent&& other) = delete;

	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void OnAddedToObject() override;

	virtual void OnOverlap(std::weak_ptr<Himinn::GameObject> other) override;

	void Spawn();

private:
	enum class SpawnType
	{
		Slick,
		Sam
	};

	void Move();
};

