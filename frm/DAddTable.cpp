#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QDebug>

#include "SLogger.h"
#include "SQLConstants.h"
#include "DAddTable.h"
#include "ui_DAddTable.h"

namespace sqliteviewer
{
  DAddTable::DAddTable(QWidget *parent) : QDialog(parent),
    _Widget_po(new Ui::DAddTable),
    _SelectedRow_i32(-1)
  {
    _Widget_po->setupUi(this);

    _Widget_po->DBTable_tw->setColumnWidth(_RIName_en, 160);
    _Widget_po->DBTable_tw->setColumnWidth(_RIType_en, 128);
    _Widget_po->DBTable_tw->setColumnWidth(_RINotNull_en, 24);
    _Widget_po->DBTable_tw->setColumnWidth(_RIDefault_en, 128);
    _Widget_po->DBTable_tw->setColumnWidth(_RIUnique_en, 24);
    _Widget_po->DBTable_tw->setColumnWidth(_RIPrimaryKey_en, 24);
    _Widget_po->DBTable_tw->setColumnWidth(_RIForeignKey_en, 24);
    _Widget_po->ButtonBox_bb->button(QDialogButtonBox::Ok)->setEnabled(false);

    connect(_Widget_po->AddRow_pb, SIGNAL(clicked()), this, SLOT(AddRow_slt()));
    connect(_Widget_po->DeleteRow_pb, SIGNAL(clicked()), this, SLOT(DeleteRow_slt()));
    connect(_Widget_po->DBTable_tw, SIGNAL(cellClicked(int, int)), this, SLOT(SelectRow_slt(int, int)));
    connect(_Widget_po->ButtonBox_bb->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(AcceptedDialog_slt()));
  }

  DAddTable::~DAddTable()
  {
    delete _Widget_po;
  }

  QString DAddTable::GetSQLQuery_str()
  {
    QString SQLQuery_str("CREATE TABLE " + _Widget_po->TableName_le->text() + "( ");
    for(int i = 0; i < _Widget_po->DBTable_tw->rowCount(); ++i) {
      SQLQuery_str += _Widget_po->DBTable_tw->item(i, _RIName_en)->text() + " ";
      SQLQuery_str += qobject_cast<QComboBox *>(_Widget_po->DBTable_tw->cellWidget(i, _RIType_en))->currentText() + " ";
      if(_Widget_po->DBTable_tw->item(i, _RINotNull_en)->checkState() == Qt::Checked) {
        SQLQuery_str += "NOT NULL ";
      }
      if(!_Widget_po->DBTable_tw->item(i, _RIDefault_en)->text().isEmpty()) {
        SQLQuery_str += "DEFAULT " + _Widget_po->DBTable_tw->item(i, _RIDefault_en)->text() + " ";
      }
      if(_Widget_po->DBTable_tw->item(i, _RIUnique_en)->checkState() == Qt::Checked) {
        SQLQuery_str += "UNIQUE ";
      }
      if(_Widget_po->DBTable_tw->item(i, _RIPrimaryKey_en)->checkState() == Qt::Checked) {
        SQLQuery_str += "PRIMARY KEY";
      }
      SQLQuery_str += ", ";
    }
    SQLQuery_str = SQLQuery_str.left(SQLQuery_str.size() - 2) + ");";
    return SQLQuery_str;
  }

