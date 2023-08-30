#include "FracturePCH.h"
#include "AnimationClipLoader.h"
#include "Serialisation/AnimationSerialiser.h"

Fracture::AnimationClipLoader::AnimationClipLoader()
{
}

std::shared_ptr<Fracture::AnimationClip> Fracture::AnimationClipLoader::LoadAnimationClip(const std::string& path)
{
    AnimationSerialiser loader(ISerialiser::IOMode::Open, ISerialiser::SerialiseFormat::Json);

    loader.Open(path);

    auto clip = loader.ReadAnimation();

    if (clip)
        return clip;

    return nullptr;
}
