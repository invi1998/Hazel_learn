#pragma once
#include "Hazel/Core/Timestep.h"
#include "Hazel/Renderer/OrthographicCamera.h"

#include "Hazel/Event/Event.h"
#include "Hazel/Event/ApplicationEvent.h"
#include "Hazel/Event/MouseEvent.h"

namespace Hazel
{
	struct OrthographicCameraBounds
	{
		float Left, Right;
		float Bottom, Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		inline OrthographicCamera& GetCamera() { return m_Camera; }
		inline const OrthographicCamera& GetCamera() const{ return m_Camera; }

		inline void SetZoomLevel(float level)
		{
			m_ZoomLevel = level;
			CalculateView();
		}
		inline float GetZoomLevel() const { return m_ZoomLevel; }

		inline OrthographicCameraBounds GetBounds() const { return m_Bounds; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

		void CalculateView();

	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		bool m_Rotation;
		OrthographicCameraBounds m_Bounds;
		OrthographicCamera m_Camera;

		float m_CameraTranslationSpeed = 1.0f;
		float m_CameraRotationSpeed = 180.0f;
		float m_CameraRotate = 0.0f;
		glm::vec3 m_CameraPosition = {0.0f, 0.0f, 0.0f};
	};
}


