#include "servidor.h"
#include "ui_servidor.h"

Servidor::Servidor (QWidget* parent) :
    QMainWindow (parent),
    ui_ (new Ui::Servidor),
    tcpServer_ (NULL),
    tcpSocket_ (NULL),
    confConnect_ (NULL),
    cont_ (0),
    rango_ (0),
    dataDir_ (QStandardPaths::standardLocations (QStandardPaths::DesktopLocation)),
    dirBase_ (dataDir_.at (0))
{
    QDir directory;
    directory.cdUp();
    directory.cd("VideoVigilancia");
    directory.cd("Servidor");
    QDir::setCurrent(directory.absolutePath());
    confConnect_ = new QSettings ("config.ini", QSettings::IniFormat, this);
    cont_ = confConnect_ -> value ("contador").toInt ();
    QDir::setCurrent (dirBase_.absolutePath ());
    ui_ -> setupUi (this);
}

Servidor::~Servidor ()
{
    confConnect_ -> setValue ("contador", cont_);
    delete ui_;
    if (tcpServer_ != NULL)
        delete tcpServer_;

    if (tcpSocket_ != NULL)
        delete tcpSocket_;
}

void Servidor::on_Escuchar_clicked ()
{
    if (tcpServer_ == NULL)
        tcpServer_ = new QTcpServer;
    tcpServer_ -> listen (QHostAddress::Any, confConnect_ -> value ("port").toInt ());
    qDebug () << "listening... ";
    connect (tcpServer_, SIGNAL (newConnection ()), this, SLOT (gest_connect_in ()));
}

void Servidor::gest_connect_in ()
{
    if (tcpSocket_ == NULL)
        tcpSocket_ = new QTcpSocket;
    tcpSocket_ = tcpServer_ -> nextPendingConnection ();
    qDebug () << "gestconect";
    connect (tcpSocket_, SIGNAL (readyRead ()), this, SLOT (recive_and_play ()));
}

void Servidor::recive_and_play ()
{
    qDebug () << "paqetito";
    char cab[8];
    int32_t tam;
    tcpSocket_ -> read ((char*)& tam, sizeof(int32_t));
    qDebug () << tam;

    while (!(tam != 0 && tcpSocket_ -> bytesAvailable () >= tam))
        tcpSocket_ -> waitForReadyRead();

    tcpSocket_ -> read (cab, 8);

    QString cabecera;
    cabecera.insert (0,cab);
    cabecera.resize (8);

    char* bytes = NULL;
    bytes = new char[tam];
    int leidos = tcpSocket_ -> read (bytes, tam);          //recibimos la cadena

    //transformacion de cadena a imagen
    if (cabecera.operator == ("R_O_Z_I_")) {               //si la cabecera coincide leemos datos
        QBuffer buffer;
        buffer.setData (bytes, leidos);
        QImageReader image;
        image.setDevice (&buffer);
        image.setFormat ("jpg");
        QImage imx;
        imx = image.read ();

        reconversion (imx);
        qDebug () << cabecera;
        qDebug () << bytes;
        almacenar_metadatos (imx);
        crear_BDD (cabecera);
        ui_ -> label -> setPixmap (QPixmap::fromImage (imx));

    }
    else {
        qDebug () << "no cabeza";
        ui_ -> label -> setText ("Error en la conexion INTRUSO");
        tcpServer_ -> close ();
        //emit error_connection ();
        //connect (this, SIGNAL (error_connection ()), this, SLOT (on_Escuchar_clicked ()));
    }
    if (bytes != NULL)
        delete bytes;
}

void Servidor::accept ()
{
    qDebug () << "desconectado";
    if (tcpServer_ != NULL)
        tcpServer_ = NULL;
    if (tcpSocket_ != NULL)
        tcpSocket_ = NULL;
    tcpServer_ = new QTcpServer;
    tcpServer_ -> listen (QHostAddress::Any, confConnect_ -> value ("port").toInt ());
    qDebug () << "listening... ";
    connect (tcpServer_, SIGNAL (newConnection ()), this, SLOT (gest_connect_in ()));
}

