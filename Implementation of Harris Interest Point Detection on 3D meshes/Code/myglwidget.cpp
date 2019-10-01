// myglwidget.cpp

#include <QtWidgets>
#include <QtOpenGL>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "math.h"


#include "myglwidget.h"

using namespace std;

MyGLWidget::MyGLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    xRot = 0;
    yRot = 0;
    zRot = 0;
    zoom = 1;
    load = 0;
    ismousemoved = 0;

    mymesh = NULL;
    myharris = NULL;


}

MyGLWidget::~MyGLWidget()
{
    if(mymesh){
        delete[] mymesh;
        mymesh = NULL;
    }
    if(myharris){
        delete[] myharris;
        myharris = NULL;
    }


}

QSize MyGLWidget::minimumSizeHint() const
{
    return QSize(80, 80);
}

QSize MyGLWidget::sizeHint() const
{
    return QSize(400, 400);
}

static void qNormalizeAngle(int &angle)
{    
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360)
        angle -= 360 * 16;
}

void MyGLWidget::setScale(int scaler)
{

    if (zoom != scaler) {
        zoom = scaler;
        updateGL();
    }
}

void MyGLWidget::setXRotation(int angle)
{    
    qNormalizeAngle(angle);

    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}


void MyGLWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;

        emit yRotationChanged(angle);
        updateGL();
    }
}

void MyGLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}

void MyGLWidget::initializeGL()
{

    qglClearColor(Qt::black);


    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);

    glShadeModel(GL_SMOOTH);

    //enable automatic normalization of normals, if scale != 1
    glEnable(GL_RESCALE_NORMAL);

    glEnable(GL_NORMALIZE);

    //To change material color
    glColorMaterial(GL_FRONT, GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_LIGHTING);

    glEnable(GL_LIGHT0);

    GLfloat light_ambient[] = { 0.0, 0.0, 1.0, 1.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
}

void MyGLWidget::paintGL()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    glTranslatef(0.0, 0.0, -10.0);

    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);


    glScalef(zoom,zoom,zoom);

    draw();
}

void MyGLWidget::resizeGL(int width, int height)
{

    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();


#ifdef QT_OPENGL_ES_1
    glOrthof(-2, +2, -2, +2, 1.0, 15.0);
#else
    glOrtho(-2, +2, -2, +2, 1.0, 15.0);
#endif

    glMatrixMode(GL_MODELVIEW);
}

void MyGLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 8 * dy);
        setYRotation(yRot + 8 * dx);        
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot + 8 * dy);
        setZRotation(zRot + 8 * dx);
    }

    lastPos = event->pos();
}

void MyGLWidget::draw()
{

    if(load == 1){
        unsigned int faceSize = mymesh->faces.size();

        double *normVector;
        //if its value is too large use minimzer = 100
        double dd = 100.0;
        double tempv = (mymesh->vertices[0].getx() + mymesh->vertices[1].getx() + mymesh->vertices[2].getx())/3;
         if(tempv <=1 && tempv >= -1)//if not normalized
             dd = 1.0;

        glBegin(GL_TRIANGLES);
        for(unsigned int j=0; j<faceSize; j++){
            unsigned int vt1 = mymesh->faces[j].getv1();
            unsigned int vt2 = mymesh->faces[j].getv2();
            unsigned int vt3 = mymesh->faces[j].getv3();

            double v1x = mymesh->vertices[vt1].getx();
            double v1y = mymesh->vertices[vt1].gety();
            double v1z = mymesh->vertices[vt1].getz();

            double v2x = mymesh->vertices[vt2].getx();
            double v2y = mymesh->vertices[vt2].gety();
            double v2z = mymesh->vertices[vt2].getz();

            double v3x = mymesh->vertices[vt3].getx();
            double v3y = mymesh->vertices[vt3].gety();
            double v3z = mymesh->vertices[vt3].getz();

            double vert1[] = {v1x, v1y, v1z};
            double vert2[] = {v2x, v2y, v2z};
            double vert3[] = {v3x, v3y, v3z};


            normVector = calculateNormal(vert1, vert2, vert3);


            glNormal3d( *normVector, *(normVector+1), *(normVector+2));

            glColor3f(0.0f, 1.0f, 0.0f);
            glVertex3d(v1x/dd, v1y/dd, v1z/dd); //one vertex
            glVertex3d(v2x/dd, v2y/dd, v2z/dd); //one vertex
            glVertex3d(v3x/dd, v3y/dd, v3z/dd);

        }
        glEnd();

        glPointSize(5*zoom);
        glBegin(GL_POINTS);
        for(unsigned int j=0; j<interestPt_selected.size(); j++){

            double v1x = mymesh->vertices[interestPt_selected[j]].getx();
            double v1y = mymesh->vertices[interestPt_selected[j]].gety();
            double v1z = mymesh->vertices[interestPt_selected[j]].getz();

            glColor3f(1.0f, 0.0f, 0.0f);
            glNormal3d(0.181636,-0.25,0.951057);
            glVertex3d(v1x/dd, v1y/dd, v1z/dd);
        }       
        glEnd();

        delete[] normVector;

    }


}


double* MyGLWidget::calculateNormal(double v1[], double v2[], double v3[]){

    double x[3];
    x[0]=  v2[0]-v1[0];
    x[1] = v2[1]-v1[1];
    x[2] = v2[2]-v1[2]; //v2 -21
    double y[3] = {v3[0]-v1[0], v3[1]-v1[1], v3[2]-v1[2]}; //v3 -21

    //cross product of x and y
    double crp[3] =  { (x[1]*y[2]) - (x[2]*y[1]), -1 * (x[0]*y[2]) + (x[2]*y[0]), (x[0]*y[1]) - (x[1]*y[0]) };

    //normalize
    double norm= sqrt(crp[2]*crp[2] + crp[1]*crp[1] + crp[0]*crp[0]);

    double *result=new double[3];
    result[0] = crp[0] / norm;
    result[1] = crp[1] / norm;
    result[2] = crp[2] / norm;

    return result;
}
void MyGLWidget::intailizeHarris(int ringSize, string filename, int selectType, double const_fraction,double k_param){

    mymesh = new mesh(filename.c_str());
    myharris = new harrisoperator(*mymesh,ringSize, selectType,const_fraction,k_param);
    interestPt_selected = myharris->selectedVrts;
    load = 1;
    updateGL();
}
