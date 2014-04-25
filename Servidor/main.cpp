#include "servidor.h"
#include <QApplication>

int main (int argc, char* argv[])
{
    QApplication a (argc, argv);
    Servidor w;
    if (!createConnection())
          return 1;
    w.show ();
    return a.exec ();
}
