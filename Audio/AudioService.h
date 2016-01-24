#pragma once

#include "IAudioService.h"
#include "AudioEventListElement.h"

#ifndef PHYRE_TOOL_BUILD

namespace sw
{
	class CAudioService
		: public IAudioService
	{
	public:

		CAudioService(void);
		virtual ~CAudioService ( void );

		virtual Phyre::PResult	Initialize( void )  ;

		virtual Phyre::PResult	Terminate( void )  ;

		//
		virtual Phyre::PResult	VinitAudio( void )  ;

		virtual Phyre::PResult	VloadAudioAssets( void )  ;

		virtual void			VUpdate( float fElapsedTime, EGameUpdateResult eUpdate );

		virtual Phyre::PResult	VplaySound( const char* eventName );

		virtual Phyre::PResult	VplaySound ( Vectormath::Aos::Vector3 &position, const char* eventName );

		virtual Phyre::PResult VplayCancelableSound ( const char* eventName );

		Phyre::PResult	playSound( Vectormath::Aos::Vector3 &position, const char* eventName );

		virtual AudioEventListElement* VplayPersistantSound(Vectormath::Aos::Vector3 position,
													const char* eventName, float fVolume  = 1.0f );

		virtual Phyre::PResult VupdatePersistantSound(Vectormath::Aos::Vector3 *position,
													Vectormath::Aos::Vector3 *velocity,
													AudioEventListElement &element )  ;

		virtual Phyre::PResult VstopPersistantSound( AudioEventListElement *element ) ;

		virtual Phyre::PResult VSetMusic( const char* eventName ) ;

		Phyre::PResult SetMusic( Vectormath::Aos::Vector3 position, const char* eventName );

		virtual void setListenerPosition( Vectormath::Aos::Vector3 listenerPos ) ;

		void	setListenerPosition(Vectormath::Aos::Vector3 listenerPos,
									Vectormath::Aos::Vector3 at,
									Vectormath::Aos::Vector3 up);

		// Audio Actions
		void stopAudioEvents();

		void setListenerWorldMatrix(PWorldMatrix *WmCamera)
		{
#ifdef PHYRE_AUDIO_PLATFORM_FMOD
			m_pListener = WmCamera;
#endif
		}

		// Event Controls
		void setEventVolume(float vol)
		{
			
			m_fEventVolume = vol;
		}

		void adjustEventVolume(float volAdj)
		{
			if (volAdj > 0)
			{
				m_fEventVolume += 0.1f;
			}
			else
			{
				m_fEventVolume -= 0.1f;
			}

			m_fEventVolume = (PhyreClamp(m_fEventVolume, 0.0f, 1.0f));
		}

		float getEventVolume() const
		{
			return m_fMusicVolume;
		}


		// Music Controls
		virtual void setMusicVolume(float vol)
		{
#ifdef PHYRE_AUDIO_PLATFORM_FMOD
			m_fMusicVolume = vol;
			m_MusicEvent.setVolume(vol);
#endif
		}

		virtual void adjustMusicVolume(float volAdj)
		{
#ifdef PHYRE_AUDIO_PLATFORM_FMOD
			m_fMusicVolume += (m_fVolumeMultiplier * PhyreClamp(volAdj, 0.0f, 1.0f));
			m_MusicEvent.setVolume(m_fMusicVolume);
#endif
		}

		virtual float getMusicVolume() const
		{
			return m_fMusicVolume;
		}

		void playMusic()
		{
			if (!m_bMusicPlaying)
			{
#ifdef PHYRE_AUDIO_PLATFORM_FMOD
				m_MusicEvent.play();
				m_bMusicPlaying = true;
#endif
			}
		}

		virtual void pauseMusic()
		{
			if (m_bMusicPlaying)
			{
#ifdef PHYRE_AUDIO_PLATFORM_FMOD
				m_MusicEvent.pause();
				m_bMusicPlaying = false;
#endif
			}
		}

		virtual void resumeMusic()
		{
			if (!m_bMusicPlaying)
			{
#ifdef PHYRE_AUDIO_PLATFORM_FMOD
				m_MusicEvent.resume();
				m_bMusicPlaying = true;
#endif
			}
		}

		// Music State fetchers
		virtual bool musicPlaying() const
		{
			return m_bMusicPlaying;
		}

	private:

#ifdef PHYRE_AUDIO_PLATFORM_FMOD
		// The cluster containing the Audio project and sound samples.
/*		Phyre::PCluster				*m_pAudioCluster;*/
		// Audio Sample Bank - contains the sound data.
		Phyre::PAudio::PAudioBank	*m_pSampleBank;
		// Audio Events Bank - contains the events the user will call.
		Phyre::PAudio::PAudioBank	*m_pEventBank;
		// Streaming Audio Bank - contains the streaming audio data.
		Phyre::PAudio::PAudioBank	*m_pStreamBank;
		// Persistent event to access the music voices.
		Phyre::PAudio::PAudioEvent	m_MusicEvent;
		// Pointer to the world transform of the listener object
		Phyre::PWorldMatrix			*m_pListener;
		Phyre::PWorldMatrix			*m_pDefaultListener;

		// Cancelable sound element **special case for tutorial audio** 
		PAudioEvent					*m_pCancelableSound;
#endif
		// Music state tracking
		float						m_fVolumeMultiplier;	 // Music multiplier to apply to the music volume.
		float						m_fMusicVolume;
		float						m_fEventVolume;
		bool						m_bMusicPlaying;

		// List of audio elements 
		AudioEventListElement		*m_lPersistantEvents;



	};

};

#endif