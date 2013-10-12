#pragma once
#include "includes.h"
#include "drawObject.h"

class drawPlot :public QWidget
{
public:
	drawPlot(QWidget * parent, int *frame, int *maxframe,vector<drawObject> *objects, int type, int * type2);
	void paintEvent(QPaintEvent *);
	~drawPlot(void);

	QPainter *mPainter;
	vector<drawObject> *mObjects;
	int *mFrame;
	int *mMaxFrame;
	int mType,* mType2;

};
