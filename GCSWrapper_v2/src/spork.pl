

:- [clpqs].
:- [clpqs_gcs].

:- dynamic object_id/3.
:- dynamic line_brace_object/2.




%%% %%%

clear_scene :-
  clpqs_gcs:reset,
  retractall(object_id(_,_,_)),
  retractall(line_brace_object(_,_)).

%%% Create Objects %%%%

create_objects(Objs) :-
  forall(
    (member(Obj, Objs), is_object(Obj)),
    create_object(Obj)
  ),
  !.

is_object(circle(_)).
is_object(line(_)).

is_object(circle(Id), Id).
is_object(line(Id), Id).

object_id(Name, Id) :- object_id(Name, Id, _).


create_object(circle(Name)) :-
  %circle(NewId, point(5,5), 7.07107),  %% for testing
  circle(NewId),
  assert(object_id(Name, NewId, type(circle))).


create_object(line(Name)) :-
  %line(NewId, point(0,0), point(10,10)),  %% for testing
  line(NewId),
  assert(object_id(Name, NewId, type(line))).




%%% Create Constraints %%%

create_constraints(Cons) :-
  forall(
    (member(Con, Cons), is_constraint(Con)),
    create_constraint(Con)
  ),
  !.


is_constraint(min_radius(_,_)).
is_constraint(max_radius(_,_)).
is_constraint(min_length(_,_)).
is_constraint(max_length(_,_)).
is_constraint(coincident(_,_)).
is_constraint(vertical(_)).
is_constraint(pp(_,_)).
is_constraint(concentric(_,_)).
is_constraint(larger(_,_)).

create_constraint(min_radius(Obj, Val)) :-
  object_id(Obj, id(ObjId)),
  Dia is 2*Val,
  clpqs_gcs:make_min_diameter(ObjId, Dia).

create_constraint(max_radius(Obj, Val)) :-
  object_id(Obj, id(ObjId)),
  Dia is 2*Val,
  clpqs_gcs:make_max_diameter(ObjId, Dia).

create_constraint(min_length(Obj, Val)) :-
  object_id(Obj, id(ObjId)),
  line_brace_(ObjId, CircId),
  clpqs_gcs:make_min_diameter(CircId, Val).

create_constraint(max_length(Obj, Val)) :-
  object_id(Obj, id(ObjId)),
  line_brace_(ObjId, CircId),
  clpqs_gcs:make_max_diameter(CircId, Val).


create_constraint(vertical(Obj)) :-
  object_id(Obj, id(ObjId)),
  clpqs_gcs:make_vertical(ObjId).

create_constraint(pp(ObjA,ObjB)) :-
  object_id(ObjA, id(IdA)),
  object_id(ObjB, id(IdB)),
  clpqs_gcs:make_ntpp(IdA, IdB).

create_constraint(larger(ObjA,ObjB)) :-
  object_id(ObjA, id(IdA), type(circle)),
  object_id(ObjB, id(IdB), type(circle)),
  clpqs_gcs:make_larger(IdA, IdB).

create_constraint(larger(length(ObjA),radius(ObjB)) ) :-
  object_id(ObjA, id(IdA), type(line)),
  object_id(ObjB, id(IdB), type(circle)),
  line_brace_(IdA, BrcA),
  clpqs_gcs:make_larger(BrcA, IdB).


create_constraint(concentric(ObjA, ObjB)) :-
  object_id(ObjA, id(IdA)),
  object_id(ObjB, id(IdB)),
  clpqs_gcs:make_concentric(IdA,IdB).

  
create_constraint(coincident(EndPoint, centre(ObjB))) :-
  %% case: A is endpoint of line, B is circle centre
  end_point_(EndPoint, ObjA, EPtCode),
  object_id(ObjA, id(IdA)),
  object_id(ObjB, id(IdB)),
  clpqs_gcs:end_point_concentric(IdA, EPtCode, IdB).



end_point_(p1(Obj), Obj, 1).
end_point_(p2(Obj), Obj, 2).

line_brace_(ObjId, CircId) :-
  line_brace_object(ObjId, CircId).

line_brace_(ObjId, CircId) :-
  not(line_brace_object(ObjId, _)),
  clpqs_gcs:new_brace_for_line(CircId, ObjId),
  assert(line_brace_object(ObjId, CircId)).


%%% Draw %%%

diagram(Objs, filename(_)) :-
  not(ground(Objs)),
  nl,write('Warning: objects sent to diagram are not ground.'),
  !, fail.

