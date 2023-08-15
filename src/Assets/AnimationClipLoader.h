#pragma once
#ifndef ANIMATIONCLIPLOADER_H
#define ANIMATIONCLIPLOADER_H

namespace Fracture
{
	struct AnimationClip;

	struct AnimationClipLoader
	{
		AnimationClipLoader();


		static std::shared_ptr<Fracture::AnimationClip> LoadAnimationClip(const std::string& path);




	};



}

#endif