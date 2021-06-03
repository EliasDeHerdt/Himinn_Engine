#include "Commands.h"

#include <iostream>

#include "CharacterComponent.h"
#include "GameObject.h"

// Move Command
//*********************
MoveCommand::MoveCommand(std::weak_ptr<Himinn::GameObject> gameObject, Himinn::QBertDirection direction)
	: Command(gameObject)
	, m_Direction(direction)
	, m_CharacterComp(gameObject.lock()->GetComponent<CharacterComponent>())
{
}

void MoveCommand::Execute()
{
	if (m_CharacterComp.expired())
	{
		std::cout << "MoveCommand: No CharacterComponent found to move.";
		return;
	}

	m_CharacterComp.lock()->Move(m_Direction);
}