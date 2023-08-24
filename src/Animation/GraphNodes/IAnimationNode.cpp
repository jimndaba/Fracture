#include "FracturePCH.h"
#include "IAnimationNode.h"

Fracture::IAnimationNode::IAnimationNode() :Result(),NodeID()
{
}

Fracture::IOperationNode::IOperationNode() :
	IAnimationNode()
{

}

Fracture::IPoseNode::IPoseNode() :
	IAnimationNode()
{
}
