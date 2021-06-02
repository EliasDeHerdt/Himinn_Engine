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
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
		//bool IsPressed(int keyCode) const;

		void AddCommand(std::string tag, Command* command);
		
		void BindButtonInput(unsigned player, unsigned keyCode, std::string commandTag, InputMode mode = InputMode::down);
		void SetAmountOfPlayers(int amountOfPlayers);
	private:
		bool m_InputMade = false;
		int m_AmountOfPlayers = 1;
		XINPUT_KEYSTROKE m_Stroke{};
		std::map<ButtonInfo, InputInfo> m_Inputs = {};
		std::map<std::string, std::shared_ptr<Command>> m_Commands= {};

		//void ResetInputs();
	};

}
