#ifndef _TOUCHEVENTDATA_H_
#define _TOUCHEVENTDATA_H_

#include "GamerCamp/EventSystem/CEventData.h"

#include "cocoa/CCGeometry.h"
#include "GCControlArea.h"

#ifndef _GCTYPES_H_
#include "GamerCamp/Core/GCTypes.h"
#endif

enum ETouchEventType
{
	ETET_BEGIN = 0,
	ETET_MOVE,
	ETET_END
};

class CGCTouchEventData
: public CEventData
{
private:

	ETouchEventType     m_eType;
	SGCTouchData*		m_pTouchData;
	CGCControlArea*		m_pControlArea;

public:

	CGCTouchEventData();
	
	virtual ~CGCTouchEventData();

	inline void SetType( ETouchEventType eType )
	{
		m_eType = eType;
	}

	inline ETouchEventType GetType( void ) const
	{
		return m_eType;
	}

	inline void SetTouchData( SGCTouchData* pTouchData )
	{
		m_pTouchData = pTouchData;
	}

	inline SGCTouchData* GetTouchData( void ) const
	{
		return m_pTouchData;
	}

	inline void SetControlArea( CGCControlArea* pControlArea )
	{
		m_pControlArea = pControlArea;
	}

	inline CGCControlArea* GetControlArea( void ) const
	{
		return m_pControlArea;
	}
};

#endif //_TOUCHEVENDATA_H_
