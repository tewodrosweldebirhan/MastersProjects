#include <iostream>
#include "vertex.h"

using namespace std;

vertex::vertex(){

}

vertex::~vertex(){

}

double vertex::getx(){

    return x;

}

double vertex::gety(){

    return y;

}


double vertex::getz(){

    return z;

}


void vertex::setx(double xval){

    x = xval;

}

void vertex::sety(double yval){

    y = yval;

}

void vertex::setz(double zval){

    z = zval;

}

void vertex::setxyz(double xval, double yval, double zval){

    x =  xval; y = yval; z = zval;

}
void vertex::addNeighbour(int v){
    neighbours.insert(v);
}
set<int> vertex::getNeighbours(){
        return neighbours;
}