diagram(Objs, filename(Fn)) :-
  ground(Objs),
  setof(Id, 
    Obj^Name^
    (member(Obj, Objs),
     is_object(Obj, Name),
     object_id(Name,Id)
    ), Ids),
  !,
  gcs_draw(objects(Ids), filename(Fn)).
  

/**

%%%

clpqs_gcs:reset,

Objs = [
 circle(spoon_head),
 min_radius(spoon_head,5),
 max_radius(spoon_head,50)
],

create_objects(Objs),
diagram(Objs, filename(t1)),
create_constraints(Objs),
diagram(Objs, filename(t2)).

%%%

clpqs_gcs:reset,

Objs = [
 circle(spoon_head),
 min_radius(spoon_head,100)
],

create_objects(Objs),
diagram(Objs, filename(t1)),
create_constraints(Objs),
diagram(Objs, filename(t2)).



%%%

clpqs_gcs:reset,

Objs = [
 line(spoon_handle),
 min_length(spoon_handle,10),
 max_length(spoon_handle,100)
],

create_objects(Objs),
diagram(Objs, filename(t1)),
create_constraints(Objs),
diagram(Objs, filename(t2)).


%%%

clpqs_gcs:reset,

Objs = [
 circle(spoon_head),
 line(spoon_handle),
 coincident(p1(spoon_handle),centre(spoon_head))
],

create_objects(Objs),
diagram(Objs, filename(t1)),
create_constraints(Objs),
diagram(Objs, filename(t2)).

%%%

clpqs_gcs:reset,

Objs = [
 circle(spoon_head),
 line(spoon_handle),
 min_radius(spoon_head,5),
 max_radius(spoon_head,20),
 min_length(spoon_handle,30),
 max_length(spoon_handle,100),
 coincident(p1(spoon_handle),centre(spoon_head))
],

create_objects(Objs),
diagram(Objs, filename(t1)),
create_constraints(Objs),
diagram(Objs, filename(t2)).


%%%

clpqs_gcs:reset,

Objs = [
 line(spoon_handle),
 vertical(spoon_handle)
],

create_objects(Objs),
diagram(Objs, filename(t1)),
create_constraints(Objs),
diagram(Objs, filename(t2)).

%%%

clpqs_gcs:reset,

Objs = [
 circle(spoon_head),
 circle(spoon_well),
 pp(spoon_well, spoon_head) 
],

create_objects(Objs),
diagram(Objs, filename(t1)),
create_constraints(Objs),
diagram(Objs, filename(t2)).


%%%

clpqs_gcs:reset,

Objs = [
 circle(spoon_head),
 circle(spoon_well),
 concentric(spoon_well, spoon_head),
 pp(spoon_well, spoon_head) 
],

create_objects(Objs),
diagram(Objs, filename(t1)),
create_constraints(Objs),
diagram(Objs, filename(t2)).


%%%

clear_scene,

Objs = [
 circle(c1),
 circle(c2),
 larger(c1,c2)
],

create_objects(Objs),
diagram(Objs, filename(t1)),
create_constraints(Objs),
diagram(Objs, filename(t2)).

%%%

clear_scene,

Objs = [
 circle(spoon_well),
 line(well_depth),
 larger(length(well_depth),radius(spoon_well))
],

create_objects(Objs),
diagram(Objs, filename(t1)),
create_constraints(Objs),
diagram(Objs, filename(t2)).

%%%

clear_scene,

Objs = [
 circle(spoon_well)
 line(well_depth)
 min_length(well_depth,1)
 max_length(well_depth,50)
 vertical(well_depth)
 larger(length(well_depth),radius(spoon_well))
],

create_objects(Objs),
diagram(Objs, filename(t1)),
create_constraints(Objs),
diagram(Objs, filename(t2)).


%%%

clear_scene,

Objs = [
 line(spoon_handle)
 circle(spoon_head)
 circle(spoon_well)
 line(well_depth)
 min_length(spoon_handle,10)
 max_length(spoon_handle,100)
 min_radius(spoon_head,5)
 max_radius(spoon_head,50)
 coincident(p1(spoon_handle),centre(spoon_head))
 spoon_head_handle_contact_centroid
 pp(spoon_well,spoon_head)
 spoon_has_well
 min_length(well_depth,1)
 max_length(well_depth,50)
 vertical(well_depth)
 larger(length(well_depth),radius(spoon_well))
 deep_well
],

create_objects(Objs),
diagram(Objs, filename(t1)),
create_constraints(Objs),
diagram(Objs, filename(t2)).





**/