// GCSTest.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include "stdio.h"
#include "Geo.h"
#include "GCS.h"
#include "Constraints.h"
//#include "Test1.h"
//#include "Test2.h"
//#include "Test3.h"
//#include "Test4.h"
//#include "Test5.h"
#include "GCSWrapper.h"

void test_line_circle_coincident()	{

	GCSWrapper *wrapper = new GCSWrapper();
	
	int s1 = wrapper->add_segment(0.0, 0.0, 10.0, 10.0);	
	wrapper->get_shape(s1);	
	
	int c1 = wrapper->add_circle(100.0, 0.0, 3.0);
	wrapper->get_shape(c1);

	std::cout<<"before solving";
	wrapper->show_values(0);

	wrapper->line_vertical(s1);
	wrapper->fix_length(s1, 10.0);
	wrapper->coincident_line_circle(s1, c1);
	wrapper->solve();

	std::cout<<"after solving";
	wrapper->show_values(0);
}

void test_circle_min_diameter()	{

	GCSWrapper *wrapper = new GCSWrapper();
	
	int c1 = wrapper->add_circle(100.0, 0.0, 3.0);
	
	std::cout<<"before solving"<<"\n";
	wrapper->show_values(0);

	wrapper->circle_min_diameter(c1, 10.0);
	wrapper->solve();

	std::cout<<"after solving"<<"\n";
	wrapper->show_values(0);
}

void test_circle_fix_radius()	{

	GCSWrapper *wrapper = new GCSWrapper();
	
	int c1 = wrapper->add_circle(100.0, 0.0, 5.0);
	
	std::cout<<"before solving"<<"\n";
	wrapper->show_values(0);

	wrapper->fix_circle_radius(c1, 3.0);
	wrapper->solve();

	std::cout<<"after solving"<<"\n";
	wrapper->show_values(0);
}

// change it to test_circle_min_radius laters
void test_circle_min_radius_inconsistent()	
{
	GCSWrapper *wrapper = new GCSWrapper();
	
	int c1 = wrapper->add_circle(100.0, 0.0, 3.0);
	
	std::cout<<"before solving"<<"\n";
	wrapper->show_values(0);

	wrapper->circle_min_diameter(c1, 10.0);
	
	wrapper->fix_circle_radius(c1, 3.0);

	wrapper->solve();

	std::cout<<"after solving"<<"\n";
	wrapper->show_values(0);
}

void test_circle_min_radius_consistent()	
{
	GCSWrapper *wrapper = new GCSWrapper();
	
	int c1 = wrapper->add_circle(100.0, 0.0, 3.0);
	
	std::cout<<"before solving"<<"\n";
	wrapper->show_values(0);

	wrapper->circle_min_diameter(c1, 10.0);
	
	wrapper->fix_circle_radius(c1, 100.0);

	wrapper->solve();

	std::cout<<"\n"<<"after solving"<<"\n";
	wrapper->show_values(0);
}

void test_circle_min_radius_consistent_borderline()	
{
	GCSWrapper *wrapper = new GCSWrapper();
	
	int c1 = wrapper->add_circle(100.0, 0.0, 3.0);
	
	std::cout<<"before solving"<<"\n";
	wrapper->show_values(0);

	wrapper->circle_min_diameter(c1, 10.0);
	
	wrapper->fix_circle_radius(c1, 5.0);

	wrapper->solve();

	std::cout<<"\n"<<"after solving"<<"\n";
	wrapper->show_values(0);
}

void test_point_segment_coincidence()
{
	GCSWrapper* wrapper = new GCSWrapper();

	int p1 = wrapper->add_point(10.0, 10.0);	// if values of both points are same, it fails to solve.
	int l1 = wrapper->add_segment(10.0, 0.0, 30.0, 0.0);

	wrapper->point_segment_coincidence(p1, l1);

	std::cout<<"\n"<<"before solving"<<"\n";
	wrapper->show_values(0);

	wrapper->solve();

	std::cout<<"\n"<<"after solving"<<"\n";
	wrapper->show_values(0);
}

/////////////////////////////
// Tests for horizontal line
/////////////////////////////
void test_point_segment_coincidence_consistent_boundary()
{
	GCSWrapper* wrapper = new GCSWrapper();

	int p1 = wrapper->add_point(1.0, 0.0);
	int l1 = wrapper->add_segment(0.0, 0.0, 10.0, 0.0);

	wrapper->fix_point(p1);
	wrapper->fix_line(l1);
	wrapper->point_segment_coincidence(p1, l1);

	std::cout<<"\n"<<"before solving"<<"\n";
	wrapper->show_values(0);

	wrapper->solve();

	std::cout<<"\n"<<"after solving"<<"\n";
	wrapper->show_values(0);
}

