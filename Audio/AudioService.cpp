#include "AudioService.h"

#ifndef PHYRE_TOOL_BUILD

using namespace Phyre;

#ifdef PHYRE_AUDIO_PLATFORM_FMOD

using namespace PAudio;

#endif

namespace sw
{
	CAudioService::CAudioService()
#ifdef PHYRE_AUDIO_PLATFORM_FMOD
		: m_pSampleBank(NULL)
		, m_pEventBank(NULL)
		, m_pStreamBank(NULL)
		, m_pListener(NULL)
		, m_fVolumeMultiplier(1.0f)
		, m_fMusicVolume(1.0f)
		, m_fEventVolume(1.0f)
		, m_bMusicPlaying(false)
		, m_lPersistantEvents(NULL)
		, m_pCancelableSound (NULL)
#endif
	{
	
	}

	CAudioService::~CAudioService(void)
	{

	}

	Phyre::PResult CAudioService::Initialize(void)
	{
#ifdef PHYRE_AUDIO_PLATFORM_FMOD
		m_pDefaultListener = new PWorldMatrix();
		m_pDefaultListener->setMatrix(SWMatrix4::identity());

		m_pListener = m_pDefaultListener;
		
		PHYRE_TRY(VinitAudio());
		PHYRE_TRY(VloadAudioAssets());
#endif	
		return PE_RESULT_NO_ERROR;
	}

	Phyre::PResult CAudioService::Terminate(void)
	{
#ifdef PHYRE_AUDIO_PLATFORM_FMOD
		delete m_pDefaultListener;
		
		stopAudioEvents();

		// Terminate the audio utility.
		PAudio::PAudioInterface &audio = PAudio::PAudioInterface::GetInstance();
		PHYRE_TRY(audio.terminate());
#endif
		return PE_RESULT_NO_ERROR;
	}

	// Description:
	// Does the initial setup of the audio system, fetching the audio directory.
	// PString Media Dir
	// Return Value List:
	// PE_RESULT_NO_ERROR - Successfully initialised everything.
	// Other - Error occurred during initialisation.
	Phyre::PResult CAudioService::VinitAudio()
	{
#ifdef PHYRE_AUDIO_PLATFORM_FMOD
		// Set the media directory as this is used by the Audio library.
		PChar scePhyre[1024];
		PhyreOS::GetCurrentDir(scePhyre, sizeof(scePhyre) - 1);
		PStringBuilder mediaDir(scePhyre);
		mediaDir += "/Media";
		// Initialize the audio system
		PAudio::PAudioInterface &audio = PAudio::PAudioInterface::GetInstance();
		PHYRE_TRY(audio.initialize(mediaDir.c_str()));
#endif
		return PE_RESULT_NO_ERROR;
	}
	// Description:
	// Loads in audio assets and readies them for use.
	// Return Value List:
	// PE_RESULT_NO_ERROR - Successfully initialised everything.
	// Other - Error occurred during initialisation.
	Phyre::PResult CAudioService::VloadAudioAssets()
	{
	
		PResult result = PE_RESULT_NO_ERROR;

#ifdef PHYRE_AUDIO_PLATFORM_FMOD	

		CLUSTER_SERVICE->VLoadAsset("Media/" PHYRE_PLATFORM_ID "/SW/Audio.phyre");

		// Find the audio banks in the audio cluster.
		PSharray<PAssetReference *> audioBanks;
		PAssetReference::Find(audioBanks, NULL, NULL, &PHYRE_CLASS(PAudio::PAudioBank));


		// Set the listener position, update in the animate call.
		Vectormath::Aos::Vector3 listenerPos(0.0f, 0.0f, 0.0f);
		setListenerPosition(listenerPos);



		for (PUInt32 i = 0; i<audioBanks.getCount(); i++)
		{
			PAudio::PAudioBank &audioBank = (PAudio::PAudioBank &)audioBanks[i]->getAsset();

			switch (audioBank.getType())
			{
			case FMODFileFEV:
				m_pEventBank = &audioBank;
				break;
			case FMODFileFSB:
				m_pSampleBank = &audioBank;
				break;
			case FMODFileFSBStream:
				m_pStreamBank = &audioBank;
				break;
			default:
				break;
			}
		}

		// Check if audio banks have loaded.
		if (!m_pEventBank)
		{
			return PHYRE_SET_LAST_ERROR(PE_RESULT_OBJECT_NOT_FOUND, "Unable to find event bank in audio cluster");
		}

		if (!m_pSampleBank)
		{
			return PHYRE_SET_LAST_ERROR(PE_RESULT_OBJECT_NOT_FOUND, "Unable to find sample bank in audio cluster");
		}

		if (m_pStreamBank)
		{
			// Unregister the bank so we can re-register it.
			result = m_pStreamBank->unregisterBank();
			if (result != PE_RESULT_NO_ERROR)
				PHYRE_WARN("Error %d unregistering stream bank\n", result);
			// Set the event bank to reduce the memory footprint
			// and support multiple streaming sounds per sound bank.
			result = m_pStreamBank->setEventBank(m_pEventBank);
			if (result != PE_RESULT_NO_ERROR)
				PHYRE_WARN("Error %d setting event bank\n", result);
			result = m_pStreamBank->registerBank("Music/menu_main");
			if (result != PE_RESULT_NO_ERROR)
				PHYRE_WARN("Error %d registering stream bank\n", result);
		}
		else
		{
			return PHYRE_SET_LAST_ERROR(PE_RESULT_OBJECT_NOT_FOUND,
				"Unable to find stream bank in audio cluster");
		}

		// Create music event.
		result = m_pEventBank->createEvent("Music/menu_main", m_MusicEvent);
		if (result != PE_RESULT_NO_ERROR)
		{
			PHYRE_WARN("Error %d creating music event\n", result);
		}
		else
		{
			result = m_MusicEvent.setVolume(m_fMusicVolume);
			if (result != PE_RESULT_NO_ERROR)
				PHYRE_WARN("Error %d playing music\n", result);
		}
#endif
		return result;
	}

