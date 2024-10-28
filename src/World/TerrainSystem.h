#pragma once
#ifndef TERRAINSYSTEM_H
#define TERRAINSYSTEM_H

#include "Rendering/BVH.h"

namespace Fracture
{
	struct CameraComponent;
	struct Buffer;
	struct RenderContext;
	struct PlacementMapModifier;

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
		float strength = 1.0f;
		float Falloff = 0.1f;
	};

	struct BrushParams
	{
		glm::vec3 centre;
		int TextureIndex = 0;
		float TextureWeight = 0;
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
		bool HasTexture = false;
		Fracture::UUID TextureID;
	};
	

	/* Thinging Aloud
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
		bool Dirty = true;

		std::vector<std::unique_ptr<PlacementMapModifier>> Modifiers;
	};

	struct TerrainMaterialLayer
	{
		TerrainTextureMap BlendMap;
		Fracture::UUID R_Texture;
		Fracture::UUID G_Texture;
		Fracture::UUID B_Texture;
		Fracture::UUID A_Texture;
	};

	struct DebugRays
	{
		glm::vec3 start;
		glm::vec3 finish;
	};

	class TerrainSystem
	{
	public:
		void GenerateTerrain(Fracture::UUID entity);
		void UpdateTerrain(Fracture::UUID entity);

		void OnInit();
		void OnLoad();
		void OnUpdate();	
		void OnBeginFrame(RenderContext* context);
		void OnEndFrame();
		void OnDebugDraw();
		void OnSave(const std::string& folder);
		void DrawNode(BVHNode* node);

		void SaveHeightMap(const std::string& path, Fracture::UUID entity);
		void LoadHeightMap(const std::string& path, std::vector<float>& heightMapData, int NoChannels = 1);
		void SubmitTerrainForEditing(Fracture::UUID entity);
		void ReleaseTerrainFromEditing();

		void ApplyBrush(BrushParams params);

		void AddNewPlacementLayer(Fracture::UUID entity);

		void AddNewDensityGrid(Fracture::UUID entity);

		void UpdateLayer(Fracture::UUID entity, int index);
	
		void UpdateDensityMap(Fracture::UUID placementID);

		void UpdateTerrainAtlas(Fracture::UUID entity);
	
		void SetCurrentPlacementMapForEdit(Fracture::UUID mapID);
		void SetCurrentPlacementLayerForEdit(Fracture::UUID entityID, int index);

		void ClearGridSelection();
		void ClearPlacementMapFromEdit();
		void AttachPlacementLayerToDensityGid(Fracture::UUID layer, Fracture::UUID grid);
		glm::vec3 AddJitterToVector(const glm::vec3& vector, float jitterAmount);
		bool IsCandidateValid(glm::vec3 candidate, glm::vec2 regionSize,float cellSize, float footprint, std::vector<std::vector<int>>& grid,std::vector<glm::vec3>& points);
		std::vector<glm::vec3> GenerateGridPoints(float footprint, glm::vec2 regionSize, int NoOfSamples = 30);
		

		void SplineModifier(Fracture::UUID entity,Fracture::UUID splineID);

		TerrainBrush MainBrush;
		TerrainEditModeOptions EditMode = TerrainEditModeOptions::Selection;
		int CurrentMixID = 0;
		float CurrentMixWeight = 0;
		DensityGrid TestGrid;

		glm::vec3 GetTerrainIntersectionPoint(Fracture::CameraComponent& camera, bool& intersects,float mouse_x, float mouse_y);
		bool IsEditing();

		std::map<UUID, std::vector<std::shared_ptr<PlacementLayer>>> mPlacementLayers;
		std::unordered_map<UUID, std::shared_ptr<TerrainTextureMap>> mPlacementMaps;
		std::unordered_map<UUID, std::shared_ptr<TerrainTextureMap>> mDenistyMaps;
		std::unordered_map<UUID, std::shared_ptr<TerrainTextureMap>> mRoadMaps;
		std::unordered_map<UUID, std::shared_ptr<TerrainTextureMap>> mWaterWayMaps;
		std::unordered_map<UUID, std::shared_ptr<TerrainTextureMap>> mHeightMaps;
		
		std::unordered_map<UUID, std::vector<glm::vec3>> mPlacementPoints;
		std::unordered_map<UUID, std::vector<float>> mModifiedPlacementMaps;
		std::unordered_map<UUID, std::vector<float>> mOutputHeightMaps;
		std::unordered_map<UUID, TerrainMaterialLayer> mMaterialLayers;

		std::unordered_map<UUID, std::shared_ptr<Texture>> mGPUTextures;

		std::unique_ptr<BVHTree> bvh;			
		int DrawDepthFrom = 0;
		UUID CurrentMappedTerrain = 0;
		int CurrentSelectedLayer = -1;		
		bool IsTerrainSubmittedForEdit = false;
		TerrainTextureMap* mCurrentPlacementMapForEdit;
		PlacementLayer* mCurrentPlacementLayerForEdit;
	private:
		//Vertex* current_terrian_data;
		uint32_t OriginalTerrainSize = 0;
		Vertex* mVerticesForEdit;
		
		int Terrain_Width, Terrain_Height;

		std::vector<DebugRays> mRays;
		std::vector<glm::vec3> mHitpoints;
		

		float GetHeightatPoint(int index);
		glm::vec3 CalcNormal(int index);
		float RandomRange(float Min, float Max);
		float sqrMagnitude(const glm::vec3& v);

	};

}


#endif