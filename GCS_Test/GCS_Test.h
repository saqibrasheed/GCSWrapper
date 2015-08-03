// GCS_Test.h

//#pragma once

using namespace System;

//namespace GCS_Test {

	public ref class Class1
	{
		// TODO: Add your methods for this class here.
	};
//}


#include "stdafx.h"
#include "stdio.h"
#include "Geo.h"
#include "GCS.h"
#include "Constraints.h"

//namespace GCS
//{

enum Shape_Type {POINT, LINE, CIRCLE, NONE};

class SaShape	{

protected:
	int _id;
	Shape_Type _type;

public:
	int get_id(){ return _id; }
	Shape_Type get_type() { return _type; }
	~SaShape()	{
		std::cout<<"shape destructed";
	}
};

class SaPoint : public SaShape	{

	GCS::Point _gcs_point;

public:
	SaPoint(int i, double x, double y);
	GCS::Point& get_gcs_point();
};

class SaLine : public SaShape	{
	
	GCS::Line _gcs_line;

public:
	SaLine(int i, double x1, double y1, double x2, double y2);
	GCS::Line& get_gcs_line();
};

class SaCircle : public SaShape	{

	GCS::Circle _gcs_circle;

public:
	SaCircle(int i, double x1, double y1, double r);
	GCS::Circle& get_gcs_circle();
};

public ref class GCS_Test
{
public:
	GCSWrapper(void);
	~GCSWrapper(void);

	void reset();

	// create shapes
	int add_point(double x1, double y1);
	int add_segment(double x1, double y1, double x2, double y2);
	int add_vertical_segment(double x1, double y1, double y2);
	int add_circle(double x1, double y1, double radius);
	
	// add constraints
	void perpendicular(int id1, int id2);
	void parallel(int id1, int id2);
	//void tangent(int id1, int id2);

	void line_vertical(int id1);
	void point_vertical(int id1, int id2);

	void fix_length(int id, double length);	// to set the constraint to fix the length of a line

	//void Coincident(int id1, int id2);
		
	bool solve();

	//std::string get_shape_type(GCS::Shape_Type type);
	SaShape* get_shape(int id);
	Shape_Type get_shape_type(int id);	
	void show_values(int id);
	void clear();
	

private:
	int id;
	std::vector<SaShape*> _Shapes;

	GCS::System gcs_sys;

	std::vector<double*> _Parameters;
	std::vector<double*> _FixedParameters;


};