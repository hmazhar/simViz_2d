#include "drawObject.h"

drawObject::drawObject(void)
{
	
}

drawObject::~drawObject(void)
{
}

void drawObject::paint(QPainter * mPainter, int frame, float scale){
		mPainter->save();
		float3 t=mData.Qt[frame];
		QMatrix transformation;
		transformation.scale(scale,scale);
		transformation.translate( t.x, t.y );
		transformation.rotate( t.p*RAD2DEG);
		mPolygon=transformation.map(QPolygonF(mRect));
		mPainter->drawPolygon(mPolygon);
		mPainter->restore();
}

void drawObject::setShape(float ax,float ay,float bx,float by){
	mRect=QRectF(QPointF(ax,ay), QPointF(bx, by));
}