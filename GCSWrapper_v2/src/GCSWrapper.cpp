#include "GCSWrapper.h"

//namespace GCS 
//{

SaPoint::SaPoint(int i, double x, double y):SaShape()	
{
	_id = i;
	_type = POINT;
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
	_type = LINE;
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
	_type = CIRCLE;
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

int GCSWrapper::add_join_segment(double x1, double y1, double x2, double y2)	{

	SaLine* ln = new SaLine(++id, x1, y1, x2, y2);

	_Shapes.push_back(ln);
	return ln->get_id();
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

	return NONE;
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

	double l1_lgh;
	double l1_midx;
	double l1_midy;
	calculate_line_length(l1, l1_lgh);
	calculate_line_midpoint(l1, l1_midx, l1_midy);

    //int p = add_point(5.0, 5.0);
	//int c = add_circle(10.0, 10.0, 3.0);
	int c = add_circle(l1_midx, l1_midy, l1_lgh/2);
	SaCircle* circle = (SaCircle*)get_shape(c);


	coincident_line_circle(id2, c);	
	//solve();
	gcs_sys.addConstraintPointOnLine(circle->get_gcs_circle().center, l1->get_gcs_line(), 1);
	//solve();

	collinear_point_line(id1, id2);
	//solve();
	
	double l2_x1, l2_y1, l2_x2, l2_y2;
	calculate_rotate_line(l1, l2_x1, l2_y1, l2_x2, l2_y2);
	int l2 = add_segment(l2_x1, l2_y1, l2_x2, l2_y2);

	perpendicular(id2, l2);
	//solve();
	
	coincident_line_circle(l2, c);
	//solve();

	//collinear_point_line(p, id2);
	collinear_point_line(id1, l2);
	//solve();
	
	 //circle->get_gcs_circle().center;	
}

void GCSWrapper::test_angle(int id1, int id2)
{	
	SaPoint* p = (SaPoint*)get_shape(id1);
	SaLine* l = (SaLine*)get_shape(id2);

	double x = *p->get_gcs_point().x;
	double y = *p->get_gcs_point().y;

	int p1 = add_point(x, y);

	//fix_line(id2);
	collinear_point_line(id1, id2);
	
	SaPoint* _p1 = (SaPoint*)get_shape(p1);

	double _x1 = *p->get_gcs_point().x;
	double _y1 = *p->get_gcs_point().y;
	double _x2 = *_p1->get_gcs_point().x;
	double _y2 = *_p1->get_gcs_point().y;

	int l1 = add_segment(_x1, _y1, _x2, _y2);
}

void GCSWrapper::angle_1(int id1, int id2, double angle)
{
	SaPoint* p = (SaPoint*)get_shape(id1);
	SaLine* l = (SaLine*)get_shape(id2);

	double* ang = new double(angle);
	_FixedParameters.push_back(ang);
	
	gcs_sys.addConstraintL2LAngle(l->get_gcs_line().p1, l->get_gcs_line().p2, 
		l->get_gcs_line().p1, p->get_gcs_point(), ang, 1);
}
 
void GCSWrapper::left_of(int id1, int id2)
{
	SaPoint* p = (SaPoint*)get_shape(id1);
	SaLine* l = (SaLine*)get_shape(id2);

	// getting values from point
	double p_x = *(p->get_gcs_point().x);	
	double p_y = *(p->get_gcs_point().y);

	// getting values from line
	double l_x1 = *(l->get_gcs_line().p1.x);
	double l_x2 = *(l->get_gcs_line().p2.x);

	// making new point - point C
	double c_x = *(p->get_gcs_point().x);
	double c_y = *(p->get_gcs_point().y);
	
//	std::cout<<c_x<<"\n";
	int c = add_point(c_x, c_y);

	//fix_line(id2);
	collinear_point_line(c, id2);
	show_values(0);

	solve();

	std::cout<<"Values after first solve"<<"\n";
	show_values(0);

	double x = *(_Parameters[_Parameters.size()-2]);
	double y = *(_Parameters[_Parameters.size()-1]);

	std::cout<<"Joining new and old points"<<"\n";
	//int _l = add_join_segment(p_x, p_y, x, y);
	int _l = add_segment(p_x, p_y, x, y);
	SaLine* l2 = (SaLine*)get_shape(_l);
	SaPoint* c_p = (SaPoint*)get_shape(c);

	gcs_sys.addConstraintP2PCoincident(l2->get_gcs_line().p1, p->get_gcs_point(), 1);
	gcs_sys.addConstraintP2PCoincident(l2->get_gcs_line().p2, c_p->get_gcs_point(), 1);
	
	solve();
	show_values(0);

	//double tmpAng = 90;
	double tmpAng = 1.570796;
	if(!calculate_is_left_of(p, l))
		tmpAng = -1 * (tmpAng);

	double* ang = new double(tmpAng);
	_FixedParameters.push_back(ang);

	std::cout<<"tmpAng: "<<tmpAng<<"\n";

	//gcs_sys.addConstraintL2LAngle(l2->get_gcs_line(), l->get_gcs_line(), ang, 1);
	//gcs_sys.addConstraintL2LAngle(l2->get_gcs_line().p1, l2->get_gcs_line().p2, l->get_gcs_line().p1, l->get_gcs_line().p2, ang, 1);
	gcs_sys.addConstraintL2LAngle(l2->get_gcs_line().p2, l2->get_gcs_line().p1, l->get_gcs_line().p1, l->get_gcs_line().p2, ang, 1);

	solve();

	std::cout<<"Angle after solve: "<<*ang<<"\n";

	//SaPoint* _p = (SaPoint*)get_shape(c);

	//double l_x2 = *(_p->get_gcs_point().x);

	//std::cout<<*_Parameters[_Parameters.size()-4];
	//std::cout<<*_Parameters[_Parameters.size()-3];
	
	//double* d = &(_p->get_gcs_point().x);

	//double l_x2 = (_p->get_gcs_point().x);
	//double l_y2 = *(_p->get_gcs_point().y);


	//int l2 = add_segment(l_x1, l_y1, l_x2, l_y2);
/*
	

	if(p_x >= l_x1 || p_x <= l_x2)
	{
		fix_line(id2);
		collinear_point_line(c, id2);
		gcs_sys.addConstraintL2LAngle(l->get_gcs_line().p1, l->get_gcs_line().p2, 
			l->get_gcs_line().p1, p->get_gcs_point(), ang, 1);
	}
	else
	{
		gcs_sys.addConstraintL2LAngle(l->get_gcs_line().p1, l->get_gcs_line().p2, 
			l->get_gcs_line().p1, p->get_gcs_point(), ang);
	}
	*/
}

void GCSWrapper::brace(int id1, int id2)	// id1 = line, id2 = circle
{
	SaLine* l = (SaLine*)get_shape(id1);
	SaCircle* c = (SaCircle*)get_shape(id2);

	coincident_line_circle(id1, id2);
	
	double c_midx = *c->get_gcs_circle().center.x;
	double c_midy = *c->get_gcs_circle().center.y;

	int p = add_point(c_midx, c_midy);
	
	collinear_point_line(p, id1);
}

void GCSWrapper::externally_connected(int id1, int id2)	// id1 = circle1, id2 = circle2
{
	SaCircle* c1 = (SaCircle*)get_shape(id1);
	SaCircle* c2 = (SaCircle*)get_shape(id2);

	double c1_rad = *(c1->get_gcs_circle().rad);
	double c1_x = *(c1->get_gcs_circle().center.x);
	double c1_y = *(c1->get_gcs_circle().center.y);

	double c2_x = *(c2->get_gcs_circle().center.x);
	double c2_y = *(c2->get_gcs_circle().center.y);

	int l = add_segment(c1_x, c1_y, c2_x, c2_y);
	SaLine* l1 = (SaLine*)get_shape(l);
	gcs_sys.addConstraintP2PCoincident(l1->get_gcs_line().p1, c1->get_gcs_circle().center, 1);
	gcs_sys.addConstraintP2PCoincident(l1->get_gcs_line().p2, c2->get_gcs_circle().center, 1);
	
	double dem_x = *(l1->get_gcs_line().p2.x);
	double dem_y = *(l1->get_gcs_line().p2.y);
	double p_x, p_y;

	// to avoid zero-division
	if(dem_x == 0)
		p_x = *(l1->get_gcs_line().p1.x) + rand() % 1;
	else	
		p_x = *(l1->get_gcs_line().p1.x) + rand() % (int)dem_x;
	
	if(dem_y == 0)
		p_y = *(l1->get_gcs_line().p1.y) + rand() % 1;
	else
		p_y = *(l1->get_gcs_line().p1.y) + rand() % (int)dem_y;

	int p1 = add_point(p_x, p_y);
	coincident_point_circle(p1, id1);
	coincident_point_circle(p1, id2);
	
	SaPoint* p = (SaPoint*)get_shape(p1);
	gcs_sys.addConstraintPointOnLine(p->get_gcs_point(), l1->get_gcs_line(), 1);
}

void GCSWrapper::proper_part(int id1, int id2)		// id1 = circle1, id2 = circle2
{
}

void GCSWrapper::angle_line_circle(int id1, int id2, double angle)	// id1 = line, id2 = circle
{
	SaLine* l = (SaLine*)get_shape(id1);
	SaCircle* c = (SaCircle*)get_shape(id2);

	GCS::Point p = c->get_gcs_circle().center;
	
	// getting values from line
	double l_x1 = *(l->get_gcs_line().p1.x);
	double l_x2 = *(l->get_gcs_line().p2.x);

	// making new point - point C
	double c_x = *p.x;
	double c_y = *p.y;
	
	//std::cout<<c_x<<"\n";
	//std::cout<<c_y<<"\n";

	// std::cout<<c_x<<"\n";
	int p1 = add_point(c_x, c_y);
	show_values(0);

	collinear_point_line(p1, id1);
	
	solve();

	std::cout<<"Values after first solve"<<"\n";
	show_values(0);

	double x = *(_Parameters[_Parameters.size()-2]);
	double y = *(_Parameters[_Parameters.size()-1]);

	std::cout<<"Joining new and old points"<<"\n";
	int _l = add_segment(*(c->get_gcs_circle().center.x), *(c->get_gcs_circle().center.y), x, y);
	show_values(0);
	SaLine* l2 = (SaLine*)get_shape(_l);
	
	solve();
	show_values(0);

	double* ang = new double(angle);
	_FixedParameters.push_back(ang);

	gcs_sys.addConstraintL2LAngle(l2->get_gcs_line(), l->get_gcs_line(), ang, 1);

	solve();
	show_values(0);

	double p2_x = *(c->get_gcs_circle().center.x) + *(c->get_gcs_circle().rad);
	double p2_y = *(c->get_gcs_circle().center.y);
	
	int p2 = add_point(p2_x, p2_y);

	coincident_point_circle(p2, id2);

	solve();
	show_values(0);

	collinear_point_line(p2, _l);

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

		std::cout<<"Param "<<i+1<<": "<<*_Parameters[i]<<std::endl;
	}
}

