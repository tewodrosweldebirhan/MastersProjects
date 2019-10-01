#include <iostream>
#include <set>
#include <iterator>
#include <algorithm> //for difference
#include <numeric> //for sum
#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Eigenvalues>
#include "harrisoperator.h"

using namespace std;


harrisoperator::harrisoperator(){

}
harrisoperator::harrisoperator(mesh msh, int rSize,int selectType,double const_fraction, double k_param){
    meshes = msh;
    ringSize = rSize;
    typeSelection = selectType;
    fraction_constant = const_fraction;
    k_parameter = k_param;
    calculateHarrisResponse();
}
harrisoperator::~harrisoperator(){

}

//calculate the neighbourhood at ring 1,
//currently not being used
set<int> harrisoperator::calculateNeighbourhood(int indexVertex, vector<face> fc){

    set<int> neighbour;
    unsigned int fcSize = fc.size(); //getting the size of the face

    //checking if the face has the vertex
    for(unsigned int i=0; i < fcSize; i++){

        if (fc[i].getv1() == indexVertex){

            neighbour.insert(fc[i].getv2());
            neighbour.insert(fc[i].getv3());

        }if (fc[i].getv2() == indexVertex){

            neighbour.insert(fc[i].getv1());
            neighbour.insert(fc[i].getv3());

        }if (fc[i].getv3() == indexVertex){

            neighbour.insert(fc[i].getv1());
            neighbour.insert(fc[i].getv2());

        }

    }
    return neighbour;
}

//calculate the neighbourhood at ring N
set<int> harrisoperator::calculateRingNeighbourhood(int indexVertex){

    set<int> s_prev, s_current, newring, s_total, s_ring, temp;
    set<int> nbhood = meshes.vertices[indexVertex].getNeighbours();

    s_prev.insert( indexVertex); //insert the index of the vertex
    s_current.insert(nbhood.begin(),nbhood.end()); //insert the neighbourhood at ring 1

    s_total.insert(nbhood.begin(),nbhood.end()); //set of all neighbours of the vertex

    //store neighbours at each ring
    set<int> *nhd = new set<int>[ringSize];
    nhd[0].insert(nbhood.begin(), nbhood.end()); // at ring 1
    set<int> set_nhd;

    for(int j=1;j<ringSize;++j){

        set<int>::iterator itr;
        for(itr=nhd[j-1].begin(); itr!=nhd[j-1].end(); ++itr){

            set_nhd = meshes.vertices[*itr].getNeighbours();
            s_ring.insert(set_nhd.begin(),set_nhd.end()); //add neighbours of each vertex at ring j-1
            set_nhd.clear();

        }


        //calculating the difference s_ring - s_current
        set_difference(s_ring.begin(), s_ring.end(), s_current.begin(), s_current.end(),inserter(temp, temp.end()));

        //calculating the difference (s_ring - s_current) - s_prev
        set_difference(temp.begin(), temp.end(), s_prev.begin(), s_prev.end(),inserter(newring, newring.end()));

        s_prev.insert(s_current.begin(), s_current.end());

        s_current.insert(s_ring.begin(), s_ring.end());
        s_ring.clear();

        // add it to the array of rings
        s_total.insert(newring.begin(), newring.end());
        nhd[j].insert(newring.begin(), newring.end());

    }

    delete[] nhd;

    return s_total;
}

