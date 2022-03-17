#pragma once

#if defined(_WIN64) || defined(_WIN32)

// WINDOWS
#include <windows.h>

#define HYBRIS_EVT_EXPORT __declspec(dllexport)
#define HYBRIS_HANDLE_TYPE HMODULE
#define HYBRIS_PROGRAM_HANDLE GetModuleHandleW(nullptr)
#define HYBRIS_LOAD_EXTERN GetProcAddress

#elif defined(__APPLE__) || defined(__linux)

// APPLE & LINUX
#include <dlfcn.h>

#define HYBRIS_EVT_EXPORT
#define HYBRIS_HANDLE_TYPE void*
#define HYBRIS_PROGRAM_HANDLE dlopen(nullptr, RTLD_LAZY)
#define HYBRIS_LOAD_EXTERN dlsym

#endif

namespace Mod {
        // Events that will be triggered from the main process.
	extern "C" {
		HYBRIS_EVT_EXPORT void onInit();
		HYBRIS_EVT_EXPORT void onLoad();
		HYBRIS_EVT_EXPORT void onShutdown();
	}

	// Methods to manipulate the main process.
	int(*testCall) (int val);

	// Handle to the main process.
	HYBRIS_HANDLE_TYPE hybris;
}