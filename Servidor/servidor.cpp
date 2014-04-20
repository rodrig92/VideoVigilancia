#include "servidor.h"
#include "ui_servidor.h"

Servidor::Servidor (QWidget* parent) :
    QMainWindow (parent),
    ui_ (new Ui::Servidor),
    tcpServer_ (NULL),
    tcpSocket_ (NULL),
    port_ (5010),
    ip_ ("127.0.0.1")
{
    ui_ -> setupUi (this);
}

Servidor::~Servidor ()
{
    delete ui_;
    if (tcpServer_ != NULL)
        delete tcpServer_;
    if (tcpSocket_ != NULL)
        delete tcpSocket_;
}

void Servidor::on_Escuchar_clicked ()
{
    tcpServer_ = new QTcpServer;
    tcpServer_ -> listen (ip_, port_);
    qDebug () << "listening... ";
    connect (tcpServer_, SIGNAL (newConnection ()), this, SLOT (gest_connect_in ()));
}

void Servidor::gest_connect_in ()
{
    qDebug ()<< "gest_conm";
    tcpSocket_ = new QTcpSocket;
    tcpSocket_ = tcpServer_ -> nextPendingConnection ();
    connect (tcpSocket_, SIGNAL (readyRead ()), this, SLOT (recive_and_play ()));
}

void Servidor::recive_and_play ()
{

    char cabecera[8];
    /*char* cabecera = NULL;
    cabecera = new char[8];*/
    tcpSocket_ -> read (cabecera, 8);
    qDebug () << cabecera;
    char* bytes = NULL;

        int sz = tcpSocket_ -> bytesAvailable ();           //leemos tamanio de la imagen
        bytes = new char[sz];
        int leidos = tcpSocket_ -> read (bytes, sz);          //recibimos la cadena

        //transformacion de cadena a imagen
        QBuffer buffer;
        buffer.setData (bytes, leidos);
        QImageReader image;
        image.setDevice (&buffer);
        image.setFormat ("jpg");
    if (strcmp (cabecera, "R_O_Z_I_") == 0) {               //si la cabecera coincide leemos datos
        QImage pix;
        pix = image.read ();
        ui_ -> label -> setPixmap (QPixmap::fromImage (pix));
    }
   else {
        qDebug () << "no cabeza";
        ui_ -> label -> setText ("Error en la conexion");
        //tcpServer_ -> close ();
        //emit error_connection ();
        //connect (this, SIGNAL (error_connection ()), this, SLOT (on_Escuchar_clicked ()));
    }
    if (bytes != NULL)
        delete bytes;
    if (tcpSocket_->atEnd())
        qDebug () << "AQII";
    /*if (cabecera != NULL)
        delete cabecera;*/
    connect(tcpSocket_, SIGNAL (disconnected()), this, SLOT (accept ()));

}

void Servidor::accept(){
    qDebug () << "desconectado";
    if (tcpServer_ != NULL)
        tcpServer_ = NULL;
    if (tcpSocket_ != NULL)
        tcpSocket_ = NULL;
    tcpServer_ = new QTcpServer;
    tcpServer_ -> listen (ip_, port_);
    qDebug () << "listening... ";
    connect (tcpServer_, SIGNAL (newConnection ()), this, SLOT (gest_connect_in ()));
}
