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

	Himinn::IVector2 position{ m_CharacterComp.lock()->GetGridPosition() };
	switch (m_Direction)
	{
	case Himinn::QBertDirection::TopLeft:
		m_CharacterComp.lock()->SetGridPosition(position.x - 1, position.y - 1);
		break;
	case Himinn::QBertDirection::TopRight:
		m_CharacterComp.lock()->SetGridPosition(position.x - 1, position.y);
		break;
	case Himinn::QBertDirection::BottomLeft:
		m_CharacterComp.lock()->SetGridPosition(position.x + 1, position.y);
		break;
	case Himinn::QBertDirection::BottomRight:
		m_CharacterComp.lock()->SetGridPosition(position.x + 1, position.y + 1);
		break;
	default: ;
	}
}