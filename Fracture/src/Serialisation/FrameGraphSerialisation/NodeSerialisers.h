#pragma once
#ifndef NodeSerialisers_H
#define NodeSerialisers_H

#include "FrameNodeSerialiser.h"

namespace Fracture
{
	class LinkSerialiser :public FrameNodeSerialiser
	{
	public:
		LinkSerialiser();		
		json visitLink(const Link& link)const override;
	};
	class SinkNodeSerialiser :public FrameNodeSerialiser
	{
	public:
		SinkNodeSerialiser();
		json visitSinkNode(const SinkNode& node)const override;
	};
	class SourceNodeSerialiser :public FrameNodeSerialiser
	{
	public:
		SourceNodeSerialiser();		
		json visitSourceNode(const SourceNode& node)const override;		
	};
	class ClearFrameSerialiser :public FrameNodeSerialiser
	{
	public:
		ClearFrameSerialiser();
		json visitClearFrameNode(const ClearFrame& node)const override;
	};
	class AdditiveMixNodeSerialiser :public FrameNodeSerialiser
	{
	public:
		AdditiveMixNodeSerialiser();
		json visitAdditiveMixNode(const AdditiveMixNode& node)const override;
	};
	class MixNodeSerialiser :public FrameNodeSerialiser
	{
	public:
		MixNodeSerialiser();
		json visitMixNode(const MixNode& node)const override;
	};
	class GaussianBlurNodeSerialiser :public FrameNodeSerialiser
	{
	public:
		GaussianBlurNodeSerialiser();
		json visitGaussianBlurNode(const GaussianBlurNode& node)const override;
		
	};
	class DepthNodeSerialiser :public FrameNodeSerialiser
	{
	public:
		DepthNodeSerialiser();
		json visitDepthNode(const DepthNode& node)const override;	
	};
	class IntermediateNodeSerialiser :public FrameNodeSerialiser
	{
	public:
		IntermediateNodeSerialiser();
		json visitIntermediateNode(const IntermediateNode& node)const override;	
	};
	class LambertianNodeSerialiser :public FrameNodeSerialiser
	{
	public:
		LambertianNodeSerialiser();
		json visitLambertianNode(const LambertianNode& node)const override;
	};
	class MultiplyMixNodeSerialiser :public FrameNodeSerialiser
	{
	public:
		MultiplyMixNodeSerialiser();
		json visitMultiplyMixNode(const MultiplyMixNode& node)const override;
	};
	class OutlineNodeSerialiser :public FrameNodeSerialiser
	{
	public:
		OutlineNodeSerialiser();
		json visitOutlineNode(const OutlineNode& node)const override;
	};
	class SSAONodeSerialiser :public FrameNodeSerialiser
	{
	public:
		SSAONodeSerialiser();
		json visitSSAONode(const SSAONode& node)const override;
	};
	class ThresholdNodeSerialiser :public FrameNodeSerialiser
	{
	public:
		ThresholdNodeSerialiser();
		json visitThresholdNode(const ThresholdNode& node)const override;
	};
	class ToneMappingNodeSerialiser :public FrameNodeSerialiser
	{
	public:
		ToneMappingNodeSerialiser();
		json visitToneMappingNode(const ToneMappingNode& node)const override;
	};
}

#endif