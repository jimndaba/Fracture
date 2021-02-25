#include "Link.h"


Fracture::Link::Link()
{
}

nlohmann::json Fracture::Link::Accept(const std::shared_ptr<FrameNodeSerialiser>& visitor)
{
	return visitor->visitLink(*this);
}
