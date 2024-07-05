#pragma once
#ifndef TERRAINSYSTEM_H
#define TERRAINSYSTEM_H

#include "Rendering/Vertex.h"

namespace Fracture
{
	struct CameraComponent;
	struct Buffer;
	struct RenderContext;

	struct TerrainBrush {
		enum BrushTypeOptions {
			RAISE,
			LOWER,
			FLATTEN,
			FLOODFILL,
			// Add more types as needed
		};

		BrushTypeOptions BrushType = BrushTypeOptions::RAISE;
		float radius = 10.0f;
		float strength = 5.0f;
	};

	enum class TerrainEditModeOptions
	{
		Selection,
		Sculpting,
		Painting,
		Placement,
	};

	struct TerrainSystemPerFrameData
	{
		glm::vec4 MousePos_BrushRadius;
	};

	struct DensityGrid
	{
		Fracture::UUID ID;
		std::string Name = "NewGrid";
		Fracture::UUID TerrainComponent;
		Fracture::UUID PlacementMap;
		
		std::vector<glm::vec3> GridCellPositions;
		bool IsDirty = true;
	};

	struct TerrainTextureMap
	{
		Fracture::UUID ID;
		std::string Name = "NewGrid";
		std::vector<float> Data;
		int Width = 1;
		int Height = 1;
		int Channels = 1;
		bool IsDirty = false;
	};
	
	/* Thinging Aloud
	struct TerrainMapModifier
	{
		virtual void Modify() = 0;
		bool Enabled;
	};

	struct Ecotope
	{
		std::string Name;
		std::vector<std::shared_ptr<TerrainMapModifier>> Modifiers;
	};
	*/

	struct PlacementLayer
	{
		Fracture::UUID LayerID;
		std::string Name = "NewLayer";
		Fracture::UUID MeshID;
		Fracture::UUID PlacemenMapID;
		float FootPrint = 8.0f;
		float DrawDistance = 300.0f;
		glm::vec3 Rotation;
		glm::vec3 Scale = glm::vec3(1.0f);
		bool IsMeshSet = false;
		bool IsFootPrintDirty = true;
	};

	class TerrainSystem
	{
	public:
		void GenerateTerrain(Fracture::UUID entity);
		void UpdateTerrain(Fracture::UUID entity);

		void OnInit();
		void OnUpdate();	
		void OnBeginFrame(RenderContext* context);
		void OnEndFrame();
		void OnDebugDraw();
		void SaveHeightMap(const std::string& path);
		void SubmitTerrainForEditing(Fracture::UUID entity);
		void ReleaseTerrainFromEditing();

		void ApplyBrush(float dt, glm::vec3 centre);

		void AddNewPlacementLayer(Fracture::UUID entity);

		void AddNewDensityGrid(Fracture::UUID entity);
		

		//void UpdateDensityGrid(Fracture::UUID placementID);
		void UpdateDensityMap(Fracture::UUID placementID);
	
		void SetCurrentPlacementMapForEdit(Fracture::UUID mapID);
		void SetCurrentPlacementLayerForEdit(Fracture::UUID entityID, int index);

		void ClearGridSelection();
		void ClearPlacementMapFromEdit();
		void AttachPlacementLayerToDensityGid(Fracture::UUID layer, Fracture::UUID grid);
		glm::vec3 AddJitterToVector(const glm::vec3& vector, float jitterAmount);
		bool IsCandidateValid(glm::vec3 candidate, glm::vec2 regionSize,float cellSize, float footprint, std::vector<std::vector<int>>& grid,std::vector<glm::vec3>& points);
		std::vector<glm::vec3> GenerateGridPoints(float footprint, glm::vec2 regionSize, int NoOfSamples = 30);
		

		TerrainBrush RaiseBrush;
		TerrainEditModeOptions EditMode = TerrainEditModeOptions::Selection;
		DensityGrid TestGrid;

		glm::vec3 GetTerrainIntersectionPoint(Fracture::CameraComponent& camera, bool& intersects,float mouse_x, float mouse_y, float width, float height);
		bool IsEditing();

		//Terrain -> Many PlacementLayers;
		std::map<UUID, std::vector<std::shared_ptr<PlacementLayer>>> mPlacementLayers;
		std::unordered_map<UUID, std::shared_ptr<TerrainTextureMap>> mPlacementMaps;
		std::unordered_map<UUID, std::shared_ptr<TerrainTextureMap>> mDenistyMaps;
		std::unordered_map<UUID, std::vector<glm::vec3>> mPlacementPoints;

		//std::map<UUID, std::shared_ptr<DensityGrid>> mDensityGrids;
		std::unordered_map<UUID, std::shared_ptr<TerrainTextureMap>> mSplatMaps;



	
		UUID CurrentMappedTerrain = 0;
		int CurrentSelectedLayer = -1;		
		TerrainTextureMap* mCurrentPlacementMapForEdit;
		PlacementLayer* mCurrentPlacementLayerForEdit;
	private:
		Vertex* current_terrian_data;
		uint32_t OriginalTerrainSize = 0;
		Vertex* mVerticesForEdit;
		
		int Terrain_Width, Terrain_Height;

		float GetHeightatPoint(int index);
		glm::vec3 CalcNormal(int index);
		float RandomRange(float Min, float Max);
		float sqrMagnitude(const glm::vec3& v);

	};

}


#endif