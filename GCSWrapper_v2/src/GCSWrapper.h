#ifndef PLANEGCS_GCSWRAPPER_H
#define PLANEGCS_GCSWRAPPER_H

//#include "stdafx.h"
#include <iostream>
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
	//std::string name = '';
	//std::vector<double*> Parameters;

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

class GCSWrapper
{
public:
	GCSWrapper(void);
	~GCSWrapper(void);

	void reset();

	// create shapes
	int add_point(double x1, double y1);
	int add_join_segment(double x1, double y1, double x2, double y2);
	int add_segment(double x1, double y1, double x2, double y2);
	//int add_vertical_segment(double x1, double y1, double y2);
	int add_circle(double x1, double y1, double radius);
	
	// add constraints
	void perpendicular(int id1, int id2);
	void parallel(int id1, int id2);
	//void tangent(int id1, int id2);
	

	void collinear_point_line(int id1, int id2);	// id1 = point, id2 = line
	void collinear_point_line(int id1, int id2, int id3);	// id1 = point, id2 = line-p1, id3 = line-p2
	void coincident_point_circle(int id1, int id2);	// id1 = point, id2 = circle
	void coincident_line_circle(int id1, int id2);	// id1 = line, id2 = circle
	void coincident_line_line(int id1, int id2);

	//void angle(int id1, int id2, int id3, double angle);

	void line_vertical(int id1);
	//void point_vertical(int id1, int id2);

	void fix_length(int id, double length);	// to set the constraint to fix the length of a line
	void fix_circle_radius(int id, double radius);
	void fix_circle(int id);
    void fix_circle_centroid(int id);
	//void fix_point(int id, double x, double y);
	void fix_point(int id);
	void fix_line(int id);

	void test_angle(int id1, int id2);
	//void angle_1(int id1, int id2, double angle);	
	
	
	bool left_of(int id1, int id2);
    bool left_of(int id1, int id2, int id3);
    bool left_of_centroids(int id1, int id2, int id3);
	void circle_min_diameter(int id, double diameter);
	void point_segment_coincidence(int id1, int id2);
	void angle_line_circle(int id1, int id2, double angle);
	//void Coincident(int id1, int id2);

	void brace(int id1, int id2);	// id1 = line, id2 = circle
	bool externally_connected(int id1, int id2);	// id1 = circle1, id2 = circle2
	void proper_part(int id1, int id2);	// id1 = circle1, id2 = circle2
	void tangential_proper_part(int id1, int id2);	// id1 = circle1, id2 = circle2
	void non_tangential_proper_part(int id1, int id2);	// id1 = circle1, id2 = circle2
	void part_of(int id1, int id2);	// id1 = circle1, id2 = circle2
	void disconnected(int id1, int id2);	// id1 = circle1, id2 = circle2
	void p_disjunction(int id1, int id2);
	void dr_disjunction(int id1, int id2);

	void calculate_line_length(SaLine* l, double& lgh);
	void calculate_line_midpoint(SaLine* l, double& x, double& y);
	void calculate_rotate_line(SaLine* l, double& x1,  double& y1, double& x2,  double& y2);
	bool calculate_is_left_of(SaPoint* p, SaLine* l);
	bool calculate_is_left_of(SaPoint* p, SaPoint* p1, SaPoint* p2);
    void calculate_projection(SaPoint* p, SaPoint* p1, SaPoint* p2, double &prjX, double &prjY);
    void calculate_projection(SaCircle* p, SaCircle* p1, SaCircle* p2, double &prjX, double &prjY);

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
//}

#endif
