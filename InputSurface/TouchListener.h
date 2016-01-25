#ifndef _TOUCHLISTENER_H_
#define _TOUCHLISTENER_H_

class CGCTouchEventData;

//Interface to be implented by classes that wish to hear for touch events.
class ITouchListener
{
protected:

	ITouchListener()
	{
	}

public:

	virtual ~ITouchListener()
	{
	}

	virtual void VOnTouchBegin ( const CGCTouchEventData* pTouchEventData )
	{}

	virtual void VOnTouchMoved ( const CGCTouchEventData* pTouchEventData )
	{}

	virtual void VOnTouchEnd   ( const CGCTouchEventData* pTouchEventData )
	{}
};

#endif //_TOUCHLISTENER_H_