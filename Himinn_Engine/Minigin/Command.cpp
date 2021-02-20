#include "MiniginPCH.h"
#include "Command.h"
#include <iostream>

void FireCommand::Execute(const GameObject&)
{
	std::cout << "Fire\n";
}

void DuckCommand::Execute(const GameObject&)
{
	std::cout << "Duck\n";
}

void JumpCommand::Execute(const GameObject&)
{
	std::cout << "Jump\n";
}

void FartCommand::Execute(const GameObject&)
{
	std::cout << "Fart\n";
}
