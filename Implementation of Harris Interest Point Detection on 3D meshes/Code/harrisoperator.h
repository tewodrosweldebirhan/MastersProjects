#ifndef HARRISOPERATOR_H
#define HARRISOPERATOR_H
#include <iostream>
#include <vector>
#include <set>
#include "face.h"
#include "vertex.h"
#include "mesh.h"

using namespace std;


class harrisoperator{
private:
    int ringSize;
    vector<double> harrisRPoints;// save the harrisresponse of each vertex

    mesh meshes;
public:
    harrisoperator();
    harrisoperator(mesh, int ringSize=5,int selectType=0,double const_fraction=0.01,double k_param=0.04);
    ~harrisoperator();

    int typeSelection;
    double fraction_constant;
    double k_parameter;
    vector<int> selectedVrts;
    set<int> calculateNeighbourhood(int, vector<face>);
    set<int> calculateRingNeighbourhood(int); //neighbourhood considering k rings
    void calculateHarrisResponse();
    bool isLocalMaxima(unsigned int);

};

#endif // HARRISOPERATOR_H
