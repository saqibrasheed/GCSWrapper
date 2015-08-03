#include "stdafx.h"
#include "GCSWrapper.h"

#include "stdio.h"
#include <time.h>
#include <iostream>

//namespace GCS 
//{

SaPoint::SaPoint(int i, double x, double y):SaShape()	
{
	_id = i;
	_type = Shape_Type::POINT;
	double* dx = new double(x);
	double* dy = new double(y);
	_gcs_point.x = dx;
	_gcs_point.y = dy;
}

GCS::Point& SaPoint::get_gcs_point()
{
	return _gcs_point;
}

SaLine::SaLine(int i, double x1, double y1, double x2, double y2):SaShape()	
{
	_id = i;
	_type = Shape_Type::LINE;
	double* dx1 = new double(x1);
	double* dy1 = new double(y1);
	double* dx2 = new double(x2);
	double* dy2 = new double(y2);
	
	//GCS::Point* p1, p2;
	GCS::Point* p1 = new GCS::Point();
	GCS::Point* p2 = new GCS::Point();

	p1->x = dx1;
	p1->y = dy1;
	p2->x = dx2;
	p2->y = dy2;

	_gcs_line.p1 = *p1;
	_gcs_line.p2 = *p2;
}

GCS::Line& SaLine::get_gcs_line()
{
	return _gcs_line;
}

SaCircle::SaCircle(int i, double x1, double y1, double r):SaShape()	
{
	_id = i;
	_type = Shape_Type::CIRCLE;
	double* dx1 = new double(x1);
	double* dy1 = new double(y1);
	double* rad = new double(r);
	
	GCS::Point* p1 = new GCS::Point();
	//GCS::Point* p2 = new GCS::Point();

	p1->x = dx1;
	p1->y = dy1;
	
	//p2->x = dx2;
	//p2->y = dy2;

	_gcs_circle.center = *p1;
	_gcs_circle.rad = rad;
	//_gcs_line.p2 = *p2;	
}

GCS::Circle& SaCircle::get_gcs_circle()
{
	return _gcs_circle;
}

GCSWrapper::GCSWrapper(void)
{
	id = 0;
}

GCSWrapper::~GCSWrapper(void)
{
}

void GCSWrapper::reset()	{

	id = 0;
	_Shapes.clear();
	//std::cout<<"reset";
}

void GCSWrapper::clear()	{

	//_Parameters.clear();
	//_FixedParameters.clear();
}

int GCSWrapper::add_point(double x1, double y1)	{

	SaPoint* pt = new SaPoint(++id, x1, y1);

	_Shapes.push_back(pt);
	_Parameters.push_back(pt->get_gcs_point().x);
	_Parameters.push_back(pt->get_gcs_point().y);

	return pt->get_id();

	//std::cout<<"point";

	/*GCS::shape shp;
	shp.id = ++id;
	shp.Parameters.push_back(new double(x1));
	shp.Parameters.push_back(new double(y1));

	_Parameters.push_back(shp.Parameters[0]);
	_Parameters.push_back(shp.Parameters[1]);

	shp.type = GCS::Shape_Type::POINT;
	_Shapes.push_back(shp);

	std::cout<<"Point added with ID: "<<shp.id<<"\n";
	//std::cout<<shape.id;
	//std::cout<<"\n";
	return shp.id;
	*/
}

//void GCSWrapper::fix_point(int id, double x, double y)	
void GCSWrapper::fix_point(int id)	
{
	SaPoint* p = (SaPoint*)get_shape(id);
	double x = *(p->get_gcs_point().x);
	double y = *(p->get_gcs_point().y);

	double* xp = new double(x);
	double* yp = new double(y);

	_FixedParameters.push_back(xp);
	_FixedParameters.push_back(yp);

	gcs_sys.addConstraintCoordinateX(p->get_gcs_point(), xp, 1);
	gcs_sys.addConstraintCoordinateY(p->get_gcs_point(), yp, 1);

	//int System::addConstraintCoordinateX(Point &p, double *x, int tagId)
}

