#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QGraphicsScene>

#include "SLogger.h"
#include "WPerfomance.h"
#include "WViewDBTable.h"

namespace sqliteviewer
{
  WPerfomance::WPerfomance(QWidget *parent) : QGraphicsView(parent),
    _Scene_po(new QGraphicsScene)
  {
    this->setScene(_Scene_po);
  }

  WPerfomance::WPerfomance(QGraphicsScene * scene, QWidget *parent) : QGraphicsView(scene, parent),
    _Scene_po(scene)
  {
  }

  WPerfomance::~WPerfomance()
  {
    foreach (WViewDBTable * WViewDBTable_po, _AllDBTables_map) {
      delete WViewDBTable_po;
    }
    _AllDBTables_map.clear();
  }

  void WPerfomance::SetAllDBTableNames_v(const QStringList & AllDBTables_lst, const QString & DataBaseName_str)
  {
    foreach (const QString & TableName_str, AllDBTables_lst) {
      QSqlDatabase QSqlDatabase_o = QSqlDatabase::database();
      QSqlDatabase_o.setDatabaseName(DataBaseName_str);
      if(!QSqlDatabase_o.open()) {
        slogger->error(QSqlDatabase_o.lastError().text().toStdString());
        QSqlDatabase_o.close();
        return;
      }
      QSqlQuery QSqlQuery_o(QSqlDatabase_o);
      if(!QSqlQuery_o.exec("PRAGMA table_info(\'" + TableName_str + "\');")) {
        slogger->error(QSqlDatabase_o.lastError().text().toStdString());
        QSqlDatabase_o.close();
        return;
      }

      if(!_AllDBTables_map.contains(TableName_str)) {
        WViewDBTable * WViewDBTable_po = new WViewDBTable;
        while(QSqlQuery_o.next()) {
          QString Name_str = QSqlQuery_o.value("name").toString();
          QString Type_str = QSqlQuery_o.value("type").toString();
          bool NotNull_b = (QSqlQuery_o.value("notnull").toInt() == 1);
          QString Default_str = QSqlQuery_o.value("dflt_value").type() != QVariant::Invalid ?
                QSqlQuery_o.value("dflt_value").toString() : "";
          bool PrimaryKey_b = (QSqlQuery_o.value("pk").toInt() == 1);
          WViewDBTable_po->AddValueData_v(Name_str, Type_str, NotNull_b, Default_str, PrimaryKey_b);
        }
        _AllDBTables_map[TableName_str] = WViewDBTable_po;
      }
      QSqlDatabase_o.close();
    }
  }

}
