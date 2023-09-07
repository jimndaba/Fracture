#include "FracturePCH.h"
#include "PrefabFactor.h"

#include "SceneManager.h"
#include "Serialisation/Serialiser.h"
#include "Scripting/ScriptManager.h"
#include "Scripting/LuaScript.h"

Fracture::PrefabFactory::PrefabFactory()
{
}

bool Fracture::PrefabFactory::IsTemplateAvailable(Fracture::UUID sceneID)
{
	if (mPrefabTemplates.find(sceneID) != mPrefabTemplates.end())
	{
		return true;
	}
	return false;
}

void Fracture::PrefabFactory::Instance(ScenePrefab prefab, glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, bool isRootOfNested)
{
	if (mPrefabTemplates.find(prefab.SceneID) != mPrefabTemplates.end())
	{
		if (mPrefabTracker[prefab.SceneID])
		{
			LoadPrefabTemplate(SceneManager::mSceneRegister[prefab.SceneID].Path);
			for (const auto& r : MeshesToLoad)
				Eventbus::Publish<AsyncLoadMeshEvent>(r);
			mPrefabTracker[prefab.SceneID] = false;
		}

		auto new_entity = prefab.PrefabID;
		{			
			auto& prefabTemp = mPrefabTemplates[prefab.SceneID].mEntities[0];

			if (prefabTemp.HasTagComponent)
			{
				auto component = std::make_shared<TagComponent>(*prefabTemp.tag.get(), new_entity);
				SceneManager::AddComponentByInstance(new_entity, component);
			}

			if (prefabTemp.HasTranformComponent)
			{
				auto component = std::make_shared<TransformComponent>(*prefabTemp.transform.get(), new_entity);
				component->Position = position;
				component->Scale = scale;
				component->Rotation = glm::quat(glm::radians(rotation));
				SceneManager::AddComponentByInstance(new_entity, component);
			}

			if (prefabTemp.HasHierachyComponent)
			{
				auto component = std::make_shared<HierachyComponent>(prefab.PrefabID);
				const auto& parent = SceneManager::GetComponent<HierachyComponent>(prefab.ParentID);
				parent->Prefabs.push_back(prefab.PrefabID);
				component->HasParent = true;
				component->Parent = prefab.ParentID;
				SceneManager::AddComponentByInstance(prefab.PrefabID, component);
			}

			if (prefabTemp.HasMeshComponent)
			{
				if (isRootOfNested)
				{
					auto component = std::make_shared<PrefabInstanceComponent>(*prefabTemp.mesh_component.get(), new_entity);
					SceneManager::AddComponentByInstance(new_entity, component);
				}
				else
				{
					auto component = std::make_shared<PrefabInstanceComponent>(*prefabTemp.mesh_component.get(), prefab.ParentID);				
					SceneManager::AddComponentByInstance(new_entity, component);
				}
			}

			if (prefabTemp.HasCameraComponent)
			{
				auto component = std::make_shared<CameraComponent>(*prefabTemp.camera.get(), new_entity);
				SceneManager::AddComponentByInstance(new_entity, component);
			}

			if (prefabTemp.HasAnimationComponent)
			{
				auto component = std::make_shared<AnimationComponent>(*prefabTemp.animation.get(), new_entity);
				SceneManager::AddComponentByInstance(new_entity, component);
			}

			if (prefabTemp.HasAudioSourceComponent)
			{
				auto component = std::make_shared<AudioSourceComponent>(*prefabTemp.audiosource.get(), new_entity);
				SceneManager::AddComponentByInstance(new_entity, component);
			}

			if (prefabTemp.HasColliderComponent)
			{
				auto component = std::make_shared<ColliderComponent>(*prefabTemp.collider.get(), new_entity);
				SceneManager::AddComponentByInstance(new_entity, component);
			}

			if (prefabTemp.HaslightProbeComponent)
			{
				auto component = std::make_shared<LightProbeComponent>(*prefabTemp.lightprobe.get(), new_entity);
				SceneManager::AddComponentByInstance(new_entity, component);
			}

			if (prefabTemp.HasPointlightComponent)
			{
				auto component = std::make_shared<PointlightComponent>(*prefabTemp.pointlight.get(), new_entity);
				SceneManager::AddComponentByInstance(new_entity, component);
			}

			if (prefabTemp.HasSpotlightComponent)
			{
				auto component = std::make_shared<SpotlightComponent>(*prefabTemp.spotlight.get(), new_entity);
				SceneManager::AddComponentByInstance(new_entity, component);
			}

			if (prefabTemp.HasSunlightComponent)
			{
				auto component = std::make_shared<SunlightComponent>(*prefabTemp.sunlight.get(), new_entity);
				SceneManager::AddComponentByInstance(new_entity, component);
			}

			if (prefabTemp.HasRigidbodyComponent)
			{
				auto component = std::make_shared<RigidbodyComponent>(*prefabTemp.rigidbody.get(), new_entity);
				SceneManager::AddComponentByInstance(new_entity, component);
			}

			for (const auto& script : prefabTemp.scripts)
			{
				SceneManager::AttachScript(new_entity, script);
			}
			

			if (prefabTemp.HasReflectionProbeComponent)
			{
				auto component = std::make_shared<ReflectionProbeComponent>(*prefabTemp.reflectionprobe.get(), new_entity);
				SceneManager::AddComponentByInstance(new_entity, component);
			}

			if (prefabTemp.HasSkyboxComponent)
			{
				auto component = std::make_shared<SkyboxComponent>(*prefabTemp.skybox.get(), new_entity);
				SceneManager::AddComponentByInstance(new_entity, component);
			}
		}

		for (int i = 1;  i < mPrefabTemplates[prefab.SceneID].mEntities.size();i++)
		{

			Fracture::UUID mEntity = Fracture::UUID();
		
			auto& prefabTemp = mPrefabTemplates[prefab.SceneID].mEntities[i];

			if (prefabTemp.HasTagComponent)
			{
				auto component = std::make_shared<TagComponent>(*prefabTemp.tag.get(), mEntity);
				SceneManager::AddComponentByInstance(mEntity, component);
			}

			if (prefabTemp.HasTranformComponent)
			{
				auto component = std::make_shared<TransformComponent>(*prefabTemp.transform.get(), mEntity);		
				SceneManager::AddComponentByInstance(mEntity, component);
			}

			if (prefabTemp.HasHierachyComponent)
			{
				auto component = std::make_shared<HierachyComponent>(mEntity );
				const auto& parent = SceneManager::GetComponent<HierachyComponent>(prefab.PrefabID);
				parent->Prefabs.push_back(mEntity );

				component->HasParent = true;
				component->Parent = prefab.PrefabID;
				SceneManager::AddComponentByInstance(mEntity , component);
			}

			if (prefabTemp.HasMeshComponent)
			{
				if (isRootOfNested)
				{
					auto comp = std::make_shared<PrefabInstanceComponent>(mEntity, prefab.PrefabID);
					comp->Mesh = prefabTemp.mesh_component->Mesh;
					comp->Materials = prefabTemp.mesh_component->Materials;
					comp->meshType = prefabTemp.mesh_component->meshType;
					MeshesToLoad.push_back(comp->Mesh);
					SceneManager::AddComponentByInstance(mEntity, comp);
				}
				else
				{
					auto comp = std::make_shared<PrefabInstanceComponent>(mEntity, prefab.ParentID);
					comp->Mesh = prefabTemp.mesh_component->Mesh;
					comp->Materials = prefabTemp.mesh_component->Materials;
					comp->meshType = prefabTemp.mesh_component->meshType;
					MeshesToLoad.push_back(comp->Mesh);
					SceneManager::AddComponentByInstance(mEntity, comp);
				}
			}

			if (prefabTemp.HasCameraComponent)
			{
				auto component = std::make_shared<CameraComponent>(*prefabTemp.camera.get(), mEntity );
				SceneManager::AddComponentByInstance(mEntity , component);
			}

			if (prefabTemp.HasAnimationComponent)
			{
				auto component = std::make_shared<AnimationComponent>(*prefabTemp.animation.get(), mEntity );
				SceneManager::AddComponentByInstance(mEntity , component);
			}

			if (prefabTemp.HasAudioSourceComponent)
			{
				auto component = std::make_shared<AudioSourceComponent>(*prefabTemp.audiosource.get(), mEntity );
				SceneManager::AddComponentByInstance(mEntity , component);
			}

			if (prefabTemp.HasColliderComponent)
			{
				auto component = std::make_shared<ColliderComponent>(*prefabTemp.collider.get(), mEntity );
				SceneManager::AddComponentByInstance(mEntity , component);
			}

			if (prefabTemp.HaslightProbeComponent)
			{
				auto component = std::make_shared<LightProbeComponent>(*prefabTemp.lightprobe.get(), mEntity );
				SceneManager::AddComponentByInstance(mEntity , component);
			}

			if (prefabTemp.HasPointlightComponent)
			{
				auto component = std::make_shared<PointlightComponent>(*prefabTemp.pointlight.get(), mEntity );
				SceneManager::AddComponentByInstance(mEntity , component);
			}

			if (prefabTemp.HasSpotlightComponent)
			{
				auto component = std::make_shared<SpotlightComponent>(*prefabTemp.spotlight.get(), mEntity );
				SceneManager::AddComponentByInstance(mEntity , component);
			}

			if (prefabTemp.HasSunlightComponent)
			{
				auto component = std::make_shared<SunlightComponent>(*prefabTemp.sunlight.get(), mEntity );
				SceneManager::AddComponentByInstance(mEntity , component);
			}

			if (prefabTemp.HasRigidbodyComponent)
			{
				auto component = std::make_shared<RigidbodyComponent>(*prefabTemp.rigidbody.get(), mEntity );
				SceneManager::AddComponentByInstance(mEntity , component);
			}


			for (const auto& script : prefabTemp.scripts)
			{
				SceneManager::AttachScript(mEntity, script);				
			}

			if (prefabTemp.HasReflectionProbeComponent)
			{
				auto component = std::make_shared<ReflectionProbeComponent>(*prefabTemp.reflectionprobe.get(), mEntity );
				SceneManager::AddComponentByInstance(mEntity , component);
			}

			if (prefabTemp.HasSkyboxComponent)
			{
				auto component = std::make_shared<SkyboxComponent>(*prefabTemp.skybox.get(), mEntity );
				SceneManager::AddComponentByInstance(mEntity , component);
			}
		}

		for (const auto& nested_prefab : mPrefabTemplates[prefab.SceneID].mPrefablist)
		{
			nested_prefab.PrefabID = UUID();
			nested_prefab.ParentID = prefab.PrefabID;

			if (!IsTemplateAvailable(nested_prefab.SceneID))
			{
				auto it = SceneManager::mSceneRegister.find(prefab.SceneID);
				if (it != SceneManager::mSceneRegister.end())
				{
					LoadPrefabTemplate(SceneManager::mSceneRegister[prefab.SceneID].Path);
					for (const auto& r : MeshesToLoad)
						Eventbus::Publish<AsyncLoadMeshEvent>(r);
				}			
				mPrefabTracker[nested_prefab.SceneID] = false;
			}
			
			Instance(nested_prefab, nested_prefab.Position, nested_prefab.Scale, glm::degrees(glm::eulerAngles(nested_prefab.Rotation)));
		
		}
	}
}

