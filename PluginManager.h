#pragma once
#include "Plugin.h"

#include <filesystem>
#include <vector>

class PluginManager{
	private:
		bool hasSuffix(const std::wstring & s, const std::wstring & suffix);

		std::vector<Plugin*> *m_pluginList;
	public:
		PluginManager();
		~PluginManager();
};