void GCSWrapper::fix_line(int id)	
{
	SaLine* l = (SaLine*)get_shape(id);
	double x1 = *(l->get_gcs_line().p1.x);
	double y1 = *(l->get_gcs_line().p1.y);
	double x2 = *(l->get_gcs_line().p2.x);
	double y2 = *(l->get_gcs_line().p2.y);

	double* x1p = new double(x1);
	double* y1p = new double(y1);
	double* x2p = new double(x2);
	double* y2p = new double(y2);

	_FixedParameters.push_back(x1p);
	_FixedParameters.push_back(y1p);
	_FixedParameters.push_back(x2p);
	_FixedParameters.push_back(y2p);

	gcs_sys.addConstraintCoordinateX(l->get_gcs_line().p1, x1p, 1);
	gcs_sys.addConstraintCoordinateY(l->get_gcs_line().p1, y1p, 1);

	gcs_sys.addConstraintCoordinateX(l->get_gcs_line().p2, x2p, 1);
	gcs_sys.addConstraintCoordinateY(l->get_gcs_line().p2, y2p, 1);

	//int System::addConstraintCoordinateX(Point &p, double *x, int tagId)
}

int GCSWrapper::add_segment(double x1, double y1, double x2, double y2)	{

	//std::cout<<"segment";

	SaLine* ln = new SaLine(++id, x1, y1, x2, y2);

	_Shapes.push_back(ln);
	_Parameters.push_back(ln->get_gcs_line().p1.x);
	_Parameters.push_back(ln->get_gcs_line().p1.y);
	_Parameters.push_back(ln->get_gcs_line().p2.x);
	_Parameters.push_back(ln->get_gcs_line().p2.y);

	return ln->get_id();

	/*
	GCS::shape shp;
	shp.id = ++id;
	shp.Parameters.push_back(new double(x1));
	shp.Parameters.push_back(new double(y1));
	shp.Parameters.push_back(new double(x2));
	shp.Parameters.push_back(new double(y2));

	_Parameters.push_back(shp.Parameters[0]);
	_Parameters.push_back(shp.Parameters[1]);
	_Parameters.push_back(shp.Parameters[2]);
	_Parameters.push_back(shp.Parameters[3]);

	shp.type = GCS::Shape_Type::LINE;
	_Shapes.push_back(shp);

	//std::cout<<shape.id;
	std::cout<<"Segment added with ID: "<<shp.id<<"\n";
	return shp.id;
	*/
}

/*
int GCSWrapper::add_vertical_segment(double x1, double y1, double y2)	{

	SaLine* ln = new SaLine(++id, x1, y1, x1, y2);

	_Shapes.push_back(ln);
	_Parameters.push_back(ln->get_gcs_line().p1.x);
	_Parameters.push_back(ln->get_gcs_line().p1.y);
	_Parameters.push_back(ln->get_gcs_line().p1.x);
	_Parameters.push_back(ln->get_gcs_line().p2.y);

	return ln->get_id();
}*/

int GCSWrapper::add_circle(double x1, double y1, double r)	{

	SaCircle* c = new SaCircle(++id, x1, y1, r);

	_Shapes.push_back(c);
	_Parameters.push_back(c->get_gcs_circle().center.x);
	_Parameters.push_back(c->get_gcs_circle().center.y);
	_Parameters.push_back(c->get_gcs_circle().rad);

	return c->get_id();
}

/*
int GCSWrapper::add_circle(double x1, double y1, double radius)	{

	//std::cout<<"circle";

	GCS::shapes shape;
	shape.id = ++id;
	shape.Parameters.push_back(new double(x1));
	shape.Parameters.push_back(new double(y1));	
	shape.Parameters.push_back(new double(radius));

	shape.type = GCS::Shape_Type::CIRCLE;
	Shapes.push_back(shape);

	//std::cout<<shape.id;
	//std::cout<<"\n";
	std::cout<<"Circle added with ID: "<<shape.id<<"\n";
	return shape.id;

	return -1;
}
*/

