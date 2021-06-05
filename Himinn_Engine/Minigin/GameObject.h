#pragma once
#include "Transform.h"
#include <vector>

#include "Component.h"

using namespace std;
namespace Himinn
{
	struct IVector2;
	class Texture2D;
	class GameObject final
	{
	public:
		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;
		
		void FixedUpdate();
		void Update();
		void LateUpdate();
		void Render() const;

		void MarkForDestruction();
		bool ShouldBeDestroyed() const;

		void SetPosition(float x, float y);
		void SetPosition(IVector2 position);
		const Transform& GetTransform() const;

		template<typename T>
		typename std::enable_if<std::is_base_of<Component, T>::value, bool>::type
		AddComponent(weak_ptr<Component> component)
		{
			if (component.expired()
				|| component.use_count() != 1
				|| !GetComponent<T>().expired())
				return false;
			
			m_Components.push_back(component.lock());
			std::static_pointer_cast<Component>(component.lock())->OnAddedToObject();
			return true;
		}
		template<typename T>
		typename std::enable_if<std::is_base_of<Component, T>::value, bool>::type
		RemoveComponent()
		{
			weak_ptr<T> search = GetComponent<T>();
			if (search.expired())
				return false;

			m_Components.erase(std::remove_if(m_Components.begin(), m_Components.end(), [search](shared_ptr<Component> rhs)
			{
				return search.lock() == rhs;
			}), m_Components.end());
			return true;
		}
		template<typename T>
		weak_ptr<T> GetComponent() const {
			weak_ptr<T> weak;
			for (shared_ptr<Component> comp : m_Components) {

				weak = std::dynamic_pointer_cast<T>(comp);
				if (weak.lock() != nullptr) {
					return weak;
				}
			}

			return weak;
		}
	private:
		bool m_Destroy;
		
		Transform m_Transform;
		vector<shared_ptr<Component>> m_Components{};
	};
}
