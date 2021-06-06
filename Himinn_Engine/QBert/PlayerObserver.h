#pragma once
#include "Observer.h"

namespace Himinn {
	class LivesComponent;
	class ScoreComponent;
}

enum class PlayerObserverEvent
{
	PlayerDied,
	PlayerScore
};

class PlayerManagerComponent;
class PlayerObserver final : public Himinn::Observer
{
public:
	PlayerObserver() = default;
	~PlayerObserver() override = default;
	PlayerObserver(const PlayerObserver& other) = delete;
	PlayerObserver& operator=(const PlayerObserver& other) = delete;
	PlayerObserver(PlayerObserver&& other) = delete;
	PlayerObserver& operator=(PlayerObserver&& other) = delete;

	void OnNotify(Himinn::EventInfo eventInfo, unsigned int event) override;

	bool SetLivesComponent(std::weak_ptr<Himinn::LivesComponent> livesComp);
	bool SetScoreComponent(std::weak_ptr<Himinn::ScoreComponent> pointsComp);
	bool SetPlayerManangerComponent(std::weak_ptr<PlayerManagerComponent> playerManangerComp);
private:
	std::weak_ptr<Himinn::LivesComponent> m_pLivesText;
	std::weak_ptr<Himinn::ScoreComponent> m_pScoreText;
	std::weak_ptr<PlayerManagerComponent> m_pPlayerManager;
};