void test_point_segment_coincidence_consistent_non_boundary()
{
	GCSWrapper* wrapper = new GCSWrapper();

	int p1 = wrapper->add_point(5.0, 0.0);
	int l1 = wrapper->add_segment(0.0, 0.0, 10.0, 0.0);

	//wrapper->fix_point(p1);
	//wrapper->fix_line(l1);
	wrapper->point_segment_coincidence(p1, l1);

	std::cout<<"\n"<<"before solving"<<"\n";
	wrapper->show_values(0);

	wrapper->solve();

	std::cout<<"\n"<<"after solving"<<"\n";
	wrapper->show_values(0);
}

void test_point_segment_coincidence_inconsistent_boundary()
{
	GCSWrapper* wrapper = new GCSWrapper();

	//int p1 = wrapper->add_point(-20.0, 0.0);
	int p1 = wrapper->add_point(10.001, 0.0);
	int l1 = wrapper->add_segment(00.0, 0.0, 10.0, 0.0);

	//wrapper->fix_point(p1);
	//wrapper->fix_line(l1);
	wrapper->point_segment_coincidence(p1, l1);

	std::cout<<"\n"<<"before solving"<<"\n";
	wrapper->show_values(0);

	wrapper->solve();

	std::cout<<"\n"<<"after solving"<<"\n";
	wrapper->show_values(0);
}

void test_point_segment_coincidence_inconsistent_non_boundary()
{
	GCSWrapper* wrapper = new GCSWrapper();

	int p1 = wrapper->add_point(25.0, 0.0);
	int l1 = wrapper->add_segment(0.0, 0.0, 10.0, 0.0);

	//wrapper->fix_point(p1);
	//wrapper->fix_line(l1);
	wrapper->point_segment_coincidence(p1, l1);

	std::cout<<"\n"<<"before solving"<<"\n";
	wrapper->show_values(0);

	wrapper->solve();

	std::cout<<"\n"<<"after solving"<<"\n";
	wrapper->show_values(0);
}

///////////////////////////
// Tests for vertical line
///////////////////////////
void test_point_segment_coincidence_consistent_boundary_vertical()
{
	GCSWrapper* wrapper = new GCSWrapper();

	int p1 = wrapper->add_point(0.0, 0.0);
	//int l1 = wrapper->add_segment(0.0, 0.0, 10.0, 0.0);
	int l1 = wrapper->add_segment(0.0, 0.0, 0.0, 10.0);

	wrapper->fix_point(p1);
	wrapper->fix_line(l1);
	wrapper->point_segment_coincidence(p1, l1);

	std::cout<<"\n"<<"before solving"<<"\n";
	wrapper->show_values(0);

	wrapper->solve();

	std::cout<<"\n"<<"after solving"<<"\n";
	wrapper->show_values(0);
}

void test_point_segment_coincidence_consistent_non_boundary_vertical()
{
	GCSWrapper* wrapper = new GCSWrapper();

	//int p1 = wrapper->add_point(5.0, 0.0);
	//int l1 = wrapper->add_segment(0.0, 0.0, 10.0, 0.0);

	int p1 = wrapper->add_point(0.0, 5.0);
	int l1 = wrapper->add_segment(0.0, 0.0, 0.0, 10.0);

	wrapper->fix_point(p1);
	wrapper->fix_line(l1);
	wrapper->point_segment_coincidence(p1, l1);

	std::cout<<"\n"<<"before solving"<<"\n";
	wrapper->show_values(0);

	wrapper->solve();

	std::cout<<"\n"<<"after solving"<<"\n";
	wrapper->show_values(0);
}

void test_point_segment_coincidence_inconsistent_boundary_vertical()
{
	GCSWrapper* wrapper = new GCSWrapper();

	//int p1 = wrapper->add_point(-20.0, 0.0);
	//int p1 = wrapper->add_point(10.001, 0.0);
	//int l1 = wrapper->add_segment(00.0, 0.0, 10.0, 0.0);

	int p1 = wrapper->add_point(0.001, 10.001);
	int l1 = wrapper->add_segment(00.0, 0.0, 0.0, 10.0);

	wrapper->fix_point(p1);
	wrapper->fix_line(l1);
	wrapper->point_segment_coincidence(p1, l1);

	std::cout<<"\n"<<"before solving"<<"\n";
	wrapper->show_values(0);

	wrapper->solve();

	std::cout<<"\n"<<"after solving"<<"\n";
	wrapper->show_values(0);
}

