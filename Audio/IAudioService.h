#ifndef AUDIO_SERVICE_H
#define AUDIO_SERVICE_H

//#include "SWFramework/SWFramework.h"

namespace Phyre
{
	class AudioEventListElement;
}

namespace sw
{
	
	
	class IAudioService
		: public IService
	{
	public:

		virtual Phyre::PResult VinitAudio			( void ) = 0;

		virtual Phyre::PResult VloadAudioAssets		( void ) = 0;

		virtual Phyre::PResult VplaySound			( const char* eventName ) = 0;

		virtual Phyre::PResult VplaySound			( Vectormath::Aos::Vector3 &position, const char* eventName ) = 0;

		virtual Phyre::PResult VplayCancelableSound ( const char* eventName ) = 0;

		virtual AudioEventListElement* VplayPersistantSound(Vectormath::Aos::Vector3 position,
													const char* eventName, float fVolume  = 1.0f)
													= 0;

		virtual Phyre::PResult VupdatePersistantSound(Vectormath::Aos::Vector3 *position,
													Vectormath::Aos::Vector3 *velocity,
													AudioEventListElement &element)
													= 0;

		virtual Phyre::PResult VstopPersistantSound	( AudioEventListElement *element ) = 0;

		virtual Phyre::PResult VSetMusic			( const char* eventName ) = 0;

		virtual void		   setListenerPosition	( Vectormath::Aos::Vector3 listenerPos ) = 0;

		virtual void		   setMusicVolume		( float vol ) = 0;

		virtual void		   adjustMusicVolume	( float volAdj ) = 0;

		virtual float		   getMusicVolume		( void ) const = 0;
		
		virtual void		   playMusic			( void ) = 0;

		virtual void		   pauseMusic			( void ) = 0;

		virtual void		   resumeMusic			( void ) = 0;

		virtual bool		   musicPlaying			( void ) const = 0;

	};

};

#endif