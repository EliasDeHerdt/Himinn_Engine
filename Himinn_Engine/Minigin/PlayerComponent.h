#pragma once
#include "Component.h"

namespace Himinn
{
	class SubjectComponent;
	class PlayerComponent : public Component
	{
	public:
		PlayerComponent(const std::weak_ptr<GameObject>& owner, int lives);
		virtual ~PlayerComponent() = default;
		PlayerComponent(const PlayerComponent& other) = delete;
		PlayerComponent& operator=(const PlayerComponent& other) = delete;
		PlayerComponent(PlayerComponent&& other) = delete;
		PlayerComponent& operator=(PlayerComponent&& other) = delete;

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
	private:

		// Components
		std::weak_ptr<SubjectComponent> m_pSubjectComponent = {};
	};
}

