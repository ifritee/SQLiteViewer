#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <SLogger.h>

#include "CSVConverter.h"
#include "DCreateCSVTable.h"

namespace sqliteviewer
{
  CSVConverter::CSVConverter(const QFile & CSVFile_rc, const QString & DatabaseName_str, QObject *parent) : QObject(parent),
    _CSVFile_po(new QFile(CSVFile_rc.fileName())),
    _DBFileName_po(new QString(DatabaseName_str))
  {

  }

  bool CSVConverter::IsReady_b()
  {
    QSqlDatabase QSqlDatabase_o;
    if(QSqlDatabase::contains(QSqlDatabase::defaultConnection))
      QSqlDatabase_o = QSqlDatabase::database();
    else
      QSqlDatabase_o = QSqlDatabase::addDatabase("QSQLITE");
    QSqlDatabase_o.setDatabaseName(*_DBFileName_po);
    if(!QSqlDatabase_o.open()) {
      QSqlDatabase_o.close();
      return false;
    }
    QSqlDatabase_o.close();
    if(!_CSVFile_po->exists()) {
      return false;
    }
    return true;
  }

  bool CSVConverter::CreateTable_b()
  {
    if(!_CSVFile_po->open(QIODevice::ReadOnly)) {
      return false;
    }

    QSqlDatabase QSqlDatabase_o = QSqlDatabase::database();
    QSqlDatabase_o.setDatabaseName(*_DBFileName_po);
    if(!QSqlDatabase_o.open()) {
      slogger->error(QSqlDatabase_o.lastError().text().toStdString());
      _CSVFile_po->close();
      QSqlDatabase_o.close();
      return false;
    }

    QString Header_str = _CSVFile_po->readLine();
    DCreateCSVTable DCreateCSVTable_o(Header_str.split(","));
    if(DCreateCSVTable_o.exec() == QDialog::Accepted) {
      QString Query_str = "CREATE TABLE " + QFileInfo(*_CSVFile_po).baseName().toUpper() + " (" + DCreateCSVTable_o.GetCSVHeaderWithTypes_str() + ");";
      slogger->info(Query_str.toStdString());
      QSqlQuery QSqlQuery_o(QSqlDatabase_o);
      if(!QSqlQuery_o.exec(Query_str)) {
        slogger->error(QSqlQuery_o.lastError().text().toStdString());
        _CSVFile_po->close();
        QSqlDatabase_o.close();
        return false;
      }
    }
    QSqlDatabase_o.close();
    _CSVFile_po->close();
    return true;
  }

  bool CSVConverter::ConvertData_b()
  {
    if(!_CSVFile_po->open(QIODevice::ReadOnly)) {
      return false;
    }

    QSqlDatabase QSqlDatabase_o = QSqlDatabase::database();
    QSqlDatabase_o.setDatabaseName(*_DBFileName_po);
    if(!QSqlDatabase_o.open()) {
      slogger->error(QSqlDatabase_o.lastError().text().toStdString());
      QSqlDatabase_o.close();
      return false;
    }
    QString Line_str = _CSVFile_po->readLine();  // Считываем первую строку
    QSqlDatabase_o.transaction();
    while(true) {
      Line_str = _CSVFile_po->readLine();
      if(Line_str.isEmpty()) { // Нечего считывать
        break;
      }

      QStringList Values_lst = Line_str.split(",");
      Line_str.clear();
      foreach (QString Value_str, Values_lst) {
        Line_str += "\"" + Value_str + "\", ";
      }
      Line_str = Line_str.left(Line_str.size() - 2);

      QString Query_str = "INSERT INTO " + QFileInfo(*_CSVFile_po).baseName().toUpper() + " VALUES " + " (" + Line_str.simplified() + ");";
      QSqlQuery QSqlQuery_o(QSqlDatabase_o);
      if(!QSqlQuery_o.exec(Query_str)) {
        slogger->error(Query_str.toStdString());
        slogger->error(QSqlQuery_o.lastError().text().toStdString());
        QSqlDatabase_o.close();
        _CSVFile_po->close();
        return false;
      }
    }
    QSqlDatabase_o.commit();
    QSqlDatabase_o.close();
    _CSVFile_po->close();
    return true;
  }
}
