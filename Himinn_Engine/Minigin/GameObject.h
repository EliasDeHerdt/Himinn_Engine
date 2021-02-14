#pragma once
#include "Transform.h"
#include "SceneObject.h"
#include <vector>

#include "Component.h"
#include "TextComponent.h"

using namespace std;
namespace Himinn
{
	class Texture2D;
	class GameObject : public SceneObject
	{
	public:
		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;
		
		void FixedUpdate() override;
		void Update() override;
		void LateUpdate() override;
		void Render() const override;

		void SetPosition(float x, float y);
		const Transform& GetTransform() const;

		bool AddComponent(shared_ptr<Component> component);
		template<typename T>
		shared_ptr<T> GetComponent() const {
			shared_ptr<T> pointer;
			for (shared_ptr<Component> comp : m_Components) {

				pointer = dynamic_cast<shared_ptr<T>>(comp);
				if (pointer != nullptr)
					return pointer;
			}

			return nullptr;
		}
	private:
		bool m_ShouldRender = true;
		Transform m_Transform;
		vector<shared_ptr<Component>> m_Components{};
	};
}
