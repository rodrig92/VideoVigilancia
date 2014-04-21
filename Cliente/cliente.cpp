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
    if (tcpSocket_ != NULL)
        delete tcpSocket_;
    if (camera_ != NULL)
        delete camera_;
    if (frames_ != NULL)
        delete frames_;
}


void Cliente::on_pushButton_clicked ()
{
    qDebug () << "aqiiiiiiiiii";
     if (tcpSocket_ == NULL)
        tcpSocket_ = new QTcpSocket (this);
    tcpSocket_ -> connectToHost ("127.0.0.1", 5010)/*(ip_, port_)*/;

    connect (tcpSocket_, SIGNAL (connected ()), this, SLOT (capture ()));    //mientras no conecte no reproduce
}

void Cliente::capture ()
{
    QList<QByteArray> devices = QCamera::availableDevices ();
    //qDebug () << "Capturando de... ";
    //         << QCamera::deviceDescription (devices[selCam_]);*/
    if (tcpSocket_ -> state () == 0){
        qDebug () << "Estado: " << tcpSocket_ -> state ();
        emit desconexion();
        //camera_ -> stop ();
        connect (this, SIGNAL (desconexion ()), this, SLOT (on_Desconectar_clicked ()));
    }
    else {
        if (camera_ == NULL)
            camera_ = new QCamera;
        if (frames_ == NULL)
            frames_ = new Frames;
        camera_ -> setViewfinder (frames_);
        camera_ -> setCaptureMode (QCamera::CaptureViewfinder);
        camera_ -> start ();
        connect (frames_, SIGNAL (rep (const QImage&)), this, SLOT (send_and_play (const QImage&)));
    }
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
    ui_ -> label -> clear();

    if (tcpSocket_ -> state () != 0) {
        tcpSocket_ -> write (buffer.buffer().constData(), buffer.buffer().size());
        tcpSocket_ -> connected ();
       // qDebug () << "alaaaaaaaaa";
        ui_ -> label -> setPixmap (QPixmap::fromImage (image));
    }
    else {
        ui_ -> label -> setText("intentando conectar....");
        emit desconexion();
        connect (this, SIGNAL (desconexion ()), this, SLOT (on_pushButton_clicked ()));
    }

}

void Cliente::background (QImage& fondo)
{
   /* for (int i = 0; i < fondo.height (); i++)
        for (int j = 0; j < fondo.width (); j++)
*/

}

void Cliente::on_Desconectar_clicked()
{
    //tcpSocket_->disconnected();
    qDebug () << "Conexion perdida";
    camera_ -> stop ();


    /* frames_ = NULL;
    camera_ = NULL;*/
}
