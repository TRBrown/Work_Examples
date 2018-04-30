#ifndef _GCTOUCHDATA_H_
#define _GCTOUCHDATA_H_

#include "Common/b2Math.h"

struct SGCTouchData
{
	u8                  uIndex;
	bool				bTouching;
	bool				bTouchingLastFrame;
	b2Vec2	            v2Origin;
	b2Vec2				v2CurrentPos;
};

#endif //_GCTOUCHDATA_H_