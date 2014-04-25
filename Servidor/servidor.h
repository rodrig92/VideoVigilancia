#ifndef SERVIDOR_H
#define SERVIDOR_H

#include <QMainWindow>
#include <QtNetwork>
#include <QImageWriter>
#include <QMovie>
#include <QSettings>
#include <string>
#include <stdint.h>
#include "SqlLite.h"

namespace Ui {
class Servidor;
}

class Servidor : public QMainWindow
{
    Q_OBJECT

public:
    explicit Servidor (QWidget* parent = 0);
    ~Servidor ();

private:
    void almacenar_metadatos (QImage& imx);
    void crear_BDD (const QString& cabecera);
    void reconversion (QImage& imx);

private slots:
    void on_Escuchar_clicked ();
    void gest_connect_in ();
    void accept ();
    void recive_and_play ();

signals:
   // void error_connection ();

private:
    Ui::Servidor* ui_;
    QTcpServer* tcpServer_;
    QTcpSocket* tcpSocket_;
    QSettings* confConnect_;
    int16_t rango_;
    int16_t cont_;
    QStringList dataDir_;
    QDir dirBase_;
};

#endif // SERVIDOR_H
