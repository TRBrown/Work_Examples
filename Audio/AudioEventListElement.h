#pragma once

using namespace Phyre;

#ifdef PHYRE_ENABLE_AUDIO
#include <Audio/PhyreAudio.h>
using namespace PAudio;
#endif

namespace Phyre
{

	class AudioEventListElement 
		: 
#ifdef PHYRE_AUDIO_PLATFORM_FMOD
		public PAudioEvent ,
#endif
		 public PSimpleListElement < AudioEventListElement >
	{

	protected:

	public:
		AudioEventListElement()
		{}

		virtual ~AudioEventListElement()
		{}

	};
}