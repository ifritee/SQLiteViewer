#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QComboBox>
#include <QCheckBox>
#include <QFile>
#include <QDebug>

#include "SLogger.h"
#include "DEditTable.h"
#include "ui_DAddTable.h"

namespace sqliteviewer
{
  DEditTable::DEditTable(const QString & DBName_str, QWidget *parent) : DAddTable(parent),
    _DataBaseName_str(DBName_str),
    _OldRowCounts_i32(0)
  {
    log_assert(!DBName_str.isEmpty());
    log_assert(QFile(DBName_str).exists());
    setWindowTitle(tr("Редактирование таблицы"));
    _Widget_po->ButtonBox_bb->button(QDialogButtonBox::Ok)->setEnabled(true);
  }

  DEditTable::~DEditTable()
  {

  }

  void DEditTable::SetTableName_v(const QString & QString_ro)
  {
    _Widget_po->TableName_le->setText(QString_ro);
    _LastTableName_str = QString_ro;
    ShowTableInfo_v();
  }

  QString DEditTable::GetSQLQuery_str()
  {
    slogger->error("Not worked method in this object!!!");
    return QString();
  }

  void DEditTable::ShowTableInfo_v()
  {
    QSqlDatabase QSqlDatabase_o = QSqlDatabase::database();
    QSqlDatabase_o.setDatabaseName(_DataBaseName_str);
    if(!QSqlDatabase_o.open()) {
      slogger->error(QSqlDatabase_o.lastError().text().toStdString());
      QSqlDatabase_o.close();
      return;
    }
    QSqlQuery QSqlQuery_o(QSqlDatabase_o);
    if(!QSqlQuery_o.exec("PRAGMA table_info(\'" + _LastTableName_str + "\');")) {
      slogger->error(QSqlDatabase_o.lastError().text().toStdString());
      QSqlDatabase_o.close();
      return;
    }
    _OldRowCounts_i32 = 0;
    while(QSqlQuery_o.next()) {
      QString Name_str = QSqlQuery_o.value("name").toString();
      QString Type_str = QSqlQuery_o.value("type").toString();
      bool NotNull_b = (QSqlQuery_o.value("notnull").toInt() == 1);
      QString Default_str = QSqlQuery_o.value("dflt_value").type() != QVariant::Invalid ?
            QSqlQuery_o.value("dflt_value").toString() : "";
      bool PrimaryKey_b = (QSqlQuery_o.value("pk").toInt() == 1);
      AddRow_v(Name_str, Type_str, NotNull_b, Default_str, false, PrimaryKey_b);
      ++_OldRowCounts_i32;
    }

    QSqlDatabase_o.close();
  }

  void DEditTable::AddRow_v(const QString &Name_str, const QString &Type_str, bool IsNotNull_b,
                            const QString &Default_str, bool IsUnique_b, bool IsPrimaryKey_b, bool IsForeignKey_b,
                            const QString & /*Reference_str*/)
  {
    AddRow_slt();
    int RowNum_i = _Widget_po->DBTable_tw->rowCount() - 1;
    QTableWidgetItem * ItemName_po = _Widget_po->DBTable_tw->item(RowNum_i, DAddTable::_RIName_en);
    ItemName_po->setText(Name_str);
    QComboBox * ComboBox_po = qobject_cast<QComboBox *>(_Widget_po->DBTable_tw->cellWidget(RowNum_i,
                                                                                           DAddTable::_RIType_en));
    log_assert(ComboBox_po);
    ComboBox_po->setCurrentText(Type_str);
    QTableWidgetItem * CheckBoxNN_po = _Widget_po->DBTable_tw->item(RowNum_i, DAddTable::_RINotNull_en);
    CheckBoxNN_po->setCheckState(IsNotNull_b ? Qt::Checked : Qt::Unchecked);
    QTableWidgetItem * ItemDefault_po = _Widget_po->DBTable_tw->item(RowNum_i, DAddTable::_RIDefault_en);
    ItemDefault_po->setText(Default_str);
    QTableWidgetItem * CheckBoxUQ_po = _Widget_po->DBTable_tw->item(RowNum_i, DAddTable::_RIUnique_en);
    CheckBoxUQ_po->setCheckState(IsUnique_b ? Qt::Checked : Qt::Unchecked);
    QTableWidgetItem * CheckBoxPK_po = _Widget_po->DBTable_tw->item(RowNum_i, DAddTable::_RIPrimaryKey_en);
    CheckBoxPK_po->setCheckState(IsPrimaryKey_b ? Qt::Checked : Qt::Unchecked);
    QTableWidgetItem * CheckBoxFK_po = _Widget_po->DBTable_tw->item(RowNum_i, DAddTable::_RIForeignKey_en);
    CheckBoxFK_po->setCheckState(IsForeignKey_b ? Qt::Checked : Qt::Unchecked);
    //------ Делаем элементы недоступными -----
    ItemName_po->setFlags(ItemName_po->flags() & Qt::ItemIsEditable);
    ComboBox_po->setEnabled(false);
    CheckBoxNN_po->setFlags(CheckBoxNN_po->flags() & Qt::ItemIsEditable);
    ItemDefault_po->setFlags(ItemDefault_po->flags() & Qt::ItemIsEditable);
    CheckBoxUQ_po->setFlags(CheckBoxUQ_po->flags() & Qt::ItemIsEditable);
    CheckBoxPK_po->setFlags(CheckBoxPK_po->flags() & Qt::ItemIsEditable);
    CheckBoxFK_po->setFlags(CheckBoxFK_po->flags() & Qt::ItemIsEditable);
  }

