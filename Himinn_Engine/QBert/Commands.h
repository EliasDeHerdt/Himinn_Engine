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
	MoveCommand(const std::weak_ptr<Himinn::GameObject>& gameObject, Himinn::QBertDirection direction);
	~MoveCommand() override = default;
	void Execute() override;

	//BAD: done like this due to a lack of time
	void SetGameObject(std::shared_ptr<Himinn::GameObject> gameObject);
private:
	Himinn::QBertDirection m_Direction;
	std::weak_ptr<ControllerComponent> m_CharacterComp;
};