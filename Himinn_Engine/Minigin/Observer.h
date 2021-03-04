#pragma once

namespace Himinn
{
	struct EventInfo
	{
		int intInfo;
		float floatInfo;
		std::string stringInfo;
	};
	
	enum class ObserverEvent
	{
		PlayerDied,
		PlayerScore,
		PointsColorChange,
		PointsDefeatCoily,
		PointsDiscsLeft,
		PointsCatchSlick,
		PointsCatchSam
	};
	
	class GameObject;
	class Observer
	{
	public:
		Observer() = default;
		virtual ~Observer() = default;
		Observer(const Observer& other) = delete;
		Observer& operator=(const Observer& other) = delete;
		Observer(Observer&& other) = delete;
		Observer& operator=(Observer&& other) = delete;

		virtual void OnNotify(EventInfo eventInfo, ObserverEvent event) = 0;
	};
}