#pragma once
#include "Singleton.h"

namespace Himinn {
	class Time : public Singleton<Time>
	{
	public:
		void SetDeltaTime(float dt);
		float GetDeltaTime() const;
	private:
		friend class Singleton<Time>;
		Time() = default;
		float m_DeltaTime = 0.f;
	};
}


