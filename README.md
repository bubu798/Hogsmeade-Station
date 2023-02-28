# Hogsmeade-Station

## Objective: 

The objective of this topic is to understand and deepen lists and operations
on lists.

## Statement: 

Hogsmeade Station is growing fast and needs a digital a
information to plan their activities more easily. Our goal is to create a program
to solve the most important operations of the station, to make the work of the employees easier.
To be able to implement the necessary functions, we will use three data structures:

1. TrainStation is the structure that secures the station. The station has several platforms, and on each
platform can park a train. The platforms_no field records the platforms of the station, and
The platforms field is a vector containing the platforms and the stationary trains. Is not
mandatory for each person to have a train stationed at a certain time.

struct {

int platforms_no;

Train **platforms;

} Station;

2. Train is the structure that has a train. Each train consists of a locomotive and a
sequence of wagons. The locomotive_power field the maximum size it can
transport the locomotive. If the total weight of the wagons exceeds this value, the train does not
will be able to leave the station. The train_cars field the first car in the train sequence a
the train (it is the wagon attached to the locomotive).

struct {

int locomotive_power;

TrainCar* train_cars;

} Train;

3. TrainCar is the structure that has a carriage. Each wagon has an associated weight which
is retained in the weight field. Next field the next carriage in the sequence of
train cars.

struct {

int weight;

TrainCar *next;

} TrainCar;

We will implement 3 types of functions for Hogsmeade station:

* operating functions - these are functions that change the structure of trains on the platforms. We can
add or remove a train from a platform, add carriages or
remove wagons from a train.
* Search functions - are functions that help find trains with certain characteristics.
We can search for the fastest, best loaded, improperly loaded or with
poor weight distribution.
* Organisation functions - are functions that help to better organise the weight
trains. We can reorder wagons on a train or remove a wagon from a train.
improperly loaded train.

The project has its own makefile where all functions are explained but in Romanian language.
