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

	virtual void OnOverlap() override;

	void Spawn();

private:
	enum class SpawnType
	{
		Slick,
		Sam
	};
	
	bool m_Active;
	float m_MoveTime;
	float m_MoveDelay;
	Himinn::IVector2 m_GridPosition;

	std::weak_ptr<GridComponent> m_pGridComponent;
	std::weak_ptr<Himinn::ImageComponent> m_pImageComponent;

	void Move();
	void AddToNode() const;
	void RemoveFromNode() const;
	bool CheckValidMove() const;
};

