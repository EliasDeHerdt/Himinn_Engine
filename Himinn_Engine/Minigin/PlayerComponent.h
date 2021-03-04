#pragma once
#include "Component.h"

namespace Himinn
{
	enum class ScoreGain
	{
		ColorChange = 25,
		DiscRemaining = 50,
		Catch = 300,
		DefeatCoily = 500
	};
	
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

		int GetLives() const;
		int GetScore() const;

		void LoseLife();
		void SetLives(int lives);
		void GainScore(ScoreGain score);
	private:
		int m_Lives;
		int m_Score;

		// Components
		std::weak_ptr<SubjectComponent> m_pSubjectComponent = {};
	};
}

