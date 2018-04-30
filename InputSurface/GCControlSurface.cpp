#include "GCControlSurface.h"
#include "GCControlArea.h"

//**********************************************************************************
// Control Surface

CGCControlSurface::CGCControlSurface()
{
	m_stdvTouches.resize( 4 );

	m_touchBeginData.SetType( ETouchEventType::ETET_BEGIN );
	m_touchMoveData.SetType ( ETouchEventType::ETET_MOVE  );
	m_touchEndData.SetType  ( ETouchEventType::ETET_END   );
}

CGCControlSurface::~CGCControlSurface()
{

}

void CGCControlSurface::ccTouchesBegan( cocos2d::CCSet* pTouches, cocos2d::CCEvent* pEvent )
{
	cocos2d::CCSetIterator it;

	for( it = pTouches->begin(); it != pTouches->end(); ++it )
	{
		if( *it )
		{
			cocos2d::CCTouch* pTouch = ( cocos2d::CCTouch* )*it;
			cocos2d::CCPoint touchLocation = pTouch->getLocationInView();
			touchLocation = cocos2d::CCDirector::sharedDirector()->convertToGL( touchLocation );
			
			u32 uTouchId = pTouch->getID();
			m_stdvTouches[ uTouchId ].v2CurrentPos = b2Vec2( touchLocation.x, touchLocation.y );
			m_stdvTouches[ uTouchId ].v2Origin =     m_stdvTouches[ pTouch->getID() ].v2CurrentPos;
			m_stdvTouches[ uTouchId ].bTouching = true;
			m_stdvTouches[ uTouchId ].bTouchingLastFrame = false;


			CGCControlArea* pControlArea = NULL;
			if( NULL != ( pControlArea = DetectControlTouch( touchLocation ) ) )
			{
				m_stdvTouches[ pTouch->getID() ].uIndex = uTouchId;
				m_touchBeginData.SetTouchData( &m_stdvTouches[ uTouchId ] );
				m_touchBeginData.SetControlArea( pControlArea );

				pControlArea->SetTouchData( &m_stdvTouches[ uTouchId ] );

				CreateControlEvent( &m_touchBeginData );
			}

		}
	}

	CCLOG( "TouchBegan" );
}

void CGCControlSurface::ccTouchesMoved( cocos2d::CCSet* pTouches, cocos2d::CCEvent* pEvent )
{
	cocos2d::CCSetIterator it;

	for( it = pTouches->begin(); it != pTouches->end(); ++it )
	{
		if( *it )
		{
			cocos2d::CCTouch* pTouch = ( cocos2d::CCTouch* )*it;
			cocos2d::CCPoint touchLocation = pTouch->getLocationInView();
			touchLocation = cocos2d::CCDirector::sharedDirector()->convertToGL( touchLocation );

			u32 uTouchId = pTouch->getID();
			m_stdvTouches[ uTouchId ].v2CurrentPos =b2Vec2( touchLocation.x, touchLocation.y );
			m_stdvTouches[ uTouchId ].bTouchingLastFrame = true;

			CGCControlArea* pControlArea = NULL;
			if( NULL != ( pControlArea = DetectControlTouch( touchLocation ) ) )
			{
				m_stdvTouches[ uTouchId ].uIndex = uTouchId;
				m_touchMoveData.SetTouchData( &m_stdvTouches[ uTouchId ] );
				m_touchMoveData.SetControlArea( pControlArea );
				
				pControlArea->SetTouchData( &m_stdvTouches[ uTouchId ] );

				CreateControlEvent( &m_touchMoveData );
			}
		}
	}

	CCLOG( "TouchMoved" );
}

void CGCControlSurface::ccTouchesEnded( cocos2d::CCSet* pTouches, cocos2d::CCEvent* pEvent )
{
	cocos2d::CCSetIterator it;

	for( it = pTouches->begin(); it != pTouches->end(); ++it )
	{
		if( *it )
		{
			cocos2d::CCTouch* pTouch = ( cocos2d::CCTouch* )*it;
			cocos2d::CCPoint touchLocation = pTouch->getLocationInView();
			touchLocation = cocos2d::CCDirector::sharedDirector()->convertToGL( touchLocation );

			u32 uTouchId = pTouch->getID();
			m_stdvTouches[ uTouchId ].bTouching = false;

			CGCControlArea* pControlArea = NULL;
			if( NULL != ( pControlArea = DetectControlTouch( touchLocation ) ) )
			{
				m_stdvTouches[ pTouch->getID() ].uIndex = uTouchId;
				m_touchEndData.SetTouchData( &m_stdvTouches[ uTouchId ] );
				m_touchEndData.SetControlArea( pControlArea );

				pControlArea->SetTouchData( &m_stdvTouches[ uTouchId ] );

				CreateControlEvent( &m_touchEndData );
			}
		}
	}

	CCLOG( "ToucEnded" );
}

//**********************************************************************
//Defines an touchable area control surface of the screen and returns 
//a CGCControlArea pointer.

CGCControlArea* CGCControlSurface::AddControlArea( const cocos2d::CCRect& ControlAreaRect,
	u32 uControlIndentifier )
{
	CGCControlArea newArea;
	newArea.CreateControlArea( ControlAreaRect, uControlIndentifier );
	//Push the control area
	m_stdvControlAreas.push_back( newArea );

	return m_stdvControlAreas.end();
}

//***********************************************************************
//Checks for a touch with in a control area and returns the CGCControlArea
//it was detected on.



CGCControlArea* CGCControlSurface::DetectControlTouch( const cocos2d::CCPoint& TouchPosition )
{
	u32 uTotalControlAreas = m_stdvControlAreas.size();
	for( u32 i = 0; i < uTotalControlAreas; i++ )
	{
		if( m_stdvControlAreas[ i ].CheckControlArea( TouchPosition ) )
		{
			return &m_stdvControlAreas[ i ];
		}
	}

	return NULL;
}

//*************************************************************************
//Creates a control event from the supplied data

void CGCControlSurface::CreateControlEvent( CEventData* pData )
{
	
	CCAssert(pData, "Null control event data pointer");

	CEventManager* pEventManager = CServiceLocator< CEventManager >::GetService();

	CEvent* pEvent = pEventManager->CreateEvent( pData, this );
	if( pEvent )
	{
		pEventManager->FireEvent( pEvent, EEventPropagationMode::ALL );
	}
}


CGCControlArea* CGCControlSurface::GetControlAreaByID( u32 uId )
{
	u32 uTotalControlAreas = m_stdvControlAreas.size();
	for( u32 i = 0; i < uTotalControlAreas; i++ )
	{
		if( m_stdvControlAreas[ i ].GetControlIndentifier() == uId )
		{
			return &m_stdvControlAreas[ i ];
		}
	}
	return NULL;
}
