/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_ECS_COMPONENTS_H
#define MAR_ENGINE_ECS_COMPONENTS_H

#include "../../../mar.h"


namespace mar {
	namespace ecs {


		enum class EntityComponents {
			DEFAULT_COMPONENT,
			COLOR,
			TEXTURE2D,
			CUBEMAP,
			RENDERABLE,
			TAG,
			TRANSFORM,
			LIGHT
		};

		inline static const std::vector<std::pair<EntityComponents, const char*>> AllExistingComponents {
				{ EntityComponents::DEFAULT_COMPONENT, "DefaultComponent" },
				{ EntityComponents::COLOR, "ColorComponent" },
				{ EntityComponents::TEXTURE2D, "Texture2DComponent" },
				{ EntityComponents::CUBEMAP, "TextureCubemapComponent" },
				{ EntityComponents::RENDERABLE, "RenderableComponent" },
				{ EntityComponents::TAG, "TagComponent" },
				{ EntityComponents::TRANSFORM, "TransformComponent" },
				{ EntityComponents::LIGHT, "LightComponent" }
		};

		struct Components {
			std::vector<EntityComponents> components;

			Components() = default;
			Components(const Components& com) = default;
			Components(const std::vector<EntityComponents>& vec) 
				: components(vec)
			{}

			operator std::vector<EntityComponents>&() { return components; }
			operator const std::vector<EntityComponents>& () const { return components; }
		};

		struct TagComponent {
			std::string tag{ "empty" };

			TagComponent() = default;
			TagComponent(const TagComponent& id) = default;
			TagComponent(std::string t)
				: tag(t)
			{}

			operator std::string&() { return tag; }
			operator const std::string&() const { return tag; }
		};

		struct RenderableComponent {
			std::string id;
			std::vector<float> vertices;
			std::vector<uint32_t> indices;

			RenderableComponent() = default;
			RenderableComponent(const RenderableComponent& ren) = default;
			RenderableComponent(std::string i)
				: id(i)
			{}
			RenderableComponent(std::string i, const std::vector<float>& ver, const std::vector<uint32_t>& ind)
				: id(i),
				vertices(ver),
				indices(ind)
			{}

			operator const std::string& () const { return id; }
			operator const std::vector<float>&() const { return vertices; }
			operator const std::vector<uint32_t>&() const { return indices; }
		};

		struct TransformComponent {
			float general_scale{ 1.f };
			maths::vec3 center{ 0.f, 0.f, 0.f };
			maths::vec3 angles{ 0.f, 0.f, 0.f };
			maths::vec3 scale{1.f, 1.f, 1.f };

			maths::mat4 transform{ 1.f };

			TransformComponent() = default;
			TransformComponent(const TransformComponent& tc) = default;

			operator maths::mat4&() { return transform; }
			operator const maths::mat4&() const { return transform; }
		};

		struct ColorComponent {
			maths::vec3 color{ 0.5f, 0.5f, 0.5f };

			ColorComponent() = default;
			ColorComponent(const ColorComponent& cc) = default;
			ColorComponent(const maths::vec3& col)
				: color(col)
			{}

			operator maths::vec3&() { return color; }
			operator const maths::vec3&() const { return color; }
		};

		struct Texture2DComponent {
			std::string texture{ "empty" };

			Texture2DComponent() = default;
			Texture2DComponent(const Texture2DComponent& tex) = default;
			Texture2DComponent(const std::string& tex)
				: texture(tex)
			{}

			operator std::string&() { return texture; }
			operator const std::string&() const { return texture; }
		};

		struct TextureCubemapComponent {
			std::string cubemap{ "empty" };

			TextureCubemapComponent() = default;
			TextureCubemapComponent(const TextureCubemapComponent& cub) = default;
			TextureCubemapComponent(const std::string& cub)
				: cubemap(cub)
			{}

			operator std::string&() { return cubemap; }
			operator const std::string&() const { return cubemap; }
		};

		struct LightComponent {
			maths::vec3 ambient{ 0.5f, 0.5f, 0.5f };
			maths::vec3 diffuse{ 0.9f, 0.9f, 0.9f };
			maths::vec3 specular{ 0.5f, 0.5f, 0.5f };

			maths::vec3 ambientStrength{ 0.2f, 0.2f, 0.2f };
			maths::vec3 diffuseStrength{ 0.7f, 0.7f, 0.7f };
			maths::vec3 specularStrength{ 1.0f, 1.0f, 1.0f };

			float constant{ 1.0f };
			float linear{ 0.045f };
			float quadratic{ 0.0075f };

			float shininess{ 64.0f };

			LightComponent() = default;
			LightComponent(const LightComponent& li) = default;
		};


} }


#define ECS_DEFAULT ::mar::ecs::EntityComponents::DEFAULT_COMPONENT
#define ECS_RENDERABLE ::mar::ecs::EntityComponents::RENDERABLE
#define ECS_COLOR ::mar::ecs::EntityComponents::COLOR
#define ECS_TEXTURE2D ::mar::ecs::EntityComponents::TEXTURE2D
#define ECS_CUBEMAP ::mar::ecs::EntityComponents::CUBEMAP
#define ECS_TAG ::mar::ecs::EntityComponents::TAG
#define ECS_TRANSFORM ::mar::ecs::EntityComponents::TRANSFORM
#define ECS_LIGHT ::mar::ecs::EntityComponents::LIGHT


#endif // !MAR_ENGINE_ECS_COMPONENTS_H