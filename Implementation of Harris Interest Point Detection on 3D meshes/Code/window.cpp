#include <QtWidgets>
#include <QFileDialog>
#include <QMessageBox>
#include <string>

#include "window.h"
#include "ui_window.h"

#include "myglwidget.h"

Window::Window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Window)
{
    ui->setupUi(this);
    ui->selectionHarrisCmb->addItem("Highest_Harris");
    ui->selectionHarrisCmb->addItem("Clustering");

    connect(ui->myGLWidget, SIGNAL(xRotationChanged(int)), ui->rotXSlider, SLOT(setValue(int)));
    connect(ui->myGLWidget, SIGNAL(yRotationChanged(int)), ui->rotYSlider, SLOT(setValue(int)));
    connect(ui->myGLWidget, SIGNAL(zRotationChanged(int)), ui->rotZSlider, SLOT(setValue(int)));
}

Window::~Window()
{
    delete ui;
}

void Window::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
        close();
    else
        QWidget::keyPressEvent(e);
}

void Window::on_caclulateHarrisBtn_clicked()
{
    string fname,cmbselection;
    int ringNo;
    int selectType=0;
    double const_fract,k_param;
    fname = ui->fileNameLbl->text().toLocal8Bit().constData(); //convert to string
    ringNo = ui->ringNoSpinBox->value();
    QString str = QString::number(ringNo); //fromStdString(fname);
    cmbselection = ui->selectionHarrisCmb->currentText().toLocal8Bit().constData();
    const_fract = ui->fractionConstantCmb->value();
    k_param = ui->paramKSpinBox->value();

    if(cmbselection == "Clustering")
        selectType = 1;

    ui->myGLWidget->intailizeHarris(ringNo,fname,selectType,const_fract,k_param);
}

void Window::on_chooseFileBtn_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("Select OFF file"),"C:\\Users\\Teddyw\\Documents\\DevC++\\OFF_File\\","OFF File (*.off)" );
    ui->fileNameLbl->setText(filename);
}
