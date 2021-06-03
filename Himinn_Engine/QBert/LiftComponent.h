#pragma once
#include <memory>
#include <string>
#include <vector>

#include "Component.h"
#include "DataTypes.h"

namespace Himinn {
	class ImageComponent;
}

class LiftComponent : public Himinn::Component
{
public:
	LiftComponent(const weak_ptr<Himinn::GameObject>& owner, const std::string& texture, Himinn::IVector2 gridPosition, Himinn::IVector2 playerOffset = { 0, 0 });
	virtual ~LiftComponent() override = default;
	LiftComponent(const LiftComponent& other) = delete;
	LiftComponent& operator=(const LiftComponent& other) = delete;
	LiftComponent(LiftComponent&& other) = delete;
	LiftComponent& operator=(LiftComponent&& other) = delete;

	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

	const Himinn::IVector2& GetGridPosition() const;
	const Himinn::IVector2& GetPlayerOffset() const;
	void SetPlayerOffset(Himinn::IVector2 offset);


	void Activate();

private:
	std::string m_TexturePaths;
	Himinn::IVector2 m_PlayerOffset;
	Himinn::IVector2 m_GridPosition;

	// Components
	std::weak_ptr<Himinn::ImageComponent> m_pImageComponent;
};
