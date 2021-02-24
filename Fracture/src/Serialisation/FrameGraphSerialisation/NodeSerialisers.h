#pragma once
#ifndef NodeSerialisers_H
#define NodeSerialisers_H

#include "FrameNodeSerialiser.h"

namespace Fracture
{

	class LinkSerialiser :public FrameNodeSerialiser
	{
	public:
		LinkSerialiser(json node);		
		void visitLink(const std::shared_ptr<Link>& link)const override;
	};
	class SinkNodeSerialiser :public FrameNodeSerialiser
	{
	public:
		SinkNodeSerialiser(json node);
		void visitSinkNode(const std::shared_ptr<SinkNode>& node)const override;
	};
	class SourceNodeSerialiser :public FrameNodeSerialiser
	{
	public:
		SourceNodeSerialiser(json node);		
		void visitSourceNode(const std::shared_ptr<SourceNode>& node)const override;		
	};
	class ClearFrameSerialiser :public FrameNodeSerialiser
	{
	public:
		ClearFrameSerialiser(json node);
		void visitClearFrameNode(const std::shared_ptr<ClearFrame>& node)const override;
	};
	class AdditiveMixNodeSerialiser :public FrameNodeSerialiser
	{
	public:
		AdditiveMixNodeSerialiser(json node);
		void visitAdditiveMixNode(const std::shared_ptr<AdditiveMixNode>& node)const override;
	};
	class GaussianBlurNodeSerialiser :public FrameNodeSerialiser
	{
	public:
		GaussianBlurNodeSerialiser(json node);
		void visitGaussianBlurNode(const std::shared_ptr<GaussianBlurNode>& node)const override;
		
	};
	class DepthNodeSerialiser :public FrameNodeSerialiser
	{
	public:
		DepthNodeSerialiser(json node);
		void visitDepthNode(const std::shared_ptr<DepthNode>& node)const override;	
	};
	class IntermediateNodeSerialiser :public FrameNodeSerialiser
	{
	public:
		IntermediateNodeSerialiser(json node);
		void visitIntermediateNode(const std::shared_ptr<IntermediateNode>& node)const override;	
	};
	class LambertianNodeSerialiser :public FrameNodeSerialiser
	{
	public:
		LambertianNodeSerialiser(json node);
		void visitLambertianNode(const std::shared_ptr<LambertianNode>& node)const override;
	};
		
	//class MixNode;
	//class MultiplyMixNode;
	//class OutlineNode;
	//class SSAONode;
	//class ThresholdNode;
	//class ToneMappingNode;
	
}

#endif