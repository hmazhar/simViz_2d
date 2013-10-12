#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include <QPaintEngine>
#include <QWidget>
#include <QTimer>
#include <QResizeEvent>
#include <QMouseEvent>

using namespace std;

struct float3{
	float x,y,p;
};

struct BData{
	vector<float3> Qt;
	vector<float3> Qdt;
	vector<float3> Qddt;
};
#define RAD2DEG 180.0/3.14159265 