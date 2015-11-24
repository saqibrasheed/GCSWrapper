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

SaLine::SaLine(int i, SaPoint* p1, SaPoint* p2):SaShape()	
{
	_id = i;
	_type = LINE;
	
	GCS::Point* _p1 = new GCS::Point();
	GCS::Point* _p2 = new GCS::Point();

	_p1->x = p1->get_gcs_point().x;
	_p1->y = p1->get_gcs_point().y;
	_p2->x = p2->get_gcs_point().x;
	_p2->y = p2->get_gcs_point().y;

	_gcs_line.p1 = *_p1;
	_gcs_line.p2 = *_p2;
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

	gcs_sys.clear();
	_Parameters.clear();
	_FixedParameters.clear();
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
bool GCSWrapper::fix_point(int id)	
{
	SaPoint* p = (SaPoint*)get_shape(id);
	double x = *(p->get_gcs_point().x);
	double y = *(p->get_gcs_point().y);

	double* xp = new double(x);
	double* yp = new double(y);

	_FixedParameters.push_back(xp);
	_FixedParameters.push_back(yp);

	gcs_sys.addConstraintCoordinateX(p->get_gcs_point(), xp, 1);
	if(!solve()) return false;
	gcs_sys.addConstraintCoordinateY(p->get_gcs_point(), yp, 1);
    if(!solve()) return false;

	return true;

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

int GCSWrapper::add_segment(SaPoint* p1, SaPoint* p2)	{

	SaLine* ln = new SaLine(++id, p1, p2);	
	_Shapes.push_back(ln);
	return ln->get_id();
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

bool GCSWrapper::line_vertical(int id)
{
	SaLine* l = (SaLine*)get_shape(id);
	gcs_sys.addConstraintVertical(l->get_gcs_line(), 1);
    if(!solve()) return false;

    return true;
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

void GCSWrapper::coincident_line_line(int id1, int id2)	
{
	SaLine* l1 = (SaLine*)get_shape(id1);
	SaLine* l2 = (SaLine*)get_shape(id2);

	gcs_sys.addConstraintPointOnLine(l1->get_gcs_line().p1, l2->get_gcs_line(), 1);
	gcs_sys.addConstraintPointOnLine(l1->get_gcs_line().p2, l2->get_gcs_line(), 1);
}

void GCSWrapper::collinear_point_line(int id1, int idlp1, int idlp2)
{
	SaPoint* p = (SaPoint*)get_shape(id1);
	SaPoint* l1 = (SaPoint*)get_shape(idlp1);
	SaPoint* l2 = (SaPoint*)get_shape(idlp2);
    
	gcs_sys.addConstraintPointOnLine(p->get_gcs_point(), l1->get_gcs_point(),l2->get_gcs_point(), 1);
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

/*void GCSWrapper::angle_1(int id1, int id2, double angle)
{
	SaPoint* p = (SaPoint*)get_shape(id1);
	SaLine* l = (SaLine*)get_shape(id2);

	double* ang = new double(angle);
	_FixedParameters.push_back(ang);
	
	gcs_sys.addConstraintL2LAngle(l->get_gcs_line().p1, l->get_gcs_line().p2, 
		l->get_gcs_line().p1, p->get_gcs_point(), ang, 1);
}*/
 

bool GCSWrapper::left_of(int id1, int id2)
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
	
	//std::cout<<c_x<<"\n";
	int c = add_point(c_x, c_y);

	//fix_line(id2);
	collinear_point_line(c, id2);
	show_values(0);

	if(!solve()) return false;

	//std::cout<<"Values after first solve"<<"\n";
	//show_values(0);

	double x = *(_Parameters[_Parameters.size()-2]);
	double y = *(_Parameters[_Parameters.size()-1]);

	//std::cout<<"Joining new and old points"<<"\n";
	//int _l = add_join_segment(p_x, p_y, x, y);
	int _l = add_segment(p_x, p_y, x, y);
	SaLine* l2 = (SaLine*)get_shape(_l);
	SaPoint* c_p = (SaPoint*)get_shape(c);

	gcs_sys.addConstraintP2PCoincident(l2->get_gcs_line().p1, p->get_gcs_point(), 1);
	gcs_sys.addConstraintP2PCoincident(l2->get_gcs_line().p2, c_p->get_gcs_point(), 1);
	
	if(!solve()) return false;
	//show_values(0);

	//double tmpAng = 90;
	double tmpAng = 1.570796;
	if(!calculate_is_left_of(p, l))
		tmpAng = -1 * (tmpAng);

	double* ang = new double(tmpAng);
	_FixedParameters.push_back(ang);

	//std::cout<<"tmpAng: "<<tmpAng<<"\n";

	gcs_sys.addConstraintL2LAngle(l2->get_gcs_line().p2, l2->get_gcs_line().p1, l->get_gcs_line().p1, l->get_gcs_line().p2, ang, 1);

	if(!solve()) return false;

	//std::cout<<"Angle after solve: "<<*ang<<"\n";
	return true;
}

bool GCSWrapper::left_of(int id1, int id2, int id3)
{
	SaPoint* p = (SaPoint*)get_shape(id1);
	SaPoint* p1 = (SaPoint*)get_shape(id2);
	SaPoint* p2 = (SaPoint*)get_shape(id3);
    
	// making new point - point C
    double c_x = 0;
    double c_y = 0;
    calculate_projection(p, p1, p2, c_x, c_y);

	int c = add_point(c_x, c_y);
    SaPoint* c_p = (SaPoint*)get_shape(c);
    
	//fix_line(id2);
	collinear_point_line(c, id2, id3);
	//	show_values(0);
    
    if(!solve()) return false;
    
	double x = *(_Parameters[_Parameters.size()-2]);
	double y = *(_Parameters[_Parameters.size()-1]);
    
	double tmpAng = 1.570796;    
	double* ang = new double(tmpAng);
	_FixedParameters.push_back(ang);

    gcs_sys.addConstraintL2LAngle(p1->get_gcs_point(), p2->get_gcs_point(), c_p->get_gcs_point(), p->get_gcs_point(), ang, 1);

    if(!solve()) return false;
    
    return true;
}

bool GCSWrapper::left_of_centroids(int id1, int id2, int id3)
{
	SaCircle* p =  (SaCircle*)get_shape(id1);
	SaCircle* p1 = (SaCircle*)get_shape(id2);
	SaCircle* p2 = (SaCircle*)get_shape(id3);
    
	// making new point - point C
    double c_x = 0;
    double c_y = 0;
    calculate_projection(p, p1, p2, c_x, c_y);
    
	int c = add_point(c_x, c_y);
    SaPoint* c_p = (SaPoint*)get_shape(c);
    
    gcs_sys.addConstraintPointOnLine(c_p->get_gcs_point(), p1->get_gcs_circle().center,p2->get_gcs_circle().center, 1);

	if(!solve()) return false;
	
	double x = *(_Parameters[_Parameters.size()-2]);
	double y = *(_Parameters[_Parameters.size()-1]);
    
	double tmpAng = 1.570796;
	double* ang = new double(tmpAng);
	_FixedParameters.push_back(ang);

	gcs_sys.addConstraintL2LAngle(p1->get_gcs_circle().center, p2->get_gcs_circle().center, c_p->get_gcs_point(), p->get_gcs_circle().center, ang, 1);
	if(!solve()) return false;

	return true;
}

void GCSWrapper::brace(int id1, int id2)	// id1 = line, id2 = circle
{
	SaLine* l = (SaLine*)get_shape(id1);
	SaCircle* c = (SaCircle*)get_shape(id2);

	coincident_line_circle(id1, id2);
	solve();

	double c_midx = *c->get_gcs_circle().center.x;
	double c_midy = *c->get_gcs_circle().center.y;

	int p = add_point(c_midx, c_midy);
	collinear_point_line(p, id1);
}

bool GCSWrapper::externally_connected(int id1, int id2)	// id1 = circle1, id2 = circle2
{
	SaCircle* c1 = (SaCircle*)get_shape(id1);
 	SaCircle* c2 = (SaCircle*)get_shape(id2);

	double c1_rad = *(c1->get_gcs_circle().rad);
 	double c1_x = *(c1->get_gcs_circle().center.x);
 	double c1_y = *(c1->get_gcs_circle().center.y);

	double c2_x = *(c2->get_gcs_circle().center.x);
 	double c2_y = *(c2->get_gcs_circle().center.y);

	//- add line between centroids of circles
 	int l = add_segment(c1_x, c1_y, c2_x, c2_y);
 	SaLine* l1 = (SaLine*)get_shape(l);
 	gcs_sys.addConstraintP2PCoincident(l1->get_gcs_line().p1, c1->get_gcs_circle().center, 1);
    if(!solve()) return false;
 	
	gcs_sys.addConstraintP2PCoincident(l1->get_gcs_line().p2, c2->get_gcs_circle().center, 1);
	if(!solve()) return false;

	//- get midpoint of line segment
    double midx = ((c2_x - c1_x) / 2) + c1_x;
    double midy = ((c2_y - c1_y) / 2) + c1_y;
	
    //- add point coincident to line between centroids
	int p1 = add_point(midx, midy);
    point_segment_coincidence(p1, l);
    if(!solve()) return false;
    
    //- make point the contact point of the circles
 	coincident_point_circle(p1, id1);
    if(!solve()) return false;

	coincident_point_circle(p1, id2);
    if(!solve()) return false;

	return true;
	//SaPoint* p = (SaPoint*)get_shape(p1);
	//gcs_sys.addConstraintPointOnLine(p->get_gcs_point(), l1->get_gcs_line(), 1);
    //point_segment_coincidence(p1, l);
}

//void GCSWrapper::externally_connected(int id1, int id2)	// id1 = circle1, id2 = circle2
 //{
 //	SaCircle* c1 = (SaCircle*)get_shape(id1);
 //	SaCircle* c2 = (SaCircle*)get_shape(id2);
 //
 //	double c1_rad = *(c1->get_gcs_circle().rad);
 //	double c1_x = *(c1->get_gcs_circle().center.x);
 //	double c1_y = *(c1->get_gcs_circle().center.y);
 //
 //	double c2_x = *(c2->get_gcs_circle().center.x);
 //	double c2_y = *(c2->get_gcs_circle().center.y);
 //
 //	int l = add_segment(c1_x, c1_y, c2_x, c2_y);
 //	SaLine* l1 = (SaLine*)get_shape(l);
 //	gcs_sys.addConstraintP2PCoincident(l1->get_gcs_line().p1, c1->get_gcs_circle().center, 1);
 //	gcs_sys.addConstraintP2PCoincident(l1->get_gcs_line().p2, c2->get_gcs_circle().center, 1);
 //	
 //	double dem_x = *(l1->get_gcs_line().p2.x);
 //	double dem_y = *(l1->get_gcs_line().p2.y);
 //	double p_x, p_y;
 //
 //	// to avoid zero-division
 //	if(dem_x == 0)
 //		p_x = *(l1->get_gcs_line().p1.x) + rand() % 1;
 //	else	
 //		p_x = *(l1->get_gcs_line().p1.x) + rand() % (int)dem_x;
 //	
 //	if(dem_y == 0)
 //		p_y = *(l1->get_gcs_line().p1.y) + rand() % 1;
 //	else
 //		p_y = *(l1->get_gcs_line().p1.y) + rand() % (int)dem_y;
 //
 //	int p1 = add_point(p_x, p_y);
 //	coincident_point_circle(p1, id1);
 //	coincident_point_circle(p1, id2);
 //	
 //	SaPoint* p = (SaPoint*)get_shape(p1);
 //	//gcs_sys.addConstraintPointOnLine(p->get_gcs_point(), l1->get_gcs_line(), 1);
 //    point_segment_coincidence(p1, l);
 //}

bool GCSWrapper::proper_part(int id1, int id2)		// id1 = circle1, id2 = circle2
{
	SaCircle* c1 = (SaCircle*)get_shape(id1);
	SaCircle* c2 = (SaCircle*)get_shape(id2);

	double r1 = *(c1->get_gcs_circle().rad);
	double r2 = *(c2->get_gcs_circle().rad);
	
	double circum1 = r1 * 2;
	double circum2 = r2 * 2;

	// for C2
	double l1_ax = *(c2->get_gcs_circle().center.x) - r2;
	double l1_ay = *(c2->get_gcs_circle().center.y);
	double l1_bx = l1_ax + circum2;

	int l1 = add_segment(l1_ax, l1_ay, l1_bx, l1_ay);
	brace(l1, id2);	// id2 = id for circle2
	if(!solve()) return false;

	// for C1
	double l2_cx = *(c1->get_gcs_circle().center.x) - r1;
	double l2_cy = *(c1->get_gcs_circle().center.y);
	double l2_dx = l2_cx + circum1;

	int l2 = add_segment(l2_cx, l2_cy, l2_dx, l2_cy);
	brace(l2, id1);	// id1 = id for circle1
	if(!solve()) return false;

	int p_c = add_point(l2_cx, l2_cy);
	int p_d = add_point(l2_dx, l2_cy);
	SaPoint* c = (SaPoint*)get_shape(p_c);
	SaPoint* d = (SaPoint*)get_shape(p_d);

	SaLine* l_ab = (SaLine*)get_shape(l1);
	//gcs_sys.addConstraintPointOnLine(c->get_gcs_point(), l_ab->get_gcs_line(), 1);
	point_segment_coincidence(p_c, l1);
	if(!solve()) return false;

	//gcs_sys.addConstraintPointOnLine(d->get_gcs_point(), l_ab->get_gcs_line(), 1);
	point_segment_coincidence(p_d, l1);
	if(!solve()) return false;
	
	return true;
}

bool GCSWrapper::proper_part_n(int id1, int id2)		// id1 = circle1, id2 = circle2
{
	SaCircle* c1 = (SaCircle*) get_shape(id1);
	SaCircle* c2 = (SaCircle*) get_shape(id2);

	double c1_x1 = (*(c1->get_gcs_circle().center.x) - *(c1->get_gcs_circle().rad));
	double c1_x2 = (*(c1->get_gcs_circle().center.x) + *(c1->get_gcs_circle().rad));
	double c1_y = *(c1->get_gcs_circle().center.y);	

	double c2_x1 = (*(c2->get_gcs_circle().center.x) - *(c2->get_gcs_circle().rad));
	double c2_x2 = (*(c2->get_gcs_circle().center.x) + *(c2->get_gcs_circle().rad));
	double c2_y = *(c2->get_gcs_circle().center.y);

	int c1_p1 = add_point(c1_x1, c1_y);
	int c1_p2 = add_point(c1_x2, c1_y);

	SaPoint* p1 = (SaPoint*) get_shape(c1_p1);
	SaPoint* p2 = (SaPoint*) get_shape(c1_p2);

	int l1 = add_segment(p1, p2);
	brace(l1, id1);
	if(!solve()) return false;
	
	int c2_p1 = add_point(c2_x1, c2_y);
	int c2_p2 = add_point(c2_x2, c2_y);
	
	SaPoint* p3 = (SaPoint*) get_shape(c2_p1);
	SaPoint* p4 = (SaPoint*) get_shape(c2_p2);
	
	int l2 = add_segment(p3, p4);
	brace(l2, id2);
	if(!solve()) return false;

	point_segment_coincidence(c1_p1, l2);
	if(!solve()) return false;

	point_segment_coincidence(c1_p2, l2);
	if(!solve()) return false;

	return true;
}

/*bool GCSWrapper::tangential_proper_part(int id1, int id2)	// id1 = circle1, id2 = circle2
{
	SaCircle* c1 = (SaCircle*) get_shape(id1);
	SaCircle* c2 = (SaCircle*) get_shape(id2);

	double c1_x1 = (*(c1->get_gcs_circle().center.x) - *(c1->get_gcs_circle().rad));
	double c1_x2 = (*(c1->get_gcs_circle().center.x) + *(c1->get_gcs_circle().rad));
	double c1_y = *(c1->get_gcs_circle().center.y);

	double c2_x1 = (*(c2->get_gcs_circle().center.x) - *(c2->get_gcs_circle().rad));
	double c2_x2 = (*(c2->get_gcs_circle().center.x) + *(c2->get_gcs_circle().rad));
	double c2_y = *(c2->get_gcs_circle().center.y);

	int c1_p1 = add_point(c1_x1, c1_y);
	int c1_p2 = add_point(c1_x2, c1_y);

	SaPoint* a = (SaPoint*) get_shape(c1_p1);
	SaPoint* c = (SaPoint*) get_shape(c1_p2);

	int c2_p2 = add_point(c2_x2, c2_y);
	SaPoint* b = (SaPoint*) get_shape(c2_p2);

	// L(a,b)
	int l_ab = add_segment(a, b);
	
	// brace(L(a,b),c2)
	brace(l_ab, id2);
	if(!solve()) return false;

	// L(a,c)
	int l_ac = add_segment(a, c);

	// brace(L(a,c),c1)
	brace(l_ac, id1);
	if(!solve()) return false;

	// coincident(c, l(a,b))
	point_segment_coincidence(c1_p2, l_ab);
	if(!solve()) return false;

	return true;
}*/

// With ID assignment
/*bool GCSWrapper::tangential_proper_part(int id1, int id2)	// id1 = circle1, id2 = circle2
{
	SaCircle* c1 = (SaCircle*) get_shape(id1);
	SaCircle* c2 = (SaCircle*) get_shape(id2);
		
	double c2_x1 = (*(c2->get_gcs_circle().center.x) - *(c2->get_gcs_circle().rad));
	double c2_x2 = (*(c2->get_gcs_circle().center.x) + *(c2->get_gcs_circle().rad));
	double c2_y = *(c2->get_gcs_circle().center.y);

	int p_a = add_point(c2_x1, c2_y);
	int p_b = add_point(c2_x2, c2_y);

	SaPoint* a = (SaPoint*) get_shape(p_a);
	SaPoint* b = (SaPoint*) get_shape(p_b);

	int l_ab = add_segment(a, b);
	//int l_ab = add_segment(c2_x1, c2_y, c2_x2, c2_y);
	if(!solve()) return false;

	// BRACE(L(a,b), C2)
	brace(l_ab, id2);
	if(!solve()) return false;
	
	double c1_rad = *(c1->get_gcs_circle().rad);
	double c3_x1 = c2_x1 + c1_rad;
	double c3_y = c2_y;
	double c3_rad = c1_rad;
	double c3_x2 = c3_x1 + c1_rad;

	int c_c3 = add_circle(c3_x1, c3_y, c3_rad);
	
	int l_c3 = add_segment(a, c);
	//int l_c3 = add_segment(c2_x1, c2_y, c3_x2, c3_y);
	id1 = c_c3;	// c_c3 is the circle which will replace c1. so assigning id of c_c3 to id1

	// BRACE(L(a,c), C1)
	//brace(l_c3, c_c3);
	brace(l_c3, id1);
	if(!solve()) return false;

	int p_c = add_point(c3_x2, c3_y);
	SaPoint* c = (SaPoint*) get_shape(p_c);

	point_segment_coincidence(p_c, l_ab);
	if(!solve()) return false;	

	return true;
}*/

bool GCSWrapper::tangential_proper_part(int id1, int id2)	// id1 = circle1, id2 = circle2
{
	SaCircle* c1 = (SaCircle*) get_shape(id1);
	SaCircle* c2 = (SaCircle*) get_shape(id2);
		
	double c2_x1 = (*(c2->get_gcs_circle().center.x) - *(c2->get_gcs_circle().rad));
	double c2_x2 = (*(c2->get_gcs_circle().center.x) + *(c2->get_gcs_circle().rad));
	double c2_y = *(c2->get_gcs_circle().center.y);

	int p_a = add_point(c2_x1, c2_y);
	int p_b = add_point(c2_x2, c2_y);

	SaPoint* a = (SaPoint*) get_shape(p_a);
	SaPoint* b = (SaPoint*) get_shape(p_b);

	int l_ab = add_segment(a, b);	
	
	// BRACE(L(a,b), C2)
	brace(l_ab, id2);
	if(!solve()) return false;	

	double c1_rad = *(c1->get_gcs_circle().rad);
	double c1_new_x = c2_x1 + c1_rad;

	c1->get_gcs_circle().center.x = &c1_new_x;
	c1->get_gcs_circle().center.y = &c2_y;

	//double c1_x1 = c2_x1;
	double c1_x2 = c1_new_x + c1_rad;
	double c1_y = c2_y;

	int p_c1_c = add_point(c1_x2, c1_y);

	//SaPoint* c1_p_a = (SaPoint*) get_shape(p_c1_a);
	SaPoint* c = (SaPoint*) get_shape(p_c1_c);

	int l_c1 = add_segment(a, c);

	brace(l_c1, id1);
	if(!solve()) return false;

	point_segment_coincidence(p_c1_c, l_ab);
	if(!solve()) return false;

/*
	int c_c3 = add_circle(c3_x1, c3_y, c3_rad);
	
	int l_c3 = add_segment(a, c);
	
	id1 = c_c3;

	// BRACE(L(a,c), C1)
	//brace(l_c3, c_c3);
	brace(l_c3, id1);
	if(!solve()) return false;

	int p_c = add_point(c3_x2, c3_y);
	SaPoint* c = (SaPoint*) get_shape(p_c);

	point_segment_coincidence(p_c, l_ab);
	if(!solve()) return false;	
*/
	return true;
}

bool GCSWrapper::tangential_proper_part_n(int id1, int id2)	// id1 = circle1, id2 = circle2
{
	SaCircle* c1 = (SaCircle*) get_shape(id1);
	SaCircle* c2 = (SaCircle*) get_shape(id2);

	double c1_x = *(c1->get_gcs_circle().center.x);
	double c1_y = *(c1->get_gcs_circle().center.y);
	double c1_rad = *(c1->get_gcs_circle().rad);

	double c2_x = *(c2->get_gcs_circle().center.x);
	double c2_y = *(c2->get_gcs_circle().center.y);
	double c2_rad = *(c2->get_gcs_circle().rad);

	double vect_x = c2_x - c1_x;
	double vect_y = c2_y - c1_y;

	double w_x = sqrt((pow(c1_x, 2)) + (pow(c2_x, 2)));
	double u_c1_x = c1_x/w_x;
	double u_c2_x = c2_x/w_x;
	double u_vect_x = sqrt((pow(u_c1_x, 2)) + (pow(u_c2_x, 2)));

	double w_y = sqrt((pow(c1_y, 2)) + (pow(c2_y, 2)));
	double u_c1_y = (w_y == 0) ? 0 : c1_y/w_y;
	double u_c2_y = (w_y == 0) ? 0 : c2_y/w_y;
	double u_vect_y = sqrt((pow(u_c1_y, 2)) + (pow(u_c2_y, 2)));

	double new_temp_p1_x = u_vect_x * c1_rad;
	double new_temp_p1_y = u_vect_y / c1_rad;

	double new_temp_p2_x = u_vect_x * c2_rad;
	double new_temp_p2_y = u_vect_y / c2_rad;

	double new_c1_p1_x = c1_x + new_temp_p1_x;
	double new_c1_p1_y = c1_y + new_temp_p1_y;

	double new_c1_p2_x = c1_x - new_temp_p1_x;
	double new_c1_p2_y = c1_y - new_temp_p1_y;

	double new_c2_p1_x = c2_x - new_temp_p2_x;
	double new_c2_p1_y = c2_y - new_temp_p2_y;

	double new_c2_p2_x = c2_x + new_temp_p2_x;
	double new_c2_p2_y = c2_y + new_temp_p2_y;

	int c1_p1 = add_point(new_c1_p1_x, new_c1_p1_y);
	int c1_p2 = add_point(new_c1_p2_x, new_c1_p2_y);

	int c2_p1 = add_point(new_c2_p1_x, new_c2_p1_y);
	int c2_p2 = add_point(new_c2_p2_x, new_c2_p2_y);
	
	return true;
}

// working TPP version
bool GCSWrapper::tpp(int id1, int id2)
{
	SaCircle* c1 = (SaCircle*)get_shape(id1);
 	SaCircle* c2 = (SaCircle*)get_shape(id2);

	double c1_rad = *(c1->get_gcs_circle().rad);
	double c1_x = *(c1->get_gcs_circle().center.x);
	double c1_y = *(c1->get_gcs_circle().center.y);
	
	double c2_rad = *(c2->get_gcs_circle().rad);
	double c2_x = *(c2->get_gcs_circle().center.x);
	double c2_y = *(c2->get_gcs_circle().center.y);	
	
	double dist = ((c2_x - c1_x) * (c2_x - c1_x)) + ((c2_y - c1_y) * (c2_y - c1_y));
	double rad_sum = (c1_rad + c2_rad) * (c1_rad + c2_rad);
	double rad_diff = (c1_rad - c2_rad) * (c1_rad - c2_rad);
	double dist_sqrt = std::sqrt(dist);

	// adding geometry Pa
	// vector code
	double v_x = c1_x - c2_x;
	double v_y = c1_y - c2_y;

	double v_n_x = v_x / dist_sqrt;
	double v_n_y = v_y / dist_sqrt;

	if(dist == 0)
	{
		v_n_x = -1;
		v_n_y = 0;
	}

	double v1_x = v_n_x * c1_rad;
	double v1_y = v_n_y * c1_rad;

	double v2_x = v_n_x * c2_rad;
	double v2_y = v_n_y * c2_rad;

	double pi_1_x = v1_x + c1_x;
	double pi_1_y = v1_y + c1_y;

	double pi_2_x = 0.0;
	double pi_2_y = 0.0;
	if(dist_sqrt < c2_rad) 
	{
		pi_2_x = v2_x + c2_x;
		pi_2_y = v2_y + c2_y;
	}
	else
	{
		pi_2_x = c1_x;
		pi_2_y = c1_y;
	}

	double p_a_x = (pi_1_x + pi_2_x) / 2;
	double p_a_y = (pi_1_y + pi_2_y) / 2;
	
	int pnt_a = add_point(p_a_x, p_a_y);
	SaPoint* p_a = (SaPoint*)get_shape(pnt_a);

	gcs_sys.addConstraintPointOnCircle(p_a->get_gcs_point(), c1->get_gcs_circle(), 1);
	if(!solve()) return false;

	gcs_sys.addConstraintPointOnCircle(p_a->get_gcs_point(), c2->get_gcs_circle(), 1);
	if(!solve()) return false;

	// placing geometry Pb
	c1_rad = *(c1->get_gcs_circle().rad);
	c1_x = *(c1->get_gcs_circle().center.x);
	c1_y = *(c1->get_gcs_circle().center.y);
	
	c2_rad = *(c2->get_gcs_circle().rad);
	c2_x = *(c2->get_gcs_circle().center.x);
	c2_y = *(c2->get_gcs_circle().center.y);	
	
	dist = ((c2_x - c1_x) * (c2_x - c1_x)) + ((c2_y - c1_y) * (c2_y - c1_y));
	rad_sum = (c1_rad + c2_rad) * (c1_rad + c2_rad);
	rad_diff = (c1_rad - c2_rad) * (c1_rad - c2_rad);
	dist_sqrt = std::sqrt(dist);
	
	double p_b_x = 2 * c2_x - p_a_x;
	double p_b_y = 2 * c2_y - p_a_y;

	int pnt_b = add_point(p_b_x, p_b_y);
	SaPoint* p_b = (SaPoint*)get_shape(pnt_b);

	gcs_sys.addConstraintPointOnCircle(p_b->get_gcs_point(), c2->get_gcs_circle(), 1);
	if(!solve()) return false;

	gcs_sys.addConstraintPointOnLine(c2->get_gcs_circle().center, p_a->get_gcs_point(), p_b->get_gcs_point(), 1);
	if(!solve()) return false;

	// placing geometry Pc
	c1_rad = *(c1->get_gcs_circle().rad);
	c1_x = *(c1->get_gcs_circle().center.x);
	c1_y = *(c1->get_gcs_circle().center.y);
	
	c2_rad = *(c2->get_gcs_circle().rad);
	c2_x = *(c2->get_gcs_circle().center.x);
	c2_y = *(c2->get_gcs_circle().center.y);	
	
	dist = ((c2_x - c1_x) * (c2_x - c1_x)) + ((c2_y - c1_y) * (c2_y - c1_y));
	rad_sum = (c1_rad + c2_rad) * (c1_rad + c2_rad);
	rad_diff = (c1_rad - c2_rad) * (c1_rad - c2_rad);
	dist_sqrt = std::sqrt(dist);
	
//	double p_c_x = 2 * c1_x - p_a_x;
//	double p_c_y = 2 * c1_y - p_a_y;
    
    double p_c_x = 0;
    double p_c_y = 0;
    
    if(c1_rad < c2_rad)
    {
        p_c_x = c1_x + v_n_x * -1 * c1_rad;
        p_c_y = c1_y + v_n_y * -1 * c1_rad;
    } else
    {
        p_c_x = c2_x + v_n_x * -1 * c2_rad * 0.95;
        p_c_y = c2_y + v_n_y * -1 * c2_rad * 0.95;
        
    }

	int pnt_c = add_point(p_c_x, p_c_y);
	SaPoint* p_c = (SaPoint*)get_shape(pnt_c);

	gcs_sys.addConstraintPointOnCircle(p_c->get_gcs_point(), c1->get_gcs_circle(), 1);
	if(!solve()) return false;

	gcs_sys.addConstraintPointOnLine(c1->get_gcs_circle().center, p_a->get_gcs_point(), p_c->get_gcs_point(), 1);
	if(!solve()) return false;

	// placing geometry Pd
	c1_rad = *(c1->get_gcs_circle().rad);
	c1_x = *(c1->get_gcs_circle().center.x);
	c1_y = *(c1->get_gcs_circle().center.y);
	
	c2_rad = *(c2->get_gcs_circle().rad);
	c2_x = *(c2->get_gcs_circle().center.x);
	c2_y = *(c2->get_gcs_circle().center.y);	
	
	dist = ((c2_x - c1_x) * (c2_x - c1_x)) + ((c2_y - c1_y) * (c2_y - c1_y));
	rad_sum = (c1_rad + c2_rad) * (c1_rad + c2_rad);
	rad_diff = (c1_rad - c2_rad) * (c1_rad - c2_rad);
	dist_sqrt = std::sqrt(dist);

	p_a_x = *(p_a->get_gcs_point().x);
	p_a_y = *(p_a->get_gcs_point().y);
	p_c_x = *(p_c->get_gcs_point().x);
	p_c_y = *(p_c->get_gcs_point().y);

	double v_d_x = p_c_x - p_a_x;
	double v_d_y = p_c_y - p_a_y;

	// for perpendicular
	double v_d_p_x = -1 * v_d_y;
	double v_d_p_y = v_d_x;

	// for normalized
	double v_d_n_x = v_d_p_x / (2 * c1_rad);
	double v_d_n_y = v_d_p_y / (2 * c1_rad);

	double dist_pc_p2 = ((p_c_x - c2_x) * (p_c_x - c2_x)) + ((p_c_y - c2_y) * (p_c_y - c2_y));
	double perp_magn = std::sqrt((c2_rad * c2_rad) - dist_pc_p2);

	double p_d_x = v_d_n_x * perp_magn + p_c_x;
	double p_d_y = v_d_n_y * perp_magn + p_c_y;

	// adding point Pd
	int pnt_d = add_point(p_d_x, p_d_y);
	SaPoint* p_d = (SaPoint*)get_shape(pnt_d);

	gcs_sys.addConstraintPointOnCircle(p_d->get_gcs_point(), c2->get_gcs_circle(), 1);
	if(!solve()) return false;

	gcs_sys.addConstraintPerpendicular(p_a->get_gcs_point(), p_c->get_gcs_point(), p_c->get_gcs_point(), p_d->get_gcs_point(), 1);
	if(!solve()) return false;

	return true;
}
/*
bool GCSWrapper::ntpp(int id1, int id2)
{
	SaCircle* c1 = (SaCircle*)get_shape(id1);
 	SaCircle* c2 = (SaCircle*)get_shape(id2);

	double c1_rad = *(c1->get_gcs_circle().rad);
	double c1_x = *(c1->get_gcs_circle().center.x);
	double c1_y = *(c1->get_gcs_circle().center.y);
	
	double c2_rad = *(c2->get_gcs_circle().rad);
	double c2_x = *(c2->get_gcs_circle().center.x);
	double c2_y = *(c2->get_gcs_circle().center.y);	
	
	double dist = ((c2_x - c1_x) * (c2_x - c1_x)) + ((c2_y - c1_y) * (c2_y - c1_y));
	double rad_sum = (c1_rad + c2_rad) * (c1_rad + c2_rad);
	double rad_diff = (c1_rad - c2_rad) * (c1_rad - c2_rad);
	double dist_sqrt = std::sqrt(dist);
	
	// adding geometry Pa
	// vector code
	double v_x = c1_x - c2_x;
	double v_y = c1_y - c2_y;

	double v_n_x = v_x / dist_sqrt;
	double v_n_y = v_y / dist_sqrt;

	if(dist == 0)
	{
		v_n_x = -1;
		v_n_y = 0;
	}

	double v1_x = v_n_x * c1_rad;
	double v1_y = v_n_y * c1_rad;

	double v2_x = v_n_x * c2_rad;
	double v2_y = v_n_y * c2_rad;

	double pi_1_x = v1_x + c1_x;
	double pi_1_y = v1_y + c1_y;

	double pi_2_x = 0.0;
	double pi_2_y = 0.0;
	
	if(dist_sqrt < c2_rad) 
	{
		pi_2_x = c1_x;
		pi_2_y = c1_y;

		//pi_2_x = v2_x + c2_x;
		//pi_2_y = v2_y + c2_y;
	}
	else
	{
		//pi_2_x = c1_x;
		//pi_2_y = c1_y;

		//pi_2_x = v2_x + c2_x;
		//pi_2_y = v2_y + c2_y;

		pi_2_x = v2_x + v1_x;
		pi_2_y = v2_y + v1_y;
	}
	
	double p_a_x = (pi_1_x + pi_2_x) / 2;
	double p_a_y = (pi_1_y + pi_2_y) / 2;
	
	int pnt_a = add_point(p_a_x, p_a_y);
	SaPoint* p_a = (SaPoint*)get_shape(pnt_a);

	gcs_sys.addConstraintPointOnCircle(p_a->get_gcs_point(), c1->get_gcs_circle(), 1);
	if(!solve()) return false;
*/
	//gcs_sys.addConstraintPointOnCircle(p_a->get_gcs_point(), c2->get_gcs_circle(), 1);
	//if(!solve()) return false;

	// placing geometry Pb
	/*c1_rad = *(c1->get_gcs_circle().rad);
	c1_x = *(c1->get_gcs_circle().center.x);
	c1_y = *(c1->get_gcs_circle().center.y);
	
	c2_rad = *(c2->get_gcs_circle().rad);
	c2_x = *(c2->get_gcs_circle().center.x);
	c2_y = *(c2->get_gcs_circle().center.y);	
	
	dist = ((c2_x - c1_x) * (c2_x - c1_x)) + ((c2_y - c1_y) * (c2_y - c1_y));
	rad_sum = (c1_rad + c2_rad) * (c1_rad + c2_rad);
	rad_diff = (c1_rad - c2_rad) * (c1_rad - c2_rad);
	dist_sqrt = std::sqrt(dist);
	
	double p_b_x = 2 * c2_x - p_a_x;
	double p_b_y = 2 * c2_y - p_a_y;

	int pnt_b = add_point(p_b_x, p_b_y);
	SaPoint* p_b = (SaPoint*)get_shape(pnt_b);

	gcs_sys.addConstraintPointOnCircle(p_b->get_gcs_point(), c2->get_gcs_circle(), 1);
	if(!solve()) return false;

	gcs_sys.addConstraintPointOnLine(c2->get_gcs_circle().center, p_a->get_gcs_point(), p_b->get_gcs_point(), 1);
	if(!solve()) return false;

	// placing geometry Pc
	c1_rad = *(c1->get_gcs_circle().rad);
	c1_x = *(c1->get_gcs_circle().center.x);
	c1_y = *(c1->get_gcs_circle().center.y);
	
	c2_rad = *(c2->get_gcs_circle().rad);
	c2_x = *(c2->get_gcs_circle().center.x);
	c2_y = *(c2->get_gcs_circle().center.y);	
	
	dist = ((c2_x - c1_x) * (c2_x - c1_x)) + ((c2_y - c1_y) * (c2_y - c1_y));
	rad_sum = (c1_rad + c2_rad) * (c1_rad + c2_rad);
	rad_diff = (c1_rad - c2_rad) * (c1_rad - c2_rad);
	dist_sqrt = std::sqrt(dist);
	
	double p_c_x = 2 * c1_x - p_a_x;
	double p_c_y = 2 * c1_y - p_a_y;

	int pnt_c = add_point(p_c_x, p_c_y);
	SaPoint* p_c = (SaPoint*)get_shape(pnt_c);

	gcs_sys.addConstraintPointOnCircle(p_c->get_gcs_point(), c1->get_gcs_circle(), 1);
	if(!solve()) return false;

	gcs_sys.addConstraintPointOnLine(c1->get_gcs_circle().center, p_a->get_gcs_point(), p_c->get_gcs_point(), 1);
	if(!solve()) return false;

	// placing geometry Pd
	c1_rad = *(c1->get_gcs_circle().rad);
	c1_x = *(c1->get_gcs_circle().center.x);
	c1_y = *(c1->get_gcs_circle().center.y);
	
	c2_rad = *(c2->get_gcs_circle().rad);
	c2_x = *(c2->get_gcs_circle().center.x);
	c2_y = *(c2->get_gcs_circle().center.y);	
	
	dist = ((c2_x - c1_x) * (c2_x - c1_x)) + ((c2_y - c1_y) * (c2_y - c1_y));
	rad_sum = (c1_rad + c2_rad) * (c1_rad + c2_rad);
	rad_diff = (c1_rad - c2_rad) * (c1_rad - c2_rad);
	dist_sqrt = std::sqrt(dist);

	p_a_x = *(p_a->get_gcs_point().x);
	p_a_y = *(p_a->get_gcs_point().y);
	p_c_x = *(p_c->get_gcs_point().x);
	p_c_y = *(p_c->get_gcs_point().y);

	double v_d_x = p_c_x - p_a_x;
	double v_d_y = p_c_y - p_a_y;

	// for perpendicular
	double v_d_p_x = -1 * v_d_y;
	double v_d_p_y = v_d_x;

	// for normalized
	double v_d_n_x = v_d_p_x / (2 * c1_rad);
	double v_d_n_y = v_d_p_y / (2 * c1_rad);

	double dist_pc_p2 = ((p_c_x - c2_x) * (p_c_x - c2_x)) + ((p_c_y - c2_y) * (p_c_y - c2_y));
	double perp_magn = std::sqrt((c2_rad * c2_rad) - dist_pc_p2);

	double p_d_x = v_d_n_x * perp_magn + p_c_x;
	double p_d_y = v_d_n_y * perp_magn + p_c_y;

	// adding point Pd
	int pnt_d = add_point(p_d_x, p_d_y);
	SaPoint* p_d = (SaPoint*)get_shape(pnt_d);

	gcs_sys.addConstraintPointOnCircle(p_d->get_gcs_point(), c2->get_gcs_circle(), 1);
	if(!solve()) return false;

	gcs_sys.addConstraintPerpendicular(p_a->get_gcs_point(), p_c->get_gcs_point(), p_c->get_gcs_point(), p_d->get_gcs_point(), 1);
	if(!solve()) return false;
	*/
/*
	return true;
}*/

// TPP with Carl on 10 November 2015
//bool GCSWrapper::tpp(int id1, int id2)
//{
//	SaCircle* c1 = (SaCircle*)get_shape(id1);
// 	SaCircle* c2 = (SaCircle*)get_shape(id2);
//
//	double c1_rad = *(c1->get_gcs_circle().rad);
//	double c1_x = *(c1->get_gcs_circle().center.x);
//	double c1_y = *(c1->get_gcs_circle().center.y);
//	
//	double c2_rad = *(c2->get_gcs_circle().rad);
//	double c2_x = *(c2->get_gcs_circle().center.x);
//	double c2_y = *(c2->get_gcs_circle().center.y);	
//	
//	double dist = ((c2_x - c1_x) * (c2_x - c1_x)) + ((c2_y - c1_y) * (c2_y - c1_y));
//	double rad_sum = (c1_rad + c2_rad) * (c1_rad + c2_rad);
//	double rad_diff = (c1_rad - c2_rad) * (c1_rad - c2_rad);
//	double dist_sqrt = std::sqrt(dist);
//
//	if(calculate_is_tpp(c1_rad, c1_x, c1_y, c2_rad, c2_x, c2_y, dist, rad_diff, rad_sum))
//	{
//		// vector code
//		double v_x = c1_x - c2_x;
//		double v_y = c1_y - c2_y;
//
//		double v_n_x = v_x / dist_sqrt;
//		double v_n_y = v_y / dist_sqrt;
//
//		double v2_x = v_n_x * c2_rad;
//		double v2_y = v_n_y * c2_rad;
//
//		double p_a_x = c2_x + v2_x;
//		double p_a_y = c2_y + v2_y;
//
//		int pnt_a = add_point(p_a_x, p_a_y);
//		SaPoint* p_a = (SaPoint*)get_shape(pnt_a);
//
//		gcs_sys.addConstraintPointOnCircle(p_a->get_gcs_point(), c1->get_gcs_circle(), 1);
//		if(!solve()) return false;
//
//		gcs_sys.addConstraintPointOnCircle(p_a->get_gcs_point(), c2->get_gcs_circle(), 1);
//		if(!solve()) return false;
//	}
//	else if(calculate_is_ntpp(c1_rad, c1_x, c1_y, c2_rad, c2_x, c2_y, dist, rad_diff, rad_sum)
//		|| calculate_is_eq(c1_rad, c1_x, c1_y, c2_rad, c2_x, c2_y, dist, rad_diff, rad_sum))
//	{
//		// vector code
//		double v_x = c1_x - c2_x;
//		double v_y = c1_y - c2_y;
//
//		double v_n_x = v_x / dist_sqrt;
//		double v_n_y = v_y / dist_sqrt;
//
//		if(dist == 0)
//		{
//			v_n_x = -1;
//			v_n_y = 0;
//		}
//
//		double v1_x = v_n_x * c1_rad;
//		double v1_y = v_n_y * c1_rad;
//
//		double v2_x = v_n_x * c2_rad;
//		double v2_y = v_n_y * c2_rad;
//
//		double pi_1_x = v1_x + c1_x;
//		double pi_1_y = v1_y + c1_y;
//
//		double pi_2_x = v2_x + c2_x;
//		double pi_2_y = v2_y + c2_y;
//
//		double p_a_x = (pi_1_x + pi_2_x) / 2;
//		double p_a_y = (pi_1_y + pi_2_y) / 2;
//		
//		int pnt_a = add_point(p_a_x, p_a_y);
//		SaPoint* p_a = (SaPoint*)get_shape(pnt_a);
//
//		gcs_sys.addConstraintPointOnCircle(p_a->get_gcs_point(), c1->get_gcs_circle(), 1);
//		if(!solve()) return false;
//
//		gcs_sys.addConstraintPointOnCircle(p_a->get_gcs_point(), c2->get_gcs_circle(), 1);
//		if(!solve()) return false;
//	}
//	else if(calculate_is_po(c1_rad, c1_x, c1_y, c2_rad, c2_x, c2_y, dist, rad_diff, rad_sum))
//	{
//		// vector code
//		double v_x = c1_x - c2_x;
//		double v_y = c1_y - c2_y;
//
//		double v_n_x = v_x / dist_sqrt;
//		double v_n_y = v_y / dist_sqrt;		
//
//		double v1_x = v_n_x * c1_rad;
//		double v1_y = v_n_y * c1_rad;
//
//		double v2_x = v_n_x * c2_rad;
//		double v2_y = v_n_y * c2_rad;
//
//		double pi_1_x = v1_x + c1_x;
//		double pi_1_y = v1_y + c1_y;
//
//		double pi_2_x = v2_x + c2_x;
//		double pi_2_y = v2_y + c2_y;
//
//		double p_a_x = (pi_1_x + pi_2_x) / 2;
//		double p_a_y = (pi_1_y + pi_2_y) / 2;
//		
//		int pnt_a = add_point(p_a_x, p_a_y);
//		SaPoint* p_a = (SaPoint*)get_shape(pnt_a);
//
//		gcs_sys.addConstraintPointOnCircle(p_a->get_gcs_point(), c1->get_gcs_circle(), 1);
//		if(!solve()) return false;
//
//		gcs_sys.addConstraintPointOnCircle(p_a->get_gcs_point(), c2->get_gcs_circle(), 1);
//		if(!solve()) return false;
//	}
//
//	return true;
//}

bool GCSWrapper::non_tangential_proper_part(int id1, int id2)	// id1 = circle1, id2 = circle2
{
	SaCircle* c1 = (SaCircle*)get_shape(id1);	// inner circle
	SaCircle* c2 = (SaCircle*)get_shape(id2);	// outer circle

	double c1_x1 = (*(c1->get_gcs_circle().center.x) - *(c1->get_gcs_circle().rad));
	double c1_x2 = (*(c1->get_gcs_circle().center.x) + *(c1->get_gcs_circle().rad));
	double c1_y = *(c1->get_gcs_circle().center.y);

	double c2_x1 = (*(c2->get_gcs_circle().center.x) - *(c2->get_gcs_circle().rad));
	double c2_x2 = (*(c2->get_gcs_circle().center.x) + *(c2->get_gcs_circle().rad));
	double c2_y = *(c2->get_gcs_circle().center.y);

	int c1_p1 = add_point(c1_x1, c1_y);
	int c1_p2 = add_point(c1_x2, c1_y);

	int c2_p1 = add_point(c2_x1, c2_y);
	int c2_p2 = add_point(c2_x2, c2_y);

	SaPoint* c = (SaPoint*) get_shape(c1_p1);
	SaPoint* d = (SaPoint*) get_shape(c1_p2);

	// L(c,d)
	int l_cd = add_segment(c, d);

	// brace(L(c,d), c1)
	brace(l_cd, id1);
	if(!solve()) return false;

	SaPoint* a = (SaPoint*) get_shape(c2_p1);
	SaPoint* b = (SaPoint*) get_shape(c2_p2);
	
	// L(a,b)
	int l_ab = add_segment(a, b);
	
	// brace(L(a,b), c2)
	brace(l_ab, id2);
	if(!solve()) return false;

	coincident_line_line(l_cd, l_ab);
	if(!solve()) return false;

	return true;
}

bool GCSWrapper::ntpp(int id1, int id2)
{
	SaCircle* c1 = (SaCircle*)get_shape(id1);
 	SaCircle* c2 = (SaCircle*)get_shape(id2);

	double c1_rad = *(c1->get_gcs_circle().rad);
	double c1_x = *(c1->get_gcs_circle().center.x);
	double c1_y = *(c1->get_gcs_circle().center.y);
	
	double c2_rad = *(c2->get_gcs_circle().rad);
	double c2_x = *(c2->get_gcs_circle().center.x);
	double c2_y = *(c2->get_gcs_circle().center.y);	
	
	double dist = ((c2_x - c1_x) * (c2_x - c1_x)) + ((c2_y - c1_y) * (c2_y - c1_y));
	double rad_sum = (c1_rad + c2_rad) * (c1_rad + c2_rad);
	double rad_diff = (c1_rad - c2_rad) * (c1_rad - c2_rad);
	double dist_sqrt = std::sqrt(dist);

	if((calculate_is_tpp(c1_rad, c1_x, c1_y, c2_rad, c2_x, c2_y, dist, rad_diff, rad_sum))
		|| (calculate_is_ntpp(c1_rad, c1_x, c1_y, c2_rad, c2_x, c2_y, dist, rad_diff, rad_sum))
		|| (calculate_is_eq(c1_rad, c1_x, c1_y, c2_rad, c2_x, c2_y, dist, rad_diff, rad_sum))
		)
	{
		double c_avg_x = (c1_x + c2_x) / 2;
		double c_avg_y = (c1_y + c2_y) / 2;

		int p = add_point(c_avg_x, c_avg_y);
		SaPoint* p_cen = (SaPoint*)get_shape(p);

		gcs_sys.addConstraintP2PCoincident(p_cen->get_gcs_point(), c1->get_gcs_circle().center, 1);
		if(!solve()) return false;

		// Placing geometry a
		c1_rad = *(c1->get_gcs_circle().rad);
		c1_x = *(c1->get_gcs_circle().center.x);
		c1_y = *(c1->get_gcs_circle().center.y);
	
		c2_rad = *(c2->get_gcs_circle().rad);
		c2_x = *(c2->get_gcs_circle().center.x);
		c2_y = *(c2->get_gcs_circle().center.y);	
	
		dist = ((c2_x - c1_x) * (c2_x - c1_x)) + ((c2_y - c1_y) * (c2_y - c1_y));
		rad_sum = (c1_rad + c2_rad) * (c1_rad + c2_rad);
		rad_diff = (c1_rad - c2_rad) * (c1_rad - c2_rad);
		dist_sqrt = std::sqrt(dist);		

		double v_x = c1_x - c2_x;
		double v_y = c1_y - c2_y;

		double v_n_x = v_x / dist_sqrt;
		double v_n_y = v_y / dist_sqrt;		

		if(dist == 0)
		{
			v_n_x = -1;
			v_n_y = 0;
		}

		double v2_x = v_n_x * c2_rad;
		double v2_y = v_n_y * c2_rad;

		double p_a_x = c2_x + v2_x;
		double p_a_y = c2_y + v2_y;
		
		int pnt_a = add_point(p_a_x, p_a_y);
		SaPoint* p_a = (SaPoint*)get_shape(pnt_a);			

		gcs_sys.addConstraintPointOnCircle(p_a->get_gcs_point(), c2->get_gcs_circle(), 1);
		if(!solve()) return false;

		gcs_sys.addConstraintPointOnLine(c2->get_gcs_circle().center, p_a->get_gcs_point(), c1->get_gcs_circle().center, 1);
		if(!solve()) return false;

		// Placing geometry c		
		c1_rad = *(c1->get_gcs_circle().rad);
		c1_x = *(c1->get_gcs_circle().center.x);
		c1_y = *(c1->get_gcs_circle().center.y);
	
		c2_rad = *(c2->get_gcs_circle().rad);
		c2_x = *(c2->get_gcs_circle().center.x);
		c2_y = *(c2->get_gcs_circle().center.y);	
	
		dist = ((c2_x - c1_x) * (c2_x - c1_x)) + ((c2_y - c1_y) * (c2_y - c1_y));
		rad_sum = (c1_rad + c2_rad) * (c1_rad + c2_rad);
		rad_diff = (c1_rad - c2_rad) * (c1_rad - c2_rad);
		dist_sqrt = std::sqrt(dist);		
		
		v_x = c1_x - c2_x;
		v_y = c1_y - c2_y;

		v_n_x = v_x / dist_sqrt;
		v_n_y = v_y / dist_sqrt;

		if(dist == 0)
		{
			v_n_x = -1;
			v_n_y = 0;
		}

		double v1_x = v_n_x * c1_rad;
		double v1_y = v_n_y * c1_rad;

		double p_c_x = c1_x + v1_x;
		double p_c_y = c1_y + v1_y;

		int pnt_c = add_point(p_c_x, p_c_y);
		SaPoint* p_c = (SaPoint*)get_shape(pnt_c);

		gcs_sys.addConstraintPointOnCircle(p_c->get_gcs_point(), c1->get_gcs_circle(), 1);
		if(!solve()) return false;

		gcs_sys.addConstraintPointOnLine(c2->get_gcs_circle().center, p_a->get_gcs_point(), p_c->get_gcs_point(), 1);
		if(!solve()) return false;

		// placing geometry Pd
		c1_rad = *(c1->get_gcs_circle().rad);
		c1_x = *(c1->get_gcs_circle().center.x);
		c1_y = *(c1->get_gcs_circle().center.y);
		
		c2_rad = *(c2->get_gcs_circle().rad);
		c2_x = *(c2->get_gcs_circle().center.x);
		c2_y = *(c2->get_gcs_circle().center.y);	
	
		dist = ((c2_x - c1_x) * (c2_x - c1_x)) + ((c2_y - c1_y) * (c2_y - c1_y));
		rad_sum = (c1_rad + c2_rad) * (c1_rad + c2_rad);
		rad_diff = (c1_rad - c2_rad) * (c1_rad - c2_rad);
		dist_sqrt = std::sqrt(dist);
	
		double p_d_x = 2 * c1_x - p_c_x;
		double p_d_y = 2 * c1_y - p_c_y;

		int pnt_d = add_point(p_d_x, p_d_y);
		SaPoint* p_d = (SaPoint*)get_shape(pnt_d);

		gcs_sys.addConstraintPointOnCircle(p_d->get_gcs_point(), c1->get_gcs_circle(), 1);
		if(!solve()) return false;

		gcs_sys.addConstraintPointOnLine(c1->get_gcs_circle().center, p_a->get_gcs_point(), p_c->get_gcs_point(), 1);
		if(!solve()) return false;

		// placing geometry Pb
		c1_rad = *(c1->get_gcs_circle().rad);
		c1_x = *(c1->get_gcs_circle().center.x);
		c1_y = *(c1->get_gcs_circle().center.y);
		
		c2_rad = *(c2->get_gcs_circle().rad);
		c2_x = *(c2->get_gcs_circle().center.x);
		c2_y = *(c2->get_gcs_circle().center.y);	
	
		dist = ((c2_x - c1_x) * (c2_x - c1_x)) + ((c2_y - c1_y) * (c2_y - c1_y));
		rad_sum = (c1_rad + c2_rad) * (c1_rad + c2_rad);
		rad_diff = (c1_rad - c2_rad) * (c1_rad - c2_rad);
		dist_sqrt = std::sqrt(dist);
	
		double p_b_x = 2 * c2_x - p_a_x;
		double p_b_y = 2 * c2_y - p_a_y;

		int pnt_b = add_point(p_b_x, p_b_y);
		SaPoint* p_b = (SaPoint*)get_shape(pnt_b);

		gcs_sys.addConstraintPointOnCircle(p_b->get_gcs_point(), c2->get_gcs_circle(), 1);
		if(!solve()) return false;

		gcs_sys.addConstraintPointOnLine(c2->get_gcs_circle().center, p_a->get_gcs_point(), p_b->get_gcs_point(), 1);
		if(!solve()) return false;

		double v_d_x = p_c_x - p_a_x;
		double v_d_y = p_c_y - p_a_y;

		// placing geometry Pe
		c1_rad = *(c1->get_gcs_circle().rad);
		c1_x = *(c1->get_gcs_circle().center.x);
		c1_y = *(c1->get_gcs_circle().center.y);
		
		c2_rad = *(c2->get_gcs_circle().rad);
		c2_x = *(c2->get_gcs_circle().center.x);
		c2_y = *(c2->get_gcs_circle().center.y);	
	
		dist = ((c2_x - c1_x) * (c2_x - c1_x)) + ((c2_y - c1_y) * (c2_y - c1_y));
		rad_sum = (c1_rad + c2_rad) * (c1_rad + c2_rad);
		rad_diff = (c1_rad - c2_rad) * (c1_rad - c2_rad);
		dist_sqrt = std::sqrt(dist);

		p_a_x = *(p_a->get_gcs_point().x);
		p_a_y = *(p_a->get_gcs_point().y);
		p_c_x = *(p_c->get_gcs_point().x);
		p_c_y = *(p_c->get_gcs_point().y);

		double v_e_x = p_d_x - p_c_x;
		double v_e_y = p_d_y - p_c_y;

		// for perpendicular
		double v_e_p_x = -1 * v_e_y;
		double v_e_p_y = v_e_x;

		// for normalized
		double v_e_n_x = v_e_p_x / (2 * c1_rad);
		double v_e_n_y = v_e_p_y / (2 * c1_rad);

		double dist_pc_p2 = ((p_c_x - c2_x) * (p_c_x - c2_x)) + ((p_c_y - c2_y) * (p_c_y - c2_y));
		double perp_magn = std::sqrt((c2_rad * c2_rad) - dist_pc_p2);

		double p_e_x = v_e_n_x * perp_magn + p_c_x;
		double p_e_y = v_e_n_y * perp_magn + p_c_y;

		// adding point Pe
		int pnt_e = add_point(p_e_x, p_e_y);
		SaPoint* p_e = (SaPoint*)get_shape(pnt_e);

		gcs_sys.addConstraintPointOnCircle(p_e->get_gcs_point(), c2->get_gcs_circle(), 1);
		if(!solve()) return false;

		gcs_sys.addConstraintPerpendicular(p_a->get_gcs_point(), p_c->get_gcs_point(), p_c->get_gcs_point(), p_e->get_gcs_point(), 1);
		if(!solve()) return false;

		// placing geometry Pf
		c1_rad = *(c1->get_gcs_circle().rad);
		c1_x = *(c1->get_gcs_circle().center.x);
		c1_y = *(c1->get_gcs_circle().center.y);
		
		c2_rad = *(c2->get_gcs_circle().rad);
		c2_x = *(c2->get_gcs_circle().center.x);
		c2_y = *(c2->get_gcs_circle().center.y);	
	
		dist = ((c2_x - c1_x) * (c2_x - c1_x)) + ((c2_y - c1_y) * (c2_y - c1_y));
		rad_sum = (c1_rad + c2_rad) * (c1_rad + c2_rad);
		rad_diff = (c1_rad - c2_rad) * (c1_rad - c2_rad);
		dist_sqrt = std::sqrt(dist);

		p_a_x = *(p_a->get_gcs_point().x);
		p_a_y = *(p_a->get_gcs_point().y);
		p_b_x = *(p_b->get_gcs_point().x);
		p_b_y = *(p_b->get_gcs_point().y);

		p_c_x = *(p_c->get_gcs_point().x);
		p_c_y = *(p_c->get_gcs_point().y);		
		p_d_x = *(p_d->get_gcs_point().x);
		p_d_y = *(p_d->get_gcs_point().y);

		double v_f_x = p_d_x - p_c_x;
		double v_f_y = p_d_y - p_c_y;

		// for perpendicular
		double v_f_p_x = -1 * v_f_y;
		double v_f_p_y = v_f_x;

		// for normalized
		double v_f_n_x = v_f_p_x / (2 * c1_rad);
		double v_f_n_y = v_f_p_y / (2 * c1_rad);
		
		double dist_pd_p2 = ((p_d_x - c2_x) * (p_d_x - c2_x)) + ((p_d_y - c2_y) * (p_d_y - c2_y));
		double perp_magn_pf = std::sqrt((c2_rad * c2_rad) - dist_pd_p2);
		
		double p_f_x = v_f_n_x * perp_magn_pf + p_d_x;
		double p_f_y = v_f_n_y * perp_magn_pf + p_d_y;

		// adding point Pf
		int pnt_f = add_point(p_f_x, p_f_y);
		SaPoint* p_f = (SaPoint*)get_shape(pnt_f);

		gcs_sys.addConstraintPointOnCircle(p_f->get_gcs_point(), c2->get_gcs_circle(), 1);
		if(!solve()) return false;

		gcs_sys.addConstraintPerpendicular(p_d->get_gcs_point(), p_b->get_gcs_point(), p_d->get_gcs_point(), p_f->get_gcs_point(), 1);
		if(!solve()) return false;
	}

	else if((calculate_is_po(c1_rad, c1_x, c1_y, c2_rad, c2_x, c2_y, dist, rad_diff, rad_sum))
		|| (calculate_is_ec(c1_rad, c1_x, c1_y, c2_rad, c2_x, c2_y, dist, rad_diff, rad_sum))
		|| (calculate_is_dc(c1_rad, c1_x, c1_y, c2_rad, c2_x, c2_y, dist, rad_diff, rad_sum)))
	{
		double c_avg_x = (c1_x + c2_x) / 2;
		double c_avg_y = (c1_y + c2_y) / 2;

		int p = add_point(c_avg_x, c_avg_y);
		SaPoint* p_cen = (SaPoint*)get_shape(p);

		gcs_sys.addConstraintP2PCoincident(p_cen->get_gcs_point(), c1->get_gcs_circle().center, 1);
		if(!solve()) return false;

		gcs_sys.addConstraintP2PCoincident(p_cen->get_gcs_point(), c2->get_gcs_circle().center, 1);
		if(!solve()) return false;

		// Placing geometry a
		c1_rad = *(c1->get_gcs_circle().rad);
		c1_x = *(c1->get_gcs_circle().center.x);
		c1_y = *(c1->get_gcs_circle().center.y);
	
		c2_rad = *(c2->get_gcs_circle().rad);
		c2_x = *(c2->get_gcs_circle().center.x);
		c2_y = *(c2->get_gcs_circle().center.y);	
	
		dist = ((c2_x - c1_x) * (c2_x - c1_x)) + ((c2_y - c1_y) * (c2_y - c1_y));
		rad_sum = (c1_rad + c2_rad) * (c1_rad + c2_rad);
		rad_diff = (c1_rad - c2_rad) * (c1_rad - c2_rad);
		dist_sqrt = std::sqrt(dist);		

		double v_x = c1_x - c2_x;
		double v_y = c1_y - c2_y;

		double v_n_x = v_x / dist_sqrt;
		double v_n_y = v_y / dist_sqrt;		

		if(dist == 0)
		{
			v_n_x = -1;
			v_n_y = 0;
		}

		double v2_x = v_n_x * c2_rad;
		double v2_y = v_n_y * c2_rad;

		double p_a_x = c2_x + v2_x;
		double p_a_y = c2_y + v2_y;
		
		int pnt_a = add_point(p_a_x, p_a_y);
		SaPoint* p_a = (SaPoint*)get_shape(pnt_a);			

		gcs_sys.addConstraintPointOnCircle(p_a->get_gcs_point(), c2->get_gcs_circle(), 1);
		if(!solve()) return false;

		gcs_sys.addConstraintPointOnLine(c2->get_gcs_circle().center, p_a->get_gcs_point(), c1->get_gcs_circle().center, 1);
		if(!solve()) return false;

		// Placing geometry c		
		c1_rad = *(c1->get_gcs_circle().rad);
		c1_x = *(c1->get_gcs_circle().center.x);
		c1_y = *(c1->get_gcs_circle().center.y);
	
		c2_rad = *(c2->get_gcs_circle().rad);
		c2_x = *(c2->get_gcs_circle().center.x);
		c2_y = *(c2->get_gcs_circle().center.y);	
	
		dist = ((c2_x - c1_x) * (c2_x - c1_x)) + ((c2_y - c1_y) * (c2_y - c1_y));
		rad_sum = (c1_rad + c2_rad) * (c1_rad + c2_rad);
		rad_diff = (c1_rad - c2_rad) * (c1_rad - c2_rad);
		dist_sqrt = std::sqrt(dist);		
		
		v_x = c1_x - c2_x;
		v_y = c1_y - c2_y;

		v_n_x = v_x / dist_sqrt;
		v_n_y = v_y / dist_sqrt;

		if(dist == 0)
		{
			v_n_x = -1;
			v_n_y = 0;
		}

		double v1_x = v_n_x * c1_rad;
		double v1_y = v_n_y * c1_rad;

		double p_c_x = c1_x + v1_x;
		double p_c_y = c1_y + v1_y;

		int pnt_c = add_point(p_c_x, p_c_y);
		SaPoint* p_c = (SaPoint*)get_shape(pnt_c);

		gcs_sys.addConstraintPointOnCircle(p_c->get_gcs_point(), c1->get_gcs_circle(), 1);
		if(!solve()) return false;

		gcs_sys.addConstraintPointOnLine(c2->get_gcs_circle().center, p_a->get_gcs_point(), p_c->get_gcs_point(), 1);
		if(!solve()) return false;

		// placing geometry Pd
		c1_rad = *(c1->get_gcs_circle().rad);
		c1_x = *(c1->get_gcs_circle().center.x);
		c1_y = *(c1->get_gcs_circle().center.y);
		
		c2_rad = *(c2->get_gcs_circle().rad);
		c2_x = *(c2->get_gcs_circle().center.x);
		c2_y = *(c2->get_gcs_circle().center.y);	
	
		dist = ((c2_x - c1_x) * (c2_x - c1_x)) + ((c2_y - c1_y) * (c2_y - c1_y));
		rad_sum = (c1_rad + c2_rad) * (c1_rad + c2_rad);
		rad_diff = (c1_rad - c2_rad) * (c1_rad - c2_rad);
		dist_sqrt = std::sqrt(dist);
	
		double p_d_x = 2 * c1_x - p_c_x;
		double p_d_y = 2 * c1_y - p_c_y;

		// adding point d
		int pnt_d = add_point(p_d_x, p_d_y);
		SaPoint* p_d = (SaPoint*)get_shape(pnt_d);

		gcs_sys.addConstraintPointOnCircle(p_d->get_gcs_point(), c1->get_gcs_circle(), 1);
		if(!solve()) return false;

		gcs_sys.addConstraintPointOnLine(c1->get_gcs_circle().center, p_a->get_gcs_point(), p_c->get_gcs_point(), 1);
		if(!solve()) return false;

		// placing geometry Pb
		c1_rad = *(c1->get_gcs_circle().rad);
		c1_x = *(c1->get_gcs_circle().center.x);
		c1_y = *(c1->get_gcs_circle().center.y);
		
		c2_rad = *(c2->get_gcs_circle().rad);
		c2_x = *(c2->get_gcs_circle().center.x);
		c2_y = *(c2->get_gcs_circle().center.y);	
	
		dist = ((c2_x - c1_x) * (c2_x - c1_x)) + ((c2_y - c1_y) * (c2_y - c1_y));
		rad_sum = (c1_rad + c2_rad) * (c1_rad + c2_rad);
		rad_diff = (c1_rad - c2_rad) * (c1_rad - c2_rad);
		dist_sqrt = std::sqrt(dist);
	
		double p_b_x = 2 * c2_x - p_a_x;
		double p_b_y = 2 * c2_y - p_a_y;

		int pnt_b = add_point(p_b_x, p_b_y);
		SaPoint* p_b = (SaPoint*)get_shape(pnt_b);

		gcs_sys.addConstraintPointOnCircle(p_b->get_gcs_point(), c2->get_gcs_circle(), 1);
		if(!solve()) return false;

		gcs_sys.addConstraintPointOnLine(c2->get_gcs_circle().center, p_a->get_gcs_point(), p_b->get_gcs_point(), 1);
		if(!solve()) return false;

		double v_d_x = p_c_x - p_a_x;
		double v_d_y = p_c_y - p_a_y;

		// placing geometry Pe
		c1_rad = *(c1->get_gcs_circle().rad);
		c1_x = *(c1->get_gcs_circle().center.x);
		c1_y = *(c1->get_gcs_circle().center.y);
		
		c2_rad = *(c2->get_gcs_circle().rad);
		c2_x = *(c2->get_gcs_circle().center.x);
		c2_y = *(c2->get_gcs_circle().center.y);	
	
		dist = ((c2_x - c1_x) * (c2_x - c1_x)) + ((c2_y - c1_y) * (c2_y - c1_y));
		rad_sum = (c1_rad + c2_rad) * (c1_rad + c2_rad);
		rad_diff = (c1_rad - c2_rad) * (c1_rad - c2_rad);
		dist_sqrt = std::sqrt(dist);

		p_a_x = *(p_a->get_gcs_point().x);
		p_a_y = *(p_a->get_gcs_point().y);
		p_c_x = *(p_c->get_gcs_point().x);
		p_c_y = *(p_c->get_gcs_point().y);

		double v_e_x = p_d_x - p_c_x;
		double v_e_y = p_d_y - p_c_y;

		// for perpendicular
		double v_e_p_x = -1 * v_e_y;
		double v_e_p_y = v_e_x;

		// for normalized
		double v_e_n_x = v_e_p_x / (2 * c1_rad);
		double v_e_n_y = v_e_p_y / (2 * c1_rad);

		double dist_pc_p2 = ((p_c_x - c2_x) * (p_c_x - c2_x)) + ((p_c_y - c2_y) * (p_c_y - c2_y));
		double perp_magn = std::sqrt((c2_rad * c2_rad) - dist_pc_p2);

		double p_e_x = v_e_n_x * perp_magn + p_c_x;
		double p_e_y = v_e_n_y * perp_magn + p_c_y;

		// adding point Pe
		int pnt_e = add_point(p_e_x, p_e_y);
		SaPoint* p_e = (SaPoint*)get_shape(pnt_e);

		gcs_sys.addConstraintPointOnCircle(p_e->get_gcs_point(), c2->get_gcs_circle(), 1);
		if(!solve()) return false;

		gcs_sys.addConstraintPerpendicular(p_a->get_gcs_point(), p_c->get_gcs_point(), p_c->get_gcs_point(), p_e->get_gcs_point(), 1);
		if(!solve()) return false;

		// placing geometry Pf
		c1_rad = *(c1->get_gcs_circle().rad);
		c1_x = *(c1->get_gcs_circle().center.x);
		c1_y = *(c1->get_gcs_circle().center.y);
		
		c2_rad = *(c2->get_gcs_circle().rad);
		c2_x = *(c2->get_gcs_circle().center.x);
		c2_y = *(c2->get_gcs_circle().center.y);	
	
		dist = ((c2_x - c1_x) * (c2_x - c1_x)) + ((c2_y - c1_y) * (c2_y - c1_y));
		rad_sum = (c1_rad + c2_rad) * (c1_rad + c2_rad);
		rad_diff = (c1_rad - c2_rad) * (c1_rad - c2_rad);
		dist_sqrt = std::sqrt(dist);

		p_a_x = *(p_a->get_gcs_point().x);
		p_a_y = *(p_a->get_gcs_point().y);
		p_b_x = *(p_b->get_gcs_point().x);
		p_b_y = *(p_b->get_gcs_point().y);

		p_c_x = *(p_c->get_gcs_point().x);
		p_c_y = *(p_c->get_gcs_point().y);		
		p_d_x = *(p_d->get_gcs_point().x);
		p_d_y = *(p_d->get_gcs_point().y);

		double v_f_x = p_d_x - p_c_x;
		double v_f_y = p_d_y - p_c_y;

		// for perpendicular
		double v_f_p_x = -1 * v_f_y;
		double v_f_p_y = v_f_x;

		// for normalized
		double v_f_n_x = v_f_p_x / (2 * c1_rad);
		double v_f_n_y = v_f_p_y / (2 * c1_rad);
		
		double dist_pd_p2 = ((p_d_x - c2_x) * (p_d_x - c2_x)) + ((p_d_y - c2_y) * (p_d_y - c2_y));
		double perp_magn_pf = std::sqrt((c2_rad * c2_rad) - dist_pd_p2);
		
		double p_f_x = v_f_n_x * perp_magn_pf + p_d_x;
		double p_f_y = v_f_n_y * perp_magn_pf + p_d_y;

		// adding point Pf
		int pnt_f = add_point(p_f_x, p_f_y);
		SaPoint* p_f = (SaPoint*)get_shape(pnt_f);

		gcs_sys.addConstraintPointOnCircle(p_f->get_gcs_point(), c2->get_gcs_circle(), 1);
		if(!solve()) return false;

		gcs_sys.addConstraintPerpendicular(p_d->get_gcs_point(), p_b->get_gcs_point(), p_d->get_gcs_point(), p_f->get_gcs_point(), 1);
		if(!solve()) return false;
	}

	return true;
}


bool GCSWrapper::part_of(int id1, int id2)	// id1 = circle1, id2 = circle2
{
	/*SaCircle* c1 = (SaCircle*)get_shape(id1);
	SaCircle* c2 = (SaCircle*)get_shape(id2);

	double c1_x1 = (*(c1->get_gcs_circle().center.x) - *(c1->get_gcs_circle().rad));
	double c1_x2 = (*(c1->get_gcs_circle().center.x) + *(c1->get_gcs_circle().rad));
	double c1_y = *(c1->get_gcs_circle().center.y);

	double c2_x1 = (*(c2->get_gcs_circle().center.x) - *(c2->get_gcs_circle().rad));
	double c2_x2 = (*(c2->get_gcs_circle().center.x) + *(c2->get_gcs_circle().rad));
	double c2_y = *(c2->get_gcs_circle().center.y);
	
	int c1_p1 = add_point(c1_x1, c1_y);
	int c1_p2 = add_point(c1_x2, c1_y);

	SaPoint* c = (SaPoint*) get_shape(c1_p1);
	SaPoint* d = (SaPoint*) get_shape(c1_p2);

	// L(c, d)
	int l_cd = add_segment(c, d);

	// brace(L(c,d), C1)
	brace(l_cd, id1);
	if(!solve()) return false;

	int c2_p1 = add_point(c2_x1, c2_y);
	int c2_p2 = add_point(c2_x2, c2_y);

	SaPoint* a = (SaPoint*) get_shape(c2_p1);
	SaPoint* b = (SaPoint*) get_shape(c2_p2);

	// L(a,b)
	int l_ab = add_segment(a, b);
	
	// brace(L(a,b), C2)
	brace(l_ab, id2);
	if(!solve()) return false;

	//coincidence(a, L(c,d))
	point_segment_coincidence(c2_p1, l_cd);
	if(!solve()) return false;

	//coincidence(d, L(a,b))
	point_segment_coincidence(c1_p2, l_ab);
	if(!solve()) return false;

	return true;*/

	SaCircle* c1 = (SaCircle*)get_shape(id1);
	SaCircle* c2 = (SaCircle*)get_shape(id2);

	double c1_rad = *(c1->get_gcs_circle().rad);
	double c1_x1 = *(c1->get_gcs_circle().center.x) - c1_rad;
	double c1_y1 = *(c1->get_gcs_circle().center.y);
	double c1_x2 = *(c1->get_gcs_circle().center.x) + c1_rad;
	double c1_y2 = *(c1->get_gcs_circle().center.y);

	double c2_rad = *(c2->get_gcs_circle().rad);
	double c2_x1 = *(c2->get_gcs_circle().center.x) - c2_rad;
	double c2_y1 = *(c2->get_gcs_circle().center.y);
	double c2_x2 = *(c2->get_gcs_circle().center.x) + c2_rad;
	double c2_y2 = *(c2->get_gcs_circle().center.y);

	int c1_l = add_segment(c1_x1, c1_y1, c1_x2, c1_y2);
	int c2_l = add_segment(c2_x1, c2_y1, c2_x2, c2_y2);

	SaLine* l1 = (SaLine*)get_shape(c1_l);

	// brace starts here 
	gcs_sys.addConstraintPointOnCircle(l1->get_gcs_line().p1, c1->get_gcs_circle(), 1);
	if(!solve()) return false;

	gcs_sys.addConstraintPointOnCircle(l1->get_gcs_line().p2, c1->get_gcs_circle(), 1);
	if(!solve()) return false;

	gcs_sys.addConstraintPointOnLine(c1->get_gcs_circle().center, l1->get_gcs_line().p1, l1->get_gcs_line().p2, 1);
	if(!solve()) return false;
	// brace ends here

	// brace starts here
	SaLine* l2 = (SaLine*)get_shape(c2_l);
	gcs_sys.addConstraintPointOnCircle(l2->get_gcs_line().p1, c2->get_gcs_circle(), 1);
	if(!solve()) return false;

	gcs_sys.addConstraintPointOnCircle(l2->get_gcs_line().p2, c2->get_gcs_circle(), 1);
	if(!solve()) return false;

	gcs_sys.addConstraintPointOnLine(c2->get_gcs_circle().center, l2->get_gcs_line().p1, l2->get_gcs_line().p2, 1);
	if(!solve()) return false;
	// brace ends here

	double l3_x = *(c2->get_gcs_circle().center.x);
	double l3_y1 = *(c2->get_gcs_circle().center.y);
	double l3_y2 = *(c2->get_gcs_circle().center.y) + *(c2->get_gcs_circle().rad);

	double l4_x = *(c1->get_gcs_circle().center.x);
	double l4_y1 = *(c1->get_gcs_circle().center.y);
	double l4_y2 = *(c1->get_gcs_circle().center.y) + *(c1->get_gcs_circle().rad);

	int line3 = add_segment(l3_x, l3_y1, l3_x, l3_y2);
	int line4 = add_segment(l4_x, l4_y1, l4_x, l4_y2);
	
	SaLine* l3 = (SaLine*)get_shape(line3);
	SaLine* l4 = (SaLine*)get_shape(line4);
		
	gcs_sys.addConstraintPerpendicular(l2->get_gcs_line(), l3->get_gcs_line(), 1);
	if(!solve()) return false;

	gcs_sys.addConstraintPerpendicular(l1->get_gcs_line(), l4->get_gcs_line(), 1);
	if(!solve()) return false;

	////
	gcs_sys.addConstraintPointOnLine(l3->get_gcs_line().p1, l2->get_gcs_line().p1, l2->get_gcs_line().p2, 1);
	if(!solve()) return false;

	gcs_sys.addConstraintPointOnCircle(l3->get_gcs_line().p2, c2->get_gcs_circle(), 1);
	if(!solve()) return false;

	// point on point
	gcs_sys.addConstraintP2PCoincident(l1->get_gcs_line().p2, l3->get_gcs_line().p1, 1);
	if(!solve()) return false;

	//////
	gcs_sys.addConstraintPointOnLine(l4->get_gcs_line().p1, l1->get_gcs_line().p1, l1->get_gcs_line().p2, 1);
	if(!solve()) return false;

	gcs_sys.addConstraintPointOnCircle(l4->get_gcs_line().p2, c1->get_gcs_circle(), 1);
	if(!solve()) return false;

	// point on point
	gcs_sys.addConstraintP2PCoincident(l2->get_gcs_line().p1, l4->get_gcs_line().p1, 1);
	if(!solve()) return false;

	// point on line
	gcs_sys.addConstraintPointOnLine(l2->get_gcs_line().p1, l1->get_gcs_line().p1, l1->get_gcs_line().p2, 1);
	if(!solve()) return false;

	gcs_sys.addConstraintPointOnLine(l1->get_gcs_line().p2, l2->get_gcs_line().p1, l2->get_gcs_line().p2, 1);
	if(!solve()) return false;	

	return true;
}

bool GCSWrapper::partially_overlap(int id1, int id2)
{
	SaCircle* c1 = (SaCircle*) get_shape(id1);
	SaCircle* c2 = (SaCircle*) get_shape(id2);

	double c1_rad = *(c1->get_gcs_circle().rad);
	double c1_x = *(c1->get_gcs_circle().center.x);
	double c1_y = *(c1->get_gcs_circle().center.y);
	
	double c2_rad = *(c2->get_gcs_circle().rad);
	double c2_x = *(c2->get_gcs_circle().center.x);
	double c2_y = *(c2->get_gcs_circle().center.y);	
	
	double dist = ((c2_x - c1_x) * (c2_x - c1_x)) + ((c2_y - c1_y) * (c2_y - c1_y));
	double rad_sum = (c1_rad + c2_rad) * (c1_rad + c2_rad);
	double rad_diff = (c1_rad - c2_rad) * (c1_rad - c2_rad);
	double dist_sqrt = std::sqrt(dist);

	/*
	if((calculate_is_tpp(c1_rad, c1_x, c1_y, c2_rad, c2_x, c2_y, dist, rad_diff, rad_sum))
		|| (calculate_is_ntpp(c1_rad, c1_x, c1_y, c2_rad, c2_x, c2_y, dist, rad_diff, rad_sum))
		|| (calculate_is_eq(c1_rad, c1_x, c1_y, c2_rad, c2_x, c2_y, dist, rad_diff, rad_sum))
		)
	{
		double c_avg_x = (c1_x + c2_x) / 2;
		double c_avg_y = (c1_y + c2_y) / 2;

		int p = add_point(c_avg_x, c_avg_y);
		SaPoint* p_cen = (SaPoint*)get_shape(p);

		gcs_sys.addConstraintP2PCoincident(p_cen->get_gcs_point(), c1->get_gcs_circle().center, 1);
		if(!solve()) return false;

		// Placing geometry a
		c1_rad = *(c1->get_gcs_circle().rad);
		c1_x = *(c1->get_gcs_circle().center.x);
		c1_y = *(c1->get_gcs_circle().center.y);
	
		c2_rad = *(c2->get_gcs_circle().rad);
		c2_x = *(c2->get_gcs_circle().center.x);
		c2_y = *(c2->get_gcs_circle().center.y);	
	
		dist = ((c2_x - c1_x) * (c2_x - c1_x)) + ((c2_y - c1_y) * (c2_y - c1_y));
		rad_sum = (c1_rad + c2_rad) * (c1_rad + c2_rad);
		rad_diff = (c1_rad - c2_rad) * (c1_rad - c2_rad);
		dist_sqrt = std::sqrt(dist);		

		double v_x = c1_x - c2_x;
		double v_y = c1_y - c2_y;

		double v_n_x = v_x / dist_sqrt;
		double v_n_y = v_y / dist_sqrt;		

		if(dist == 0)
		{
			v_n_x = -1;
			v_n_y = 0;
		}

		double v2_x = v_n_x * c2_rad;
		double v2_y = v_n_y * c2_rad;

		double p_a_x = c2_x + v2_x;
		double p_a_y = c2_y + v2_y;
		
		int pnt_a = add_point(p_a_x, p_a_y);
		SaPoint* p_a = (SaPoint*)get_shape(pnt_a);			

		gcs_sys.addConstraintPointOnCircle(p_a->get_gcs_point(), c2->get_gcs_circle(), 1);
		if(!solve()) return false;

		gcs_sys.addConstraintPointOnLine(c2->get_gcs_circle().center, p_a->get_gcs_point(), c1->get_gcs_circle().center, 1);
		if(!solve()) return false;*/

	return true;
}

bool GCSWrapper::equal(int id1, int id2)
{
	SaCircle* c1 = (SaCircle*)get_shape(id1);
	SaCircle* c2 = (SaCircle*)get_shape(id2);

	gcs_sys.addConstraintEqual(c2->get_gcs_circle().center.x, c1->get_gcs_circle().center.x, 1);
	if(!solve()) return false;

	gcs_sys.addConstraintEqual(c2->get_gcs_circle().center.y, c1->get_gcs_circle().center.y, 1);
	if(!solve()) return false;

	gcs_sys.addConstraintEqualRadius(c2->get_gcs_circle(), c1->get_gcs_circle(), 1);
	if(!solve()) return false;

	return true;
}

bool GCSWrapper::disconnected(int id1, int id2)	// id1 = circle1, id2 = circle2
{
	SaCircle* c1 = (SaCircle*)get_shape(id1);
	SaCircle* c2 = (SaCircle*)get_shape(id2);

	double c1_x1 = *(c1->get_gcs_circle().center.x);
	double c1_x2 = (*(c1->get_gcs_circle().center.x) + *(c1->get_gcs_circle().rad));
	double c1_y = *(c1->get_gcs_circle().center.y);

	double c2_x1 = *(c2->get_gcs_circle().center.x);
	double c2_x2 = (*(c2->get_gcs_circle().center.x) + *(c2->get_gcs_circle().rad));
	double c2_y = *(c2->get_gcs_circle().center.y);

	int c1_p = add_point(c1_x1, c1_y);
	int c2_p = add_point(c2_x1, c2_y);

	SaPoint* p1 = (SaPoint*) get_shape(c1_p);
	SaPoint* p2 = (SaPoint*) get_shape(c2_p);

	int l_p1p2 = add_segment(p1, p2);
	SaLine* l = (SaLine*) get_shape(l_p1p2);

	double mid_x, mid_y, len;
	// calculating mid-point of line
	calculate_line_midpoint(l, mid_x, mid_y);

	// calculating length of line
	calculate_line_length(l, len);
	
	double rad = len/2;
	int p_p3 = add_point(mid_x, mid_y);
	SaPoint* p3 = (SaPoint*) get_shape(p_p3);

	int c_c3 = add_circle(*p3->get_gcs_point().x, *p3->get_gcs_point().y, rad);
	if(!solve()) return false;

	// brace(L(p1,p2), C3)
	brace(l_p1p2, c_c3);
	if(!solve()) return false;

	int c1_a = add_point(c1_x2, c1_y);
	int c2_b = add_point(c2_x2, c2_y);

	int l_p1p3 = add_segment(p1, p3);
	int l_p2p3 = add_segment(p2, p3);

	// coincidence(a, C1)
	coincident_point_circle(c1_a, id1);
	if(!solve()) return false;

	// coincidence(a, L(p1,p3)
	point_segment_coincidence(c1_a, l_p1p3);
	if(!solve()) return false;

	// coincidence(b, C2)
	coincident_point_circle(c2_b, id2);
	if(!solve()) return false;

	// coincidence(b, L(p2,p3)
	point_segment_coincidence(c2_b, l_p2p3);
	if(!solve()) return false;

	return true;











	/*
	SaCircle* c1 = (SaCircle*)get_shape(id1);
	SaCircle* c2 = (SaCircle*)get_shape(id2);

	double x1 = *(c1->get_gcs_circle().center.x);
	double y1 = *(c1->get_gcs_circle().center.y);
	double x2 = *(c2->get_gcs_circle().center.x);
	double y2 = *(c2->get_gcs_circle().center.y);

	int l_p1p2_id = add_segment(x1, y1, x2, y2);	// l(p1, p2)

	double l1_x1, l1_y1, l1_length;
	SaLine* l_p1p2 = (SaLine*)get_shape(l_p1p2_id);

	calculate_line_midpoint(l_p1p2, l1_x1, l1_y1);
	calculate_line_length(l_p1p2, l1_length);
	
	double rad1 = l1_length / 2;
	int c3_id = add_circle(l1_x1, l1_y1, rad1);	// circle3
	SaCircle* c3 = (SaCircle*)get_shape(c3_id);

	brace(l_p1p2_id, c3_id);	// BRACE
	solve();

	double p_ax = *(c1->get_gcs_circle().center.x) + rad1;
	double p_ay = *(c1->get_gcs_circle().center.y);

	int a = add_point(p_ax, p_ay);
	SaPoint* p_a = (SaPoint*)get_shape(a);

	double p_bx = *(c2->get_gcs_circle().center.x) - *(c2->get_gcs_circle().rad);
	double p_by = *(c2->get_gcs_circle().center.y);
	
	int b = add_point(p_bx, p_by);
	SaPoint* p_b = (SaPoint*)get_shape(b);

	// COIN(A, L(P1, P3)
	gcs_sys.addConstraintPointOnLine(p_a->get_gcs_point(), c1->get_gcs_circle().center, c3->get_gcs_circle().center, 1);
	solve();

	// COIN(A, C1);
	coincident_point_circle(a, id1);
	solve();

	// COIN(B, L(P2, P3)
	gcs_sys.addConstraintPointOnLine(p_b->get_gcs_point(), c2->get_gcs_circle().center, c3->get_gcs_circle().center, 1);
	solve();

	// COIN(B, C2)
	coincident_point_circle(b, id2);
	*/
}

void GCSWrapper::p_disjunction(int id1, int id2)	// id1 = circle1, id2 = circle2
{
	SaCircle* c1 = (SaCircle*)get_shape(id1);
	SaCircle* c2 = (SaCircle*)get_shape(id2);

	double l_cd_x1 = *(c1->get_gcs_circle().center.x) - *(c1->get_gcs_circle().rad);
	double l_cd_y1 = *(c1->get_gcs_circle().center.y);
	double l_cd_x2 = *(c1->get_gcs_circle().center.x) + *(c1->get_gcs_circle().rad);
	double l_cd_y2 = *(c1->get_gcs_circle().center.y);

	double l_ab_x1 = *(c2->get_gcs_circle().center.x) - *(c2->get_gcs_circle().rad);
	double l_ab_y1 = *(c2->get_gcs_circle().center.y);
	double l_ab_x2 = *(c2->get_gcs_circle().center.x) + *(c2->get_gcs_circle().rad);
	double l_ab_y2 = *(c2->get_gcs_circle().center.y);

	int l_ab = add_segment(l_ab_x1, l_ab_y1, l_ab_x2, l_ab_y2);
	int l_cd = add_segment(l_cd_x1, l_cd_y1, l_cd_x2, l_cd_y2);

	// BRACE(L(a,b), C2)
	brace(l_ab, id2);

	// BRACE(L(c,d), C1)
	brace(l_cd, id1);

	// COIN(L(c,d), L(a,b))
	coincident_line_line(l_cd, l_ab);
}

void GCSWrapper::dr_disjunction(int id1, int id2)	// id1 = circle1, id2 = circle2
{
	SaCircle* c1 = (SaCircle*)get_shape(id1);
	SaCircle* c2 = (SaCircle*)get_shape(id2);

	double p1_x = *(c1->get_gcs_circle().center.x);
	double p1_y = *(c1->get_gcs_circle().center.y);

	double p2_x = *(c2->get_gcs_circle().center.x);
	double p2_y = *(c2->get_gcs_circle().center.y);

	int p1p2 = add_segment(p1_x, p1_y, p2_x, p2_y);
	SaLine* l_p1p2 = (SaLine*)get_shape(p1p2);

	double c_x, c_y, c_len;
	calculate_line_midpoint(l_p1p2, c_x, c_y);
	calculate_line_length(l_p1p2, c_len);

	double c_rad = c_len / 2;
	
	int c_c3 = add_circle(c_x, c_y, c_rad);
	SaCircle* c3 = (SaCircle*)get_shape(c_c3);

	// brace
	brace(p1p2, c_c3);

	int p_a_x = p1_x + *(c1->get_gcs_circle().rad);
	int p_a_y = p1_y;

	int p_a = add_point(p_a_x, p_a_y);

	int p1p3 = add_segment(p1_x, p1_y, *(c3->get_gcs_circle().center.x), *(c3->get_gcs_circle().center.y));
	SaLine* l_p1p3 = (SaLine*)get_shape(p1p3);
	fix_line(p1p3);

	// COIN
	coincident_line_circle(p_a, p1p3);

	// COIN
	coincident_point_circle(p_a, id1);

	int p_b_x = p2_x - *(c2->get_gcs_circle().rad);
	int p_b_y = p2_y;

	int p_b = add_point(p_b_x, p_b_y);

	int p2p3 = add_segment(p2_x, p2_y, *(c3->get_gcs_circle().center.x), *(c3->get_gcs_circle().center.y));
	SaLine* l_p2p3 = (SaLine*)get_shape(p2p3);
	fix_line(p2p3);

	// COIN
	collinear_point_line(p_b, p2p3);

	// COIN
	coincident_point_circle(p_b, id2);
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


bool GCSWrapper::circle_min_diameter(int id, double diameter)
{
    SaCircle* c1 = (SaCircle*)get_shape(id);
    double c1_rad = *(c1->get_gcs_circle().rad);
	double c1_x = *(c1->get_gcs_circle().center.x);
	double c1_y = *(c1->get_gcs_circle().center.y);

    //- find coordinates for new line segment
    double min_rad = diameter/2;
    double ly1 = c1_y + min_rad;
    double ly2 = c1_y - min_rad;
    
    double lx=0;
    if(c1_rad <= min_rad)
    {   //- case: circle is already smaller than this minimum bound
        lx = c1_x;
        ly1 = c1_y + c1_rad;
        ly2 = c1_y - c1_rad;
    } else
    {   //- case: circle is larger than minimum bound, place vertical segment at correct position
        lx = c1_x + std::sqrt(c1_rad * c1_rad - min_rad * min_rad);
    }
    
    //- create line segment
    int s1 = add_segment(lx, ly1, lx, ly2);
    SaLine* l = (SaLine*)get_shape(s1);
    
//    std::cout << "V1: " << std::endl;
//    show_values(0);

    //- make vertical
	gcs_sys.addConstraintVertical(l->get_gcs_line(), 1);
    if(!solve()) return false;
    
//    std::cout << "V2: " << std::endl;
//    show_values(0);
    
    //- make line coincident to circle
    gcs_sys.addConstraintPointOnCircle(l->get_gcs_line().p1, c1->get_gcs_circle(), 1);
    if(!solve()) return false;
    
//    std::cout << "V4a: " << std::endl;
//    show_values(0);
    
    gcs_sys.addConstraintPointOnCircle(l->get_gcs_line().p2, c1->get_gcs_circle(), 1);
    if(!solve()) return false;
    
//    std::cout << "V4b: " << std::endl;
//    show_values(0);

    
    //- fix line length
    double* dia = new double(diameter);
	_FixedParameters.push_back(dia);
//    gcs_sys.addConstraintDifference(l->get_gcs_line().p2.y, l->get_gcs_line().p1.y, dia, 1);
	gcs_sys.addConstraintP2PDistance(l->get_gcs_line().p1, l->get_gcs_line().p2, dia, 1);
    if(!solve()) return false;

//    std::cout << "V7: " << std::endl;
//    show_values(0);
    

    return true;
    
//	int s1 = add_segment(0.0, 0.0, 0.0, diameter);		
//	
//	line_vertical(s1);
//	fix_length(s1, diameter);
//	coincident_line_circle(s1, id);
}



bool GCSWrapper::circle_max_diameter(int id, double diameter)
{
    SaCircle* c1 = (SaCircle*)get_shape(id);
    double c1_rad = *(c1->get_gcs_circle().rad);
	double c1_x = *(c1->get_gcs_circle().center.x);
	double c1_y = *(c1->get_gcs_circle().center.y);
    
    double max_rad = diameter/2;
    
    //- create pa on circle
    int pa_id = add_point(c1_x + c1_rad, c1_y);
    SaPoint* pa = (SaPoint*)get_shape(pa_id);
    gcs_sys.addConstraintPointOnCircle(pa->get_gcs_point(), c1->get_gcs_circle(), 1);
    if(!solve()) return false;
    
//    std::cout << "V1: " << std::endl;
//    show_values(0);
    
    //- constrain point to be horizontally aligned with circle centroid
    gcs_sys.addConstraintHorizontal(c1->get_gcs_circle().center, pa->get_gcs_point(), 1);
    if(!solve()) return false;

//    std::cout << "V2: " << std::endl;
//    show_values(0);

    //- create distance point
    double pbx = std::min(c1_rad, max_rad);
    double pby = std::sqrt(max_rad * max_rad - pbx * pbx);
    int pb_id = add_point(pbx + c1_x, pby + c1_y);
    SaPoint* pb = (SaPoint*)get_shape(pb_id);
    
    //- fix maximum distance
    double* mxrd = new double(max_rad);
	_FixedParameters.push_back(mxrd);
	gcs_sys.addConstraintP2PDistance(c1->get_gcs_circle().center, pb->get_gcs_point(), mxrd, 1);
    if(!solve()) return false;
    
//    std::cout << "V3: " << std::endl;
//    show_values(0);
    
    //- constrain distance point and pa to be vertically aligned
    gcs_sys.addConstraintVertical(pa->get_gcs_point(), pb->get_gcs_point(), 1);
    if(!solve()) return false;

//    std::cout << "V4: " << std::endl;
//    show_values(0);

    return true;
}




void GCSWrapper::fix_circle_radius(int id, double radius)
{
	SaCircle* c = (SaCircle*)get_shape(id);

	double* d = new double(radius);
	_FixedParameters.push_back(d);
	
	gcs_sys.addConstraintCircleRadius(c->get_gcs_circle(), d, 1);

	//gcs_sys.addConstraint
}

bool GCSWrapper::fix_circle(int id)
{
	SaCircle* c = (SaCircle*)get_shape(id);
    double radius = *(c->get_gcs_circle().rad);

    //- fix the circle radius
    
	double* d = new double(radius);
	_FixedParameters.push_back(d);
	
	gcs_sys.addConstraintCircleRadius(c->get_gcs_circle(), d, 1);
    if(!solve()) return false;
    
    //- fix the centre point
    double x = *(c->get_gcs_circle().center.x);
    double y = *(c->get_gcs_circle().center.y);
    
    double* xp = new double(x);
	double* yp = new double(y);
    
	_FixedParameters.push_back(xp);
	_FixedParameters.push_back(yp);
    
	gcs_sys.addConstraintCoordinateX(c->get_gcs_circle().center, xp, 1);
    if(!solve()) return false;
	gcs_sys.addConstraintCoordinateY(c->get_gcs_circle().center, yp, 1);
    if(!solve()) return false;
	
	return true;

    
}

void GCSWrapper::fix_circle_centroid(int id)
{
	SaCircle* c = (SaCircle*)get_shape(id);
    
    //- fix the centre point
    double x = *(c->get_gcs_circle().center.x);
    double y = *(c->get_gcs_circle().center.y);
    
    double* xp = new double(x);
	double* yp = new double(y);
    
	_FixedParameters.push_back(xp);
	_FixedParameters.push_back(yp);
    
	gcs_sys.addConstraintCoordinateX(c->get_gcs_circle().center, xp, 1);
    solve();
	gcs_sys.addConstraintCoordinateY(c->get_gcs_circle().center, yp, 1);
    solve();
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
	double* d = new double(length);
	_FixedParameters.push_back(d);

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


bool GCSWrapper::end_point_concentric(int line_id, int endPt, int circ_id)
{
    //- constraint end point of line_id (p1 or p2) to the centroid of the circle with circ_id
    //- this is not an ideal set of methods to create as we can't directly relate parts of objects
    //- without creating specialised methods, but for now permissable
    
    SaLine* l = (SaLine*)get_shape(line_id);
    SaCircle* c = (SaCircle*)get_shape(circ_id);
    
    if(endPt == 1)
        gcs_sys.addConstraintP2PCoincident(l->get_gcs_line().p1, c->get_gcs_circle().center, 1);
    else
        gcs_sys.addConstraintP2PCoincident(l->get_gcs_line().p2, c->get_gcs_circle().center, 1);
    
    if(!solve()) return false;

    return true;
    
}

bool GCSWrapper::create_brace_for_line(int line_id, int& c_id)	{
    SaLine* l = (SaLine*)get_shape(line_id);
    double mid_x = (*(l->get_gcs_line().p1.x) + *(l->get_gcs_line().p2.x)) / 2;
    double mid_y = (*(l->get_gcs_line().p1.y) + *(l->get_gcs_line().p2.y)) / 2;
    double dx = *(l->get_gcs_line().p2.x) - *(l->get_gcs_line().p1.x);
    double dy = *(l->get_gcs_line().p2.y) - *(l->get_gcs_line().p1.y);
    double len = std::sqrt(dx*dx + dy*dy);
    
//    std::cout << "Brace before:" << std::endl;
//    show_values(0);
    
    //- create brace circle
    c_id = add_circle(mid_x, mid_y, len/2);
	SaCircle* c = (SaCircle*)get_shape(c_id);
    
    //- make centroid collinear to line
    gcs_sys.addConstraintPointOnLine(c->get_gcs_circle().center, l->get_gcs_line().p1, l->get_gcs_line().p2, 1);
    //gcs_sys.addConstraintPointOnLine(c->get_gcs_circle().center, l->get_gcs_line(), 1);
    if(!solve()) return false;

    //- make endpoints of line coincident to circle
    gcs_sys.addConstraintPointOnCircle(l->get_gcs_line().p1, c->get_gcs_circle(), 1);
    if(!solve()) return false;
    gcs_sys.addConstraintPointOnCircle(l->get_gcs_line().p2, c->get_gcs_circle(), 1);
    if(!solve()) return false;
    
//    std::cout << "Brace after:" << std::endl;
//    show_values(0);


    return true;
    
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
}

void GCSWrapper::calculate_line_midpoint(SaLine* l, double& x, double& y)
{
	double x1 = *(l->get_gcs_line().p1.x);
	double x2 = *(l->get_gcs_line().p2.x);
	
	double y1 = *(l->get_gcs_line().p1.y);
	double y2 = *(l->get_gcs_line().p2.y);

	x = x1 + ((x2 - x1) / 2);
	y = y1 + ((y2 - y1) / 2);
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


void GCSWrapper::calculate_projection(SaPoint* p, SaPoint* p1, SaPoint* p2, double &prjX, double &prjY)
{
	double xa = *(p1->get_gcs_point().x);
	double ya = *(p1->get_gcs_point().y);
	double xb = *(p2->get_gcs_point().x);
	double yb = *(p2->get_gcs_point().y);
    
	double xp = *(p->get_gcs_point().x);
	double yp = *(p->get_gcs_point().y);
    
    double prjval = (xp - xa) * (xb - xa) + (yp - ya) * (yb - ya);
    
    double lim = (xb - xa)*(xb - xa) + (yb - ya)*(yb - ya);
    double norm = prjval / lim;
    
    prjX = (xb - xa) * norm + xa;
    prjY = (yb - ya) * norm + ya;
}

void GCSWrapper::calculate_projection(SaCircle* p, SaCircle* p1, SaCircle* p2, double &prjX, double &prjY)
{
    //	double p_x = *(p->get_gcs_circle().center.x);

	double xa = *(p1->get_gcs_circle().center.x);
	double ya = *(p1->get_gcs_circle().center.y);
	double xb = *(p2->get_gcs_circle().center.x);
	double yb = *(p2->get_gcs_circle().center.y);
    
	double xp = *(p->get_gcs_circle().center.x);
	double yp = *(p->get_gcs_circle().center.y);
    
    double prjval = (xp - xa) * (xb - xa) + (yp - ya) * (yb - ya);
    
    double lim = (xb - xa)*(xb - xa) + (yb - ya)*(yb - ya);
    double norm = prjval / lim;
    
    prjX = (xb - xa) * norm + xa;
    prjY = (yb - ya) * norm + ya;
    
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

bool GCSWrapper::calculate_is_left_of(SaPoint* p, SaPoint* p1, SaPoint* p2)
{
	double xa = *(p1->get_gcs_point().x);
	double ya = *(p1->get_gcs_point().y);
	double xb = *(p2->get_gcs_point().x);
	double yb = *(p2->get_gcs_point().y);
    
	double xp = *(p->get_gcs_point().x);
	double yp = *(p->get_gcs_point().y);
    
	return (xb - xa)*(yp - ya) > (yb - ya)*(xp - xa);
}

bool GCSWrapper::calculate_is_tpp(SaCircle* c1, SaCircle* c2)	
{
	double c1_rad = *(c1->get_gcs_circle().rad);
	double c1_x = *(c1->get_gcs_circle().center.x);
	double c1_y = *(c1->get_gcs_circle().center.y);

	double c2_rad = *(c2->get_gcs_circle().rad);
	double c2_x = *(c2->get_gcs_circle().center.x);
	double c2_y = *(c2->get_gcs_circle().center.y);

	double dist = ((c2_x - c1_x) * (c2_x - c1_x)) + ((c2_y - c1_y) * (c2_y - c1_y));

	return c1_rad < c2_rad && dist == ((c1_rad - c2_rad) * (c1_rad - c2_rad));
}

bool GCSWrapper::calculate_is_tpp(double c1_r, double c1_x, double c1_y, 
								  double c2_r, double c2_x, double c2_y,
								  double dist, double rad_diff, double rad_sum)
{
	return (c1_r < c2_r) && (dist == rad_diff);
}

bool GCSWrapper::calculate_is_ntpp(double c1_r, double c1_x, double c1_y, 
								  double c2_r, double c2_x, double c2_y,
								  double dist, double rad_diff, double rad_sum)
{
	return (c1_r < c2_r) && (dist < rad_diff);
}

bool GCSWrapper::calculate_is_po(double c1_r, double c1_x, double c1_y, 
								  double c2_r, double c2_x, double c2_y,
								  double dist, double rad_diff, double rad_sum)
{
	return (dist > rad_diff) && (dist < rad_sum);
}

bool GCSWrapper::calculate_is_ec(double c1_r, double c1_x, double c1_y, 
								  double c2_r, double c2_x, double c2_y,
								  double dist, double rad_diff, double rad_sum)
{
	return (dist == rad_sum);
}

bool GCSWrapper::calculate_is_dc(double c1_r, double c1_x, double c1_y, 
								  double c2_r, double c2_x, double c2_y,
								  double dist, double rad_diff, double rad_sum)
{
	return (dist > rad_sum);
}

bool GCSWrapper::calculate_is_eq(double c1_r, double c1_x, double c1_y, 
								  double c2_r, double c2_x, double c2_y,
								  double dist, double rad_diff, double rad_sum)
{
	return (dist == 0) && (c1_r == c2_r);
}