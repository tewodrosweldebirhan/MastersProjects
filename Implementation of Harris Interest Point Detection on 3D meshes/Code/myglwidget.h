#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QGLWidget>
#include <iostream>
#include <fstream>
#include "harrisoperator.h"

using namespace std;

class MyGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit MyGLWidget(QWidget *parent = 0);
    ~MyGLWidget();
    void intailizeHarris(int, string, int, double, double);
signals:

public slots:

protected:

    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    double* calculateNormal(double *, double *, double *);

public slots:

    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    void setScale(int scaler);

signals:

    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);

private:
    void draw();

    int xRot;
    int yRot;
    int zRot;
    int zoom;

    QPoint lastPos;

    int load;
    int ismousemoved;
    vector<int> interestPt_selected;
    mesh *mymesh;
    harrisoperator *myharris;
};

#endif // MYGLWIDGET_H

