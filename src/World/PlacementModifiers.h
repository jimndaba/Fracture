#pragma once
#ifndef PLACEMENTMODIFIERS_H
#define PLACEMENTMODIFIERS_H

namespace Fracture
{
	class TerrainSystem;

	struct PlacementContext
	{
		Fracture::UUID LayerID;
		Fracture::UUID PlacementMapID;	
		int width;
		int height;
		TerrainSystem* _system;
	};	

	struct PlacementMapModifier
	{
		virtual void Modify(PlacementContext& context, std::vector<float>& OutMap) = 0;
		bool Enabled;
	};

	struct MultiplyMapsModifier : PlacementMapModifier
	{
		void Modify(PlacementContext& context, std::vector<float>& OutMap);
		Fracture::UUID rhMap;
	};

	struct InvertMapModifier : PlacementMapModifier
	{
		void Modify(PlacementContext& context, std::vector<float>& OutMap);
		bool Enabled;
	};

}

#endif