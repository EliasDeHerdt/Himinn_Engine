#pragma once
#include "Component.h"
#include "DataTypes.h"

namespace Himinn {

	class Texture2D;
	class RenderComponent : public Component
	{
		public:
			RenderComponent(const std::weak_ptr<GameObject>& owner);

			virtual void FixedUpdate() override;
			virtual void Update() override;
			virtual void LateUpdate() override;
			virtual void Render() override;

			virtual void SetTexture(const std::shared_ptr<Texture2D>& texture);
			IVector2 GetTextureDimensions() const;
	
		private:
			std::shared_ptr<Texture2D> m_Texture;
	};
}

