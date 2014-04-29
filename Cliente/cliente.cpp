#include "cliente.h"
#include "ui_cliente.h"
#include <math.h>

Cliente::Cliente (QWidget* parent) :
    QMainWindow (parent),
    ui_ (new Ui::Cliente),
    tcpSocket_ (NULL),
    confConnect_ (NULL),
    frames_ (NULL),
    camera_ (NULL),
    fondo_(NULL)

{
    QDir directory;
    directory.cdUp();
    directory.cd("VideoVigilancia");
    directory.cd("Cliente");
    QDir::setCurrent(directory.absolutePath());
    confConnect_ = new QSettings ("config.ini", QSettings::IniFormat, this);
    cnt=0;
    connect (this, SIGNAL (grabando (QImage)), &fondoCambio_, SLOT (background (QImage)));
    connect (&fondoCambio_, SIGNAL (cambio_fondo (QImage, QVector<QRect>)),
             this, SLOT (enviar (QImage, const QVector<QRect>)));
    fondoCambio_.moveToThread (&hiloFondo);
    hiloFondo.start ();

    ui_ -> setupUi (this);
}

Cliente::~Cliente ()
{
    delete ui_;
    hiloFondo.quit ();
    hiloFondo.wait ();
    if (tcpSocket_ != NULL)
        delete tcpSocket_;

    if (camera_ != NULL)
        delete camera_;

    if (frames_ != NULL)
        delete frames_;
}

void Cliente::on_pushButton_clicked ()
{
    if (camera_ != NULL) {
        camera_ -> stop ();

    }
    if (tcpSocket_ == NULL)
        tcpSocket_ = new QTcpSocket (this);
    tcpSocket_ -> connectToHost (confConnect_ -> value ("ip").toString (), confConnect_ -> value ("port").toInt ());
    connect (tcpSocket_, SIGNAL (connected ()), this, SLOT (capture ()));    //mientras no conecte no reproduce
}

void Cliente::capture ()
{
    QList<QByteArray> devices = QCamera::availableDevices ();
    /*qDebug () << "Capturando de... "
             << QCamera::deviceDescription (devices[selCam_]);*/
    if (frames_ == NULL)
        frames_ = new Frames;

    if (camera_ == NULL){
        camera_ = new QCamera;

        camera_ -> setViewfinder (frames_);
        camera_ -> setCaptureMode (QCamera::CaptureViewfinder);
        camera_ -> start ();
    }

    connect (frames_, SIGNAL (rep (const QImage&)), this, SLOT (send_and_play (const QImage&)));
}

void Cliente::send_and_play (const QImage& imx)
{  /* cnt++;
    if (fondo_.isNull () || cnt == 30) {
        fondo_ = imx;         //guardamos el fondo
        enviar ();
        cnt = 0;
    }
*/
    emit grabando (imx);

    ui_ -> label -> setPixmap (QPixmap::fromImage (imx));
}

void Cliente::enviar (const QImage& devuelta, const QVector<QRect> rectangulos)
{
    QImage aux = devuelta;
    conversion (aux);       //"cifrado"

    QBuffer buffer;

    aux.save (&buffer, "jpg");
    buffer.buffer().prepend("R_O_Z_I_");          //incluimos una cabecera

    int32_t bufferSize = buffer.size();

    tcpSocket_ -> write ((const char*)& bufferSize, sizeof (bufferSize));
    tcpSocket_ -> write (buffer.buffer ().constData (), buffer.buffer ().size ());
    tcpSocket_-> connected ();

}

void Cliente::on_Desconectar_clicked ()
{
    tcpSocket_-> disconnect ();
}

void Cliente::conversion (QImage &imx)
{
    QImage image;
    image = imx;
    for (int i = 1; i < imx.height () - 1; i++) {
        for (int j = 1; j < imx.width () - 1; j++) {

            // |1-2-3|  -->  |5-7-4|
            // |4-5-6|  -->  |3-8-1|
            // |7-8-9|  -->  |9-6-2|
            // TERCIO SUPERIOR
            if ((i < imx.height () / 3) && (j < imx.width () / 3)) // 1-5
                imx.setPixel (j, i, image.pixel (imx.width () / 3 + j, imx.height () / 3 + i));

            else if ((i < imx.height () / 3) && (j > imx.width () / 3) && (j < (imx.width () / 3 * 2))) // 2-7
                imx.setPixel (j, i, image.pixel (j - imx.width () / 3, i + imx.height () / 3 * 2));

            else if ((i < imx.height () / 3) && (j > (imx.width () / 3 * 2)))// 3-4
                imx.setPixel (j, i, image.pixel (j - imx.width () / 3 * 2, imx.height () / 3 + i));

            // TERCIO DEL MEDIO
            if ((i < imx.height () / 3 * 2) && (i > imx.height () / 3) && (j < imx.width () / 3)) // 4-3
                imx.setPixel (j, i, image.pixel (imx.width () / 3 * 2 + j, i - imx.height () / 3));

            else if ((i < imx.height () / 3 * 2) && (i > imx.height () / 3) && (j > imx.width () / 3) && (j < (imx.width () / 3 * 2)))//5-8
                imx.setPixel (j, i, image.pixel (j, imx.height () / 3 + i));

            else if ((i < imx.height () / 3 * 2) && (i > imx.height () / 3) && (j > (imx.width () / 3 * 2))) // 6-1
                imx.setPixel (j, i, image.pixel (j - imx.width () / 3 * 2, i - imx.height () / 3));

            // TERCIO INFERIOR
            if ((i > imx.height () / 3 * 2) && (j < imx.width () / 3)) //7-9
                imx.setPixel (j, i, image.pixel (j + imx.width () / 3 * 2, i));

            else if ((i > imx.height () / 3 * 2) && (j > imx.width () / 3) && (j < imx.width () / 3 * 2)) // 8-6
                imx.setPixel (j, i, image.pixel (j + imx.width () / 3, i - imx.height () / 3));

            else if ((i > imx.height () / 3 * 2) && (j < imx.width ()) && (j > (imx.width () / 3 * 2))) // 9-2
                imx.setPixel (j, i, image.pixel (j - imx.width () / 3, i - imx.height () / 3 * 2));


        }
    }

}
