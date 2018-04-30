#ifndef _GCCONTROLSURFACE_H_
#define _GCCONTROLSURFACE_H_

#ifndef __COCOS2D_H__
#include "cocos2d.h"
#endif

#ifndef B2_MATH_H
#include "Box2D.h" 
#endif

#ifndef _GCTYPES_H_
#include "GamerCamp/Core/GCTypes.h"
#endif

#include "GamerCamp/EventSystem/IEventSource.h"
#include "GamerCamp/EventSystem/CEventManager.h"
#include "GamerCamp/Core/ServiceLocator.h"
#include "GamerCamp/Input/GCTouchData.h"
#include "GamerCamp/Input/GCTouchEventData.h"
#include <functional>

//////////////////////////////////////////////////////////////////////////
// forward declare

class IGCGameLayer;

//////////////////////////////////////////////////////////////////////////
// A Control surface management layer that sit in between a menu or game
// layer and cocos2d layer to simplify the creation of touchable control
// zones
//
//
//////////////////////////////////////////////////////////////////////////

class CGCControlSurface
: public IEventSource
{
private:

	CGCTouchEventData m_touchBeginData,
		              m_touchMoveData ,
					  m_touchEndData  ;

	std::vector<SGCTouchData> m_stdvTouches;
	std::vector<CGCControlArea> m_stdvControlAreas;

	// revoke copy
	CGCControlSurface( const CGCControlSurface& );
	CGCControlSurface& operator=( const CGCControlSurface& );

public:

	CGCControlSurface( void );

	virtual	~CGCControlSurface( void );

	void ccTouchesBegan( cocos2d::CCSet* pTouches, cocos2d::CCEvent* pEvent );
	void ccTouchesMoved( cocos2d::CCSet* pTouches, cocos2d::CCEvent* pEvent );
	void ccTouchesEnded( cocos2d::CCSet* pTouches, cocos2d::CCEvent* pEvent );

	CGCControlArea*	AddControlArea    ( const cocos2d::CCRect&  controlArea, u32 uControlIndentifier );
	
	CGCControlArea*	DetectControlTouch( const cocos2d::CCPoint& touchPosition );
	void			CreateControlEvent( CEventData* pData );

	CGCControlArea*	GetControlAreaByID( u32 uId );



};
#endif