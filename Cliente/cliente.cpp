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
    QImage imx;
    imx=image;
    background(imx);
    //conversion(imx);
    connect (this, SIGNAL (cambio_fondo(const QImage&)), this, SLOT (enviar (const QImage&)));
   /* QImage aux = imx;
    /*
     * aprender fondo
     * para enviar y cifrar
     *
     *
    QBuffer buffer;
    aux.save (&buffer, "jpg");
    buffer.buffer().prepend("R_O_Z_I_");          //incluimos una cabecera
    char s_z[5];
    //itoa(buffer.buffer().size(),s_z,10);
    buffer.buffer().prepend(s_z);
    tcpSocket_ -> write (buffer.buffer().constData(), buffer.buffer().size());
    tcpSocket_-> connected ();*/
    ui_ -> label -> setPixmap (QPixmap::fromImage (image));
}

void Cliente::enviar (const QImage& imx)
{

    QImage aux = imx;
    conversion (aux);
        /*
         * aprender fondo
         * para enviar y cifrar
         *
         **/
        QBuffer buffer;
        aux.save (&buffer, "jpg");
        buffer.buffer().prepend("R_O_Z_I_");          //incluimos una cabecera
        char s_z[5];
        //itoa(buffer.buffer().size(),s_z,10);
        buffer.buffer().prepend(s_z);
        tcpSocket_ -> write (buffer.buffer().constData(), buffer.buffer().size());
        tcpSocket_-> connected ();
}

void Cliente::on_Desconectar_clicked()
{
    tcpSocket_->disconnect();
}

void Cliente::conversion(QImage &imx){
    QImage image;
    image=imx;
    for(int i=1;i<imx.height()-1;i++){
        for(int j=1;j<imx.width()-1;j++){

               // |1-2-3|  -->  |5-7-4|
               // |4-5-6|  -->  |3-8-1|
               // |7-8-9|  -->  |9-6-2|
               // TERCIO SUPERIOR
               if((i<imx.height()/3)&&(j<imx.width()/3)) // 1-5
                   imx.setPixel(j,i,image.pixel(imx.width()/3+j,imx.height()/3+i));

               if((i<imx.height()/3)&&(j>imx.width()/3)&&(j<(imx.width()/3*2))) // 2-7
                   imx.setPixel(j,i,image.pixel(j-imx.width()/3,i+imx.height()/3*2));

               if((i<imx.height()/3)&&(j>(imx.width()/3*2)))// 3-4
                   imx.setPixel(j,i,image.pixel(j-imx.width()/3*2,imx.height()/3+i));

               // TERCIO DEL MEDIO
               if((i<imx.height()/3*2)&&(i>imx.height()/3)&&(j<imx.width()/3)) // 4-3
                    imx.setPixel(j,i,image.pixel(imx.width()/3*2+j,i-imx.height()/3));

               if((i<imx.height()/3*2)&&(i>imx.height()/3)&&(j>imx.width()/3)&&(j<(imx.width()/3*2)))//5-8
                     imx.setPixel(j,i,image.pixel(j,imx.height()/3+i));

              if((i<imx.height()/3*2)&&(i>imx.height()/3)&&(j>(imx.width()/3*2))) // 6-1
                    imx.setPixel(j,i,image.pixel(j-imx.width()/3*2,i-imx.height()/3));

               // TERCIO INFERIOR
               if((i>imx.height()/3*2)&&(j<imx.width()/3)) //7-9
                       imx.setPixel(j,i,image.pixel(j+imx.width()/3*2,i));

               if((i>imx.height()/3*2)&&(j>imx.width()/3)&&(j<imx.width()/3*2)) // 8-6
                       imx.setPixel(j,i,image.pixel(j+imx.width()/3,i-imx.height()/3));

               if((i>imx.height()/3*2)&&(j<imx.width())&&(j>(imx.width()/3*2))) // 9-2
                       imx.setPixel(j,i,image.pixel(j-imx.width()/3,i-imx.height()/3*2));


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

        for (ContoursType::const_iterator i = contours.begin(); i < contours.end(); ++i)
        {
            cv::Rect rect = cv::boundingRect(*i);
            QRect qrect (rect.x, rect.y, rect.width, rect.height);
            vrectangulos.push_back (qrect);
        }

        emit cambio_fondo (fondo);
}
