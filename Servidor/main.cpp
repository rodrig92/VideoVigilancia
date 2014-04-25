#include "servidor.h"
#include <QApplication>
#include "SqlLite.h"

int main (int argc, char* argv[])
{
    QApplication a (argc, argv);
    Servidor w;
    //Si no puede realizar la conexión finaliza el programa
     if (!createConnection())
           return 1;

    w.show ();
    return a.exec ();
}