void test_point_segment_coincidence_inconsistent_non_boundary_vertical()
{
	GCSWrapper* wrapper = new GCSWrapper();

	//int p1 = wrapper->add_point(25.0, 0.0);
	//int l1 = wrapper->add_segment(0.0, 0.0, 10.0, 0.0);

	int p1 = wrapper->add_point(0.0, 25.0);
	int l1 = wrapper->add_segment(0.0, 0.0, 0.0, 10.0);

	wrapper->fix_point(p1);
	wrapper->fix_line(l1);
	wrapper->point_segment_coincidence(p1, l1);

	std::cout<<"\n"<<"before solving"<<"\n";
	wrapper->show_values(0);

	wrapper->solve();

	std::cout<<"\n"<<"after solving"<<"\n";
	wrapper->show_values(0);
}

//////////////////////////////////////////////////
//Tests for vertical line with vertical constraint
//////////////////////////////////////////////////
void test_point_segment_coincidence_consistent_boundary_vertical_constraint()
{
	GCSWrapper* wrapper = new GCSWrapper();

	int p1 = wrapper->add_point(0.0, 0.0);
	int l1 = wrapper->add_segment(0.0, 0.0, 10.0, 0.0);

	wrapper->line_vertical(l1);
	wrapper->point_segment_coincidence(p1, l1);

	std::cout<<"\n"<<"before solving"<<"\n";
	wrapper->show_values(0);

	wrapper->solve();

	std::cout<<"\n"<<"after solving"<<"\n";
	wrapper->show_values(0);
}

void test_point_segment_coincidence_consistent_non_boundary_vertical_constraint()
{
	GCSWrapper* wrapper = new GCSWrapper();

	int p1 = wrapper->add_point(5.0, 0.0);
	int l1 = wrapper->add_segment(0.0, 0.0, 10.0, 0.0);

	wrapper->line_vertical(l1);
	wrapper->point_segment_coincidence(p1, l1);

	std::cout<<"\n"<<"before solving"<<"\n";
	wrapper->show_values(0);

	wrapper->solve();

	std::cout<<"\n"<<"after solving"<<"\n";
	wrapper->show_values(0);
}

void test_point_segment_coincidence_inconsistent_boundary_vertical_constraint()
{
	GCSWrapper* wrapper = new GCSWrapper();

	//int p1 = wrapper->add_point(-20.0, 0.0);
	int p1 = wrapper->add_point(10.001, 0.0);
	int l1 = wrapper->add_segment(00.0, 0.0, 10.0, 0.0);
	
	wrapper->line_vertical(l1);
	wrapper->point_segment_coincidence(p1, l1);

	std::cout<<"\n"<<"before solving"<<"\n";
	wrapper->show_values(0);

	wrapper->solve();

	std::cout<<"\n"<<"after solving"<<"\n";
	wrapper->show_values(0);
}

void test_point_segment_coincidence_inconsistent_non_boundary_vertical_constraint()
{
	GCSWrapper* wrapper = new GCSWrapper();

	int p1 = wrapper->add_point(25.0, 0.0);
	int l1 = wrapper->add_segment(0.0, 0.0, 10.0, 0.0);
	
	wrapper->line_vertical(l1);
	wrapper->point_segment_coincidence(p1, l1);

	std::cout<<"\n"<<"before solving"<<"\n";
	wrapper->show_values(0);

	wrapper->solve();

	std::cout<<"\n"<<"after solving"<<"\n";
	wrapper->show_values(0);
}
//////////////////////////////////////////////
// Tests for Angle between a Point and a Line
//////////////////////////////////////////////
void test_angle_point_line()
{
	GCSWrapper* wrapper = new GCSWrapper();

	int p1 = wrapper->add_point(1.0, 10.0);
	int l1 = wrapper->add_segment(5.0, 0.0, 10.0, 0.0);

	//int p1 = wrapper->add_point(5.0, 10.0);
	//int l1 = wrapper->add_segment(5.0, 0.0, 15.0, 0.0);

	std::cout<<"\n"<<"before solving"<<"\n";
	wrapper->show_values(0);

	wrapper->left_of(p1, l1);
	//wrapper->angle_2(p1, l1, -90.0);

	wrapper->solve();

	std::cout<<"\n"<<"after solving"<<"\n";
	wrapper->show_values(0);
}

void test_angle_point_line_consistent_boundary()
{
	GCSWrapper* wrapper = new GCSWrapper();

	int p1 = wrapper->add_point(0.0, 10.0);
	int l1 = wrapper->add_segment(0.0, 0.0, 10.0, 0.0);

	std::cout<<"\n"<<"before solving"<<"\n";
	wrapper->show_values(0);

	wrapper->fix_point(p1);
	wrapper->fix_line(l1);
	wrapper->left_of(p1, l1);
	//wrapper->angle_2(p1, l1, -90.0);

	wrapper->solve();

	std::cout<<"\n"<<"after solving"<<"\n";
	wrapper->show_values(0);
}

