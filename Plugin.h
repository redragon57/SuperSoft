#pragma once
#include <experimental/filesystem>
#include <iostream>

#if defined(_WIN64) || defined(_WIN32)

	// WINDOWS
	#include <windows.h>

	#define HYBRIS_EVT_EXPORT __declspec(dllexport)
	#define HYBRIS_HANDLE_TYPE HMODULE
	#define HYBRIS_PROGRAM_HANDLE LoadLibraryW(filename.c_str());
	#define HYBRIS_LOAD_EXTERN GetProcAddress
	#define HYBRIS_CLOSE_EXTERN FreeLibrary
	#define HYBRIS_LIBRARY_SUFFIX L".dll"

#elif defined(__APPLE__) || defined(__linux)
	// APPLE & LINUX
	#include <dlfcn.h>
	#define HYBRIS_EVT_EXPORT
	#define HYBRIS_HANDLE_TYPE void*
	#define HYBRIS_PROGRAM_HANDLE dlopen(filename.c_str(), RTLD_LAZY);
	#define HYBRIS_LOAD_EXTERN dlsym
	#define HYBRIS_CLOSE_EXTERN dlclose
	#define HYBRIS_LIBRARY_SUFFIX L".so"
#endif


class Plugin{
	private:
		typedef void	(*onInitType)();
		typedef void	(*onLoadType)();
		typedef void	(*onShutdownType)();
		HYBRIS_HANDLE_TYPE m_lib;

	public:
		Plugin(std::experimental::filesystem::path filename);
		~Plugin();

		onInitType	   onInit;
		onLoadType	   onLoad;
		onShutdownType onShutdown;
	};

	namespace PluginCall {
		extern "C" {
			HYBRIS_EVT_EXPORT int testCall(int val);
		}
}