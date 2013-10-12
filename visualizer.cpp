#include "visualizer.h"
#include <QMovie>
visualizer::visualizer(QWidget *parent, Qt::WFlags flags)
: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	mFrame=0;
	mVarD=mVarT=0;
	aboutDialog = new QDialog(this);
	uiAbout.setupUi(aboutDialog);
	plotDialog = new QMainWindow(this);
	mloaded=false;
	mSimSpeed=0;
	mInterFrame=0;
	uiPlot.setupUi(plotDialog);
	qwtPlot=uiPlot.qwtPlot;
	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(quit()));
	connect(ui.actionAbout, SIGNAL(triggered()),this,SLOT(about()));
	connect(ui.playButton, SIGNAL(clicked()),this,SLOT(play()));
	connect(ui.stopButton, SIGNAL(clicked()),this,SLOT(stop()));
	connect(ui.loadButton, SIGNAL(clicked()),this,SLOT(load()));
	connect(ui.plotButton, SIGNAL(clicked()),this,SLOT(plot()));
	connect(uiPlot.xypBox, SIGNAL(currentIndexChanged(int)),this,SLOT(updtD(int)));
	connect(uiPlot.pvaBox, SIGNAL(currentIndexChanged(int)),this,SLOT(updtT(int)));
	connect(ui.frameSlider, SIGNAL(sliderPressed()),this,SLOT(drag()));

	mDrawView=new drawView(ui.widget,&mFrame,&mScale,&mObjects);
	mType=0;
	//mPlotXView=new drawPlot(ui.xWidget, &mFrame, &mMaxFrame,&mObjects,     0, &mType);
	//mPlotYView=new drawPlot(ui.yWidget, &mFrame, &mMaxFrame,&mObjects,     1, &mType);
	//mPlotPhiView=new drawPlot(ui.phiWidget, &mFrame, &mMaxFrame,&mObjects, 2, &mType);

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(20);

	mState=-1;
	plotcurve = new QwtPlotCurve("PLOT");
	plotcurve->attach(qwtPlot);
	mark= new QwtPlotMarker();
	mark->setValue(0.0, 0.0);
	mark->setLineStyle(QwtPlotMarker::VLine);
	mark->setLabelAlignment(Qt::AlignRight | Qt::AlignBottom);
	mark->setLinePen(QPen(Qt::red, 3, Qt::DashDotLine));
	mark->attach(qwtPlot);

}
visualizer::~visualizer(){}

