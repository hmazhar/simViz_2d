#pragma once
#include "includes.h"
#include "drawObject.h"

class drawView : public QWidget
{
public:
	drawView(QWidget * parent, int *frame, float *scale,vector<drawObject> *objects);
	~drawView(void);
	void paintEvent(QPaintEvent *);
	void mouseReleaseEvent ( QMouseEvent * event );
	QPainter *mPainter;
	vector<drawObject> *mObjects;
	int* mFrame;
	float* mScale;
	QString mPos,mVel,mAcc;
	int index;
	float mTime;
};
