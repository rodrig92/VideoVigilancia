#ifndef GESTIONSERVER_H
#define GESTIONSERVER_H

#include <QTcpServer>

class GestionServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit GestionServer(QObject *parent = 0);

signals:

public slots:

};

#endif // GESTIONSERVER_H
