#pragma once
#include "EnemyComponent.h"

#include <memory>
#include "DataTypes.h"

namespace Himinn {
	class ImageComponent;
}

class GridComponent;
class SlickAndSamComponent final : public EnemyComponent
{
public:
	SlickAndSamComponent(const std::weak_ptr<Himinn::GameObject>& owner, const std::weak_ptr<GridComponent>& grid, float moveDelay);
	~SlickAndSamComponent() override = default;
	SlickAndSamComponent(const SlickAndSamComponent& other) = delete;
	SlickAndSamComponent& operator=(const SlickAndSamComponent& other) = delete;
	SlickAndSamComponent(SlickAndSamComponent&& other) = delete;
	SlickAndSamComponent& operator=(SlickAndSamComponent&& other) = delete;

	void FixedUpdate() override;
	void Update() override;
	void LateUpdate() override;
	void Render() override;
	void OnAddedToObject() override;

	void OnOverlap(std::weak_ptr<Himinn::GameObject> other) override;

	void Spawn();

private:
	enum class SpawnType
	{
		Slick,
		Sam
	};

	void Move();
	void AddToNode() const override;
};

