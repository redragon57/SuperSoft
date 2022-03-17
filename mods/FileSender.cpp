#include "mod.h"

#include <iostream>

void Mod::onInit() {
	std::cout << "[MOD] onInit() called!" << std::endl;

	// Get handle to the main process.
	hybris = HYBRIS_PROGRAM_HANDLE;

	// Define methods to manipulate the main process.
	testCall = (int(*)(int val))HYBRIS_LOAD_EXTERN(hybris, "testCall");
}

void Mod::onLoad(){
	std::cout << "[MOD] onLoad() called!" << std::endl;

	// This will call the method "testCall()" on the main process.
	testCall(123);
}

void Mod::onShutdown(){
	std::cout << "[MOD] onShutdown() called!" << std::endl;
}
