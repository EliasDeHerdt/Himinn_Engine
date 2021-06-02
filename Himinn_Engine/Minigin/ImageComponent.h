#pragma once
#include "Component.h"
#include "RenderComponent.h"
#include "DataTypes.h"

namespace Himinn {

	class ImageComponent : public Component
	{
	public:
		ImageComponent(const std::weak_ptr<GameObject>& owner, const string& filename);

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

		void SetTexture(const std::string& filename) const;
		IVector2 GetTextureDimensions() const;

	private:
		// Components
		std::weak_ptr<RenderComponent> m_RenderComponent;
	};
}