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


	};


}

#endif
