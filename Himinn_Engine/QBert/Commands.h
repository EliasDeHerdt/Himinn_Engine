#pragma once
#include <memory>

#include "Command.h"
#include "DataTypes.h"

namespace Himinn {
	class GameObject;
}

enum class ScoreGain;
class ControllerComponent;

class MoveCommand final : public Himinn::Command
{
public:
	MoveCommand(std::weak_ptr<Himinn::GameObject> gameObject, Himinn::QBertDirection direction);
	~MoveCommand() override = default;
	void Execute() override;
private:
	Himinn::QBertDirection m_Direction;
	std::weak_ptr<ControllerComponent> m_CharacterComp;
};