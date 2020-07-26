/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_ECS_COMPONENTS_H
#define MAR_ENGINE_ECS_COMPONENTS_H

#include "../../../mar.h"


namespace mar {
	namespace ecs {


		struct IdentifierComponent {
			float id{ 0.f };
			std::string name{ "empty" };

			IdentifierComponent() = default;
			IdentifierComponent(const IdentifierComponent& id) = default;
			IdentifierComponent(float i, std::string n)
				: id(i),
				name(n)
			{}

			operator float() { return id; }
			operator const float() const { return id; }

			operator std::string() { return name; }
			operator const std::string() const { return name; }
		};

		struct RenderableComponent {
			std::vector<float> vertices;
			std::vector<uint32_t> indices;
			std::vector<uint32_t> layout;
			uint32_t stride;

			RenderableComponent() = default;
			RenderableComponent(const RenderableComponent& ren) = default;
			RenderableComponent(const std::vector<float>& ver, const std::vector<uint32_t>& ind,
								const std::vector<uint32_t>& lay, const uint32_t& str)
				: vertices(ver),
				indices(ind),
				layout(lay),
				stride(str)
			{}

		};

		struct TransformComponent {
			maths::mat4 scale;
			maths::mat4 rotation;
			maths::mat4 translation;

			maths::mat4 transform{ 1.f };

			TransformComponent() = default;
			TransformComponent(const TransformComponent& tc) = default;
			TransformComponent(const maths::mat4& mat)
				: transform(mat) 
			{}

			operator maths::mat4() { return transform; }
			operator const maths::mat4() const { return transform; }
		};

		struct ColorComponent {
			float id{ 0.f };
			maths::vec3 color{ 0.5f, 0.5f, 0.5f };

			ColorComponent() = default;
			ColorComponent(const ColorComponent& cc) = default;
			ColorComponent(const maths::vec3& col)
				: color(col)
			{}

			operator maths::vec3() { return color; }
			operator const maths::vec3() const { return color; }
		};

		struct TextureComponent {
			float id{ 0.f };
			std::string texture{ "empty" };

			TextureComponent() = default;
			TextureComponent(const TextureComponent& tex) = default;
			TextureComponent(const std::string& tex)
				: texture(tex)
			{}

			operator std::string() { return texture; }
			operator const std::string() const { return texture; }
		};

} }


#endif // !MAR_ENGINE_ECS_COMPONENTS_H