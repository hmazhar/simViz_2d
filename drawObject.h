#pragma once
#include "includes.h"

class drawObject
{
public:
	drawObject(void);
	~drawObject(void);
	void paint(QPainter * mPainter, int frame, float scale);
	void setShape(float ax,float ay,float bx,float by);
	int mType, mIndex;
	float ax,ay,bx,by;
	BData mData;
	QRectF mRect;
	QMatrix mTransform;
	QPolygonF mPolygon;
};