void test_angle_point_line_inconsistent_boundary()
{
	GCSWrapper* wrapper = new GCSWrapper();

	int p1 = wrapper->add_point(0.0, -0.00001);
	int l1 = wrapper->add_segment(0.0, 0.0, 10.0, 0.0);

	std::cout<<"\n"<<"before solving"<<"\n";
	wrapper->show_values(0);

	wrapper->fix_point(p1);
	wrapper->fix_line(l1);
	//wrapper->angle_point_line(p1, l1, 90.0);
	wrapper->left_of(p1, l1);
	//wrapper->angle_point_line(p1, l1, -90.0);

	wrapper->solve();

	std::cout<<"\n"<<"after solving"<<"\n";
	wrapper->show_values(0);
}

void test_angle_point_line_consistent_nonboundary()
{
	GCSWrapper* wrapper = new GCSWrapper();

	int p1 = wrapper->add_point(5.0, 10.0);
	int l1 = wrapper->add_segment(0.0, 0.0, 10.0, 0.0);

	std::cout<<"\n"<<"before solving"<<"\n";
	wrapper->show_values(0);

	wrapper->fix_point(p1);
	wrapper->fix_line(l1);
	wrapper->left_of(p1, l1);
	//wrapper->angle_point_line(p1, l1, -90.0);

	wrapper->solve();

	std::cout<<"\n"<<"after solving"<<"\n";
	wrapper->show_values(0);
}

void test_angle_point_line_inconsistent_nonboundary()
{
	GCSWrapper* wrapper = new GCSWrapper();

	int p1 = wrapper->add_point(5.0, -10.0);
	int l1 = wrapper->add_segment(0.0, 0.0, 10.0, 0.0);

	std::cout<<"\n"<<"before solving"<<"\n";
	wrapper->show_values(0);

	wrapper->fix_point(p1);
	wrapper->fix_line(l1);
	wrapper->left_of(p1, l1);

	//wrapper->solve();

	std::cout<<"\n"<<"after solving"<<"\n";
	wrapper->show_values(0);
}

///////////////////////////////////////////////
// Tests for Angle between a Line and a Circle
///////////////////////////////////////////////
void test_angle_line_circle()
{
	GCSWrapper* wrapper = new GCSWrapper();

	//int p1 = wrapper->add_point(1.0, 10.0);
	int c1 = wrapper->add_circle(5.0, 5.0, 3.0);
	int l1 = wrapper->add_segment(5.0, 0.0, 10.0, 0.0);

	//int p1 = wrapper->add_point(5.0, 10.0);
	//int l1 = wrapper->add_segment(5.0, 0.0, 15.0, 0.0);

	std::cout<<"\n"<<"before solving"<<"\n";
	wrapper->show_values(0);

	wrapper->angle_line_circle(l1, c1, 90.0);

	wrapper->solve();

	std::cout<<"\n"<<"after solving"<<"\n";
	wrapper->show_values(0);
}

/*
void test_test_angle()	{

	GCSWrapper* wrapper = new GCSWrapper();

	int p1 = wrapper->add_point(5.0, 10.0);
	int l1 = wrapper->add_segment(5.0, 0.0, 15.0, 0.0);

	wrapper->test_angle(p1, l1);

	std::cout<<"\n"<<"before solving"<<"\n";
	wrapper->show_values(0);

	wrapper->solve();

	std::cout<<"\n"<<"after solving"<<"\n";
	wrapper->show_values(0);
}*/


