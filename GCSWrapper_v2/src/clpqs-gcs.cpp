
#include "clpqs-gcs.h"

//---- Static Objects

static GCSWrapper* _gcs_wrap;

//---- PROLOG PREDICATES


extern "C" {
    install_t
    install(){
		std::cout << "\n\n\nInstalling CLP(QS) CGCS..." << std::endl;
		
        _gcs_wrap = new GCSWrapper();
        
    }
}

PREDICATE(hello_world,0)
{
    std::cout << "Hello CGCS World" << std::endl;
    return TRUE;
}


PREDICATE(reset,0)
{
    _gcs_wrap->reset();
    return TRUE;
}

PREDICATE(shape,2)
{
    //- shape(Id+, SaShape-)
    
    double id = (double) A1;
    A2 = _gcs_wrap->get_shape(id);
    return TRUE;
}

PREDICATE(type,2)
{
    //- shape(SaShape+, Type?)
    
    SaShape* shp = (SaShape*) ((void*) A1);
    A2 = shp->get_type();
    return TRUE;
}

PREDICATE(circle_params,4)
{
    //- shape(SaShape+, X?, Y?, R?)
    
    SaCircle* shp = (SaCircle*)  (void*) A1;
    A2 = *(shp->get_gcs_circle().center.x);
    A3 = *(shp->get_gcs_circle().center.y);
    A4 = *(shp->get_gcs_circle().rad);
    
    return TRUE;
}

PREDICATE(point_params,3)
{
    //- shape(SaShape+, X?, Y?)
    
    SaPoint* shp = (SaPoint*)  (void*) A1;
    A2 = *(shp->get_gcs_point().x);
    A3 = *(shp->get_gcs_point().y);
    
    return TRUE;
}

PREDICATE(set_point_params,3)
{
    //- new_point(Id, X, Y)
    
    SaPoint* shp = (SaPoint*)  (void*) A1;

    //double x =  (double) A2;
    //double y =  (double) A3;

    //double id = (double) A1[0];
    double* x =  new double(A2);
    double* y =  new double(A3);
    
    delete shp->get_gcs_point().x;
    delete shp->get_gcs_point().y;

    shp->get_gcs_point().x = x;
    shp->get_gcs_point().y = y;

    
//    shp->get_gcs_point().x = &x;
//    shp->get_gcs_point().y = &y;
    
    return TRUE;
}




PREDICATE(new_circle,4)
{
    //- new_circle(Id, X, Y, Radius)
    
    
    //double id = (double) A1[0];
    double x =  (double) A2;
    double y =  (double) A3;
    double r =  (double) A4;
    
    A1 = _gcs_wrap->add_circle(x, y, r);
    return TRUE;
}


PREDICATE(new_point,3)
{
    //- new_point(Id, X, Y)
    
    
    //double id = (double) A1[0];
    double x =  (double) A2;
    double y =  (double) A3;
    
    A1 = _gcs_wrap->add_point(x, y);
    return TRUE;
}



//---- CONSTRAINT SYSTEM ----//

PREDICATE(solve,0)
{
    return _gcs_wrap->solve();
}

//- TODO
//PREDICATE(remove_constraint,0)
//{
//    ...
//}

//---- RELATION ----//

PREDICATE(make_ec,2)
{
    //- make_ec(IdA+, IdB+)
   	return _gcs_wrap->externally_connected((int)A1, (int)A2);
    //return TRUE;
    
}

PREDICATE(make_left_of,2)
{
    //- make_ec(IdA+, IdB+)
   	return _gcs_wrap->left_of((int)A1, (int)A2);
    //return TRUE;
    
}

PREDICATE(make_left_of,3)
{
    //- make_ec(IdA+, IdB+, IdC+)
   	return _gcs_wrap->left_of((int)A1, (int)A2, (int)A3);
    //return TRUE;
    
}

PREDICATE(make_left_of_centroids,3)
{
    //- make_ec(IdA+, IdB+, IdC+)
   	return _gcs_wrap->left_of_centroids((int)A1, (int)A2, (int)A3);
    //return TRUE;
    
}


PREDICATE(fix_point,1)
{
    //- make_ec(IdA+, IdB+)
    return _gcs_wrap->fix_point((int)A1);    
}

PREDICATE(fix_circle,1)
{
    //- make_ec(IdA+, IdB+)
    return _gcs_wrap->fix_circle((int)A1);
    
}

PREDICATE(fix_centroid,1)
{
    //- make_ec(IdA+, IdB+)
    _gcs_wrap->fix_circle_centroid((int)A1);
    return TRUE;
    
}


PREDICATE(make_min_diameter,2)
{
    //- make_min_diameter(IdA+, Val+)
    return _gcs_wrap->circle_min_diameter((int)A1, (double) A2);
}


PREDICATE(make_max_diameter,2)
{
    //- make_max_diameter(IdA+, Val+)
    return _gcs_wrap->circle_max_diameter((int)A1, (double) A2);
}




