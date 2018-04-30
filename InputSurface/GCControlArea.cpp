#include "GCControlArea.h"

CGCControlArea::CGCControlArea()
: m_pCurrentTouch( NULL )
{

}


CGCControlArea::~CGCControlArea()
{

}


void CGCControlArea::CreateControlArea( const cocos2d::CCRect& controlArea , u32 uControlIndentifier )
{
	m_pControlAreaRect    = controlArea;
	m_uControlIndentifier = uControlIndentifier;
}


bool CGCControlArea::CheckControlArea( const cocos2d::CCPoint& touchPosition )
{
	if( m_pControlAreaRect.containsPoint( touchPosition ) )
	{
		return true;
	}

	return false;
}
