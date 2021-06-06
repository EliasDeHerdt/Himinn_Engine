#include "PlayerObserver.h"

#include "GameObject.h"
#include "LivesComponent.h"
#include "ScoreComponent.h"
#include "PlayerManagerComponent.h"

void PlayerObserver::OnNotify(Himinn::EventInfo eventInfo, unsigned int event)
{
	switch (event)
	{
	case (unsigned)PlayerObserverEvent::PlayerDied:
		if (!m_pLivesText.expired())
			m_pLivesText.lock()->SetCurrentLives(eventInfo.intInfo.at(0));

		if (!m_pPlayerManager.expired())
			m_pPlayerManager.lock()->PlayerDied();
		break;
	case 1:
		if (!m_pScoreText.expired())
			m_pScoreText.lock()->SetCurrentScore(eventInfo.intInfo.at(0));
		break;
	default: break;
	}
}

bool PlayerObserver::SetLivesComponent(std::weak_ptr<Himinn::LivesComponent> livesComp)
{
	if (!livesComp.expired())
	{
		m_pLivesText = livesComp;
		return true;
	}
	return false;
}

bool PlayerObserver::SetScoreComponent(std::weak_ptr<Himinn::ScoreComponent> pointsComp)
{
	if (!pointsComp.expired())
	{
		m_pScoreText = pointsComp;
		return true;
	}
	return false;
}

bool PlayerObserver::SetPlayerManangerComponent(std::weak_ptr<PlayerManagerComponent> playerManangerComp)
{
	if (!playerManangerComp.expired())
	{
		m_pPlayerManager = playerManangerComp;
		return true;
	}
	return false;
}
