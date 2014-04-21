#ifndef CLIENTE_H
#define CLIENTE_H

#include <QMainWindow>
#include <QSettings>
#include <QtNetwork>
#include <QCamera>
#include <string>
#include <stdint.h>
#include "frames.h"

namespace Ui {
class Cliente;
}

class Cliente : public QMainWindow
{
    Q_OBJECT

public:
    explicit Cliente (QWidget* parent = 0);
    ~Cliente ();

private:
    void background (QImage& fondo);

private slots:
    void on_pushButton_clicked ();
    void capture ();
    void send_and_play (const QImage& image);
    void on_Desconectar_clicked ();

signals:
    void desconexion ();

private:
    Ui::Cliente* ui_;
    QTcpSocket* tcpSocket_;
    QHostAddress ip_;
    int16_t port_;
    Frames* frames_;
    QCamera* camera_;
    QImage fondo_;
};

#endif // CLIENTE_H
