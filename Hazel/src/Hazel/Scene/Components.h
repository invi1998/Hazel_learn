#pragma once
#include "SceneCamera.h"
#include "ScriptableEntity.h"
#include "Hazel/Renderer/OrthographicCamera.h"

namespace Hazel
{
	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;

		TagComponent(const std::string& tag) : Tag(tag) {}
	};

	struct TransformComponent
	{
		glm::mat4 Transform{1.0f};

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;

		TransformComponent(const glm::mat4& transform) : Transform(transform) {}

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color{1.0f};

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;

		SpriteRendererComponent(const glm::vec4& color) : Color(color) {}

		/*operator glm::vec4& () { return Color; }
		operator const glm::vec4& () const { return Color; }*/

	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true;
		bool FixedAspectRadio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

	};

	struct NativeScriptComponent
	{
		// ʵ��A�м���һ���ű�������ű��������B�࣬ ��ôNativeScriptComponent�����þ�����Entity��enttϵͳ��B�з���Ȩ
		// ͬʱ��B���ʵ��A���������Ҳ�з���Ȩ
		ScriptableEntity* Instance = nullptr;

		std::function<void()> InstantiateFunction;
		std::function<void()> DestroyInstanceFunction;

		std::function<void(ScriptableEntity*)> OnCreateFunction;
		std::function<void(ScriptableEntity*)> OnDestroyFunction;
		std::function<void(ScriptableEntity*, Timestep)> OnUpdateFunction;

		template<typename T>
		void Bind()
		{
			InstantiateFunction = [this]() { Instance = new T(); };
			DestroyInstanceFunction = [this]() {delete static_cast<T*>(Instance); Instance = nullptr; };

			OnCreateFunction = [](ScriptableEntity* Instance) {static_cast<T*>(Instance)->OnCreate(); };
			OnDestroyFunction = [](ScriptableEntity* Instance) {static_cast<T*>(Instance)->OnDestroy(); };
			OnUpdateFunction = [](ScriptableEntity* Instance, Timestep ts) {static_cast<T*>(Instance)->OnUpdate(ts); };
		}
	};
}