SaShape* GCSWrapper::get_shape(int id)	{

	for (std::vector<SaShape*>::iterator it=_Shapes.begin(); it!=_Shapes.end(); ++it)	{
    
		SaShape* shp = *it;		
		if(shp->get_id() == id)	{
			
			return shp;
		}
	}

	return NULL;
}

Shape_Type GCSWrapper::get_shape_type(int id)	{

	for (std::vector<SaShape*>::iterator it=_Shapes.begin(); it!=_Shapes.end(); ++it)	{
    
		SaShape* shp = *it;
		if(shp->get_id() == id)	{
			
			shp->get_type();
		}
	}

	return Shape_Type::NONE;
}

/*
std::string GCSWrapper::get_shape_type(GCS::Shape_Type type)	{

	switch(type)	{
	
		case GCS::Shape_Type::POINT:
			return "Point";
		case GCS::Shape_Type::LINE:
			return "Line";
		case GCS::Shape_Type::CIRCLE:
			return "Circle";
	}
}
*/

void GCSWrapper::perpendicular(int id1, int id2)	{

	SaLine* l1 = (SaLine*)get_shape(id1);
	SaLine* l2 = (SaLine*)get_shape(id2);	

	gcs_sys.addConstraintPerpendicular(l1->get_gcs_line(), l2->get_gcs_line(), 1);	

	//GCS::GCSWrapper::gcs_sys.declareUnknowns(Parameters);


	//std::vector<double*> Parameters;	
	//GCS::GCSWrapper::gcs_sys.clear();

	//double radius;
	//GCS::Point p1, p2, p3, p4;
	//GCS::Circle c1, c2;
	//GCS::Line l1, l2;
	
	/*
	for(std::vector<double*>::iterator it=shape1.Parameters.begin(); it!=shape1.Parameters.end(); ++it)	{    
		
		Parameters.push_back(*it);
	}

	for(std::vector<double*>::iterator it=shape2.Parameters.begin(); it!=shape2.Parameters.end(); ++it)	{    
		
		Parameters.push_back(*it);
	}*/

	/*if(shape1.type == Shape_Type::POINT)	{
		p1.x = Parameters[0];
		p1.y = Parameters[1];
	}
	else if(shape1.type == Shape_Type::LINE)	{
	
		p1.x = Parameters[0];
		p1.y = Parameters[1];
		p2.x = Parameters[2];
		p2.y = Parameters[3];

		l1.p1 = p1;
		l1.p2 = p2;		
	}
	else if(shape1.type == Shape_Type::CIRCLE)	{
	
		std::cout<<"Invalid object type\n";
		return;
	}

	int size = shape1.Parameters.size();
	
	if(shape2.type == Shape_Type::POINT)	{
		p3.x = Parameters[size];
		p3.y = Parameters[size++];
	}
	else if(shape2.type == Shape_Type::LINE)	{
	
		p3.x = Parameters[size];
		p3.y = Parameters[++size];
		p4.x = Parameters[++size];
		p4.y = Parameters[++size];

		l2.p1 = p3;
		l2.p2 = p4;		
	}
	else if(shape2.type == Shape_Type::CIRCLE)	{
	
		std::cout<<"Invalid object type\n";
		return;
	}*/	
}

