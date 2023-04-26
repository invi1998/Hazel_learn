#pragma once
#include "SceneCamera.h"
#include "ScriptableEntity.h"
#include "glm/gtx/quaternion.hpp"
#include "Hazel/Renderer/OrthographicCamera.h"

namespace Hazel
{
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
			//						 glm::mat4{1.0f}, Rotation.x, {1.0f, 0.0f, 0.0f})			 // ��x����ת
			//					 * glm::rotate(glm::mat4{1.0f}, Rotation.y, {0.0f, 1.0f, 0.0f})	 // ��y����ת
			//					 * glm::rotate(glm::mat4{1.0f}, Rotation.z, {0.0f, 0.0f, 1.0f}); // ��z����ת

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

	struct NativeScriptComponent
	{
		// ʵ��A�м���һ���ű�������ű��������B�࣬ ��ôNativeScriptComponent�����þ�����Entity��enttϵͳ��B�з���Ȩ
		// ͬʱ��B���ʵ��A���������Ҳ�з���Ȩ
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
