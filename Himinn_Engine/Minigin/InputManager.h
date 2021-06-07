#pragma once
#include <XInput.h>
#include <map>
#include <string>
#include <memory>
#include "Singleton.h"

namespace Himinn
{
	class Command;
	enum class InputMode {
		down,
		up,
		pressed
	};

	struct ButtonInfo
	{
		unsigned playerId;
		unsigned key;
	};

	inline bool operator< (const ButtonInfo& lhs, const ButtonInfo& rhs)
	{
		if (lhs.playerId < rhs.playerId)
			return true;
		if (lhs.playerId > rhs.playerId)
			return false;
		if (lhs.key < rhs.key)
			return true;
		return false;
	}

	struct InputInfo
	{
		// Returns the InputMode, but converted to the correct flag
		int GetInputMode() const
		{
			switch (mode)
			{
			case InputMode::down:
				return 1;
			case InputMode::up:
				return 2;
			case InputMode::pressed:
				return 5;
			}
			return 0;
		}

		std::weak_ptr<Command> command;
		InputMode mode = InputMode::down;

		bool IsController = true;
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
		//bool IsPressed(int keyCode) const;

		void AddCommand(std::string tag, std::shared_ptr<Command> command);
		
		void BindButtonInput(unsigned player, unsigned keyCode, std::string commandTag, InputMode mode = InputMode::down);
		void BindButtonInput(unsigned keyCode, std::string commandTag, InputMode mode = InputMode::down);
		void SetAmountOfPlayers(int amountOfPlayers);

		void StopGame() { m_StopGame = true; }
	private:
		bool m_StopGame = false;
		bool m_InputMade = false;
		int m_AmountOfControllers = 1;
		XINPUT_KEYSTROKE m_Stroke{};
		std::map<ButtonInfo, InputInfo> m_Inputs = {};
		std::map<std::string, std::shared_ptr<Command>> m_Commands= {};

		//void ResetInputs();
	};

}
