#include "MiniginPCH.h"
#include "InputManager.h"
#include "Command.h"
#include <SDL.h>

bool Himinn::InputManager::ProcessInput()
{
	//ZeroMemory(&m_Stroke, sizeof(XINPUT_KEYSTROKE));
	for (int i = 0; i < m_AmountOfControllers; ++i)
	{
		if (XInputGetKeystroke(i, 0, &m_Stroke) == 0)
		{
			for (auto& input : m_Inputs)
			{
				if (i == (int)input.first.playerId 
					&& m_Stroke.VirtualKey == input.first.key 
					&& m_Stroke.Flags == input.second.GetInputMode())
					input.second.command.lock()->Execute();
			}
		}
	}
	
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		for (auto input : m_Inputs)
		{
			if (input.second.IsController)
				continue;

			if (input.first.key == (unsigned)e.key.keysym.sym)
			{
				switch (input.second.mode)
				{
				case InputMode::down: 
					if (e.type & SDL_KEYDOWN)
						input.second.command.lock()->Execute();
					break;
				case InputMode::up:
					if (e.type & SDL_KEYUP)
						input.second.command.lock()->Execute();
					break;
				case InputMode::pressed:
					if (e.type & SDL_KEYDOWN)
						input.second.command.lock()->Execute();
					break;
				default: ;
				}
			}
		}
		if (e.type == SDL_QUIT) {
			return false;
		}
	}

	return !m_StopGame;
}

//bool Himinn::InputManager::IsPressed(int keyCode) const
//{
//	return m_Inputs.at(keyCode).trigger;
//}

// Add or override a command
// todo: potential leak on swapping command under tag (shouldn't happen in practice, but shouldn't cause problems either)
void Himinn::InputManager::AddCommand(std::string tag, std::shared_ptr<Command> command)
{
	//if (m_Commands.find(tag) != m_Commands.end())
	//	delete m_Commands.at(tag);
	
	m_Commands.emplace(tag, command);
}

void Himinn::InputManager::BindButtonInput(unsigned player, unsigned keyCode, std::string commandTag, InputMode mode)
{
	m_Inputs.emplace(ButtonInfo{ player, keyCode }, InputInfo{ m_Commands.at((commandTag)), mode });
}

void Himinn::InputManager::BindButtonInput(unsigned keyCode, std::string commandTag, InputMode mode)
{
	m_Inputs.emplace(ButtonInfo{ 0, keyCode }, InputInfo{ m_Commands.at((commandTag)), mode, false });
}

void Himinn::InputManager::SetAmountOfPlayers(int amountOfPlayers)
{
	if (amountOfPlayers > 0)
		m_AmountOfControllers = amountOfPlayers;
	else
		m_AmountOfControllers = 1;
}