void visualizer::quit(){exit(0);}
void visualizer::about(){aboutDialog->exec();}
void visualizer::plot(){
	plotDialog->show(); 
}
void visualizer::load(){
	int numBody=0;
	int numForces=0;
	vector<float> mForceData;
	QString simName=ui.simName->text();
	ifstream acf_File(QString(simName+QString(".acf")).toStdString().c_str());
	ifstream adm_File(QString(simName+QString(".adm")).toStdString().c_str());
	if(acf_File.fail()!=true){
		string line, option, tempS;
		int tempI;
		float tempF;
		while(acf_File.fail()!=true){
			getline(acf_File, line);
			stringstream ss(line);
			ss>>option;
			if(option=="simulation:")	{ss>>tempS;}
			if(option=="tend:")			{ss>>tempF;	}
			if(option=="stepSize:")		{ss>>tempF;	mSimSpeed=tempF;}
			if(option=="outputSteps:")	{ss>>tempI;	mMaxFrame=tempI;}
		}
		ui.frameSlider->setMaximum(mMaxFrame);
	}
	//cout<<"loaded acf"<<endl;
	if(adm_File.fail()!=true){
		vector<string> mFile;
		string line, option, J;
		while(adm_File.fail()!=true){
			getline(adm_File,line);

			if(adm_File.fail()!=true){
				if(line.size()>0&&line[0]!='%'){
					line.resize(line.find("%"));
					mFile.push_back(line);
				}
			}

		}
		for(int i=0; i<mFile.size();i++){
			stringstream ss;
			ss<<mFile[i];
			ss>>option>>J;
			ss.str("");

			if(option=="Body:"&&numForces==0){
				float a,b,c,d;
				numBody++;
				drawObject temp;
				ss<<mFile[i+1]<<mFile[i+2]<<mFile[i+3]<<mFile[i+4]<<mFile[i+5]<<mFile[i+6]<<mFile[i+7]<<mFile[i+8]<<mFile[i+9];
				ss>>J>>J>>J>>J>>J>>J>>J>>J>>J>>J>>J>>J>>J>>J>>J>>J>>J>>a>>b>>c>>d;
				temp.setShape(a,b,c,d);
				i+=9;
				mObjects.push_back(temp);
				cout<<"BB "<<i<<endl;
			}
			else if(
				option=="AbsoluteX:"||
				option=="AbsoluteY:"||
				option=="RevoluteJoint:"||
				option=="AbsoluteAngle:"||
				option=="RelativeX:"||
				option=="RelativeY:"||
				option=="TranslationalJoint:"||
				option=="AbsoluteDistance:"||
				option=="RelativeDistance:"||
				option=="AbsoluteX:"){
					i++;
					if(option=="RevoluteJoint:"){
						numForces+=2;
						QString temp;
						temp.setNum(numForces-2);
						//uiPlot.constraintBox->insertItem(numForces-2,temp);
						temp.setNum(numForces-1);
						//uiPlot.constraintBox->insertItem(numForces-1,temp);

					}else{

						numForces++;
						QString temp;
						temp.setNum(numForces-1);
						//uiPlot.constraintBox->insertItem(numForces-1,temp);
					}	
			}
		}	
	}
	cout<<"FORCES: " <<numForces<<endl;
	//cout<<"loaded adm"<<endl;
	ifstream res_File(QString(simName+QString(".res")).toStdString().c_str());
	if(res_File.fail()==false){
		mState=0;
		string temp;
		getline(res_File,temp);
		getline(res_File,temp);
		float3 mQt,mQdt,mQddt;
		float time;
		while(res_File.fail()==false){
			res_File>>time;
			mTime.push_back(time);
			BData mTD;
			for(int i=0; i<numBody; i++){
				res_File>>mQt.x>>mQt.y>>mQt.p>>mQdt.x>>mQdt.y>>mQdt.p>>mQddt.x>>mQddt.y>>mQddt.p;
				//mQt.y*=-1;
				mObjects[i].mData.Qt.push_back(mQt);
				mObjects[i].mData.Qdt.push_back(mQdt);
				mObjects[i].mData.Qddt.push_back(mQddt);
			}
			for(int i=0; i<numForces*3; i++){
				float temp;
				res_File>>temp;
				mForceData.push_back(temp);
			}
			mFC.push_back(mForceData);
			mForceData.clear();
		}
	}
	//cout<<"loaded res"<<endl;
	res_File.close();
	acf_File.close();
	adm_File.close();
	mloaded=true;



}
void visualizer::play(){if(mloaded){mState=1;}}
void visualizer::stop(){mState=0;}
void visualizer::drag(){if(mloaded){if(mState==0){mState=2;}}}
void visualizer::update(){
	//if(!mloaded){return;}
	if(mState!=-1){
		if(mState==1){
			mInterFrame+=1*ui.speedSlider->value()/100.0;
			mFrame=mInterFrame;
			if(mFrame>mMaxFrame){mInterFrame=mFrame=0;}
			if(mFrame<0){mInterFrame=mFrame=mMaxFrame;}
			ui.frameSlider->setValue(mFrame);
		}
		if(mState==2){
			mFrame=ui.frameSlider->value();
		}
		mScale=(ui.scaleSlider->value())/100.0;
		mDrawView->mTime=mTime[mFrame];
		mDrawView->repaint();
		int index=mDrawView->index;
		if(index!=-1){
			vector<double> t;
			vector<double> xs;
			for (int i = 0; i < mMaxFrame; i++)
			{
				t.push_back(mTime[i]);

				if(mVarD==0&&mVarT==0)	   {xs.push_back(mObjects[index].mData.Qt[i].x);					}
				else if(mVarD==1&&mVarT==0){xs.push_back(mObjects[index].mData.Qt[i].y);					}
				else if(mVarD==2&&mVarT==0){xs.push_back(mObjects[index].mData.Qt[i].p);					}
				else if(mVarD==0&&mVarT==1){xs.push_back(mObjects[index].mData.Qdt[i].x);					}
				else if(mVarD==1&&mVarT==1){xs.push_back(mObjects[index].mData.Qdt[i].y);					}
				else if(mVarD==2&&mVarT==1){xs.push_back(mObjects[index].mData.Qdt[i].p);					}
				else if(mVarD==0&&mVarT==2){xs.push_back(mObjects[index].mData.Qddt[i].x);					}
				else if(mVarD==1&&mVarT==2){xs.push_back(mObjects[index].mData.Qddt[i].y);					}
				else if(mVarD==2&&mVarT==2){xs.push_back(mObjects[index].mData.Qddt[i].p);					}

			}
			mark->setXValue(mTime[mFrame]);
			plotcurve->setRawSamples(&t[0],&xs[0],mMaxFrame);
			//qwtPlot->setAxisTitle(0,"LOL");
			qwtPlot->replot();
			xs.clear();
			t.clear();
		}
	}

	ui.scaleNumber->display(ui.scaleSlider->value()/100.0);
	ui.speedNumber->display(ui.speedSlider->value());
	mDrawView->resize(ui.widget->size());

}