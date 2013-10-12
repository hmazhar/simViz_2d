#include "drawPlot.h"

drawPlot::drawPlot(QWidget * parent, int *frame, int *maxframe,vector<drawObject> *objects, int type, int *type2){
	this->setParent(parent);
	mPainter=new QPainter(this);
	mFrame=frame;
	mMaxFrame=maxframe;
	mObjects=objects;
	mType=type;
	mType2=type2;
}

drawPlot::~drawPlot(void)
{
}

void drawPlot::paintEvent(QPaintEvent *){
	mPainter->begin(this);
	//QPen pen;
	///pen.setCapStyle(Qt::RoundCap);
	///pen.setJoinStyle(Qt::RoundJoin);
	//mPainter->setPen(pen);
	mPainter->setViewport(geometry().width()/2.0,geometry().height()/2.0,geometry().width(),geometry().height());
	////mPainter->scale(*mScale,*mScale);
	for(int i=0; i<mObjects->size(); i++){
		float3 t;//=mData.Qt[frame];

		for(int j=0; j<*mMaxFrame; j++){
			if(*mType2==0){t=mObjects->at(i).mData.Qt[j];}
			if(*mType2==1){t=mObjects->at(i).mData.Qdt[j];}
			if(*mType2==2){t=mObjects->at(i).mData.Qddt[j];}

			if(mType==0){mPainter->drawPoint(QPointF((j/4.0-geometry().width()/2.0),t.x*10));}
			if(mType==1){mPainter->drawPoint(QPointF((j/4.0-geometry().width()/2.0),t.y*10));}
			if(mType==2){mPainter->drawPoint(QPointF((j/4.0-geometry().width()/2.0),t.p*10));}
			//mPainter->drawPoint(i/10.0,t.x/10.0);

		}
	}
	mPainter->end();
}