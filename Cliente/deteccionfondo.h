#ifndef DETECCIONFONDO_H
#define DETECCIONFONDO_H

#include <QObject>
#include <QMainWindow>
#include <QThread>
#include <QPainter>
#include <QVector>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/video.hpp"
#include "cvmatandqimage.h"

class DeteccionFondo : public QObject
{
private:

    cv::BackgroundSubtractorMOG2 backgroundSubtractor;

    Q_OBJECT

signals:
    void cambio_fondo (const QImage& mandada, const QVector<QRect> &rectangulos);

public slots:
     void background (const QImage& fondo);

public:
    DeteccionFondo ();

};

#endif // DETECCIONFONDO_H
