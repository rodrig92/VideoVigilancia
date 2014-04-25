#include "cliente.h"
#include "ui_cliente.h"

Cliente::Cliente (QWidget* parent) :
    QMainWindow (parent),
    ui_ (new Ui::Cliente),
    tcpSocket_ (NULL),
    confConnect_ (NULL),
    frames_ (NULL),
    camera_ (NULL),
    backgroundSubtractor(500, 16, false),
    fondo_(NULL)

{
    QDir directory;
    directory.cdUp();
    directory.cd("VideoVigilancia");
    directory.cd("Cliente");
    QDir::setCurrent(directory.absolutePath());
    confConnect_ = new QSettings ("config.ini", QSettings::IniFormat, this);

    ui_ -> setupUi (this);
    // backgroundSubtractor.nmixtures = 3;
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
{
    if (fondo_.isNull ()) {
        fondo_ = imx;         //guardamos el fondo
        enviar ();
    }


 //   change_background (imx);
 //   connect (this, SIGNAL (cambio_fondo()), this, SLOT (enviar ()));
    // connect (tcpSocket_, SIGNAL (stateChanged(QAbstractSocket::SocketState)), this, SLOT (on_pushButton_clicked ()));

    ui_ -> label -> setPixmap (QPixmap::fromImage (imx));
}

void Cliente::enviar ()
{
    QImage aux = fondo_;
    conversion (aux);       //"cifrado"
    /*
         * aprender fondo
         * para enviar y cifrar
         *
         **/
    QBuffer buffer;

    aux.save (&buffer, "jpg");
    buffer.buffer().prepend("R_O_Z_I_");          //incluimos una cabecera

    int32_t bufferSize = buffer.size();

    tcpSocket_ -> write ((const char*)& bufferSize, sizeof (bufferSize));
    tcpSocket_ -> write (buffer.buffer ().constData (), buffer.buffer ().size ());
    tcpSocket_-> connected ();

}

void Cliente::change_background (const QImage& img)
{
    int cnt = 0;
    for (int i = 0; i < fondo_.height (); i++)
        for (int j = 0; j < fondo_.width (); j++)
            if ((img.pixel (j,i) <= (fondo_.pixel (j, i) + 10000000)) && (img.pixel (j,i) >= (fondo_.pixel (j, i) - 10000000)))
                cnt--;
            else
                cnt++;

    if (cnt > 0) {
        fondo_ = img;
        qDebug () << cnt;
        emit cambio_fondo ();
    }
    // qDebug () << cnt;
    qDebug () << img.pixel (156, 32);
    qDebug () << fondo_.pixel (156, 32);


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

void Cliente::background (QImage& fondo)
{

    typedef QVector<cv::Mat> ImagesType;
    typedef std::vector<std::vector<cv::Point> > ContoursType;

    cv::Mat recibidamat = QtOcv::image2Mat(fondo,CV_8UC(3));

    // std::vector<cv::Mat> images = <vector de imágenes en cv::Mat>

    // Sustracción del fondo:
    //  1. El objeto sustractor compara la imagen en i con su
    //     estimación del fondo y devuelve en foregroundMask una
    //     máscara (imagen binaria) con un 1 en los píxeles de
    //     primer plano.
    //  2. El objeto sustractor actualiza su estimación del fondo
    //     usando la imagen en i.
    //  for (ImagesTypes::const_iterator i = images.begin(); i < images.end(); ++i) {
    cv::Mat foregroundMask;
    backgroundSubtractor(recibidamat, foregroundMask);

    // Operaciones morfolóficas para eliminar las regiones de
    // pequeño tamaño. Erode() las encoge y dilate() las vuelve a
    // agrandar.
    cv::erode(foregroundMask, foregroundMask, cv::Mat());
    cv::dilate(foregroundMask, foregroundMask, cv::Mat());

    // Obtener los contornos que bordean las regiones externas
    // (CV_RETR_EXTERNAL) encontradas. Cada contorno es un vector
    // de puntos y se devuelve uno por región en la máscara.
    ContoursType contours;
    cv::findContours(foregroundMask, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, cv::Point(0,0));

    // Aquí va el código ódigo que usa los contornos encontrados...
    // P. ej. usar cv::boundingRect() para obtener el cuadro
    // delimitador de cada uno y pintarlo en la imagen original

    qRegisterMetaType < QVector<QRect> >("QVector<QRect>");

    QVector<QRect> vrectangulos;
    vrectangulos.clear();
    /*
        for (ContoursType::const_iterator i = contours.begin(); i < contours.end(); ++i)
        {
            cv::Rect rect = cv::boundingRect(*i);
            QRect qrect (rect.x, rect.y, rect.width, rect.height);
            vrectangulos.push_back (qrect);
        }
*/
    emit cambio_fondo ();
}