	// Description:
	// Sets a listener position only.
	void CAudioService::setListenerPosition(Vectormath::Aos::Vector3 listenerPos)
	{
#ifdef PHYRE_AUDIO_PLATFORM_FMOD
		PAudio::PAudioInterface &audio = PAudio::PAudioInterface::GetInstance();
		audio.setListener3DAttributes(&listenerPos);
#endif
	}

	// Description:
	// Sets a listener position with associated data.
	void CAudioService::setListenerPosition(Vectormath::Aos::Vector3 listenerPos,
		Vectormath::Aos::Vector3 at,
		Vectormath::Aos::Vector3 up)
	{
#ifdef PHYRE_AUDIO_PLATFORM_FMOD
		PAudio::PAudioInterface &audio = PAudio::PAudioInterface::GetInstance();
		audio.setListener3DAttributes(&listenerPos, &at, &up);
#endif
	}

	// Description:
	// Play a one-shot sound effect at the listener, sound effect referenced by string.
	// Return Value List:
	// PE_RESULT_NO_ERROR - Successfully played sound
	// Other - Error occurred while attempting to play sound
	Phyre::PResult CAudioService::VplaySound( const char* eventName )
	{
		Vectormath::Aos::Vector3 pos(0, 0, 0);

#ifdef PHYRE_AUDIO_PLATFORM_FMOD

		pos = m_pListener->getMatrix().getTranslation();

#endif

		return playSound(pos, eventName);
	}

	// Description:
	// Play a one-shot sound effect at the listener, sound effect referenced by string.
	// Return Value List:
	// PE_RESULT_NO_ERROR - Successfully played sound
	// Other - Error occurred while attempting to play sound
	Phyre::PResult CAudioService::VplaySound ( Vectormath::Aos::Vector3 &position, const char* eventName )
	{
		return playSound ( position, eventName );
	}

	// Description:
	// Play a one-shot sound effect at a particular position, sound effect
	// referenced by string.
	// Return Value List:
	// PE_RESULT_NO_ERROR - Successfully played sound
	// Other - Error occurred while attempting to play sound
	Phyre::PResult CAudioService::playSound(Vectormath::Aos::Vector3 &position,
		const char* eventName )
	{
		PResult res = PE_RESULT_NO_ERROR;

#ifdef PHYRE_AUDIO_PLATFORM_FMOD
		

		PAudioEvent audioEvent;
		res = m_pEventBank->createEvent(eventName, audioEvent);
		if (PE_RESULT_NO_ERROR != res)
		{
			PHYRE_WARN("Error %d creating %s event\n", res, eventName);
		}
		else
		{
			res = audioEvent.set3DAttributes(&position);
			if (PE_RESULT_NO_ERROR != res)
			{
				PHYRE_WARN("Error %d setting position for %s event\n", res,
					eventName);
			}
			else
			{
				res = audioEvent.setVolume(m_fEventVolume); //cant imagine this failing
				res = audioEvent.playOneShot();
				if (res != PE_RESULT_NO_ERROR)
					PHYRE_WARN("Error %d playing %s\n", res, eventName);
			}
		}
#endif
		return res;
	}

