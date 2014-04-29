#ifndef CLIENTE_H
#define CLIENTE_H

#include <QMainWindow>
#include <QSettings>
#include <QtNetwork>
#include <QCamera>
#include <string>
#include <QImage>
#include <stdint.h>
#include "frames.h"
#include "deteccionfondo.h"



namespace Ui {
class Cliente;
}

class Cliente : public QMainWindow
{
    //cv::BackgroundSubtractorMOG2 backgroundSubtractor;

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
    void enviar (const QImage& devuelta, const QVector<QRect> rectangulos);
signals:
    void grabando (const QImage& imx);
private:
    Ui::Cliente* ui_;
    QTcpSocket* tcpSocket_;
    QSettings* confConnect_;
    Frames* frames_;
    QCamera* camera_;
    QImage fondo_;
    DeteccionFondo fondoCambio_;
    QThread hiloFondo;
    int cnt;
};

#endif // CLIENTE_H