void GCSWrapper::parallel(int id1, int id2)	{

	SaLine* l1 = (SaLine*)get_shape(id1);
	SaLine* l2 = (SaLine*)get_shape(id2);	

	gcs_sys.addConstraintParallel(l1->get_gcs_line(), l2->get_gcs_line(), 1);	

	/*
	GCS::shapes shape1, shape2;
	shape1 = get_shape(id1);
	shape2 = get_shape(id2);
	
	std::vector<double*> Parameters;	
	GCS::GCSWrapper::gcs_sys.clear();

	//double radius;
	GCS::Point p1, p2, p3, p4;
	GCS::Circle c1, c2;
	GCS::Line l1, l2;
	
	for(std::vector<double*>::iterator it=shape1.Parameters.begin(); it!=shape1.Parameters.end(); ++it)	{    
		
		Parameters.push_back(*it);
	}

	for(std::vector<double*>::iterator it=shape2.Parameters.begin(); it!=shape2.Parameters.end(); ++it)	{    
		
		Parameters.push_back(*it);
	}

	if(shape1.type == Shape_Type::POINT)	{
		p1.x = Parameters[0];
		p1.y = Parameters[1];
	}
	else if(shape1.type == Shape_Type::LINE)	{
	
		p1.x = Parameters[0];
		p1.y = Parameters[1];
		p2.x = Parameters[2];
		p2.y = Parameters[3];

		l1.p1 = p1;
		l1.p2 = p2;		
	}
	else if(shape1.type == Shape_Type::CIRCLE)	{
	
		std::cout<<"Invalid object type\n";
		return;
	}

	int size = shape1.Parameters.size();
	
	if(shape2.type == Shape_Type::POINT)	{
		p3.x = Parameters[size];
		p3.y = Parameters[size++];
	}
	else if(shape2.type == Shape_Type::LINE)	{
	
		p3.x = Parameters[size];
		p3.y = Parameters[++size];
		p4.x = Parameters[++size];
		p4.y = Parameters[++size];

		l2.p1 = p3;
		l2.p2 = p4;		
	}
	else if(shape2.type == Shape_Type::CIRCLE)	{
	
		std::cout<<"Invalid object type\n";
		return;
	}

	GCS::GCSWrapper::gcs_sys.addConstraintParallel(l1, l2, 1);
	GCS::GCSWrapper::gcs_sys.declareUnknowns(Parameters);
	*/
}

void GCSWrapper::line_vertical(int id)
{
	SaLine* l = (SaLine*)get_shape(id);
	gcs_sys.addConstraintVertical(l->get_gcs_line(), 1);
}

void GCSWrapper::coincident_point_circle(int id1, int id2)
{
	SaPoint* p = (SaPoint*)get_shape(id1);
	SaCircle* c = (SaCircle*)get_shape(id2);

	gcs_sys.addConstraintPointOnCircle(p->get_gcs_point(), c->get_gcs_circle(), 1);
}

void GCSWrapper::coincident_line_circle(int id1, int id2)
{
	SaLine* l = (SaLine*)get_shape(id1);
	SaCircle* c = (SaCircle*)get_shape(id2);

	gcs_sys.addConstraintPointOnCircle(l->get_gcs_line().p1, c->get_gcs_circle(), 1);
	gcs_sys.addConstraintPointOnCircle(l->get_gcs_line().p2, c->get_gcs_circle(), 1);
}

void GCSWrapper::collinear_point_line(int id1, int id2)	
{
	SaPoint* p = (SaPoint*)get_shape(id1);
	SaLine* l = (SaLine*)get_shape(id2);

	gcs_sys.addConstraintPointOnLine(p->get_gcs_point(), l->get_gcs_line(), 1);
}

/*void GCSWrapper::point_segment_coincidence(int id1, int id2)	// id1 = point, id2 = line
{
	SaPoint* p1 = (SaPoint*)get_shape(id1);
	SaLine* l1 = (SaLine*)get_shape(id2);

    int p = add_point(5.0, 5.0);
	int c = add_circle(10.0, 10.0, 3.0);
	int l2 = add_segment(0.0, 0.0, 10.0, 0.0);

	coincident_line_circle(id2, c);
	collinear_point_line(id1, id2);

	coincident_line_circle(l2, c);
	perpendicular(id2, l2);

	collinear_point_line(p, id2);
	collinear_point_line(p, l2);
}*/

