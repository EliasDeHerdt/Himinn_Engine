#pragma once
#include <string>

namespace Himinn
{
	struct EventInfo
	{
		int intInfo;
		float floatInfo;
		std::string stringInfo;
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

		virtual void OnNotify(EventInfo eventInfo, unsigned int event) = 0;
	};
}