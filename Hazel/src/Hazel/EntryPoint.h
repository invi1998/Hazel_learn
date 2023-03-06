#pragma once

#ifdef HZ_PLATFORM_WINDOWS

extern Hazel::Application* Hazel::CreateApplication();

int main(int argc, char ** argv)
{

	Hazel::Log::Init();
	HZ_CORE_WARN("Initialized Log!");
	HZ_CORE_ERROR("Hello !");
	HZ_CORE_TRACE("Hazel Engine");

	int a = 10;
	HZ_ERROR("VALUE = {0}", a);
	HZ_INFO("info message");

	auto app = Hazel::CreateApplication();
	app->Run();
	delete app;
}

#endif

