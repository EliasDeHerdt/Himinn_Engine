#include "MiniginPCH.h"
#include "PlayerObserver.h"

#include "GameObject.h"
#include "LivesComponent.h"
#include "ScoreComponent.h"

void Himinn::PlayerObserver::OnNotify(EventInfo eventInfo, unsigned int event)
{
	switch (event)
	{
	case 0:
		m_pLivesText.lock()->SetCurrentLives(eventInfo.intInfo);
		break;
	case 1:
		m_pScoreText.lock()->SetCurrentScore(eventInfo.intInfo);
		break;
	default: break;
	}
}

bool Himinn::PlayerObserver::SetLivesComponent(std::weak_ptr<LivesComponent> livesComp)
{
	if (!livesComp.expired())
	{
		m_pLivesText = livesComp;
		return true;
	}
	return false;
}

bool Himinn::PlayerObserver::SetScoreComponent(std::weak_ptr<ScoreComponent> pointsComp)
{
	if (!pointsComp.expired())
	{
		m_pScoreText = pointsComp;
		return true;
	}
	return false;
}
