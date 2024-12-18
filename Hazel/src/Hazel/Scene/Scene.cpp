#include "hzpch.h"

#include "Scene.h"

#include "Components.h"
#include "Entity.h"

#include "ScriptableEntity.h"
#include "Hazel/Physics/Physics2D.h"

#include "Hazel/Renderer/Renderer.h"
#include "Hazel/Renderer/Renderer2D.h"


namespace Hazel
{
	template<typename ...Comp>
	static void CopyComponents(entt::registry& srcRegistry, entt::registry& dstRegistry, std::unordered_map<UUID, entt::entity>& entMap)
	{
		// 使用折叠表达式展开参数包，同时lambda表达式中使用逗号运算次执行多个语句
		auto process_type = [&]<typename T>()
		{
			// 这里的view其实也是一个类的成员函数模板，所以需要加上.template
			auto view = srcRegistry.view<T>();
			for (auto srcEntity : view)
			{
				entt::entity dstEntity = entMap.at(srcRegistry.get<IDComponent>(srcEntity).ID);
				auto& srcComponent = srcRegistry.get<T>(srcEntity);
				dstRegistry.emplace_or_replace<T>(dstEntity, srcComponent);
			}
		};

		//  .template用于显式告诉编译器，后面的operator()是一个函数模板（process_type.operator() 可能会被误认为是一个非模板成员函数，因此需要加上 .template 来明确指出这是一个模板函数。
		(process_type.template operator()<Comp>(), ...);

		//// 使用匿名lambda表达式 也是一样的
		//(
		//	[&]<typename T>()
		//	{
		//		auto view = srcRegistry.view<T>();
		//		for (auto srcEntity : view)
		//		{
		//			entt::entity dstEntity = entMap.at(srcRegistry.get<IDComponent>(srcEntity).ID);
		//			auto& srcComponent = srcRegistry.get<T>(srcEntity);
		//			dstRegistry.emplace_or_replace<T>(dstEntity, srcComponent);
		//		}
		//	}.template operator()<Comp>(), ...
		//);
	}

	//// 所以上述代码可以这样写
	//template<typename ...Comp>
	//static void CopyComponents(entt::registry& srcRegistry, entt::registry& dstRegistry, std::unordered_map<UUID, entt::entity>& entMap)
	//{
	//	(
	//		[&]()
	//		{
	//			// 正常来说，在模板函数内部调用类的模板成员函数时，需要在调用前加上 .template
	//			auto view = srcRegistry.template view<Comp>();
	//			for (auto srcEntity : view)
	//			{
	//				entt::entity dstEntity = entMap.at(srcRegistry.get<IDComponent>(srcEntity).ID);
	//				auto& srcComponent = srcRegistry.get<Comp>(srcEntity);
	//				dstRegistry.emplace_or_replace<Comp>(dstEntity, srcComponent);
	//			}
	//		}(), ...
	//	);
	//}

	template<typename ...Comp>
	static void CopyComponents(ComponentList<Comp...>, entt::registry& srcRegistry, entt::registry& dstRegistry, std::unordered_map<UUID, entt::entity>& entMap)
	{
		CopyComponents<Comp...>(srcRegistry, dstRegistry, entMap);
	}

	template<typename ...Comp>
	static void CopyComponentIfExists(Entity srcEntity, Entity dstEntity)
	{
		auto process_type = [&]<typename T>()
		{
			if (srcEntity.HasComponent<T>())
			{
				auto& srcComponent = srcEntity.GetComponent<T>();
				dstEntity.AddOrReplaceComponent<T>(srcComponent);
			}
		};

		(process_type.template operator()<Comp>(), ...);
	}

	template<typename ...Comp>
	static void CopyComponentIfExists(ComponentList<Comp...>, Entity srcEntity, Entity dstEntity)
	{
		CopyComponentIfExists<Comp...>(srcEntity, dstEntity);
	}

	Scene::Scene()
	{
		// m_Registry.on_construct<CameraComponent>().connect<&Function>();
	}

	Scene::~Scene()
	{
	}

