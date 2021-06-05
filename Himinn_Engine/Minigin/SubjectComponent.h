#pragma once
#include "Component.h"
#include "Observer.h"
#include <memory>

namespace Himinn
{
	class SubjectComponent : public Component
	{
	public:
		SubjectComponent(const std::weak_ptr<GameObject>& owner);
		virtual ~SubjectComponent() = default;
		SubjectComponent(const SubjectComponent& other) = delete;
		SubjectComponent& operator=(const SubjectComponent& other) = delete;
		SubjectComponent(SubjectComponent&& other) = delete;
		SubjectComponent& operator=(SubjectComponent&& other) = delete;

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

		void Notify(EventInfo eventInfo, unsigned int observerEvent) const;

		template<typename T>
		void NotifySpecific(EventInfo eventInfo, unsigned int observerEvent) const
		{
			for (std::shared_ptr<Observer> comp : m_Observers)
				if (std::dynamic_pointer_cast<T>(comp) != nullptr) {
					comp->OnNotify(eventInfo, observerEvent);
					return;
				}
		}

		bool AddObserver(const std::shared_ptr<Observer>& observer);
		bool RemoveObserver(const std::shared_ptr<Observer>& observer);
	private:
		std::vector<std::shared_ptr<Observer>> m_Observers;
	};
}

