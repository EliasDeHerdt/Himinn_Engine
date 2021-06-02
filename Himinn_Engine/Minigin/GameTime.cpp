#include "MiniginPCH.h"
#include "GameTime.h"


void Himinn::GameTime::SetDeltaTime(float dt)
{
	m_DeltaTime = dt;
}

float Himinn::GameTime::GetDeltaTime() const
{
	return m_DeltaTime;
}
