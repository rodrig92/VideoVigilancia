#include "servidor.h"
#include "ui_servidor.h"

Servidor::Servidor (QWidget* parent) :
    QMainWindow (parent),
    ui_ (new Ui::Servidor),
    tcpServer_ (NULL),
    tcpSocket_ (NULL),
    port_ (5010),
    ip_ ("127.0.0.1")
{
    ui_ -> setupUi (this);
}

Servidor::~Servidor ()
{
    delete ui_;
    if (tcpServer_ != NULL)
        delete tcpServer_;
    if (tcpSocket_ != NULL)
        delete tcpSocket_;
}

void Servidor::on_Escuchar_clicked ()
{
    tcpServer_ = new QTcpServer;
    tcpServer_ -> listen (ip_, port_);
    qDebug () << "listening... ";
    connect (tcpServer_, SIGNAL (newConnection ()), this, SLOT (gest_connect_in ()));
}

void Servidor::gest_connect_in ()
{
    tcpSocket_ = new QTcpSocket;
    tcpSocket_ = tcpServer_ -> nextPendingConnection ();
    connect (tcpSocket_, SIGNAL (readyRead ()), this, SLOT (recive_and_play ()));
}

void Servidor::recive_and_play ()
{
    char tamanyo[5];
    char cabecera[8];
   /*char* cabecera = NULL;
    cabecera = new char[8];*/
    tcpSocket_-> read(tamanyo,5);
    tcpSocket_ -> read (cabecera, 8);
    char* bytes = NULL;
        int sz = tcpSocket_ -> bytesAvailable ();           //leemos tamanio de la imagen
        bytes = new char[sz];
        int leidos = tcpSocket_ -> read (bytes, sz);          //recibimos la cadena

    if((atoi(tamanyo)<100000)||(atoi(tamanyo)>0)){
        //transformacion de cadena a imagen
        QBuffer buffer;
        buffer.setData (bytes, leidos);
        QImageReader image;
        image.setDevice (&buffer);
        image.setFormat ("jpg");
    //if (strcmp (cabecera, "R_O_Z_I_") == 0) {               //si la cabecera coincide leemos datos
        QImage pix;
        pix = image.read ();
        QImage imx;
        imx=pix;

                for(int i=0;i<pix.height();i++){
                    for(int j=0;j<pix.width();j++){

                           //  |5-7-4| --> |1-2-3|
                           //  |3-8-1| --> |4-5-6|
                           //  |9-6-2| --> |7-8-9|
                           // TERCIO SUPERIOR
                           if((i<imx.height()/3)&&(j<imx.width()/3)) // 1-6
                               imx.setPixel(j,i,pix.pixel(imx.width()/3*2+j,imx.height()/3+i));

                           if((i<imx.height()/3)&&(j>imx.width()/3)&&(j<(imx.width()/3*2))) // 2-9
                               imx.setPixel(j,i,pix.pixel(j+imx.width()/3,i+imx.height()/3*2));

                           if((i<imx.height()/3)&&(j>(imx.width()/3*2)))// 3-4
                               imx.setPixel(j,i,pix.pixel(j-imx.width()/3*2,imx.height()/3+i));

                           // TERCIO DEL MEDIO
                           if((i<imx.height()/3*2)&&(i>imx.height()/3)&&(j<imx.width()/3)) // 4-3
                                imx.setPixel(j,i,pix.pixel(imx.width()/3*2+j,i-imx.height()/3));

                           if((i<imx.height()/3*2)&&(i>imx.height()/3)&&(j>imx.width()/3)&&(j<(imx.width()/3*2)))//5-1
                                 imx.setPixel(j,i,pix.pixel(j-imx.width()/3,i-imx.height()/3));

                          if((i<imx.height()/3*2)&&(i>imx.height()/3)&&(j>(imx.width()/3*2))) // 6-8
                                imx.setPixel(j,i,pix.pixel(j-imx.width()/3,i+imx.height()/3));

                           // TERCIO INFERIOR
                           if((i>imx.height()/3*2)&&(j<imx.width()/3)) //7-2
                                   imx.setPixel(j,i,pix.pixel(j+imx.width()/3,i-imx.height()/3*2));

                           if((i>imx.height()/3*2)&&(j>imx.width()/3)&&(j<imx.width()/3*2)) // 8-5
                                   imx.setPixel(j,i,pix.pixel(j,i-imx.height()/3));

                           if((i>imx.height()/3*2)&&(j<imx.width())&&(j>(imx.width()/3*2))) // 9-7
                                   imx.setPixel(j,i,pix.pixel(j-imx.width()/3*2,i));


                   }
                }
        ui_ -> label -> setPixmap (QPixmap::fromImage (imx));
    //}
  /* else {
        qDebug () << "no cabeza";
        ui_ -> label -> setText ("Error en la conexion");
        //tcpServer_ -> close ();
        //emit error_connection ();
        //connect (this, SIGNAL (error_connection ()), this, SLOT (on_Escuchar_clicked ()));
    }*/
    if (bytes != NULL)
        delete bytes;
   // if (tcpSocket_->atEnd())
    /*if (cabecera != NULL)
        delete cabecera;*/
    connect(tcpSocket_, SIGNAL (disconnected()), this, SLOT (accept ()));
    }
}

void Servidor::accept(){
    qDebug () << "desconectado";
    if (tcpServer_ != NULL)
        tcpServer_ = NULL;
    if (tcpSocket_ != NULL)
        tcpSocket_ = NULL;
    tcpServer_ = new QTcpServer;
    tcpServer_ -> listen (ip_, port_);
    qDebug () << "listening... ";
    connect (tcpServer_, SIGNAL (newConnection ()), this, SLOT (gest_connect_in ()));
}
