#ifndef CLIENTE_H
#define CLIENTE_H

#include <QMainWindow>
#include <QSettings>
#include <QtNetwork>
#include <QCamera>
#include <string>
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
    void send_and_play (const QImage& image);
    void on_Desconectar_clicked();
    void conversion(QImage &imx);
    void enviar (const QImage& imx);

private:
    void background (QImage& fondo);

signals:
    void cambio_fondo (const QImage& fondo);

private:
    Ui::Cliente* ui_;
    QTcpSocket* tcpSocket_;
    QHostAddress ip_;
    int16_t port_;
    Frames* frames_;
    QCamera* camera_;
};

#endif // CLIENTE_H