int main(int argc, const char* argv[])
{
	//test_line_circle_coincident();
	//test_circle_min_diameter();
	//test_circle_fix_radius();
	//test_circle_min_radius_inconsistent();
	//test_circle_min_radius_consistent();

	//test_point_segment_coincidence();
	test_point_segment_coincidence_consistent_boundary();		// returns True
	//test_point_segment_coincidence_inconsistent_boundary();	// returns True
	//test_point_segment_coincidence_consistent_non_boundary();	// returns False
	//test_point_segment_coincidence_inconsistent_non_boundary();	// returns False

	//test_point_segment_coincidence_consistent_boundary_vertical();		// returns True
	//test_point_segment_coincidence_consistent_non_boundary_vertical();	// returns True
	//test_point_segment_coincidence_inconsistent_boundary_vertical();		// returns False
	//test_point_segment_coincidence_inconsistent_non_boundary_vertical();	// returns False

	//test_point_segment_coincidence_consistent_boundary_vertical_constraint();		// returns False
	//test_point_segment_coincidence_inconsistent_boundary_vertical_constraint();	// returns False
	//test_point_segment_coincidence_consistent_non_boundary_vertical_constraint();	// returns False
	//test_point_segment_coincidence_inconsistent_non_boundary_vertical_constraint();	// returns False

	//test_angle_point_line();
	//test_angle_point_line_consistent_boundary();
	//test_angle_point_line_inconsistent_boundary();
	//test_angle_point_line_consistent_nonboundary();
	//test_angle_point_line_inconsistent_nonboundary();
	//test_angle_line_circle();

	//GCS::Test1 *test1 = new GCS::Test1();
	//GCS::Test2 *test2 = new GCS::Test2();
	//GCS::Test3 *test3 = new GCS::Test3();
	//GCS::Test4 *test4 = new GCS::Test4();
	//GCS::Test5 *test5 = new GCS::Test5();

	//GCS::ConstraintTests *test = new GCS::ConstraintTests();
	//test->circle_ConstraintCircleRadius();

	//GCS::ContraintTestWithRandom *test = new GCS::ContraintTestWithRandom();
	//test->lineCirle_ConstraintTangent();
	//test->line_ConstraintParallel2Perpendicular();
	//test->lineCirle_ConstraintTangentRadius();

	//GCSWrapper *wrapper = new GCSWrapper();
	
	//int s1 = wrapper->add_segment(0.0, 0.0, 10.0, 10.0);	
	//wrapper->get_shape(s1);

	//int s2 = wrapper->add_segment(5.0, 5.0, 11.0, 21.0);
	//wrapper->get_shape(s2);
	
	//int c1 = wrapper->add_circle(10.0, 10.0, 5.0);
	//wrapper->get_shape(c1);

	//int s4 = wrapper->add_vertical_segment(12.0, 10.0, 15.0);

	//int p1 = wrapper->add_point(10.0, 12.0);
	//wrapper->get_shape(p1);

	//int p2 = wrapper->add_point(15.0, 8.0);
	//wrapper->get_shape(p2);

	//wrapper->coincident_point_circle(p1, c1);
	
	//int c1 = wrapper->add_circle(3, 4, 10);
	//wrapper->get_shape(c1);

	//wrapper->perpendicular(s1, s2);
	
	//wrapper->parallel(s1, s2);
	//wrapper->fix_length(s1, 5);
	//wrapper->fix_length(s2, 10);
	//wrapper->tangent(s1, s2);

	//wrapper->line_vertical(s1);
	//wrapper->point_vertical(p1, p2);


	//wrapper->solve();
	//wrapper->show_values(s1);
	//wrapper->show_values(p1);
	//wrapper->show_values(c1);

	//wrapper->show_values(s1);
	
/*
	double line_x1 = 15, line_x2 = 20, circle_x = 25, ellipse_focus_x = 20;
	double line_y = 10, circle_y = 42, ellipse_focus_y = 22;
	double radius = 5.0;

	GCS::Line *line = new GCS::Line();
	line->p1.x = &line_x1;
	line->p1.y = &line_y;
	line->p2.x = &line_x2;
	line->p2.y = &line_y;

	GCS::Circle *circle = new GCS::Circle();
	circle->rad = &radius;
	circle->center.x = &circle_x;
	circle->center.y = &circle_y;

	
	std::cout<<"\nApplying constraint";

	//GCS::ConstraintEllipseTangentLine *constraint = new GCS::ConstraintEllipseTangentLine();
	//GCS::ConstraintEllipseTangentLine *constraint = new GCS::ConstraintEllipseTangentLine(*line, *ellipse);
	//std::cout<<"\n"<<constraint->error();
	
	GCS::System *system = new GCS::System();
	int tangent = system->addConstraintTangent(*line, *circle, 1);
	std::cout<<"\ntangent: "<<tangent;

	//int solve = system->solve();
		
	std::cout<<"\nsolve with default params: "<<system->solve();
	std::cout<<"\nsolve with first param change: "<<system->solve(false);
	std::cout<<"\nsolve with second param change: "<<system->solve(true, GCS::Algorithm::LevenbergMarquardt);
	//std::cout<<"\nsolve with both param change: "<<system->solve(true, GCS::Algorithm::LevenbergMarquardt);
	std::cout<<"\nsolve with only last param change: "<<system->solve(true, GCS::Algorithm::BFGS);
	
	std::cout<<"\ncircle Y: "<<*circle->center.y;
*/
	std::cout<<"\nPress Enter to continue";
	getchar();
	return 0;
}

