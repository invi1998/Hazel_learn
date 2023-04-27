#pragma once

#include "Hazel/Core/Timestep.h"

#include "entt.hpp"
#include "Hazel/Renderer/EditorCamera.h"

namespace Hazel
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		void OnUpdateRuntime(Timestep ts);
		void OnUpdateEditor(Timestep ts, EditorCamera& camera);

		// temp
		entt::registry& Reg() { return m_Registry; }

		Entity CreateEntity(const std::string& name = std::string());

		void OnViewportResize(uint32_t width, uint32_t height);

		void DestroyEntity(Entity entity);

		Entity GetPrimaryCameraEntity();

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0;
		uint32_t m_ViewportHeight = 0;

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};

}
