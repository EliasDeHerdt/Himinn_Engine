#pragma once
#include <XInput.h>
#include <map>
#include "Singleton.h"
#include "Command.h"

namespace Himinn
{
	enum class InputMode {
		down,
		up,
		pressed
	};

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

		weak_ptr<Command> command;
		InputMode mode = InputMode::down;
		bool trigger = false;
	};

	//todo: put 2 - 4 inputmaps in an array, allow the manager to ask which player should be checked for processInput (0, 1, 2 or3)
	class InputManager final : public Singleton<InputManager>
	{
	public:
		~InputManager();
		bool ProcessInput();
		void HandleInput(const GameObject& gameObject, bool controller);
		bool IsPressed(int keyCode) const;

		void AddCommand(std::string tag, Command* command);
		
		void BindButtonInput(int keyCode, std::string commandTag, InputMode mode = InputMode::down);
	private:
		bool m_InputMade = false;
		XINPUT_KEYSTROKE m_Stroke{};
		std::map<int, InputInfo> m_Inputs = {};
		std::map<std::string, shared_ptr<Command>> m_Commands= {};

		void ResetInputs();
	};

}