void GCSWrapper::point_segment_coincidence(int id1, int id2)	// id1 = point, id2 = line
{
	SaPoint* p = (SaPoint*)get_shape(id1);
	SaLine* l1 = (SaLine*)get_shape(id2);

    //int p = add_point(5.0, 5.0);
	int c = add_circle(10.0, 10.0, 3.0);
	int l2 = add_segment(0.0, 0.0, 10.0, 0.0);

	coincident_line_circle(id2, c);
	collinear_point_line(id1, id2);

	coincident_line_circle(l2, c);
	perpendicular(id2, l2);

	//collinear_point_line(p, id2);
	collinear_point_line(id1, l2);

	SaCircle* circle = (SaCircle*)get_shape(c);
	 //circle->get_gcs_circle().center;
	
	gcs_sys.addConstraintPointOnLine(circle->get_gcs_circle().center, l1->get_gcs_line(), 1);
}

/*void GCSWrapper::segment_segment_coincidence(int id1, int id2)
{
	SaLine* l1 = (SaLine*)get_shape(id1);
	SaLine* l2 = (SaLine*)get_shape(id2);
}*/

void GCSWrapper::circle_min_diameter(int id, double diameter)
{
	int s1 = add_segment(0.0, 0.0, 0.0, diameter);		
	
	line_vertical(s1);
	fix_length(s1, diameter);
	coincident_line_circle(s1, id);
}

void GCSWrapper::fix_circle_radius(int id, double radius)
{
	SaCircle* c = (SaCircle*)get_shape(id);

	double* d = new double(radius);
	_FixedParameters.push_back(d);
	
	gcs_sys.addConstraintCircleRadius(c->get_gcs_circle(), d, 1);
}

/*
void GCSWrapper::point_vertical(int id1, int id2)	{

	SaPoint* p1 = (SaPoint*)get_shape(id1);
	SaPoint* p2 = (SaPoint*)get_shape(id2);
	gcs_sys.addConstraintVertical(p1->get_gcs_point(), p2->get_gcs_point(), 1);
}*/

