#include "ComponentSerialisers.h"
#include "Component/TagComponent.h"
#include "Component/ScriptComponent.h"
#include "Component/BoxColliderComponent.h"
#include "Component/RigidBodyComponent.h"
#include "Component/RelationshipComponent.h"
#include "Component/TransformComponent.h"
#include "Component/LightComponent.h"
#include "Component/CameraControllerComponent.h"
#include "Component/BillboardComponent.h"
#include "Component/RelationshipComponent.h"
#include "Component/RenderComponent.h"
#include "Component/AnimatorComponent.h"
#include "Rendering/Model.h"
#include "Rendering/Environment.h"
#include "Rendering/OpenGL/Texture2D.h"

Fracture::TagComponentSerialiser::TagComponentSerialiser() :ComponentSerialiser()
{
}

nlohmann::json Fracture::TagComponentSerialiser::visitTagComponent(const TagComponent& node) const
{
    json j;
    j["ComponentType"] = "TagComponent";
    j["EntityID"] = (uint32_t)node.GetID();
    j["Name"] = node.Name;
    return j;
}

Fracture::RelationShipComponentSerialiser::RelationShipComponentSerialiser() :ComponentSerialiser()
{
}

nlohmann::json Fracture::RelationShipComponentSerialiser::visitRelationShipComponent(const RelationShipComponent& node) const
{
    json j;
    j["ComponentType"] = "RelationShipComponent";
    j["EntityID"] = (uint32_t)node.GetID();
    j["HasParent"] = node.hasParent;
    j["ParentID"] = node.m_parent;
    j["NoChildren"] = node.noChildren;

    json serialised_children = json::array_t();
    const auto& children = node.GetChildren();

    for (auto& child : children)
    {
        json c;
        c["ChildID"] = (uint32_t)child;
        serialised_children.push_back(c);
    }

    j["Children"] = serialised_children;

    return j;
}

Fracture::TransformComponentSerialiser::TransformComponentSerialiser() :ComponentSerialiser()
{
}

nlohmann::json Fracture::TransformComponentSerialiser::visitTransformComponent(const TransformComponent& node) const
{
    json j;
    j["ComponentType"] = "TransformComponent";
    j["EntityID"] = (uint32_t)node.GetID();
    j["Position"] = { node.Position().x,node.Position().y ,node.Position().z };
    j["Scale"] = { node.Scale().x,node.Scale().y ,node.Scale().z };
    j["Rotation"] = { node.Rotation().x,node.Rotation().y ,node.Rotation().z };
    return j;
}

Fracture::CameraControllerComponentSerialiser::CameraControllerComponentSerialiser() :ComponentSerialiser()
{
}

nlohmann::json Fracture::CameraControllerComponentSerialiser::visitCameraControllerComponent(const CameraControllerComponent& node) const
{
    json j;
    j["ComponentType"] = "CameraController";
    j["EntityID"] = (uint32_t)node.GetID();
    j["Position"] = { node.Position.x,node.Position.y ,node.Position.z };
    return j;
}

Fracture::RigidBodyComponentSerialiser::RigidBodyComponentSerialiser() :ComponentSerialiser()
{
}

nlohmann::json Fracture::RigidBodyComponentSerialiser::visitRigidBodyComponent(const RigidBodyComponent& node) const
{
    json j;
    j["ComponentType"] = "RigidbodyComponent";
    j["EntityID"] = (uint32_t)node.GetID();
    j["Mass"] = node.Mass;
    return j;
}

Fracture::RenderComponentSerialiser::RenderComponentSerialiser() :ComponentSerialiser()
{
}

nlohmann::json Fracture::RenderComponentSerialiser::visitRenderComponent(const RenderComponent& node) const
{
    json j;
    j["ComponentType"] = "RenderComponent";
    j["EntityID"] = (uint32_t)node.GetID();
    j["Model"] = node.GetModel()->Name;
    return j;
}

Fracture::ScriptComponentSerialiser::ScriptComponentSerialiser() :ComponentSerialiser()
{
}

nlohmann::json Fracture::ScriptComponentSerialiser::visitScriptComponent(const ScriptComponent& node) const
{
    json j;
    j["ComponentType"] = "ScriptComponent";
    j["EntityID"] = (uint32_t)node.GetID();
    return j;
}

Fracture::LightComponentSerialiser::LightComponentSerialiser() :ComponentSerialiser()
{
}

nlohmann::json Fracture::LightComponentSerialiser::visitLightComponent(const LightComponent& node) const
{
    json j;
    j["ComponentType"] = "LightComponent";
    j["EntityID"] = (uint32_t)node.GetID();
    j["Direction"] = { node.GetDirection().x, node.GetDirection().y, node.GetDirection().z };
    j["Position"] = { node.GetPosition().x, node.GetPosition().y, node.GetPosition().z };
    j["Specular"] = { node.GetSpecular().x, node.GetSpecular().y, node.GetSpecular().z,node.GetSpecular().w };
    j["Diffuse"] = { node.GetDiffuse().x, node.GetDiffuse().y, node.GetDiffuse().z,node.GetDiffuse().w };
    j["Ambient"] = { node.GetAmbient().x, node.GetAmbient().y, node.GetAmbient().z,node.GetAmbient().w };
    j["Radiance"] = { node.GetRadiance().x, node.GetRadiance().y, node.GetRadiance().z };
    j["Intensity"] = node.Intensity();
    j["Cutoff"] = node.GetCutoff();
    j["OuterCutoff"] = node.GetOuterCutOff();
    j["Linear"] = node.GetLinear();
    j["Quadratic"] = node.GetQuadratic();
    j["Constant"] = node.GetConstant();
    //j["Environment Map"] = node.GetEnvironment()->m_enviroment->GetName();
    return j;
}

Fracture::BillboardComponentSerialiser::BillboardComponentSerialiser() :ComponentSerialiser()
{
}

nlohmann::json Fracture::BillboardComponentSerialiser::visitBillboardComponent(const BillboardComponent& node) const
{
    json j;
    j["ComponentType"] = "Billboard";
    j["EntityID"] = (uint32_t)node.GetID();
    return j;
}

Fracture::BoxColliderComponentSerialiser::BoxColliderComponentSerialiser() :ComponentSerialiser()
{
}

nlohmann::json Fracture::BoxColliderComponentSerialiser::visitBoxColliderComponent(const BoxColliderComponent& node) const
{
    json j;
    j["ComponentType"] = "BoxColliderComponent";
    j["EntityID"] = (uint32_t)node.GetID();
    return j;
}

Fracture::AnimatorComponentSerialiser::AnimatorComponentSerialiser() :ComponentSerialiser()
{
}

nlohmann::json Fracture::AnimatorComponentSerialiser::visitAnimatorComponent(const AnimatorComponent& node) const
{
    json j;
    j["ComponentType"] = "Tag";
    j["EntityID"] = (uint32_t)node.GetID();
    return j;
}
