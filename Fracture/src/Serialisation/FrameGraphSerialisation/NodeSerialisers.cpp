#include "NodeSerialisers.h"
#include "Rendering/Framegraph/PassLibrary/ClearFrame.h"
#include "Rendering/Framegraph/PassLibrary/LambertianNode.h"
#include "Rendering/Framegraph/PassLibrary/ToneMappingNode.h"
#include "Rendering/Framegraph/PassLibrary/ThresholdNode.h"
#include "Rendering/Framegraph/PassLibrary/AdditiveMixNode.h"
#include "Rendering/Framegraph/PassLibrary/GaussianBlurNode.h"
#include "Rendering/Framegraph/PassLibrary/DepthNode.h"
#include "Rendering/Framegraph/PassLibrary/SSAONode.h"
#include "Rendering/Framegraph/PassLibrary/MultiplyMix.h"
#include "Rendering/Framegraph/PassLibrary/PickingPass.h"
#include "Rendering/Framegraph/PassLibrary/IntermediateNode.h"
#include "Rendering/Framegraph/PassLibrary/OutlineNode.h"
#include "Rendering/Framegraph/PassLibrary/MixNode.h"
#include "Rendering/Framegraph/Link.h"
#include "Rendering/Framegraph/SourceNode.h"
#include "Rendering/Framegraph/SinkNode.h"


#pragma region CleaFrame

Fracture::ClearFrameSerialiser::ClearFrameSerialiser():FrameNodeSerialiser()
{}

nlohmann::json Fracture::ClearFrameSerialiser::visitClearFrameNode(const ClearFrame& node) const
{
	json j;
	j["Type"] = "ClearFrame";
	j["Name"] = node.GetName();
	j["Color"] = { node.Color.r, node.Color.g, node.Color.b };
	return j;
}

#pragma endregion

#pragma region AdditiveMix
Fracture::AdditiveMixNodeSerialiser::AdditiveMixNodeSerialiser() :FrameNodeSerialiser()
{}


nlohmann::json Fracture::AdditiveMixNodeSerialiser::visitAdditiveMixNode(const AdditiveMixNode& node) const
{
	json j;
	j["Type"] = "AdditiveMixNode";
	j["Name"] = node.GetName();
	return j;
}
#pragma endregion

#pragma region  Gaussian
Fracture::GaussianBlurNodeSerialiser::GaussianBlurNodeSerialiser() :FrameNodeSerialiser()
{

}


nlohmann::json Fracture::GaussianBlurNodeSerialiser::visitGaussianBlurNode(const GaussianBlurNode& node) const
{
	json j;
	j["Type"] = "GaussianBlurNode";
	j["Name"] = node.GetName();
	j["Amount"] = node.amount;
	return j;
}
#pragma endregion

#pragma region  Depth
Fracture::DepthNodeSerialiser::DepthNodeSerialiser() :FrameNodeSerialiser()
{
}

nlohmann::json Fracture::DepthNodeSerialiser::visitDepthNode(const DepthNode& node) const
{
	json j;
	j["Type"] = "DepthNode";
	j["Name"] = node.GetName();
	j["Near"] = node.NearPlane;
	j["Far"] = node.FarPlane;
	return j;
}
#pragma endregion

#pragma region  Intermediate
Fracture::IntermediateNodeSerialiser::IntermediateNodeSerialiser() :FrameNodeSerialiser()
{
}

nlohmann::json Fracture::IntermediateNodeSerialiser::visitIntermediateNode(const IntermediateNode& node) const
{
	json j;
	j["Type"] = "IntermediateNode";
	j["Name"] = node.GetName();
	return j;
}
#pragma endregion

#pragma region  Lambertian
Fracture::LambertianNodeSerialiser::LambertianNodeSerialiser():FrameNodeSerialiser()
{
}

nlohmann::json Fracture::LambertianNodeSerialiser::visitLambertianNode(const LambertianNode& node) const
{
	json j;
	j["Type"] = "LambertianNode";
	j["Name"] = node.GetName();
	return j;
}
#pragma endregion