/*
void GCSWrapper::tangent(int id1, int id2)	{

	GCS::shapes shape1, shape2;
	shape1 = get_shape(id1);
	shape2 = get_shape(id2);
	
	std::vector<double*> Parameters;	
	GCS::GCSWrapper::gcs_sys.clear();

	GCS::Circle c1, c2;
	GCS::Line l1, l2;

	for(std::vector<double*>::iterator it=shape1.Parameters.begin(); it!=shape1.Parameters.end(); ++it)	{    
		
		Parameters.push_back(*it);
	}
	for(std::vector<double*>::iterator it=shape2.Parameters.begin(); it!=shape2.Parameters.end(); ++it)	{    
		
		Parameters.push_back(*it);
	}

	if(!(shape1.type == Shape_Type::LINE && shape1.type == Shape_Type::CIRCLE)
		|| (shape1.type == Shape_Type::CIRCLE && shape2.type == Shape_Type::LINE))
		return;


	if(shape1.type == Shape_Type::POINT)	{
		p1.x = Parameters[0];
		p1.y = Parameters[1];
	}
	else if(shape1.type == Shape_Type::LINE)	{
	
		p1.x = Parameters[0];
		p1.y = Parameters[1];
		p2.x = Parameters[2];
		p2.y = Parameters[3];

		l1.p1 = p1;
		l1.p2 = p2;		
	}
	else if(shape1.type == Shape_Type::CIRCLE)	{
	
		std::cout<<"Invalid object type\n";
		return;
	}

	int size = shape1.Parameters.size();
	if(shape2.type == Shape_Type::POINT)	{
		p3.x = Parameters[size];
		p3.y = Parameters[size++];
	}
	else if(shape2.type == Shape_Type::LINE)	{
	
		p3.x = Parameters[size];
		p3.y = Parameters[++size];
		p4.x = Parameters[++size];
		p4.y = Parameters[++size];

		l2.p1 = p3;
		l2.p2 = p4;		
	}
	else if(shape2.type == Shape_Type::CIRCLE)	{
	
		std::cout<<"Invalid object type\n";
		return;
	}

	GCS::GCSWrapper::gcs_sys.addConstraintParallel(l1, l2, 1);
	GCS::GCSWrapper::gcs_sys.declareUnknowns(Parameters);
}
*/
void GCSWrapper::fix_length(int id, double length)	{

	SaLine* l1 = (SaLine*)get_shape(id);
	//SaLine* l2 = (SaLine*)get_shape(id2);	

	double* d = new double(length);
	_FixedParameters.push_back(d);
	//FixParameters.push_back(new double(20));
	
	// adding constraing
	gcs_sys.addConstraintP2PDistance(l1->get_gcs_line().p1, l1->get_gcs_line().p2, d, 1);	

	//gcs_sys.addConstraintParallel(l1->get_gcs_line(), l2->get_gcs_line(), 1);

	/*
	GCS::shapes shape1;
	shape1 = get_shape(id);
	
	std::vector<double*> FixedParameters;
	std::vector<double*> Parameters;	
	GCS::GCSWrapper::gcs_sys.clear();

	GCS::Point p1, p2;	
	GCS::Line l1;
	
	for(std::vector<double*>::iterator it=shape1.Parameters.begin(); it!=shape1.Parameters.end(); ++it)	{    
		
		Parameters.push_back(*it);
	}
	
	Parameters.push_back(new double(length));

	//FixedParameters.push_back(new double(length));
	if(shape1.type == Shape_Type::LINE)	{
	
		p1.x = Parameters[0];
		p1.y = Parameters[1];
		p2.x = Parameters[2];
		p2.y = Parameters[3];

		l1.p1 = p1;
		l1.p2 = p2;		
	}
	else	{
	
		std::cout<<"Invalid object type\n";
		return;
	}

	double* val = Parameters[4];
	GCS::GCSWrapper::gcs_sys.addConstraintP2PDistance(p1, p2, val, 1);
	GCS::GCSWrapper::gcs_sys.declareUnknowns(Parameters);*/
}



void GCSWrapper::angle(int id1, int id2, int id3, double angle)
{
	/*SaPoint* p1 = (SaPoint*)get_shape(id1);
	SaPoint* p2 = (SaPoint*)get_shape(id2);
	SaPoint* p3 = (SaPoint*)get_shape(id3);

	int l1 = add_segment(p1->get_gcs_point().x, p1->get_gcs_point().y, p2->get_gcs_point().x, p2->get_gcs_point().y);
	int l2 = add_segment(p2->get_gcs_point().x, p2->get_gcs_point().y, p3->get_gcs_point().x, p3->get_gcs_point().y);
	
	double* ang = new double(angle);
	_FixedParameters.push_back(ang);

	SaLine* line1 = (SaLine*)get_shape(l1);
	SaLine* line2 = (SaLine*)get_shape(l2);
	
	gcs_sys.addConstraintL2LAngle(line1->get_gcs_line(), line2->get_gcs_line(), ang, 1);*/
}

bool GCSWrapper::solve()	{
	
	gcs_sys.declareUnknowns(_Parameters);
	gcs_sys.initSolution();
	int ret = gcs_sys.solve(true, GCS::DogLeg);

	if(ret == GCS::Success)	{
	
		gcs_sys.applySolution();
		std::cout<<"Solved successfully."<<"\n";
		return true;
	}
	else	{
	
		std::cout<<"Failed to solve.";
		return false;
	}	
}

void GCSWrapper::show_values(int id)	{

	//GCS::shapes shape = get_shape(id);	

	//std::cout<<"Shape Id: "<<shape.id;
	//std::cout<<"Shape Type:"<<get_shape_type(shape.type)<<"\n";

	for(int i=0; i<_Parameters.size(); ++i)	{    

		std::cout<<"Param "<<i+1<<": "<<*_Parameters[i]<<"\n";
		
	}
}

//}