	// Description:
	// Play a one-shot sound effect at a particular position, sound effect
	// referenced by string.
	// Return Value List:
	// PE_RESULT_NO_ERROR - Successfully played sound
	// Other - Error occurred while attempting to play sound
	Phyre::PResult CAudioService::VplayCancelableSound ( const char* eventName )
	{
		PResult res = PE_RESULT_NO_ERROR;

#ifdef PHYRE_AUDIO_PLATFORM_FMOD

		// Stop
		if ( NULL == m_pCancelableSound )
		{
			m_pCancelableSound = PHYRE_NEW ( PAudioEvent );
		}
		else
		{
			if ( m_pCancelableSound->isPlaying () )
			{
				res = m_pCancelableSound->stop ();
			}

			res = m_pCancelableSound->release ();

			if ( PE_RESULT_NO_ERROR == res )
			{				
				PHYRE_FREE ( m_pCancelableSound );
				m_pCancelableSound = PHYRE_NEW ( PAudioEvent );
			}
			else
			{
				PHYRE_WARN ( "Error %d releasing Cancelable sound %s\n", res, eventName );
			}
		}

		res = m_pEventBank->createEvent ( eventName, *m_pCancelableSound );
		if ( PE_RESULT_NO_ERROR != res )
		{
			PHYRE_WARN ( "Error %d creating %s event\n", res, eventName );
		}
		else
		{
			Vectormath::Aos::Vector3 pos ( 0, 0, 0 );

			pos = m_pListener->getMatrix ().getTranslation ();
			
			res = m_pCancelableSound->set3DAttributes ( &pos );
			if ( PE_RESULT_NO_ERROR != res )
			{
				PHYRE_WARN ( "Error %d setting position for %s event\n", res,
					eventName );
			}
			else
			{
				res = m_pCancelableSound->setVolume ( m_fEventVolume ); //cant imagine this failing
				res = m_pCancelableSound->play();
				if ( res != PE_RESULT_NO_ERROR )
					PHYRE_WARN ( "Error %d playing %s\n", res, eventName );
			}
		}
#endif
		return res;
	}

	// Description:
	// This method is designed for repeating sounds that do not end and release them selfs
	// during their own life time. Keep track of them so they can be later stopped and 
	// released here
	// Return Value List:
	// PE_RESULT_NO_ERROR - Successfully played sound
	// Other - Error occurred while attempting to play sound
	AudioEventListElement* CAudioService::VplayPersistantSound(Vectormath::Aos::Vector3 position,
		const char* eventName, float fVolume )
	{
		PResult res = PE_RESULT_NO_ERROR;

#ifndef PHYRE_AUDIO_PLATFORM_FMOD
		AudioEventListElement* audioEvent = NULL;
#endif

#ifdef PHYRE_AUDIO_PLATFORM_FMOD

		AudioEventListElement* audioEvent = PHYRE_NEW(AudioEventListElement);

		res = m_pEventBank->createEvent(eventName, *audioEvent);
		if (PE_RESULT_NO_ERROR != res)
		{
			PHYRE_WARN("Error %d creating %s event\n", res, eventName);
		}
		else
		{
			res = audioEvent->set3DAttributes(&position);
			if (PE_RESULT_NO_ERROR != res)
			{
				PHYRE_WARN("Error %d setting position for %s event\n", res,
					eventName);
			}
			else
			{
				res = audioEvent->setVolume( fVolume ); //cant imagine this failing
				res = audioEvent->play();
				if (res != PE_RESULT_NO_ERROR)
					PHYRE_WARN("Error %d playing %s\n", res, eventName);
			}
		}
		if (NULL == m_lPersistantEvents)
		{
			m_lPersistantEvents = audioEvent;
		}
		else
		{
			m_lPersistantEvents->addElementToListEnd(&audioEvent);
			m_lPersistantEvents = audioEvent;
		}
#endif

		return audioEvent;


	}

