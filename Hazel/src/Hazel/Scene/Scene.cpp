#include "hzpch.h"

#include "Scene.h"

#include "Hazel/Renderer/Renderer.h"

namespace Hazel
{
	static void DoMaths(const glm::mat4& transform)
	{
		
	}

	static void OnTransformConstruct(entt::registry, entt::entity entity)
	{
		
	}

	Scene::Scene()
	{
		struct MeshComponent
		{
			
		};
		struct TransformComponent
		{
			glm::mat4 Transform;

			TransformComponent() = default;
			TransformComponent(const TransformComponent&) = default;

			TransformComponent(const glm::mat4& transform) : Transform(transform){}

			operator glm::mat4& () { return Transform; }
			operator const glm::mat4& () const { return Transform; }
			
		};

		entt::entity entity = m_Registry.create();

		m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));

		if (m_Registry.any_of<TransformComponent>(entity))
		{
			TransformComponent& transform = m_Registry.get<TransformComponent>(entity);
		}

		auto view = m_Registry.view<TransformComponent>();
		for (auto eti : view)
		{
			
			TransformComponent& transform = view.get<TransformComponent>(eti);;
		}

		auto group = m_Registry.group<TransformComponent>(entt::get<MeshComponent>);
		for (auto eti : group)
		{
			auto& [transform, mesh] = group.get<TransformComponent, MeshComponent>(eti);

			Renderer::Submit(mesh, transform);
		}
	}

	Scene::~Scene()
	{
	}
}
