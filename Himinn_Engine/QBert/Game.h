#pragma once
#include "Minigin.h"

class Game : public Himinn::Minigin
{
public:
	virtual void LoadGame() const;
	virtual void Cleanup();

private:
	
};

