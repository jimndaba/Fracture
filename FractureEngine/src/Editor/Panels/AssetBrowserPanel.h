#pragma once
#ifndef ASSETBROWSER_H
#define ASSETBROWSER_H

#include "Panel.h"

namespace Fracture
{

	class AssetBrowserPanel: public Panel
	{
	public:
		AssetBrowserPanel();
		~AssetBrowserPanel();	


		virtual void render() override;


	};


}

#endif
