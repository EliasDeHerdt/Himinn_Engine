#pragma once
#include "EnemyComponent.h"

#include "DataTypes.h"

namespace Himinn {
	class ImageComponent;
}

class GridComponent;
class UggAndWrongwayComponent final : public EnemyComponent
{
public:
	UggAndWrongwayComponent(const std::weak_ptr<Himinn::GameObject>& owner, const std::weak_ptr<GridComponent>& grid, float moveDelay);
	 ~UggAndWrongwayComponent() override = default;
	UggAndWrongwayComponent(const UggAndWrongwayComponent& other) = delete;
	UggAndWrongwayComponent& operator=(const UggAndWrongwayComponent& other) = delete;
	UggAndWrongwayComponent(UggAndWrongwayComponent&& other) = delete;
	UggAndWrongwayComponent& operator=(UggAndWrongwayComponent&& other) = delete;

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
		Ugg,
		Wrongway
	};
	
	SpawnType m_TypeToSpawn;

	void Move();
	bool CheckValidMove() const override;
};
