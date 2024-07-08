#include "FracturePCH.h"
#include "PlacementModifiers.h"
#include "TerrainSystem.h"

void Fracture::MultiplyMapsModifier::Modify(PlacementContext& context, std::vector<float>& OutMap)
{
	if (context._system->mPlacementMaps.find(context.PlacementMapID) == context._system->mPlacementMaps.end())
		return;

	if (context._system->mPlacementMaps.find(rhMap) == context._system->mPlacementMaps.end())
		return;

	auto& map = context._system->mPlacementMaps[context.PlacementMapID];
	auto& RHmap = context._system->mPlacementMaps[rhMap];

	for (int i = 0; i < map->Data.size(); i++)
	{
		OutMap[i] = map->Data[i] * (1 - RHmap->Data[i]);
	}
}

void Fracture::InvertMapModifier::Modify(PlacementContext& context, std::vector<float>& OutMap)
{
	if (context._system->mPlacementMaps.find(context.PlacementMapID) == context._system->mPlacementMaps.end())
		return;

	auto& map = context._system->mPlacementMaps[context.PlacementMapID];
	
	for (int i = 0; i < map->Data.size(); i++)
	{
		OutMap[i] = 1 - map->Data[i];
	}

}
