#include <iostream>
#include "face.h"

face::face(){

}

face::~face(){

}

int face::getv1(){

    return verts[0];

}

int face::getv2(){

    return verts[1];

}

int face::getv3(){

    return verts[2];

}

vector<int> face::getvertices(){

    return verts;

}

void face::addvertex(int vval){

    verts.push_back(vval);

}

void face::addvertices(int v1, int v2, int v3){

    verts.push_back(v1);
    verts.push_back(v2);
    verts.push_back(v3);

}

