#ifndef CLIENTE_H
#define CLIENTE_H

#include <QMainWindow>
#include <QSettings>
#include <QtNetwork>
#include <QCamera>
#include <string>
#include <QImage>
#include "frames.h"
#include <stdint.h>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/video.hpp"
#include "cvmatandqimage.h"

namespace Ui {
class Cliente;
}

class Cliente : public QMainWindow
{
    cv::BackgroundSubtractorMOG2 backgroundSubtractor;

    Q_OBJECT

public:
    explicit Cliente (QWidget* parent = 0);
    ~Cliente ();

private slots:
    void on_pushButton_clicked ();
    void capture ();
    void send_and_play (const QImage& imx);
    void on_Desconectar_clicked ();
    void conversion (QImage& imx);
    void enviar ();

private:
    void background (QImage& fondo);
    void change_background (const QImage& img);

signals:
    void cambio_fondo ();

private:
    Ui::Cliente* ui_;
    QTcpSocket* tcpSocket_;
    QSettings* confConnect_;
    Frames* frames_;
    QCamera* camera_;
    QImage fondo_;
};

#endif // CLIENTE_H
