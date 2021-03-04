#pragma once
#include "Observer.h"

namespace Himinn
{
	class LivesComponent;
	class ScoreComponent;
	class PlayerObserver final : public Observer
	{
	public:
		PlayerObserver() = default;
		~PlayerObserver() = default;
		PlayerObserver(const PlayerObserver& other) = delete;
		PlayerObserver& operator=(const PlayerObserver& other) = delete;
		PlayerObserver(PlayerObserver&& other) = delete;
		PlayerObserver& operator=(PlayerObserver&& other) = delete;

		void OnNotify(EventInfo eventInfo, ObserverEvent event) override;

		bool SetLivesComponent(std::weak_ptr<LivesComponent> livesComp);
		bool SetScoreComponent(std::weak_ptr<ScoreComponent> pointsComp);
	private:
		std::weak_ptr<LivesComponent> m_pLivesText = {};
		std::weak_ptr<ScoreComponent> m_pScoreText = {};
	};
}