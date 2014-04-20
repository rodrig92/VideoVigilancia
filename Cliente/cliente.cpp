#include "cliente.h"
#include "ui_cliente.h"

Cliente::Cliente (QWidget* parent) :
    QMainWindow (parent),
    ui_ (new Ui::Cliente),
    tcpSocket_ (NULL),
    ip_ ("0.0.0.0"),
    port_ (-1),
    frames_ (NULL),
    camera_ (NULL)
{
    ui_ -> setupUi (this);
}

Cliente::~Cliente ()
{
    delete ui_;
    if (tcpSocket_ != NULL) {
        delete tcpSocket_;
        qDebug () << "Borrado tcpSo";
    }
    if (camera_ != NULL) {
        delete camera_;
        qDebug () << "Borrado cama";
    }
    if (frames_ != NULL)
        delete frames_;
}


void Cliente::on_pushButton_clicked ()
{
    tcpSocket_ = new QTcpSocket (this);
    tcpSocket_ -> connectToHost ("127.0.0.1", 5010)/*(ip_, port_)*/;
    connect (tcpSocket_, SIGNAL (connected ()), this, SLOT (capture ()));    //mientras no conecte no reproduce
}

void Cliente::capture ()
{
    QList<QByteArray> devices = QCamera::availableDevices ();
    /*qDebug () << "Capturando de... "
             << QCamera::deviceDescription (devices[selCam_]);*/
    camera_ = new QCamera;
    frames_ = new Frames;
    camera_ -> setViewfinder (frames_);
    camera_ -> setCaptureMode (QCamera::CaptureViewfinder);
    camera_ -> start ();
    connect (frames_, SIGNAL (rep (const QImage&)), this, SLOT (send_and_play (const QImage&)));
}

void Cliente::send_and_play (const QImage& image)
{
    QImage aux = image;
    /*
     * aprender fondo
     * para enviar y cifrar
     *
     * */
    QBuffer buffer;
    aux.save (&buffer, "jpg");
    buffer.buffer().prepend("R_O_Z_I_");          //incluimos una cabecera

    tcpSocket_ -> write (buffer.buffer().constData(), buffer.buffer().size());
    tcpSocket_-> connected ();
    ui_ -> label -> setPixmap (QPixmap::fromImage (image));
}

void Cliente::on_Desconectar_clicked()
{
    tcpSocket_->disconnected();
}
