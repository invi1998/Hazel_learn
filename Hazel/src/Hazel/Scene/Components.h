#pragma once
#include "SceneCamera.h"
#include "glm/gtx/quaternion.hpp"
#include "Hazel/Core/UUID.h"
#include "Hazel/Renderer/OrthographicCamera.h"

namespace Hazel
{
	struct IDComponent
	{
		UUID ID;

		IDComponent() = default;
		IDComponent(const IDComponent &) = default;
		IDComponent(const UUID &id) : ID(id) {}
	};

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent &) = default;

		TagComponent(const std::string &tag) : Tag(tag) {}
	};

	struct TransformComponent
	{
		glm::vec3 Translation{0.0f, 0.0f, 0.0f};
		glm::vec3 Rotation{0.0f, 0.0f, 0.0f};
		glm::vec3 Scale{1.0f, 1.0f, 1.0f};

		TransformComponent() = default;
		TransformComponent(const TransformComponent &) = default;

		TransformComponent(const glm::vec3 &translation) : Translation(translation) {}

		glm::mat4 GetTransform() const
		{
			//glm::mat4 rotation = glm::rotate(
			//						 glm::mat4{1.0f}, Rotation.x, {1.0f, 0.0f, 0.0f})			 // 绕x轴旋转
			//					 * glm::rotate(glm::mat4{1.0f}, Rotation.y, {0.0f, 1.0f, 0.0f})	 // 绕y轴旋转
			//					 * glm::rotate(glm::mat4{1.0f}, Rotation.z, {0.0f, 0.0f, 1.0f}); // 绕z轴旋转

			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4{1.0f}, Translation) * rotation * glm::scale(glm::mat4{1.0f}, Scale);
		}
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color{1.0f};

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent &) = default;

		SpriteRendererComponent(const glm::vec4 &color) : Color(color) {}

		/*operator glm::vec4& () { return Color; }
		operator const glm::vec4& () const { return Color; }*/
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent &) = default;
	};

	class ScriptableEntity;

	struct NativeScriptComponent
	{
		// 实体A中加入一个脚本组件，脚本组件关联B类，那么NativeScriptComponent的作用就是让Entity在entt系统对B有访问权
		// 同时让B类对实体A的其他组件也有访问权;

		ScriptableEntity *Instance = nullptr;

		using InstantiateScript = ScriptableEntity *(*)();
		InstantiateScript InstantiateFunction;

		using DestroyScript = void (*)(NativeScriptComponent *);
		DestroyScript DestroyInstanceFunction;

		template <typename T>
		void Bind()
		{
			InstantiateFunction = []()
			{ return static_cast<ScriptableEntity *>(new T()); };
			DestroyInstanceFunction = [](NativeScriptComponent *nsc)
			{delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};
}
