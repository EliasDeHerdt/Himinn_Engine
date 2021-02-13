#include "MiniginPCH.h"
#include "Time.h"


void Himinn::Time::SetDeltaTime(float dt)
{
	m_DeltaTime = dt;
}

float Himinn::Time::GetDeltaTime() const
{
	return m_DeltaTime;
}
