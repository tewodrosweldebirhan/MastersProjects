#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include "mesh.h"

mesh::mesh(){

}
mesh::mesh(const char* filename){

       //load vertices and faces
       readOFF_File(filename);

        /*
         * To read tri and vert files
         *
        string file1="C:\\Users\\Teddyw\\Documents\\DevC++\\centaur1.vert";
        string file2="C:\\Users\\Teddyw\\Documents\\DevC++\\centaur1.tri";
        readFile_Intialize(file1.c_str(),file2.c_str());
        */

       //the diagonal of the bounding box, used for clustering
       //source: HarrisCode
       double xmin=0.0, xmax=0.0, ymin=0.0, ymax=0.0, zmin=0.0, zmax=0.0;

       for(int k = 0; k < vertices.size(); k++){

            if(vertices[k].getx() < xmin)
                xmin = vertices[k].getx();
            else if(vertices[k].getx() > xmax)
                xmax = vertices[k].getx();

            if(vertices[k].gety() < ymin)
                ymin = vertices[k].gety();
            else if(vertices[k].gety() > ymax)
                ymax = vertices[k].gety();

            if(vertices[k].getz() < zmin)
                zmin = vertices[k].getz();
            else if(vertices[k].getz() > zmax)
                zmax = vertices[k].getz();
       }

       diagValue = sqrt((xmax - xmin)*(xmax - xmin) + (ymax - ymin)*(ymax - ymin) + (zmax - zmin)*(zmax - zmin));

       cout<<"Starting"<<endl;

}
mesh::mesh(const char* filename_vert, const char* filename_tri){

       //load vertices and faces
       readFile_Intialize(filename_vert,filename_tri);

}
mesh::~mesh(){

}

//To read and load tri and vert files
bool mesh::readFile_Intialize(const char* filevert,const char* filetri)
{


    ifstream file_vert;
    file_vert.open (filevert);
    ifstream file_tri;
    file_tri.open (filetri);

    if (!(file_vert.is_open() && file_tri.is_open())) {
        cout<<"Unable to open file"<<endl;
        return false;
    }

    //load vertices first

    double xc,yc,zc;
    int idV=0;
    while (file_vert >> xc)
    {
        vertex v;
        file_vert>>yc>>zc;
        v.vIndex=idV;
        v.setxyz(xc,yc,zc);

        vertices.push_back(v);

        ++idV;

    }


    //then load faces

    int vt1,vt2,vt3;
    int idF=0;
    while (file_tri >> vt1)
    {
        face f;
        file_tri>>vt2>>vt3;

        f.fIndex=idF;
        vt1 = vt1 - 1;
        vt2 = vt2 - 1;
        vt3 = vt3 - 1;
        f.addvertices(vt1,vt2,vt3);

        //create 1st ring neighbourhood, credit to source code
        vertices[vt1].addNeighbour(vt2);    vertices[vt1].addNeighbour(vt3);
        vertices[vt2].addNeighbour(vt1);    vertices[vt2].addNeighbour(vt3);
        vertices[vt3].addNeighbour(vt2);    vertices[vt3].addNeighbour(vt2);

        faces.push_back(f);

        ++idF;

    }

    file_vert.close();
    file_tri.close();

    return true;

}

//reading OFF file, credit to the source code
bool mesh::readOFF_File(const char* filename){

    int numEdges, numVertices, numFaces;;

    ifstream in(filename);

    if (!(in.is_open())) {
        cout<<"Unable to open file"<<endl;
        return false;
    }

    string format;
    getline(in, format);

    in>>numVertices>>numFaces>>numEdges;
    skipline(in);


    for(int i = 0; i < numVertices; i++){
        double xc, yc, zc;
        in>>xc>>yc>>zc;
        skipline(in);
        vertex v;
        v.vIndex=i;
        v.setxyz(xc,yc,zc);

        vertices.push_back(v);
    }


    for(int i = 0;  i < numFaces; i++){
        int numVert;
        in >> numVert;

        int vt1, vt2, vt3;

        in>>vt1>>vt2>>vt3;
        skipline(in);

        face f;
        f.fIndex=i;
        f.addvertices(vt1,vt2,vt3);

        //create 1st ring neighbourhood, credit to source code
        vertices[vt1].addNeighbour(vt2);    vertices[vt1].addNeighbour(vt3);
        vertices[vt2].addNeighbour(vt1);    vertices[vt2].addNeighbour(vt3);
        vertices[vt3].addNeighbour(vt2);    vertices[vt3].addNeighbour(vt2);

        faces.push_back(f);

    }

    in.close();
    return true;

}

void mesh::skipline(istream &in){
    char c;
    while(in>>noskipws>>c && c!='\n');
    in>>skipws;
}
