#ifndef SQLLITE_H
#define SQLLITE_H


#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QMessageBox>
#include <QStandardPaths>
#include <QDir>

// Crea la conexión a la BD
static bool createConnection ()
{
    QStringList dataDir = QStandardPaths::standardLocations (QStandardPaths::DesktopLocation);
    QDir dir (dataDir.at (0));
    dir.mkdir ("BBDD");
    dir.cd ("BBDD");
    QString dbName = dir.toNativeSeparators (dir.filePath ("bbdd.sqlite"));
    QSqlDatabase db = QSqlDatabase::addDatabase ("QSQLITE");
    db.setDatabaseName (dbName);



 if (!db.open ()) {
      QMessageBox::critical (NULL, QObject::tr ("ERROR"), QObject::tr ("No se pudo acceder a los datos"));
              return false;
  }

  //Crea la tabla
 QSqlQuery query;
 query.exec ("CREATE TABLE IF NOT EXISTS tabla"
            "(nfoto INTEGER PRIMARY KEY AUTOINCREMENT,"
            "cliente varchar,"
            "tiempo varchar,"
            "frame integer)");

  return true;
}


#endif // SQLLITE_H
