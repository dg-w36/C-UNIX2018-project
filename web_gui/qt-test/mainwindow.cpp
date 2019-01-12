#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update_img() {
    QImage img =QImage((const unsigned char*)(tmp_img), 960,400,3*960, QImage::Format_RGB888);
    QGraphicsScene *scene = new QGraphicsScene;
    scene->addPixmap(QPixmap::fromImage(img));
    ui->image->setScene(scene);
    //ui->graphicsView->resize(image.width() + 5, image.height() + 5);
    ui->image->show();
}

void MainWindow::set_Qimage_buffer(uchar * pt) {
     tmp_img = pt;
}
