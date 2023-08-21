#include "FracturePCH.h"
#include "IAnimationNode.h"

Fracture::IAnimationNode::IAnimationNode()
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
