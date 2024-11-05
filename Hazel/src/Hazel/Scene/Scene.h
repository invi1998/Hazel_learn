#pragma once

#include "Hazel/Core/Timestep.h"

#include "entt.hpp"
#include "Hazel/Core/UUID.h"
#include "Hazel/Renderer/EditorCamera.h"

#include "box2d/box2d.h"

namespace Hazel
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		void OnRuntimeStart();
		void OnRuntimeEnd();

		void OnUpdateRuntime(Timestep ts);
		void OnUpdateEditor(Timestep ts, EditorCamera& camera);

		// temp
		entt::registry& Reg() { return m_Registry; }

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUUID(const std::string& name, UUID uuid);

		void OnViewportResize(uint32_t width, uint32_t height);

		void DestroyEntity(Entity entity);

		Entity GetEntityByUUID(UUID uuid);

		Entity GetPrimaryCameraEntity();

		static std::shared_ptr<Scene> Copy(std::shared_ptr<Scene> otherScene);

		Entity DuplicateEntity(Entity entity);

		void OnPhysics2DStart();
		void OnPhysics2DStop();

		void OnSimulationStart();
		void OnSimulationStop();

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0;
		uint32_t m_ViewportHeight = 0;
		
		b2WorldId m_PhysicsWorld;

		float m_RestitutionThreshold = 0.5f;	// 弹性阈值（物理计算的最小速度，如果速度小于这个值，就认为是0，防止物体抖动）

		std::unordered_map<UUID, entt::entity> m_EntityMap;

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};

}
