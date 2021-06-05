#pragma once
#include <string>
#include <vector>

#include "DataTypes.h"
#include "Component.h"

namespace Himinn
{
	struct EventInfo
	{
		std::vector<int> intInfo;
		std::vector<float> floatInfo;
		std::vector<std::string> stringInfo;
		std::vector<IVector2> iVector2Info;
		std::vector<std::weak_ptr<void>> pointerInfo;
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
