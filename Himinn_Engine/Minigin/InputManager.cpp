#include "MiniginPCH.h"
#include "InputManager.h"
#include "GameObject.h"
#include <SDL.h>

bool Himinn::InputManager::ProcessInput()
{
	ZeroMemory(&m_Stroke, sizeof(XINPUT_KEYSTROKE));
	const int controllerInputCheck = XInputGetKeystroke(0, 0, &m_Stroke);

	if (m_InputMade)
		ResetInputs();
	
	if (controllerInputCheck == 0)
	{
		for (auto& input : m_Inputs)
		{
			input.second.trigger = (m_Stroke.VirtualKey == input.first && (m_Stroke.Flags == input.second.GetInputMode()));
			if (!m_InputMade)
				m_InputMade = input.second.trigger;
		}
	}
	
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {
			
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			
		}
	}

	return true;
}

void Himinn::InputManager::HandleInput(const GameObject& gameObject, bool controller)
{
	if (controller)
	{
		for (const auto& input : m_Inputs)
			if (IsPressed(input.first))
				input.second.command.lock()->Execute(gameObject);
	}
	else
	{
		
	}
}

bool Himinn::InputManager::IsPressed(int keyCode) const
{
	return m_Inputs.at(keyCode).trigger;
}

// Add or override a command
// todo: potential leak on swapping command under tag (shouldn't happen in practice, but shouldn't cause problems either)
void Himinn::InputManager::AddCommand(std::string tag, Command* command)
{
	//if (m_Commands.find(tag) != m_Commands.end())
	//	delete m_Commands.at(tag);
	
	m_Commands.emplace(tag, command);
}

void Himinn::InputManager::BindButtonInput(int keyCode, std::string commandTag, InputMode mode)
{
	m_Inputs.emplace(keyCode, InputInfo{ m_Commands.at((commandTag)), mode });
}

void Himinn::InputManager::ResetInputs()
{
	m_InputMade = false;
	for (auto& input : m_Inputs)
		input.second.trigger = false;
}