  void DEditTable::QueriesFormation_v()
  {
    if(_Widget_po->TableName_le->text() != _LastTableName_str) {
      QString Query_str("ALTER TABLE \'" + _LastTableName_str + "\' RENAME TO \'" +
                        _Widget_po->TableName_le->text() + "\';");
      _Queries_lst.push_back(Query_str);
    }
    for(int i = _OldRowCounts_i32; i < _Widget_po->DBTable_tw->rowCount(); ++i) {
      QString Name_str = _Widget_po->DBTable_tw->item(i, _RIName_en)->text();
      QString Type_str = qobject_cast<QComboBox *>(_Widget_po->DBTable_tw->cellWidget(i, _RIType_en))
          ->currentText();
      bool IsNotNull_b = _Widget_po->DBTable_tw->item(i, _RINotNull_en)->checkState() == Qt::Checked;
      QString Default_str = _Widget_po->DBTable_tw->item(i, _RIDefault_en)->text();
      bool IsUnique_b = _Widget_po->DBTable_tw->item(i, _RIUnique_en)->checkState() == Qt::Checked;
      bool IsPK_b = _Widget_po->DBTable_tw->item(i, _RIPrimaryKey_en)->checkState() == Qt::Checked;
      QString Query_str("ALTER TABLE \'" + _LastTableName_str + "\' ADD COLUMN \'" +
                        Name_str + "\' " + Type_str + " ");
      if(IsNotNull_b) { Query_str += "NOT NULL DEFAULT " + Default_str + " "; }
      if(IsUnique_b) { Query_str += "UNIQUE "; }
      if(IsPK_b) { Query_str += "PRIMARY KEY "; }
      Query_str += ";";
      _Queries_lst.push_back(Query_str);
    }
  }

  void DEditTable::AddRow_slt()
  {
    DAddTable::AddRow_slt();
    QTableWidgetItem *ItemUQ_po = _Widget_po->DBTable_tw->item(_Widget_po->DBTable_tw->rowCount()-1,_RIUnique_en);
    ItemUQ_po->setFlags(ItemUQ_po->flags() & Qt::ItemIsEditable);
    QTableWidgetItem *ItemPK_po = _Widget_po->DBTable_tw->item(_Widget_po->DBTable_tw->rowCount()-1,_RIPrimaryKey_en);
    ItemPK_po->setFlags(ItemPK_po->flags() & Qt::ItemIsEditable);
    QTableWidgetItem *ItemFK_po = _Widget_po->DBTable_tw->item(_Widget_po->DBTable_tw->rowCount()-1,_RIForeignKey_en);
    ItemFK_po->setFlags(ItemFK_po->flags() & Qt::ItemIsEditable);
  }

  void DEditTable::DeleteRow_slt()
  {

  }

  void DEditTable::SelectRow_slt(int Row_i, int Column_i)
  {
    if(Row_i >= _OldRowCounts_i32) {
      _SelectedRow_i32 = Row_i;
      _Widget_po->DeleteRow_pb->setEnabled(true);
      if(Column_i == _RIForeignKey_en) {
        QTableWidgetItem * QTableWidgetItem_po = _Widget_po->DBTable_tw->item(Row_i, Column_i);
        QComboBox * QComboBox_po = qobject_cast<QComboBox *>(_Widget_po->DBTable_tw->cellWidget(Row_i, _RIReference_en));
        log_assert(QComboBox_po);
        QComboBox_po->setEnabled(QTableWidgetItem_po->checkState() == Qt::Checked);
      }
    }
    else {
      _SelectedRow_i32 = -1;
      _Widget_po->DBTable_tw->clearSelection();
      _Widget_po->DeleteRow_pb->setEnabled(false);
    }
  }

  void DEditTable::AcceptedDialog_slt()
  {
    bool IsAccept_b = true;
    _Widget_po->DBTable_tw->clearSelection();
    for(int i = 0; i < _Widget_po->DBTable_tw->rowCount(); ++i) {
      QTableWidgetItem * QTableWidgetItem_po = _Widget_po->DBTable_tw->item(i, _RIName_en);
      if(QTableWidgetItem_po->text().isEmpty()) {
        IsAccept_b = false;
      }
      QTableWidgetItem_po->setBackground(QTableWidgetItem_po->text().isEmpty() ? Qt::red : Qt::white);
      if(_Widget_po->DBTable_tw->item(i, _RINotNull_en)->checkState() == Qt::Checked &&
         _Widget_po->DBTable_tw->item(i, _RIDefault_en)->text().isEmpty()) {
        IsAccept_b = false;
        _Widget_po->DBTable_tw->item(i, _RIDefault_en)->setBackground(Qt::red);
      }
      else {
        _Widget_po->DBTable_tw->item(i, _RIDefault_en)->setBackground(Qt::white);
      }
    }
    if(_Widget_po->TableName_le->text().isEmpty()) {
      IsAccept_b = false;
    }

    _Widget_po->TableName_le->setStyleSheet(_Widget_po->TableName_le->text().isEmpty() ? "background: red;" : "background: white;");
    if(IsAccept_b) {
      QueriesFormation_v();
      accept();
    }
  }
}
