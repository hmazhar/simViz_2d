#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <QtGui/QMainWindow>
#include "ui_visualizer.h"
#include "ui_about.h"
#include "ui_plot.h"
#include "drawView.h"
#include "QWT/src/qwt_plot.h"
#include "QWT/src/qwt_plot_curve.h"
#include "QWT/src/qwt_series_data.h"
#include "qwt_plot_marker.h"
#include "QWT/src/qwt_symbol.h"

class visualizer : public QMainWindow
{
	Q_OBJECT

public:
	visualizer(QWidget *parent = 0, Qt::WFlags flags = 0);
	~visualizer();
public slots:
	void quit();
	void about();
	void load();
	void play();
	void stop();
	void plot();
	void update();
	void drag();
	void updtD(int a){
		mVarD=a;
		
	
	}
	void updtT(int a){mVarT=a;}

private:
	Ui::visualizerClass ui;
	Ui::About uiAbout;
	Ui::PlotWin uiPlot;
	int mState;
	int mFrame;
	float mInterFrame;
	int mMaxFrame;
	int mType;
	int mVarD, mVarT;
	bool mloaded;
	float mSimSpeed;
	float mScale;
	string mSimName;
	vector<float> mTime;
	vector<vector<float>> mFC;
	drawView *mDrawView;
	QTimer *timer;
	QDialog *aboutDialog;
	QMainWindow *plotDialog;
	vector<drawObject> mObjects;
	QwtPlot *qwtPlot;
	QwtPlotCurve *plotcurve;
	QwtPlotMarker *mark;
};

#endif // VISUALIZER_H
