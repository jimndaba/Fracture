#include "FracturePCH.h"
#include "Eventbus.h"


std::unordered_map<std::type_index, Fracture::HandlerList*> Fracture::Eventbus::subscribers;

Fracture::Eventbus::Eventbus()
{
}
