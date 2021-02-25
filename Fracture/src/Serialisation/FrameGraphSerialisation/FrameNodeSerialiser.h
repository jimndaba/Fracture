#pragma once
#ifndef FRAMENODESERIALISER_H
#define FRAMENODESERIALISER_H

#include <memory>
#include "nlohmann/json.hpp"

namespace Fracture
{
	using json = nlohmann::json;

	class FrameNode;
	class ClearFrame;
	class AdditiveMixNode;
	class GaussianBlurNode;
	class DepthNode;
	class IntermediateNode;
	class LambertianNode;
	class MixNode;
	class MultiplyMixNode;
	class OutlineNode;
	class SSAONode;
	class ThresholdNode;
	class ToneMappingNode;
	class Link;
	class SinkNode;
	class SourceNode;

	class FrameNodeSerialiser
	{
	public:
		FrameNodeSerialiser()
		{};
		~FrameNodeSerialiser() = default;
		virtual json visitClearFrameNode(const ClearFrame& node)const { json j; return j; };
		virtual json visitAdditiveMixNode(const AdditiveMixNode& node)const { json j; return j; };
		virtual json visitGaussianBlurNode(const GaussianBlurNode& node)const { json j; return j; };
		virtual json visitDepthNode(const DepthNode& node)const { json j; return j; };
		virtual json visitIntermediateNode(const IntermediateNode& node)const { json j; return j; };
		virtual json visitLambertianNode(const LambertianNode& node)const { json j; return j; };
		virtual json visitMixNode(const MixNode& node)const { json j; return j; };
		virtual json visitMultiplyMixNode(const MultiplyMixNode& node)const { json j; return j; };
		virtual json visitOutlineNode(const OutlineNode& node)const { json j; return j; };
		virtual json visitSSAONode(const SSAONode& node)const { json j; return j; };
		virtual json visitThresholdNode(const ThresholdNode& node)const { json j; return j; };
		virtual json visitToneMappingNode(const ToneMappingNode& node)const { json j; return j; };
		virtual json visitSinkNode(const SinkNode& node)const { json j; return j; };
		virtual json visitSourceNode(const SourceNode& node)const { json j; return j; };
		virtual json visitLink(const Link& link)const { json j; return j; };
	private:
	};

}

#endif