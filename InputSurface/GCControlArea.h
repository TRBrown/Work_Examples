#ifndef _GCCONTROLAREA_H_
#define _GCCONTROLAREA_H_

#ifndef __COCOS2D_H__
#include "cocos2d.h"
#endif

#ifndef _GCTYPES_H_
#include "GamerCamp/Core/GCTypes.h"
#endif

#include "GamerCamp/EventSystem/CEventData.h"
#include "GamerCamp/Input/GCTouchData.h"

//////////////////////////////////////////////////////////////////////////
// A Control surface area object defining a touchable area and its 
// appropriate touch data.
//
//////////////////////////////////////////////////////////////////////////

class CGCControlArea
{
private:

	u32					m_uControlIndentifier;
	cocos2d::CCRect		m_pControlAreaRect;
	SGCTouchData*	    m_pCurrentTouch;

public:

	CGCControlArea( void );

	~CGCControlArea( void );

	void		         CreateControlArea    ( const cocos2d::CCRect& controlArea, u32 uControlIdentifier );

	bool		         CheckControlArea     ( const cocos2d::CCPoint& touchPosition );

	inline u32			 GetControlIndentifier( void ) const;

	inline void          SetTouchData         ( SGCTouchData* pTouchData );

	inline SGCTouchData* GetTouchData         ( void );
};


inline u32	CGCControlArea::GetControlIndentifier() const
{
	return m_uControlIndentifier;
}


inline void CGCControlArea::SetTouchData( SGCTouchData* pTouchData )
{
	m_pCurrentTouch = pTouchData;
}


inline SGCTouchData* CGCControlArea::GetTouchData()
{
	return m_pCurrentTouch;
}

#endif //_GCCONTROLAREA_H_
