#include "PluginManager.h"
#include <iostream>

bool PluginManager::hasSuffix(const std::wstring & s, const std::wstring & suffix){
	return s.size() >= suffix.size() && s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
}

PluginManager::PluginManager(){
	m_pluginList = new std::vector<Plugin*>();
	std::experimental::filesystem::directory_iterator pluginDirectory("mods");
	for (auto &pluginFile : pluginDirectory) {
		std::experimental::filesystem::path pluginPath = pluginFile.path();
		if (!hasSuffix(pluginPath.wstring(), HYBRIS_LIBRARY_SUFFIX)) continue;
		Plugin *plugin = new Plugin(pluginPath);
		plugin->onInit();
		plugin->onLoad();
		m_pluginList->push_back(plugin);
	}
}

PluginManager::~PluginManager(){
	for (Plugin *plugin : *m_pluginList) {
		plugin->onShutdown();
		delete plugin;
	}
	delete m_pluginList;
}