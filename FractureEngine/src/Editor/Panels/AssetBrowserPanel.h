#pragma once
#ifndef ASSETBROWSER_H
#define ASSETBROWSER_H

#include "Panel.h"
#include "Fracture.h"

namespace Fracture
{

	enum class CurrentAssetType
	{
		Models,
		Textures,
		Scenes,
		Materials,
		Shaders
	};

	class AssetBrowserPanel: public Panel
	{
	public:
		AssetBrowserPanel();
		~AssetBrowserPanel();	


		virtual void render() override;
	private:

		static bool m_showModels;
		static bool m_showTextures;
		static bool m_showScenes;
		static bool m_showMaterials;
		static bool m_showShaders;
		static float scroll_y;

		ImGuiTableFlags m_tableflags = ImGuiTableFlags_RowBg; //ImGuiTableFlags_NoSavedSettings

	};


}

#endif
