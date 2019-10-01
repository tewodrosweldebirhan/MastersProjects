#ifndef FACE_H
#define FACE_H
#include <vector>
using namespace std;

class face{

    private:
//        int fIndex;

    public:
        face();
        ~face();
        int fIndex;
        vector<int> verts;
        int getv1();
        int getv2();
        int getv3();
        vector<int> getvertices();
        void addvertex(int );
        void addvertices(int, int , int );


};

#endif // FACE_H
