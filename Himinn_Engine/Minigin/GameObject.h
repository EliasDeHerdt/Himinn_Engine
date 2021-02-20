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
	class GameObject final : public SceneObject
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
		bool m_ShouldRender = true;
		Transform m_Transform;
		vector<shared_ptr<Component>> m_Components{};
	};
}
