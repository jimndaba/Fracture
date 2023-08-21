#include "FracturePCH.h"
#include "AnimationBlender.h"

Fracture::AnimationBlender::AnimationBlender()
{
}

void Fracture::AnimationBlender::BlendSamples(BlendFunctionType func, BlendSpaceType space, PoseSample in1, PoseSample in2, float factor)
{

}

Fracture::PoseSample Fracture::AnimationBlender::AdditiveBlend1D(PoseSample in1, PoseSample in2, float factor)
{
	PoseSample sample;
	sample.Position = glm::mix(in1.Position,in2.Position, factor);
	sample.Scale = glm::mix(in1.Scale,in2.Scale, factor);
	sample.Rotation = glm::slerp(in1.Rotation, in2.Rotation, factor);
	return sample;
}
