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
    conversion(imx);

    QImage aux = imx;
    /*
     * aprender fondo
     * para enviar y cifrar
     *
     * */
    QBuffer buffer;
    aux.save (&buffer, "jpg");
    buffer.buffer().prepend("R_O_Z_I_");          //incluimos una cabecera
    char s_z[5];
    itoa(buffer.buffer().size(),s_z,10);
    buffer.buffer().prepend(s_z);
    tcpSocket_ -> write (buffer.buffer().constData(), buffer.buffer().size());
    tcpSocket_-> connected ();
    ui_ -> label -> setPixmap (QPixmap::fromImage (aux));
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
