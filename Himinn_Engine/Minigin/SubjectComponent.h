#pragma once
#include "Component.h"

namespace Himinn
{
	struct EventInfo;
	enum class ObserverEvent;
	
	class Observer;
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

		void Notify(EventInfo eventInfo, ObserverEvent observerEvent) const;

		bool AddObserver(const std::shared_ptr<Observer>& observer);
		bool RemoveObserver(const std::shared_ptr<Observer>& observer);
	private:
		std::vector<std::shared_ptr<Observer>> m_Observers;
	};
}