//calculates the Harris reponse of each vertex
void harrisoperator::calculateHarrisResponse(){

    unsigned int vertexSize = meshes.vertices.size();

    for(unsigned int indexVertex=0; indexVertex < vertexSize; indexVertex++){ //vertexSize

        vector<double> x_coord, y_coord, z_coord;
        //caculate the neighbourhood
        set<int> set_nhd;

        //calculate the k rings neighbourhood of each vertex
        set_nhd=calculateRingNeighbourhood(indexVertex);

        set<int>::iterator itr;
        for(itr=set_nhd.begin(); itr!=set_nhd.end(); ++itr){

            //get the x,y,z coordinates
            x_coord.push_back(meshes.vertices[*itr].getx());
            y_coord.push_back(meshes.vertices[*itr].gety());
            z_coord.push_back(meshes.vertices[*itr].getz());

        }

        //adding the vertex itself to the set, the last element
        x_coord.push_back(meshes.vertices[indexVertex].getx());
        y_coord.push_back(meshes.vertices[indexVertex].gety());
        z_coord.push_back(meshes.vertices[indexVertex].getz());


        //calculate centroid of the neighbourhood Vk(v)
        unsigned int nhd_size = x_coord.size();

        double sum_x = std::accumulate(x_coord.begin(), x_coord.end(), 0.0);
        double averg_x = (double) sum_x/nhd_size;

        double sum_y = std::accumulate(y_coord.begin(), y_coord.end(), 0.0);
        double averg_y = (double) sum_y/nhd_size;

        double sum_z = std::accumulate(z_coord.begin(), z_coord.end(), 0.0);
        double averg_z = (double) sum_z/nhd_size;

        //apply PCA to get the normal of the fitting plane
        //using Eigen Library

        //translate the set of points so that centroid is on the origin
        //Matrix= 3*nhd_size

        Eigen::MatrixXd nhd_matrix(3,nhd_size);
        for ( unsigned int jj = 0; jj < nhd_size; jj++ )
        {
            //store them in Matrix
            //x_trans = x_coord - x_centroid
            nhd_matrix(0, jj) = x_coord[jj] - averg_x;
            nhd_matrix(1, jj) = y_coord[jj] - averg_y;
            nhd_matrix(2, jj) = z_coord[jj] - averg_z;
        }


        //Covariance matrix C
        // 1/n-1*X*Xt
        Eigen::Matrix3d CovM;
        CovM = (nhd_matrix * nhd_matrix.transpose())/(nhd_size-1); //creates a symmetric matrix

        // Calculate EigenVectors and EigenValues of Covaraince matrix
        Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> es(CovM); //SelfAdjointEigenSolver if the matrix is symmetric(faster)

        Eigen::MatrixXd eig_values(3,1);
        eig_values= es.eigenvalues().real(); //sorted in increasing order
        Eigen::Matrix3d principal_comps = es.eigenvectors().real();

        //sort eigenvectors in decreasing order by swaping
        Eigen::MatrixXd tmp(3,1);
        tmp = principal_comps.col(0);
        principal_comps.col(0) = principal_comps.col(2);
        principal_comps.col(2) = tmp;


        //set of points is rotated so that the normal of the fitting plane is the z-axis
        Eigen::MatrixXd rotated_points(3,nhd_size);
        rotated_points = principal_comps.transpose() * nhd_matrix;

        //translate the set of points so that the point v is in the origin of the XY-plane
        double x_vertex = rotated_points(0, nhd_size-1);
        double y_vertex = rotated_points(1, nhd_size-1);
        double z_vertex = rotated_points(2, nhd_size-1);

        Eigen::MatrixXd trans_points(3,nhd_size);
        for ( int jk = 0; jk < nhd_size; jk++ )
        {
            //trans_points = rotated_points - vertex
            trans_points(0, jk)  = rotated_points(0, jk) - x_vertex;
            trans_points(1, jk) = rotated_points(1, jk)  - y_vertex;

        }

        //fit a quadratic surface to the set of transformed points
        //z = f(x,y) =p1/2*x2 +p2*x*y + p3/2*y2 +p4*x +p5*y +p6
        Eigen::MatrixXd eqns(nhd_size, 6); // equations
        Eigen::MatrixXd bvector(nhd_size, 1);
        Eigen::MatrixXd xvector(6, 1);
        for ( int kk = 0; kk < nhd_size; kk++ )
        {
            double xv = trans_points(0, kk);
            double yv = trans_points(1, kk);
            double zv = trans_points(2, kk);

            bvector(kk,0) = zv;

            eqns(kk, 0) = (xv*xv)/2; //coefficient of p1
            eqns(kk, 1) = xv*yv; //p2
            eqns(kk, 2) = (yv*yv)/2 ; //p3
            eqns(kk, 3) = xv; //p4
            eqns(kk, 4) = yv; //p5
            eqns(kk, 5) = 1; //p6
        }

        //solve the linear system Ax=b
        xvector = eqns.colPivHouseholderQr().solve(bvector);

        //extract the solution of the linear system
        double p1 = xvector(0,0);
        double p2 = xvector(1,0);
        double p3 = xvector(2,0);
        double p4 = xvector(3,0);
        double p5 = xvector(4,0);
        double p6 = xvector(5,0);

        double A = p4*p4 + 2*p1*p1 + 2*p2*p2;
        double B = p4*p4 + 2*p2*p2 + 2*p3*p3; //difference with source code p5 = p2 =0.3..
        double C = p4*p5 + 2*p1*p2 + 2*p2*p3;

        //Harris operator value in the point v        
        double harrisV = (A*B) - (C*C) - k_parameter * ((A + B)*(A + B));
        harrisRPoints.push_back(harrisV);

    }//endforeachvertex

        //Pre-selection of the interest points
        //preserve the vertices which are local maximum
        vector<int> preselected;
        for(int nV = 0; nV < vertexSize; nV++){
            bool localMaxima = isLocalMaxima(nV);
            if(localMaxima == true){
                preselected.push_back(nV);
            }
        }

        //sort the preselected vertices, decreasing order
        sort(preselected.rbegin(),preselected.rend());

        //Selecting interest points
        vector<int> selectedVertices;

        if (typeSelection == 0){ //Highest Harris Method

            //fraction_constant
            int pre_selected_size = fraction_constant * meshes.vertices.size();
            for(int ss = 0; ss < pre_selected_size; ss++){
                selectedVertices.push_back(preselected[ss]);
            }

        }else{
            //clustering selection, source:from Harriscode
            int selected_size = preselected.size();
            for(int sg = 0; sg < selected_size; sg++){

                bool found = false;
                int j = 0;
                while(j < selectedVertices.size() && !found){
                    double difX = meshes.vertices[selectedVertices[j]].getx() - meshes.vertices[preselected[sg]].getx();
                    double difY = meshes.vertices[selectedVertices[j]].gety() - meshes.vertices[preselected[sg]].gety();
                    double difZ = meshes.vertices[selectedVertices[j]].getz() - meshes.vertices[preselected[sg]].getz();

                    if(sqrt(difX*difX + difY*difY + difZ*difZ) < (fraction_constant * meshes.diagValue))
                        found = true;
                    j++;
                }
                if(!found){
                    selectedVertices.push_back(preselected[sg]);
                }
            }
        }
        selectedVrts = selectedVertices;
        cout<<"Ending...."<<endl;


} //endfunction

//To check whether a vertex is a local maximum or not
bool harrisoperator::isLocalMaxima(unsigned int vertexIndex){


    set<int> nhd = meshes.vertices[vertexIndex].getNeighbours();
    set<int>::iterator itrr;
    for(itrr=nhd.begin(); itrr!=nhd.end(); ++itrr){
        if(harrisRPoints[vertexIndex] < harrisRPoints[*itrr]){
            return false;
        }
    }
    return true;
}
