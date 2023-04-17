#pragma once
#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include "soloud/soloud.h"
#include "soloud/soloud_wav.h"

namespace Fracture
{

	class AudioManager
	{
	public:
		AudioManager();

		void OnInit();

		void OnLoadContent();

		void OnUpdate(float dt);

		void OnShutdown();

	private:
		SoLoud::Soloud gSoloud;	
	};


}

#endif