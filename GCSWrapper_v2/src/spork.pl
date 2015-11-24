

:- [clpqs].
:- [clpqs_gcs].

:- dynamic object_id/2.
:- dynamic line_brace_object/2.



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

  


create_object(circle(Name)) :-
  %circle(NewId, point(5,5), 7.07107),  %% for testing
  circle(NewId),
  assert(object_id(Name, NewId)).


create_object(line(Name)) :-
  %line(NewId, point(0,0), point(10,10)),  %% for testing
  line(NewId),
  assert(object_id(Name, NewId)).




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


**/