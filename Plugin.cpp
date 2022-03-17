#include "Plugin.h"

Plugin::Plugin(std::experimental::filesystem::path filename){
	m_lib = HYBRIS_PROGRAM_HANDLE;
	if (!m_lib) throw "[MAIN] Unable to load Plugin!";
	std::cout << "[MAIN] Loading Plugin" << std::endl;
	onInit	   = (onLoadType)		HYBRIS_LOAD_EXTERN(m_lib, "onInit");
	onLoad	   = (onLoadType)		HYBRIS_LOAD_EXTERN(m_lib, "onLoad");
	onShutdown = (onShutdownType)	HYBRIS_LOAD_EXTERN(m_lib, "onShutdown");
}

Plugin::~Plugin(){
	std::cout << "[MAIN] Shutting down Plugin" << std::endl;
	HYBRIS_CLOSE_EXTERN(m_lib);
}

int PluginCall::testCall(int val) {
	std::cout << "[MAIN] testCall() called with value '" << val << "'" << std::endl;
	return val;
}