////////////////////////
// Calculation methods//
////////////////////////
void GCSWrapper::calculate_line_length(SaLine* l, double& lgh)
{
	double x1 = *(l->get_gcs_line().p1.x);
	double x2 = *(l->get_gcs_line().p2.x);
	
	double y1 = *(l->get_gcs_line().p1.y);
	double y2 = *(l->get_gcs_line().p2.y);

	lgh = sqrt( (pow(x2 - x1, 2)) + (pow(y2 - y1, 2)) );
	//std::cout<<lgh<<"\n";
}

void GCSWrapper::calculate_line_midpoint(SaLine* l, double& x, double& y)
{
	double x1 = *(l->get_gcs_line().p1.x);
	double x2 = *(l->get_gcs_line().p2.x);
	
	double y1 = *(l->get_gcs_line().p1.y);
	double y2 = *(l->get_gcs_line().p2.y);

	x = x1 + ((x2 - x1) / 2);
	y = y1 + ((y2 - y1) / 2);

	//std::cout<<x<<"\n"<<y;
}

void GCSWrapper::calculate_rotate_line(SaLine* l, double& x1,  double& y1, double& x2,  double& y2)
{
	double l_x1 = *(l->get_gcs_line().p1.x);
	double l_x2 = *(l->get_gcs_line().p2.x);
	
	double l_y1 = *(l->get_gcs_line().p1.y);
	double l_y2 = *(l->get_gcs_line().p2.y);

	double dx = l_x2 - l_x1;
	double dy = l_y2 - l_y1;

	double mid_x = l_x1 + dx / 2;
	double mid_y = l_y1 + dy / 2;

	double dxr = -1 * dy;
	double dyr = dx;

	x1 = mid_x - dxr / 2;
	y1 = mid_y - dyr / 2;

	x2 = mid_x + dxr / 2;
	y2 = mid_y + dyr / 2;

	std::cout<<x1<<"\n"<<y1<<"\n"<<x2<<"\n"<<y2;
}

bool GCSWrapper::calculate_is_left_of(SaPoint* p, SaLine* l)
{	
	double xa = *(l->get_gcs_line().p1.x);
	double ya = *(l->get_gcs_line().p1.y);
	double xb = *(l->get_gcs_line().p2.x);
	double yb = *(l->get_gcs_line().p2.y);

	double xp = *(p->get_gcs_point().x);
	double yp = *(p->get_gcs_point().y);

	return (xb - xa)*(yp - ya) > (yb - ya)*(xp - xa);
}