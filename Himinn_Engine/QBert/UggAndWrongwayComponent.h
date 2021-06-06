#pragma once
#include "EnemyComponent.h"

#include "DataTypes.h"

namespace Himinn {
	class ImageComponent;
}

class GridComponent;
class UggAndWrongwayComponent : public EnemyComponent
{
public:
	UggAndWrongwayComponent(const std::weak_ptr<Himinn::GameObject>& owner, const std::weak_ptr<GridComponent>& grid, float moveDelay);
	virtual ~UggAndWrongwayComponent() override = default;
	UggAndWrongwayComponent(const UggAndWrongwayComponent& other) = delete;
	UggAndWrongwayComponent& operator=(const UggAndWrongwayComponent& other) = delete;
	UggAndWrongwayComponent(UggAndWrongwayComponent&& other) = delete;
	UggAndWrongwayComponent& operator=(UggAndWrongwayComponent&& other) = delete;

	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void OnAddedToObject() override;

	virtual void OnOverlap() override;

	void Spawn();

private:
	enum class SpawnType
	{
		Ugg,
		Wrongway
	};

	bool m_Active;
	float m_MoveTime;
	float m_MoveDelay;
	SpawnType m_TypeToSpawn;
	Himinn::IVector2 m_GridPosition;

	std::weak_ptr<GridComponent> m_pGridComponent;
	std::weak_ptr<Himinn::ImageComponent> m_pImageComponent;

	void Move();
	void AddToNode() const;
	void RemoveFromNode() const;
	bool CheckValidMove() const;
};