  void DAddTable::AddRow_slt()
  {
    int RowNumber_i = _Widget_po->DBTable_tw->rowCount();
    _Widget_po->DBTable_tw->insertRow(RowNumber_i);
    QTableWidgetItem * ItemName_po = new QTableWidgetItem;
    QComboBox * ItemType_po = new QComboBox(this);
    ItemType_po->addItems(SQLConstants::GetInstance_po()->GetSQLINTEGERTypes_lst());
    ItemType_po->addItems(SQLConstants::GetInstance_po()->GetSQLTEXTTypes_lst());
    ItemType_po->addItems(SQLConstants::GetInstance_po()->GetSQLNONETypes_lst());
    ItemType_po->addItems(SQLConstants::GetInstance_po()->GetSQLREALTypes_lst());
    ItemType_po->addItems(SQLConstants::GetInstance_po()->GetSQLNUMERICTypes_lst());
    QTableWidgetItem * ItemNN_po = new QTableWidgetItem;
    ItemNN_po->setCheckState(Qt::Unchecked);
    QTableWidgetItem * ItemDef_po = new QTableWidgetItem;
    QTableWidgetItem * ItemUQ_po = new QTableWidgetItem;
    ItemUQ_po->setCheckState(Qt::Unchecked);
    QTableWidgetItem * ItemPK_po = new QTableWidgetItem;
    ItemPK_po->setCheckState(Qt::Unchecked);
    QTableWidgetItem * Item6_po = new QTableWidgetItem;
    Item6_po->setCheckState(Qt::Unchecked);
    QComboBox * Item7_po = new QComboBox(this);
    Item7_po->setEnabled(false);
    _Widget_po->DBTable_tw->setItem(RowNumber_i,        _RIName_en,        ItemName_po);
    _Widget_po->DBTable_tw->setCellWidget(RowNumber_i,  _RIType_en,        ItemType_po);
    _Widget_po->DBTable_tw->setItem(RowNumber_i,        _RINotNull_en,     ItemNN_po);
    _Widget_po->DBTable_tw->setItem(RowNumber_i,        _RIDefault_en,     ItemDef_po);
    _Widget_po->DBTable_tw->setItem(RowNumber_i,        _RIUnique_en,      ItemUQ_po);
    _Widget_po->DBTable_tw->setItem(RowNumber_i,        _RIPrimaryKey_en,  ItemPK_po);
    _Widget_po->DBTable_tw->setItem(RowNumber_i,        _RIForeignKey_en,  Item6_po);
    _Widget_po->DBTable_tw->setCellWidget(RowNumber_i,  _RIReference_en,   Item7_po);

    _Widget_po->ButtonBox_bb->button(QDialogButtonBox::Ok)->setEnabled(true);
  }

  void DAddTable::DeleteRow_slt()
  {
    if(_SelectedRow_i32 != -1) {
      _Widget_po->DBTable_tw->removeRow(_SelectedRow_i32);
      _SelectedRow_i32 = -1;
      _Widget_po->DeleteRow_pb->setEnabled(false);
      _Widget_po->DBTable_tw->clearSelection();
      if(_Widget_po->DBTable_tw->rowCount() <= 0) {
        _Widget_po->ButtonBox_bb->button(QDialogButtonBox::Ok)->setEnabled(false);
      }
    }
  }

  void DAddTable::SelectRow_slt(int Row_i, int Column_i)
  {
    _SelectedRow_i32 = Row_i;
    _Widget_po->DeleteRow_pb->setEnabled(true);
    if(Column_i == _RIForeignKey_en) {
      QTableWidgetItem * QTableWidgetItem_po = _Widget_po->DBTable_tw->item(Row_i, Column_i);
      QComboBox * QComboBox_po = qobject_cast<QComboBox *>(_Widget_po->DBTable_tw->cellWidget(Row_i, _RIReference_en));
      log_assert(QComboBox_po);
      QComboBox_po->setEnabled(QTableWidgetItem_po->checkState() == Qt::Checked);
    }
  }

  void DAddTable::AcceptedDialog_slt()
  {
    bool IsAccept_b = true;
    _Widget_po->DBTable_tw->clearSelection();
    for(int i = 0; i < _Widget_po->DBTable_tw->rowCount(); ++i) {
      QTableWidgetItem * QTableWidgetItem_po = _Widget_po->DBTable_tw->item(i, _RIName_en);
      if(QTableWidgetItem_po->text().isEmpty()) {
        IsAccept_b = false;
      }
      QTableWidgetItem_po->setBackground(QTableWidgetItem_po->text().isEmpty() ? Qt::red : Qt::white);
    }
    if(_Widget_po->TableName_le->text().isEmpty()) {
      IsAccept_b = false;
    }
    _Widget_po->TableName_le->setStyleSheet(_Widget_po->TableName_le->text().isEmpty() ? "background: red;" : "background: white;");
    if(IsAccept_b) {
      accept();
    }
  }
}
