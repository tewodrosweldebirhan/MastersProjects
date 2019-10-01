#ifndef VERTEX_H
#define VERTEX_H
#include <set>
using namespace std;
class vertex{

public:
    double x;
    double y;
    double z;
    int vIndex;
    set<int> neighbours;
    vertex();
    ~vertex();

    double getx();
    double gety();
    double getz();
    void setx(double );
    void sety(double );
    void setz(double );
    void setxyz(double, double, double);
    void addNeighbour(int );
    set<int> getNeighbours();

};

#endif // VERTEX_H
