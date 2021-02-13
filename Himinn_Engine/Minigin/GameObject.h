#pragma once
#include "Transform.h"
#include "SceneObject.h"
#include "SceneObject.h"
#include <unordered_map>

namespace Himinn
{
	class Component;
	class Texture2D;
	class GameObject : public SceneObject
	{
	public:
		void Update() override;
		void LateUpdate() override;
		void FixedUpdate() override;
		void Render() const override;

		void SetTexture(const std::string& filename);
		void SetPosition(float x, float y);

		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		/*template<typename T>
		T GetComponent() {
			if (m_Components.find(typeid(T)) != m_Components.end())
				return m_Components.at(typeid(T));
		}*/
	private:
		Transform m_Transform;
		std::shared_ptr<Texture2D> m_Texture{};
		//std::unordered_map<std::type_info, Component*> m_Components{};
	};
}