void Fracture::PrefabFactory::LoadPrefabTemplate(const std::string& path)
{
	MeshesToLoad.clear();
	ISerialiser loader = ISerialiser(Fracture::ISerialiser::IOMode::Open, ISerialiser::SerialiseFormat::Json);
	loader.Open(path);
	int e_index = 0;

	if (loader.BeginStruct("Scene"))
	{
		ScenePrefabTemplate temp;
		temp.SceneID = loader.ID("ID");
		temp.TempalteID = UUID();

		mPrefabTracker[temp.SceneID] = false;

		if (loader.BeginStruct("Root"))
		{
			EntityPrefabTemplate root;
			auto entity_id = UUID();
			root.TempalteID = entity_id;
			root.SceneID = temp.SceneID;
			temp.RootID = entity_id;
		
			root.HasTranformComponent = loader.HasKey("Transform");
			root.HasTagComponent = loader.HasKey("Tag");
			root.HasMeshComponent = loader.HasKey("Mesh");
			root.HasHierachyComponent = loader.HasKey("Hierachy");
			root.HasCameraComponent = loader.HasKey("Camera");
			root.HasSunlightComponent = loader.HasKey("SunlightComponent");
			root.HasPointlightComponent = loader.HasKey("PointlightComponent");
			root.HasSpotlightComponent = loader.HasKey("SpotlightComponent");
			root.HaslightProbeComponent = loader.HasKey("LightProbeComponent");
			root.HasReflectionProbeComponent = loader.HasKey("ReflectionProbeComponent");
			root.HasRigidbodyComponent = loader.HasKey("RigidbodyComponent");
			root.HasColliderComponent = loader.HasKey("ColliderComponent");
			root.HasScriptComponent = loader.HasKey("ScriptComponent");
			root.HasAudioSourceComponent = loader.HasKey("AudioSourceComponent");
			root.HasSkyboxComponent = loader.HasKey("SkyboxComponent");
			root.HasAnimationComponent = loader.HasKey("AnimationComponent");
		
			if (loader.BeginStruct("Tag"))
			{
				root.tag = std::make_shared<TagComponent>(entity_id, loader.STRING("Name"));
				root.tag->IsActive = loader.BOOL("IsActive");
				root.tag->Tags = loader.STRINGS_VECTOR("Tags");
				loader.EndStruct();
			}
			if (loader.BeginStruct("Transform"))
			{
				root.transform = std::make_shared<TransformComponent>(entity_id);
				root.transform->Position = loader.VEC3("Position");
				root.transform->Scale = loader.VEC3("Scale");
				root.transform->Rotation = glm::quat(glm::radians(loader.VEC3("Rotation")));
				loader.EndStruct();
			}
			if (loader.BeginStruct("Hierachy"))
			{
				root.hierachy = std::make_shared<HierachyComponent>(entity_id);
				root.hierachy->Parent = loader.ID("ParentID");
				root.hierachy->HasParent = BOOL("HasParent");
				loader.BeginCollection("Children");
				while (loader.CurrentCollectionIndex() < loader.GetCollectionSize())
				{
					if (loader.BeginStruct("Child"))
					{
						root.hierachy->Children.push_back(loader.ID("ChildID"));
						loader.EndStruct();
					}
					loader.NextInCollection();
				}
				loader.EndCollection();

				loader.EndStruct();
			}
			if (loader.BeginStruct("Mesh"))
			{
				root.mesh_component = std::make_shared<PrefabInstanceComponent>();
				root.mesh_component->Mesh = loader.ID("MeshID");
				MeshesToLoad.push_back(root.mesh_component->Mesh);
				root.mesh_component->Materials = loader.UINT32_VECTOR("Materials");
				root.mesh_component->meshType = (PrefabInstanceComponent::MeshType)INT("MeshType");
				loader.EndStruct();
			}
			if (loader.BeginStruct("SpotlightComponent"))
			{
				root.spotlight = std::make_shared<SpotlightComponent>(entity_id);
				root.spotlight->Diffuse = loader.VEC3("Diffuse");
				root.spotlight->Strength = loader.FLOAT("Strength");
				root.spotlight->Constant = loader.FLOAT("Constant");
				root.spotlight->Linear = loader.FLOAT("Linear");
				root.spotlight->Quadratic = loader.FLOAT("Quadratic");
				root.spotlight->InnerCutoff = loader.FLOAT("InnerCutoff");
				root.spotlight->OutCutoff = loader.FLOAT("OuterCutoff");
				loader.EndStruct();
			}
			if (loader.BeginStruct("PointlightComponent"))
			{
				root.pointlight = std::make_shared<PointlightComponent>(entity_id);
				root.pointlight->Diffuse = loader.VEC3("Diffuse");
				root.pointlight->Strength = loader.FLOAT("Strength");
				root.pointlight->Compression = loader.FLOAT("Compression");
				root.pointlight->Radius = loader.FLOAT("Radius");
				loader.EndStruct();
			}
			if (loader.BeginStruct("SunlightComponent"))
			{
				root.sunlight = std::make_shared<SunlightComponent>(entity_id);
				root.sunlight->Diffuse = loader.VEC3("Diffuse");
				root.sunlight->Strength = loader.FLOAT("Strength");
				loader.EndStruct();
			}
			if (loader.BeginStruct("RigidbodyComponent"))
			{
				root.rigidbody = std::make_shared<RigidbodyComponent>(entity_id);
				root.rigidbody->Mass = loader.FLOAT("Mass");
				root.rigidbody->Friction = loader.FLOAT("Friction");
				root.rigidbody->Bouncyness = loader.FLOAT("Bouncyness");
				root.rigidbody->IsDynamic = loader.BOOL("IsDynamic");
				root.rigidbody->IsKinematic = loader.BOOL("IsKinematic");

				root.rigidbody->LinearConstraints[0] = loader.FLOAT("MovementConstraintX");
				root.rigidbody->LinearConstraints[1] = loader.FLOAT("MovementConstraintY");
				root.rigidbody->LinearConstraints[2] = loader.FLOAT("MovementConstraintZ");

				root.rigidbody->AngularConstraints[0] = loader.FLOAT("RotationConstraintX");
				root.rigidbody->AngularConstraints[1] = loader.FLOAT("RotationConstraintY");
				root.rigidbody->AngularConstraints[2] = loader.FLOAT("RotationConstraintZ");

				loader.EndStruct();
			}
			if (loader.BeginStruct("ColliderComponent"))
			{
				root.collider = std::make_shared<ColliderComponent>(entity_id);
				root.collider->Size = loader.VEC3("Size");
				root.collider->Shape = (ColliderType)loader.INT("Type");
				root.collider->Radius = loader.FLOAT("Radius");
				root.collider->Height = loader.FLOAT("Height");
				root.collider->Offset = loader.VEC3("Offset");
				root.collider->IsTrigger = loader.BOOL("IsTrigger");
				root.collider->CollisionLayer = loader.ID("CollisionLayer");
				root.collider->CollisionGroup = loader.ID("CollisionGroup");
				loader.EndStruct();
			}
			if (loader.BeginStruct("ScriptComponent"))
			{
				root.script = std::make_shared<ScriptComponent>(entity_id);
				root.script->HasScriptAttached = loader.BOOL("HasScript");
				root.script->Script = loader.ID("Script");
				root.script->HasStarted = false;
				loader.EndStruct();
			}
			if (loader.BeginStruct("Camera"))
			{
				root.camera = std::make_shared<CameraComponent>(entity_id);
				root.camera->IsActiveCamera = loader.BOOL("IsActiveCamera");

				root.camera->Position = loader.VEC3("Position");
				root.camera->TargetPosition = loader.VEC3("TargetPosition");
				root.camera->Pitch = loader.FLOAT("Pitch");
				root.camera->TargetPitch = loader.FLOAT("TargetPitch");
				root.camera->Yaw = loader.FLOAT("Yaw");
				root.camera->TargetYaw = loader.FLOAT("TargetYaw");
				root.camera->Roll = loader.FLOAT("Roll");
				root.camera->TargetRoll = loader.FLOAT("TargetRoll");
				root.camera->FoV = loader.FLOAT("FoV");
				root.camera->TargetFoV = loader.FLOAT("TargetFoV");
				root.camera->Up = loader.VEC3("Up");
				root.camera->Right = loader.VEC3("Right");
				root.camera->Front = loader.VEC3("Front");
				root.camera->EnableDepthOfField = loader.BOOL("EnableDepthOfField");
				root.camera->Damping = loader.FLOAT("Damping");
				root.camera->Far = loader.FLOAT("Far");
				root.camera->Near = loader.FLOAT("Near");
				root.camera->FocalLength = loader.FLOAT("FocalLength");
				root.camera->FocalRange = loader.FLOAT("FocalRange");
				root.camera->Speed = loader.FLOAT("Speed");
				root.camera->Sensitivity = loader.FLOAT("Sensitivity");
				loader.EndStruct();
			}
			if (loader.BeginStruct("AudioSourceComponent"))
			{
				root.audiosource = std::make_shared<AudioSourceComponent>(entity_id);
				root.audiosource->AudioClip = loader.ID("AudioClip");
				root.audiosource->Pan = loader.FLOAT("Pan");
				root.audiosource->Volume = loader.FLOAT("Pan");
				root.audiosource->Is3DSource = loader.BOOL("Is3DSource");
				root.audiosource->Looping = loader.BOOL("Looping");
				root.audiosource->Mute = loader.BOOL("Mute");
				loader.EndStruct();
			}
			if (loader.BeginStruct("SkyboxComponent"))
			{
				root.skybox = std::make_shared<SkyboxComponent>(entity_id);
				root.skybox->SkyTexture = loader.ID("SkyTexture");
				root.skybox->IsSkyTextureSet = loader.BOOL("IsSkyTextureSet");
				root.skybox->IsDirty = true;
				loader.EndStruct();
			}
			if (loader.BeginStruct("LightProbeComponent"))
			{
				root.lightprobe = std::make_shared<LightProbeComponent>(entity_id);
				root.lightprobe->IsInterior = loader.BOOL("IsInterior");
				root.lightprobe->AutoBaked = loader.BOOL("AutoBake");
				root.lightprobe->ProbeType = (LightProbeComponent::LightProbeType)INT("Type");
				root.lightprobe->ProbePositions = loader.VEC4_VECTOR("ProbePositions");
				root.lightprobe->BRDFLUTResolution = loader.INT("BRDFLUT Resolution");
				root.lightprobe->BRDFResolution = loader.INT("PREFILTER Resolution");
				root.lightprobe->LightProbeResolution = loader.INT("IRRADIANCE Resolution");
				loader.EndStruct();
			}
			if (loader.BeginStruct("AnimationComponent"))
		{
			root.animation = std::make_shared<AnimationComponent>(entity_id);
			root.animation->Mesh = loader.ID("Mesh");
			root.animation->CurrentGraph = loader.ID("Graph");
			root.animation->IsGraphSet = loader.BOOL("HasGraph");
			loader.EndStruct();
		}

			if (loader.BeginCollection("Scripts"))
			{
				while (loader.CurrentCollectionIndex() < loader.GetCollectionSize())
				{
					if (loader.BeginStruct("LuaScript"))
					{
						auto script_id = loader.ID("ScriptID");
						root.scripts.push_back(script_id);
						loader.EndStruct();
					}
					loader.NextInCollection();
				}
				loader.EndCollection();
			}

			loader.EndStruct();
			temp.mEntities[e_index] = root;
			temp.mEntityIndex.push_back(e_index);
			e_index += 1;
		}
	
		if (loader.BeginCollection("Entities"))
		{			
			while (loader.CurrentCollectionIndex() < loader.GetCollectionSize())
			{
				if (loader.BeginStruct("Entity"))
				{
					EntityPrefabTemplate entity;
					auto entity_id = UUID();
					entity.TempalteID = entity_id;
					entity.SceneID = temp.SceneID;				

					if (loader.BeginCollection("Components"))
					{
						while (loader.CurrentCollectionIndex() < loader.GetCollectionSize())
						{						
							if (loader.BeginStruct("Tag"))
							{
								entity.HasTagComponent = true;
								entity.tag = std::make_shared<TagComponent>(entity_id, loader.STRING("Name"));
								entity.tag->IsActive = loader.BOOL("IsActive");
								entity.tag->Tags = loader.STRINGS_VECTOR("Tags");
								loader.EndStruct();
							}
							if (loader.BeginStruct("Transform"))
							{
								entity.HasTranformComponent = true;
								entity.transform = std::make_shared<TransformComponent>(entity_id);
								entity.transform->Position = loader.VEC3("Position");
								entity.transform->Scale = loader.VEC3("Scale");
								entity.transform->Rotation = glm::quat(glm::radians(loader.VEC3("Rotation")));
								loader.EndStruct();
							}
							if (loader.BeginStruct("Hierachy"))
							{
								entity.HasHierachyComponent = true;
								entity.hierachy = std::make_shared<HierachyComponent>(entity_id);
								entity.hierachy->Parent = temp.RootID;
								entity.hierachy->HasParent = true;
								loader.BeginCollection("Children");
								while (loader.CurrentCollectionIndex() < loader.GetCollectionSize())
								{
									if (loader.BeginStruct("Child"))
									{
										entity.hierachy->Children.push_back(loader.ID("ChildID"));
										loader.EndStruct();
									}
									loader.NextInCollection();
								}
								loader.EndCollection();

								loader.EndStruct();
							}
							if (loader.BeginStruct("Mesh"))
							{
								entity.HasMeshComponent = true;
								entity.mesh_component = std::make_shared<PrefabInstanceComponent>();
								entity.mesh_component->Mesh = loader.ID("MeshID");
								MeshesToLoad.push_back(entity.mesh_component->Mesh);
								entity.mesh_component->Materials = loader.UINT32_VECTOR("Materials");
								entity.mesh_component->meshType = (PrefabInstanceComponent::MeshType)loader.INT("MeshType");
								loader.EndStruct();
							}
							if (loader.BeginStruct("SpotlightComponent"))
							{
								entity.HasSpotlightComponent = true;
								entity.spotlight = std::make_shared<SpotlightComponent>(entity_id);
								entity.spotlight->Diffuse = loader.VEC3("Diffuse");
								entity.spotlight->Strength = loader.FLOAT("Strength");
								entity.spotlight->Constant = loader.FLOAT("Constant");
								entity.spotlight->Linear = loader.FLOAT("Linear");
								entity.spotlight->Quadratic = loader.FLOAT("Quadratic");
								entity.spotlight->InnerCutoff = loader.FLOAT("InnerCutoff");
								entity.spotlight->OutCutoff = loader.FLOAT("OuterCutoff");
								loader.EndStruct();
							}
							if (loader.BeginStruct("PointlightComponent"))
							{
								entity.HasPointlightComponent = true;
								entity.pointlight = std::make_shared<PointlightComponent>(entity_id);
								entity.pointlight->Diffuse = loader.VEC3("Diffuse");
								entity.pointlight->Strength = loader.FLOAT("Strength");
								entity.pointlight->Compression = loader.FLOAT("Compression");
								entity.pointlight->Radius = loader.FLOAT("Radius");
								loader.EndStruct();
							}
							if (loader.BeginStruct("SunlightComponent"))
							{
								entity.HasSunlightComponent = true;
								entity.sunlight = std::make_shared<SunlightComponent>(entity_id);
								entity.sunlight->Diffuse = loader.VEC3("Diffuse");
								entity.sunlight->Strength = loader.FLOAT("Strength");
								loader.EndStruct();
							}
							if (loader.BeginStruct("RigidbodyComponent"))
							{
								entity.HasRigidbodyComponent = true;
								entity.rigidbody = std::make_shared<RigidbodyComponent>(entity_id);
								entity.rigidbody->Mass = loader.FLOAT("Mass");
								entity.rigidbody->Friction = loader.FLOAT("Friction");
								entity.rigidbody->Bouncyness = loader.FLOAT("Bouncyness");
								entity.rigidbody->IsDynamic = loader.BOOL("IsDynamic");
								entity.rigidbody->IsKinematic = loader.BOOL("IsKinematic");

								entity.rigidbody->LinearConstraints[0] = loader.FLOAT("MovementConstraintX");
								entity.rigidbody->LinearConstraints[1] = loader.FLOAT("MovementConstraintY");
								entity.rigidbody->LinearConstraints[2] = loader.FLOAT("MovementConstraintZ");

								entity.rigidbody->AngularConstraints[0] = loader.FLOAT("RotationConstraintX");
								entity.rigidbody->AngularConstraints[1] = loader.FLOAT("RotationConstraintY");
								entity.rigidbody->AngularConstraints[2] = loader.FLOAT("RotationConstraintZ");

								loader.EndStruct();
							}
							if (loader.BeginStruct("ColliderComponent"))
							{
								entity.HasColliderComponent = true;
								entity.collider = std::make_shared<ColliderComponent>(entity_id);
								entity.collider->Size = loader.VEC3("Size");
								entity.collider->Shape = (ColliderType)loader.INT("Type");
								entity.collider->Radius = loader.FLOAT("Radius");
								entity.collider->Height = loader.FLOAT("Height");
								entity.collider->Offset = loader.VEC3("Offset");
								entity.collider->IsTrigger = loader.BOOL("IsTrigger");
								entity.collider->CollisionLayer = loader.ID("CollisionLayer");
								entity.collider->CollisionGroup = loader.ID("CollisionGroup");
								loader.EndStruct();
							}
							if (loader.BeginStruct("ScriptComponent"))
							{
								entity.HasScriptComponent = true;
								entity.script = std::make_shared<ScriptComponent>(entity_id);
								entity.script->Script = loader.ID("Script");
								entity.script->HasScriptAttached = loader.BOOL("HasScript");
								loader.EndStruct();
							}
							if (loader.BeginStruct("Camera"))
							{
								entity.HasCameraComponent = true;
								entity.camera = std::make_shared<CameraComponent>(entity_id);
								entity.camera->IsActiveCamera = loader.BOOL("IsActiveCamera");

								entity.camera->Position = loader.VEC3("Position");
								entity.camera->TargetPosition = loader.VEC3("TargetPosition");

								entity.camera->Pitch = loader.FLOAT("Pitch");
								entity.camera->TargetPitch = loader.FLOAT("TargetPitch");

								entity.camera->Yaw = loader.FLOAT("Yaw");
								entity.camera->TargetYaw = loader.FLOAT("TargetYaw");

								entity.camera->Roll = loader.FLOAT("Roll");
								entity.camera->TargetRoll = loader.FLOAT("TargetRoll");


								entity.camera->FoV = loader.FLOAT("FoV");
								entity.camera->TargetFoV = loader.FLOAT("TargetFoV");

								entity.camera->Up = loader.VEC3("Up");
								entity.camera->Right = loader.VEC3("Right");
								entity.camera->Front = loader.VEC3("Front");


								entity.camera->EnableDepthOfField = loader.BOOL("EnableDepthOfField");
								entity.camera->Damping = loader.FLOAT("Damping");
								entity.camera->Far = loader.FLOAT("Far");
								entity.camera->Near = loader.FLOAT("Near");
								entity.camera->FocalLength = loader.FLOAT("FocalLength");
								entity.camera->FocalRange = loader.FLOAT("FocalRange");

								entity.camera->Speed = loader.FLOAT("Speed");
								entity.camera->Sensitivity = loader.FLOAT("Sensitivity");
								loader.EndStruct();
							}
							if (loader.BeginStruct("AudioSourceComponent"))
							{
								entity.HasAudioSourceComponent = true;
								entity.audiosource = std::make_shared<AudioSourceComponent>(entity_id);
								entity.audiosource->AudioClip = loader.ID("AudioClip");
								entity.audiosource->Pan = loader.FLOAT("Pan");
								entity.audiosource->Volume = loader.FLOAT("Pan");
								entity.audiosource->Is3DSource = loader.BOOL("Is3DSource");
								entity.audiosource->Looping = loader.BOOL("Looping");
								entity.audiosource->Mute = loader.BOOL("Mute");
								loader.EndStruct();
							}
							if (loader.BeginStruct("SkyboxComponent"))
							{
								entity.HasSkyboxComponent = true;
								entity.skybox = std::make_shared<SkyboxComponent>(entity_id);
								entity.skybox->SkyTexture = loader.ID("SkyTexture");
								entity.skybox->IsSkyTextureSet = loader.BOOL("IsSkyTextureSet");
								entity.skybox->IsDirty = true;
								loader.EndStruct();
							}
							if (loader.BeginStruct("LightProbeComponent"))
							{
								entity.HaslightProbeComponent = true;
								entity.lightprobe = std::make_shared<LightProbeComponent>(entity_id);
								entity.lightprobe->IsInterior = loader.BOOL("IsInterior");
								entity.lightprobe->AutoBaked = loader.BOOL("AutoBake");
								entity.lightprobe->ProbeType = (LightProbeComponent::LightProbeType)INT("Type");
								entity.lightprobe->ProbePositions = loader.VEC4_VECTOR("ProbePositions");
								entity.lightprobe->BRDFLUTResolution = loader.INT("BRDFLUT Resolution");
								entity.lightprobe->BRDFResolution = loader.INT("PREFILTER Resolution");
								entity.lightprobe->LightProbeResolution = loader.INT("IRRADIANCE Resolution");
								loader.EndStruct();
							}
							if (loader.BeginStruct("AnimationComponent"))
							{
								entity.HasAnimationComponent = true;
								entity.animation = std::make_shared<AnimationComponent>(entity_id);
								entity.animation->Mesh = loader.ID("Mesh");
								entity.animation->CurrentGraph = loader.ID("Graph");
								entity.animation->IsGraphSet = loader.BOOL("HasGraph");
								loader.EndStruct();
							}

							loader.NextInCollection();
						}
						loader.EndCollection();
					}

					if (loader.BeginCollection("Scripts"))
					{
						while (loader.CurrentCollectionIndex() < loader.GetCollectionSize())
						{
							if (loader.BeginStruct("LuaScript"))
							{
								auto script_id = loader.ID("ScriptID");
								entity.scripts.push_back(script_id);

								auto luascript = ScriptManager::GetLuaScript(script_id);
								if (loader.BeginCollection("Properties"))
								{
									auto properties = luascript->GetProperties();
									while (loader.CurrentCollectionIndex() < loader.GetCollectionSize())
									{
										if (loader.BeginStruct("Property"))
										{
											switch ((PROPERTY_TYPE)INT("Type"))
											{
											case PROPERTY_TYPE::UUID:
											{
												auto prop = std::make_shared<ScriptProperty>();
												prop->Name = loader.STRING("Name");
												prop->Type = PROPERTY_TYPE::UUID;
												prop->ID = loader.ID("Value");
												properties[prop->Name] = prop;
												break;
											}
											case PROPERTY_TYPE::BOOL:
											{
												auto prop = std::make_shared<ScriptProperty>();
												prop->Name = loader.STRING("Name");
												prop->Type = PROPERTY_TYPE::BOOL;
												prop->Bool = loader.BOOL("Value");
												properties[prop->Name] = prop;
												break;
											}
											case PROPERTY_TYPE::COLOR3:
											{
												auto prop = std::make_shared<ScriptProperty>();
												prop->Name = loader.STRING("Name");
												prop->Type = PROPERTY_TYPE::COLOR3;
												prop->Color3 = loader.VEC3("Value");
												properties[prop->Name] = prop;
												break;
											}
											case PROPERTY_TYPE::COLOR4:
											{
												auto prop = std::make_shared<ScriptProperty>();
												prop->Name = loader.STRING("Name");
												prop->Type = PROPERTY_TYPE::COLOR4;
												prop->Color4 = loader.VEC4("Value");
												properties[prop->Name] = prop;
												break;
											}
											case PROPERTY_TYPE::FLOAT:
											{
												auto prop = std::make_shared<ScriptProperty>();
												prop->Name = loader.STRING("Name");
												prop->Type = PROPERTY_TYPE::FLOAT;
												prop->Float = loader.FLOAT("Value");
												properties[prop->Name] = prop;
												break;
											}
											case PROPERTY_TYPE::INT:
											{
												auto prop = std::make_shared<ScriptProperty>();
												prop->Name = loader.STRING("Name");
												prop->Type = PROPERTY_TYPE::INT;
												prop->Int = loader.INT("Value");
												properties[prop->Name] = prop;
												break;
											}
											case PROPERTY_TYPE::STRING:
											{
												auto prop = std::make_shared<ScriptProperty>();
												prop->Name = loader.STRING("Name");
												prop->Type = PROPERTY_TYPE::STRING;
												prop->String = loader.STRING("Value");
												properties[prop->Name] = prop;
												break;
											}
											}
											loader.EndStruct();
										}
										loader.NextInCollection();
									}
									loader.EndCollection();
								}
								loader.EndStruct();
							}
							loader.NextInCollection();
						}
						loader.EndCollection();
					}

					temp.mEntities[e_index] = entity;
					temp.mEntityIndex.push_back(e_index);
					e_index += 1;

					loader.EndStruct();				
				}
				loader.NextInCollection();
			}		
			loader.EndCollection();
		}
		
		if (loader.BeginCollection("Prefabs"))
		{
			while (loader.CurrentCollectionIndex() < loader.GetCollectionSize())
			{
				if (loader.BeginStruct("Prefab"))
				{
					ScenePrefab prefab;				
					prefab.PrefabID = loader.ID("Prefab_ID");
					prefab.SceneID = loader.ID("Scene_ID");
					prefab.ParentID = loader.ID("Parent_ID");				
					prefab.Position = loader.VEC3("Position");
					prefab.Scale = loader.VEC3("Scale");
					prefab.Rotation = glm::quat(glm::radians(loader.VEC3("Rotation")));
					temp.mPrefablist.push_back(prefab);
					loader.EndStruct();
				}
				loader.NextInCollection();
			}
			loader.EndCollection();
		}
		
		mPrefabTemplates[temp.SceneID] = temp;
		
		loader.EndStruct();
	}

}
