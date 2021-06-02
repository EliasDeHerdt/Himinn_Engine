#pragma once
#include "Singleton.h"

namespace Himinn {
	class GameTime : public Singleton<GameTime>
	{
	public:
		void SetDeltaTime(float dt);
		float GetDeltaTime() const;
	private:
		friend class Singleton<GameTime>;
		GameTime() = default;
		float m_DeltaTime = 0.f;
	};
}


