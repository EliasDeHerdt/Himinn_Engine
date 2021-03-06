#include "MiniginPCH.h"
#include "Command.h"
#include <iostream>

#include "GameObject.h"
#include "PlayerComponent.h"
#include "SoundServiceLocator.h"

Himinn::JumpCommand::JumpCommand(weak_ptr<GameObject> gameObject)
	: Command(gameObject)
{
}

void Himinn::JumpCommand::Execute()
{
	std::cout << "Jump\n";
}

Himinn::ObjectDiesCommand::ObjectDiesCommand(weak_ptr<GameObject> gameObject)
	: Command(gameObject)
{
	m_pPlayerComponent = gameObject.lock()->GetComponent<PlayerComponent>();
	if (m_pPlayerComponent.expired())
		std::cout << "ObjectDiesCommand: GameObject does not own a PlayerComponent\n";
}

void Himinn::ObjectDiesCommand::Execute()
{
	if (m_pPlayerComponent.expired())
		return;

	SoundServiceLocator::GetSoundSystem()->QueueSound(0, 100);
}

Himinn::GainScoreCommand::GainScoreCommand(std::weak_ptr<GameObject> gameObject, ScoreGain scoreGain)
	: Command(gameObject)
	, m_ScoreGain{scoreGain}
{
	m_pPlayerComponent = gameObject.lock()->GetComponent<PlayerComponent>();
	if (m_pPlayerComponent.expired())
		std::cout << "GainScoreCommand: GameObject does not own a PlayerComponent\n";
}

void Himinn::GainScoreCommand::Execute()
{
	if (m_pPlayerComponent.expired())
		return;

	SoundServiceLocator::GetSoundSystem()->QueueSound(0, 100);
}