	// Description:
	// Called when deleting a persistent sound. A pointer to the AudioEventListElement is
	// required.
	// Return Value List:
	// PE_RESULT_NO_ERROR - Successfully played sound
	// Other - Error occurred while attempting to play sound
	Phyre::PResult CAudioService::VstopPersistantSound(AudioEventListElement *element)
	{
		PResult res = PE_RESULT_NO_ERROR;

#ifdef PHYRE_AUDIO_PLATFORM_FMOD

		if (element->findElementInList(m_lPersistantEvents))
		{
			res = element->stop();
			if (PE_RESULT_NO_ERROR == res)
			{
				//AudioEventListElement *releaseptr = &element;
				element->release();
				element->removeElementFromList(&m_lPersistantEvents);
				PHYRE_FREE(element);
			}
		}

#endif

		return res;
	}

	// Description:
	// Used to update position and velocity data of a persistant audio element.
	// Return Value List:
	// PE_RESULT_NO_ERROR - Successfully played sound
	// Other - Error occurred while attempting to play sound
	Phyre::PResult CAudioService::VupdatePersistantSound(Vectormath::Aos::Vector3 *position,
														Vectormath::Aos::Vector3 *velocity,
														AudioEventListElement &element)
	{
		PResult res = PE_RESULT_NO_ERROR;

#ifdef PHYRE_AUDIO_PLATFORM_FMOD

		if (element.findElementInList(m_lPersistantEvents))
		{
			if (velocity == NULL)
			{
				element.set3DAttributes(position);
			}
			else
			{
				element.set3DAttributes(position, velocity);
			}
		}

#endif

		return res;
	}


	// Description:
	// substitutes the music with a new one of type eventName without position
	// Return Value List:
	// PE_RESULT_NO_ERROR - Successfully played sound
	// Other - Error occurred while attempting to play sound
	Phyre::PResult CAudioService::VSetMusic( const char* eventName )
	{
		Vectormath::Aos::Vector3 pos(0, 0, 0);

#ifdef PHYRE_AUDIO_PLATFORM_FMOD

		pos = m_pListener->getMatrix().getTranslation();

#endif

		return SetMusic(pos, eventName);
	}

	// Description:
	// substitutes the music with a new one of type eventName with position
	// Return Value List:
	// PE_RESULT_NO_ERROR - Successfully played sound
	// Other - Error occurred while attempting to play sound
	Phyre::PResult CAudioService::SetMusic(Vectormath::Aos::Vector3 position,
		const char* eventName )
	{
		PResult res = PE_RESULT_NO_ERROR;

#ifdef PHYRE_AUDIO_PLATFORM_FMOD

		m_MusicEvent.stop();
		Phyre::PResult result = PE_RESULT_NO_ERROR;
		result = m_pStreamBank->registerBank(eventName);
		if (result != PE_RESULT_NO_ERROR)
			PHYRE_WARN("Error %d registering stream bank\n", result);
		res = m_pEventBank->createEvent(eventName, m_MusicEvent);
		if (res != PE_RESULT_NO_ERROR)
		{
			PHYRE_WARN("Error %d creating %s event\n", res, eventName);
		}
		else
		{
			res = m_MusicEvent.set3DAttributes(&position);
			if (res != PE_RESULT_NO_ERROR)
			{
				PHYRE_WARN("Error %d setting position for %s event\n", res,
					eventName);
			}
		}
#endif

		return res;
	}

	// Description:
	// Stops all audio events
	void CAudioService::stopAudioEvents()
	{
#ifdef PHYRE_AUDIO_PLATFORM_FMOD
		PAudio::PAudioInterface &audio = PAudio::PAudioInterface::GetInstance();
		audio.stopAllEvents();
#endif
	}

	// Description:
	// Animate update method for updating listening position etc
	void CAudioService::VUpdate( float fElapsedTime, EGameUpdateResult eUpdate )
	{
		
#ifdef PHYRE_AUDIO_PLATFORM_FMOD
		
		PResult res;
		// Get the audio system interface
		PAudio::PAudioInterface &audio = PAudio::PAudioInterface::GetInstance();

		// Set the listener position to be at the new camera position
		PMatrix4 cameraTransform = PMatrix4(m_pListener->getMatrix());
		const Vectormath::Aos::Vector3 pos(cameraTransform.getTranslation());
		const Vectormath::Aos::Vector3 at(-cameraTransform.getCol2().getXYZ());
		const Vectormath::Aos::Vector3 up(cameraTransform.getCol1().getXYZ());

		res = audio.setListener3DAttributes(&pos, NULL, &at, &up);
		if (res != PE_RESULT_NO_ERROR)
			PHYRE_WARN("Error %d setting listener position\n", res);
	
		// Update the sound system
		audio.update();
#endif

	}

}

#endif