	void Scene::RenderScene(EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);

		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
		}

		Renderer2D::EndScene();
	}

	void Scene::OnRuntimeStart()
	{
		m_IsRunning = true;
		OnPhysics2DStart();
	}

	void Scene::OnRuntimeEnd()
	{
		m_IsRunning = false;
		OnPhysics2DStop();
	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
	{
		RenderScene(camera);
	}

	void Scene::OnUpdateSimulation(Timestep ts, EditorCamera& camera)
	{
		
		if (!m_IsPaused || m_StepFrames-- > 0)
		{
			// 物理模拟
			{
				const int32_t velocityIterations = 6;
				const int32_t positionIterations = 2;

				b2World_Step(m_PhysicsWorld, ts, positionIterations);
				for (auto e : m_Registry.view<TransformComponent, Rigidbody2DComponent>())
				{
					Entity entity = { e, this };
					auto& transform = entity.GetComponent<TransformComponent>();
					auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

					transform.Translation.x = b2Body_GetPosition(rb2d.RuntimeBody).x;
					transform.Translation.y = b2Body_GetPosition(rb2d.RuntimeBody).y;
					transform.Rotation.z = b2Rot_GetAngle(b2Body_GetRotation(rb2d.RuntimeBody));
				}
			}
		}
		// 渲染
		RenderScene(camera);
	}

	void Scene::OnUpdateRuntime(Timestep ts)
	{
		if (!m_IsPaused || m_StepFrames-- > 0)
		{
			// update scripts
			{
				m_Registry.view<NativeScriptComponent>().each(
					[=](auto entity, auto& nsc)
					{
						// TODO: Move to Scene::OnScenePlay
						if (!nsc.Instance)
						{
							nsc.Instance = nsc.InstantiateFunction();
							nsc.Instance->m_Entity = Entity{ entity, this };
							nsc.Instance->OnCreate();
						}

						nsc.Instance->OnUpdate(ts);
					});
			}

			// Physics
			{
				const int32_t velocityIterations = 6;
				const int32_t positionIterations = 2;

				b2World_Step(m_PhysicsWorld, ts, positionIterations);

				// Sync physics bodies with transforms
				auto view = m_Registry.view<TransformComponent, Rigidbody2DComponent>();
				for (auto e : view)
				{
					Entity entity = { e, this };
					auto& transform = entity.GetComponent<TransformComponent>();
					auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

					transform.Translation.x = b2Body_GetPosition(rb2d.RuntimeBody).x;
					transform.Translation.y = b2Body_GetPosition(rb2d.RuntimeBody).y;
					transform.Rotation.z = b2Rot_GetAngle(b2Body_GetRotation(rb2d.RuntimeBody));

				}
			}
		}

		// Render 2D
		const Camera *mainCamera = nullptr;
		glm::mat4 cameraTransform;

		{
			const auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (const auto entity : view)
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = transform.GetTransform();
					break;
				}
			}
		}

		if (mainCamera)
		{
			Renderer2D::BeginScene(*mainCamera, cameraTransform);

			const auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (const auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				// Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color, static_cast<int>(entity));
				Renderer2D::DrawSprite(transform.GetTransform(), sprite, static_cast<int>(entity));
			}

			Renderer2D::EndScene();
		}
	}

	Entity Scene::CreateEntity(const std::string &name)
	{
		return CreateEntityWithUUID(name, UUID());
	}

	Entity Scene::CreateEntityWithUUID(const std::string& name, UUID uuid)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<IDComponent>(uuid);
		entity.AddComponent<TransformComponent>();
		auto &tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;

		m_EntityMap[uuid] = entity;

		return entity;
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		// Resize our non-FixedAspectRatio Cameras
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto &cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
			{
				cameraComponent.Camera.SetViewportSize(width, height);
			}
		}
	}

	void Scene::DestroyEntity(Entity entity)
	{
		// 我们这里传递进来的entity并不是一个entity实体，而只是一个Entity抽象,
		// destroy需要一个entity实体，这里当然可以在Entity里写一个get函数
		// 当然我们也可以重写 entity 运算符，如下
		// operator entt::entity() const { return m_EntityHandle; }
		m_Registry.destroy(entity);
	}

	Entity Scene::GetEntityByUUID(UUID uuid)
	{
		auto view = m_Registry.view<IDComponent>();
		for (auto entity : view)
		{
			const auto& id = view.get<IDComponent>(entity);
			if (id.ID == uuid)
			{
				return Entity{ entity, this };
			}
		}
		return {};
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
			{
				return Entity{ entity, this };
			}
		}
		return {};
	}

	std::shared_ptr<Scene> Scene::Copy(std::shared_ptr<Scene> otherScene)
	{
		std::shared_ptr<Scene> newScene = std::make_shared<Scene>();

		newScene->m_ViewportWidth = otherScene->m_ViewportWidth;
		newScene->m_ViewportHeight = otherScene->m_ViewportHeight;
		std::unordered_map<UUID, entt::entity> entMap;

		auto& srcSceneRegistry = otherScene->m_Registry;
		auto& dstSceneRegistry = newScene->m_Registry;

		const auto idView = srcSceneRegistry.view<IDComponent>();
		for (const auto entity : idView)
		{
			UUID uuid = srcSceneRegistry.get<IDComponent>(entity).ID;
			const auto& name = srcSceneRegistry.get<TagComponent>(entity).Tag;
			const Entity newEntity = newScene->CreateEntityWithUUID(name, uuid);
			entMap[uuid] = newEntity;
		}

		CopyComponents(AllComponents{}, srcSceneRegistry, dstSceneRegistry, entMap);

		return newScene;
	}

	Entity Scene::DuplicateEntity(Entity entity)
	{
		std::string name = entity.GetName() + "_Copy";
		const Entity newEntity = CreateEntity(name);
		CopyComponentIfExists(AllComponents{}, entity, newEntity);
		return newEntity;
	}

	void Scene::OnPhysics2DStart()
	{
		b2WorldDef PhysicsWorldDef = b2DefaultWorldDef();
		b2Vec2 gravity(0.0f, -9.8f);		// 重力
		PhysicsWorldDef.gravity = gravity;
		PhysicsWorldDef.restitutionThreshold = m_RestitutionThreshold;
		m_PhysicsWorld = b2CreateWorld(&PhysicsWorldDef);

		auto view = m_Registry.view<Rigidbody2DComponent>();
		for (auto e : view)
		{
			Entity entity = { e, this };
			auto& transform = entity.GetComponent<TransformComponent>();					// 位置
			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();		// 刚体

			// 创建Box2D物理刚体
			b2BodyDef bodyDef = b2DefaultBodyDef();
			bodyDef.type = Utils::ConvertToBox2DBodyType(rb2d.Type);
			bodyDef.position = { transform.Translation.x, transform.Translation.y };
			bodyDef.rotation = b2MakeRot(transform.Rotation.z);
			b2BodyId body = b2CreateBody(m_PhysicsWorld, &bodyDef);
			if (rb2d.FixedRotation)
			{
				b2Body_IsFixedRotation(body);
			}
			rb2d.RuntimeBody = body;

			if (entity.HasComponent<BoxCollider2DComponent>())
			{
				auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
				b2Polygon box = b2MakeBox(transform.Scale.x * bc2d.Size.x, transform.Scale.y * bc2d.Size.y);
				b2ShapeDef shapeDef = b2DefaultShapeDef();
				shapeDef.density = bc2d.Density;
				shapeDef.friction = bc2d.Friction;
				shapeDef.restitution = bc2d.Restitution;

				b2ShapeId shape = b2CreatePolygonShape(body, &shapeDef, &box);
				bc2d.RuntimeFixture = shape;
			}

		}
	}

	void Scene::OnPhysics2DStop()
	{
		b2DestroyWorld(m_PhysicsWorld);
		m_PhysicsWorld = b2_nullWorldId;
	}

	void Scene::OnSimulationStart()
	{
		OnPhysics2DStart();
	}

	void Scene::OnSimulationStop()
	{
		OnPhysics2DStop();
	}

	void Scene::OnRuntimeStop()
	{
		m_IsRunning = false;

		OnPhysics2DStop();
	}

	template <typename T>
	void Scene::OnComponentAdded(Entity entity, T &component)
	{
		static_assert(sizeof(T) == 0);
	}

	template <>
	void Scene::OnComponentAdded<IDComponent>(Entity entity, IDComponent &component)
	{
	}

	template <>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent &component)
	{
	}

	template <>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent &component)
	{
		if (m_ViewportWidth > 0 && m_ViewportHeight > 0)
			component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template <>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent &component)
	{
	}

	template <>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent &component)
	{
	}

	template <>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent &component)
	{
	}

	template <>
	void Scene::OnComponentAdded<Rigidbody2DComponent>(Entity entity, Rigidbody2DComponent &component)
	{
	}

	template <>
	void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent &component)
	{
	}
}
