#include "deteccionfondo.h"
#include <QtDebug>

DeteccionFondo::DeteccionFondo () : backgroundSubtractor(500, 16, false)
{
   // backgroundSubtractor.nmixtures = 3;
}

void DeteccionFondo::background (const QImage& fondo)
{

    typedef QVector<cv::Mat> ImagesType;
    typedef std::vector<std::vector<cv::Point> > ContoursType;

    cv::Mat recibidamat = QtOcv::image2Mat (fondo, CV_8UC(3));

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
    backgroundSubtractor (recibidamat, foregroundMask);

    // Operaciones morfolóficas para eliminar las regiones de
    // pequeño tamaño. Erode() las encoge y dilate() las vuelve a
    // agrandar.
    cv::erode (foregroundMask, foregroundMask, cv::Mat());
    cv::dilate (foregroundMask, foregroundMask, cv::Mat());

    // Obtener los contornos que bordean las regiones externas
    // (CV_RETR_EXTERNAL) encontradas. Cada contorno es un vector
    // de puntos y se devuelve uno por región en la máscara.
    ContoursType contours;
    cv::findContours (foregroundMask, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, cv::Point(0,0));

    // Aquí va el código ódigo que usa los contornos encontrados...
    // P. ej. usar cv::boundingRect() para obtener el cuadro
    // delimitador de cada uno y pintarlo en la imagen original

    qRegisterMetaType < QVector<QRect> >("QVector<QRect>");

    QVector<QRect> vrectangulos;
    vrectangulos.clear ();

        for (ContoursType::const_iterator i = contours.begin(); i < contours.end(); ++i)
        {
            cv::Rect rect = cv::boundingRect (*i);
            QRect qrect (rect.x, rect.y, rect.width, rect.height);
            vrectangulos.push_back (qrect);
        }
    if (vrectangulos.size () != 0) {
        emit cambio_fondo (fondo, vrectangulos);
        qDebug () << "rect " << vrectangulos.size ();
    }
}
