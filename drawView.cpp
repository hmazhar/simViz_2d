#include "drawView.h"

drawView::drawView(QWidget * parent, int *frame, float *scale,vector<drawObject> *objects)
{
	this->setParent(parent);
	mPainter=new QPainter(this);
	mPainter->setRenderHint(QPainter::Antialiasing);

	mFrame=frame;
	mScale=scale;
	mObjects=objects;
	//this->setMouseTracking (true);
	index=-1;
	mTime=0;
}

drawView::~drawView(void)
{
}

void drawView::paintEvent(QPaintEvent *){
	mPainter->begin(this);
	QPen pen;
	pen.setCapStyle(Qt::RoundCap);
	pen.setJoinStyle(Qt::RoundJoin);
	mPainter->setPen(pen);
	mPainter->setFont(QFont("Courier New", 10));
	mPainter->setViewport(geometry().width()/2.0,geometry().height()/2.0,geometry().width(),geometry().height());

	QString xx="0",yy="0",pp="0";
	if(index!=-1){
		mPos=QString("Pos x: %1 y: %2 phi: %3")
			.arg(mObjects->at(index).mData.Qt[*mFrame].x,10,' ')
			.arg(mObjects->at(index).mData.Qt[*mFrame].y,10,' ')
			.arg(mObjects->at(index).mData.Qt[*mFrame].p,10,' ');
		mVel=QString("Vel x: %1 y: %2 phi: %3")
			.arg(mObjects->at(index).mData.Qdt[*mFrame].x,10,' ')
			.arg(mObjects->at(index).mData.Qdt[*mFrame].y,10,' ')
			.arg(mObjects->at(index).mData.Qdt[*mFrame].p,10,' ');
		mAcc=QString("Acc x: %1 y: %2 phi: %3")
			.arg(mObjects->at(index).mData.Qddt[*mFrame].x,10,' ')
			.arg(mObjects->at(index).mData.Qddt[*mFrame].y,10,' ')
			.arg(mObjects->at(index).mData.Qddt[*mFrame].p,10,' ');
	}else{
		mPos=QString("Pos x: %1 y: %2 phi: %3").arg("",10,' ').arg("",10,' ').arg("",10,' ');
		mVel=QString("Vel x: %1 y: %2 phi: %3").arg("",10,' ').arg("",10,' ').arg("",10,' ');
		mAcc=QString("Acc x: %1 y: %2 phi: %3").arg("",10,' ').arg("",10,' ').arg("",10,' ');
	}
	int b=-1;
	if(index!=-1){
		b=index;//mObjects->at(index).mData.
	}
	mPainter->drawText(-geometry().width()/2.0+10,-geometry().height()/2.0+10,QString("Time: %1 [s]").arg(mTime));
	mPainter->drawText(-geometry().width()/2.0+10,geometry().height()/2.0-40,QString("Body: %1").arg(b));
	mPainter->drawText(-geometry().width()/2.0+10,geometry().height()/2.0-30,mPos);
	mPainter->drawText(-geometry().width()/2.0+10,geometry().height()/2.0-20,mVel);
	mPainter->drawText(-geometry().width()/2.0+10,geometry().height()/2.0-10,mAcc);
	for(int i=0; i<mObjects->size(); i++){
		mPainter->save();
		mPainter->scale(1,-1);
		mPainter->scale(*mScale,*mScale);
		mObjects->at(i).paint(mPainter,*mFrame, *mScale*100);
		mPainter->restore();
	}
	mPainter->end();

}

void drawView::mouseReleaseEvent ( QMouseEvent * event ){

	float x=event->x()-geometry().width()/2.0;
	float y=-(event->y()-geometry().height()/2.0);
	mVel.setNum(x);
	QString temp;
	for(int i=0; i<mObjects->size(); i++){
		temp.setNum(i);
		if(mObjects->at(i).mPolygon.containsPoint (QPoint(x,y),Qt::WindingFill)){index=i;}
	}
}