void Servidor::reconversion (QImage& imx)
{

    QImage pix;
    pix = imx;
    for (int i = 0; i < pix.height ();i++) {
        for (int j = 0;j < pix.width (); j++) {

            //  |5-7-4| --> |1-2-3|
            //  |3-8-1| --> |4-5-6|
            //  |9-6-2| --> |7-8-9|
            // TERCIO SUPERIOR
            if ((i < imx.height () / 3) && (j < imx.width () / 3))          // 1-6
                imx.setPixel (j, i, pix.pixel (imx.width () / 3 * 2 + j, imx.height () / 3 + i));

            if ((i < imx.height () / 3) && (j > imx.width () / 3) && (j < (imx.width () / 3 * 2))) // 2-9
                imx.setPixel (j, i, pix.pixel (j + imx.width () / 3, i + imx.height () / 3 * 2));

            if ((i < imx.height () / 3) && (j > (imx.width () / 3 * 2)))    // 3-4
                imx.setPixel (j, i, pix.pixel (j - imx.width () / 3 * 2, imx.height () / 3 + i));

            // TERCIO DEL MEDIO
            if ((i < imx.height () / 3 * 2) && (i > imx.height () / 3) && (j < imx.width () / 3)) // 4-3
                imx.setPixel (j, i, pix.pixel (imx.width () / 3 * 2 + j, i - imx.height () / 3));

            if ((i < imx.height () / 3 * 2) && (i > imx.height () / 3) && (j > imx.width () / 3) && (j < (imx.width () / 3 * 2)))//5-1
                imx.setPixel(j,i,pix.pixel(j-imx.width()/3,i-imx.height()/3));

            if ((i < imx.height () / 3 * 2) && (i > imx.height () / 3) && (j > (imx.width () / 3 * 2))) // 6-8
                imx.setPixel (j, i, pix.pixel (j - imx.width () / 3, i + imx.height () / 3));

            // TERCIO INFERIOR
            if ((i > imx.height () / 3 * 2) && (j < imx.width () / 3)) //7-2
                imx.setPixel (j, i, pix.pixel (j + imx.width () / 3, i - imx.height () / 3 * 2));

            if ((i > imx.height () / 3 * 2) && (j > imx.width () / 3) && (j < imx.width () / 3 * 2)) // 8-5
                imx.setPixel (j, i, pix.pixel (j, i - imx.height () / 3));

            if ((i > imx.height () / 3 * 2) && (j < imx.width ()) && (j > (imx.width () / 3 * 2))) // 9-7
                imx.setPixel (j, i, pix.pixel (j - imx.width () / 3 * 2, i));
        }
    }
}

void Servidor::almacenar_metadatos (QImage& imx)
{
    QString cont;
    QSqlQuery consulta;

    consulta.exec ("SELECT MAX(nfoto) FROM TABLE tabla");
    qDebug () << consulta.lastQuery();
    cont.setNum (cont_);
    cont.append (".jpg");

    QStringList dataDir = QStandardPaths::standardLocations (QStandardPaths::DesktopLocation);
    QDir dir (dataDir.at (0));

    // ESTABLECIENDO 100 IMAGENES POR DIRECTORIO.
    if (rango_ == 0) {
        cont.remove (QRegularExpression (".jpg"));
        dir.mkdir (cont);
        dir.cd (cont);
        QDir::setCurrent (dir.absolutePath ());
        cont.append (".jpg");
        QFile file (cont);
        file.setFileName (cont);
        file.open (QIODevice::WriteOnly);
        imx.save (cont, "jpg");
        rango_++;
    }
    else if ((rango_ > 0) && (rango_ < 3)) {
        QFile file (cont);
        file.setFileName (cont);
        file.open (QIODevice::WriteOnly);
        imx.save (cont, "jpg");
        rango_++;
    }

    else if (rango_ == 3) {
        QFile file (cont);
        file.setFileName (cont);
        file.open (QIODevice::WriteOnly);
        imx.save (cont, "jpg");
        rango_ = 0;
        dir.cdUp ();
    }
}

void Servidor::crear_BDD (const QString& cabecera)
{
    QTime temp;
    QString tiempo;
    tiempo = (temp.currentTime ().toString ());

    QSqlQuery query;
    query.prepare ("INSERT INTO tabla (cliente,tiempo)"
                   "VALUES (:cliente, :tiempo)");
    query.bindValue (":cliente", cabecera);
    query.bindValue (":tiempo", tiempo);
    query.bindValue (":Frame", cont_);
    query.exec ();

    cont_++;
}
