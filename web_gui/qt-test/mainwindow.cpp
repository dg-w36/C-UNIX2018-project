#include "mainwindow.h"
#include "ui_mainwindow.h"

int start_socket(QString data);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->connect_socket, SIGNAL(clicked()), this, SLOT(MainWindow::socket_connet));
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
    ui->image->resize(img.width() + 5, img.height() + 5);
    ui->image->show();
}

void MainWindow::set_Qimage_buffer(uchar * pt) {
     tmp_img = pt;
}

int MainWindow::socket_connect() {
    QString data = ui->server_ip->toPlainText();
    return start_socket(data);
}

int start_socket(QString data) {
    int sock;
    struct sockaddr_in srv;
    sock = socket(AF_INET, SOCK_STREAM, 0);

    srv.sin_addr.s_addr = inet_addr(data.toLatin1().data());
    srv.sin_family = AF_INET;
    srv.sin_port = htons(18888);

    connect(sock, (struct sockaddr *)&srv, sizeof(srv));
    send(sock, "start", 5, 0);

    return sock;
}