#pragma region  Link
Fracture::LinkSerialiser::LinkSerialiser() :FrameNodeSerialiser()
{
	
}

nlohmann::json Fracture::LinkSerialiser::visitLink(const Link& link) const
{
	FRACTURE_INFO("Serialising Link");
	json j;
	j["Type"] = "Link";
	j["NodeFrom"] = link.FrameNode_From;
	j["NodeTo"] = link.FrameNode_To;
	j["NodeSource"] = link.FrameNode_Source;
	j["NodeResource"] = link.FrameNode_Resource;
	return j;
}
#pragma endregion

#pragma region  Sink
Fracture::SinkNodeSerialiser::SinkNodeSerialiser() :FrameNodeSerialiser()
{
}

nlohmann::json Fracture::SinkNodeSerialiser::visitSinkNode(const SinkNode& node) const
{
	json j;
	j["Type"] = "SinkNode";
	j["Name"] = node.GetName();
	return j;
}
#pragma endregion

#pragma region  Source
Fracture::SourceNodeSerialiser::SourceNodeSerialiser():FrameNodeSerialiser()
{	
}

nlohmann::json Fracture::SourceNodeSerialiser::visitSourceNode(const SourceNode& node) const
{
	json j;
	j["Type"] = "SourceNode";
	j["Name"] = node.GetName();
	return j;
}
#pragma endregion

#pragma region  MultiplyMix
Fracture::MultiplyMixNodeSerialiser::MultiplyMixNodeSerialiser():FrameNodeSerialiser()
{
}

nlohmann::json Fracture::MultiplyMixNodeSerialiser::visitMultiplyMixNode(const MultiplyMixNode& node) const
{
	json j;
	j["Type"] = "MultiplyMix";
	j["Name"] = node.GetName();
	return j;
}
#pragma endregion

#pragma region Outline
Fracture::OutlineNodeSerialiser::OutlineNodeSerialiser() :FrameNodeSerialiser()
{
}

nlohmann::json Fracture::OutlineNodeSerialiser::visitOutlineNode(const OutlineNode& node) const
{
	json j;
	j["Type"] = "Outline";
	j["Name"] = node.GetName();
	return j;
}
#pragma endregion

#pragma region SSAO
Fracture::SSAONodeSerialiser::SSAONodeSerialiser() :FrameNodeSerialiser()
{
}

nlohmann::json Fracture::SSAONodeSerialiser::visitSSAONode(const SSAONode& node) const
{
	json j;
	j["Type"] = "SSAO";
	j["Name"] = node.GetName();
	j["Radius"] = node.radius;
	j["Area"] = node.area;
	j["Falloff"] = node.falloff;
	j["Strenght"] = node.total_strength; 
	return j;
}
#pragma endregion

#pragma region Threshold
Fracture::ThresholdNodeSerialiser::ThresholdNodeSerialiser() :FrameNodeSerialiser()
{
}

nlohmann::json Fracture::ThresholdNodeSerialiser::visitThresholdNode(const ThresholdNode& node) const
{
	json j;
	j["Type"] = "Threshold";
	j["Name"] = node.GetName();
	j["Threshold"] = node.brightPassThreshold;
	return j;
}
#pragma endregion

#pragma region ToneMap
Fracture::ToneMappingNodeSerialiser::ToneMappingNodeSerialiser() :FrameNodeSerialiser()
{
}

nlohmann::json Fracture::ToneMappingNodeSerialiser::visitToneMappingNode(const ToneMappingNode& node) const
{
	json j;
	j["Type"] = "ToneMapping";
	j["Name"] = node.GetName();
	j["Gamma"] = node.Gamma;
	j["Exposure"] = node.Exposure;
	return j;
}
#pragma endregion

#pragma region ToneMap
Fracture::MixNodeSerialiser::MixNodeSerialiser() :FrameNodeSerialiser()
{
}

nlohmann::json Fracture::MixNodeSerialiser::visitMixNode(const MixNode& node) const
{
	json j;
	j["Type"] = "MixNode";
	j["Name"] = node.GetName();
	return j;
}
#pragma endregion
