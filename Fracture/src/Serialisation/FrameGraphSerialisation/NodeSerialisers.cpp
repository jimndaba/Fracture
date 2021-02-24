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


#pragma region CleaFrame

Fracture::ClearFrameSerialiser::ClearFrameSerialiser(json node):FrameNodeSerialiser(node)
{}

void Fracture::ClearFrameSerialiser::visitClearFrameNode(const std::shared_ptr<ClearFrame>& node) const
{
	j()["Type"] = "ClearFrame";
	j()["Name"] = node->GetName();
	j()["Color"] = { node->Color.r, node->Color.g, node->Color.b };
}

#pragma endregion

#pragma region AdditiveMix
Fracture::AdditiveMixNodeSerialiser::AdditiveMixNodeSerialiser(json node) :FrameNodeSerialiser(node)
{}


void Fracture::AdditiveMixNodeSerialiser::visitAdditiveMixNode(const std::shared_ptr<AdditiveMixNode>& node) const
{
	j()["Type"] = "AdditiveMixNode";
	j()["Name"] = node->GetName();
}
#pragma endregion

#pragma region  Gaussian
Fracture::GaussianBlurNodeSerialiser::GaussianBlurNodeSerialiser(json node) :FrameNodeSerialiser(node)
{

}


void Fracture::GaussianBlurNodeSerialiser::visitGaussianBlurNode(const std::shared_ptr<GaussianBlurNode>& node) const
{
	j()["Type"] = "GaussianBlurNode";
	j()["Name"] = node->GetName();
	j()["Amount"] = node->amount;
}
#pragma endregion

#pragma region  Depth
Fracture::DepthNodeSerialiser::DepthNodeSerialiser(json node) :FrameNodeSerialiser(node)
{
}

void Fracture::DepthNodeSerialiser::visitDepthNode(const std::shared_ptr<DepthNode>& node) const
{
	j()["Type"] = "DepthNode";
	j()["Name"] = node->GetName();
	j()["Near"] = node->NearPlane;
	j()["Far"] = node->FarPlane;
}
#pragma endregion

#pragma region  Intermediate
Fracture::IntermediateNodeSerialiser::IntermediateNodeSerialiser(json node) :FrameNodeSerialiser(node)
{
}

void Fracture::IntermediateNodeSerialiser::visitIntermediateNode(const std::shared_ptr<IntermediateNode>& node) const
{
	j()["Type"] = "IntermediateNode";
	j()["Name"] = node->GetName();
}
#pragma endregion

#pragma region  Lambertian
Fracture::LambertianNodeSerialiser::LambertianNodeSerialiser(json node):FrameNodeSerialiser(node)
{
}

void Fracture::LambertianNodeSerialiser::visitLambertianNode(const std::shared_ptr<LambertianNode>& node) const
{
	j()["Type"] = "LambertianNode";
	j()["Name"] = node->GetName();
}
#pragma endregion

#pragma region  Link
Fracture::LinkSerialiser::LinkSerialiser(json node) :FrameNodeSerialiser(node)
{
	
}

void Fracture::LinkSerialiser::visitLink(const std::shared_ptr<Link>& link) const
{
	j()["Type"] = "Link";
	j()["NodeFrom"] = link->FrameNode_From;
	j()["NodeTo"] = link->FrameNode_To;
	j()["NodeSource"] = link->FrameNode_Source;
	j()["NodeResource"] = link->FrameNode_Resource;
}
#pragma endregion

#pragma region  Sink
Fracture::SinkNodeSerialiser::SinkNodeSerialiser(json node) :FrameNodeSerialiser(node)
{
}

void Fracture::SinkNodeSerialiser::visitSinkNode(const std::shared_ptr<SinkNode>& node) const
{
	j()["Type"] = "SinkNode";
	j()["Name"] = node->GetName();
}
#pragma endregion

#pragma region  Source
Fracture::SourceNodeSerialiser::SourceNodeSerialiser(json node) :FrameNodeSerialiser(node)
{	
}

void Fracture::SourceNodeSerialiser::visitSourceNode(const std::shared_ptr<SourceNode>& node) const
{
	j()["Type"] = "SourceNode";
	j()["Name"] = node->GetName();
}
#pragma endregion