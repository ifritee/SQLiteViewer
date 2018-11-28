#include <QComboBox>

#include "DCreateCSVTable.h"
#include "SQLConstants.h"
#include "ui_DCreateCSVTable.h"

namespace sqliteviewer
{

  DCreateCSVTable::DCreateCSVTable(QStringList Columns_lst, QWidget *parent) :
    QDialog(parent),
    _Widget_po(new Ui::DCreateCSVTable)
  {
    _Widget_po->setupUi(this);

    _Translit_hash.insert(L'а', "a");
    _Translit_hash.insert(L'б', "b");
    _Translit_hash.insert(L'в', "v");
    _Translit_hash.insert(L'г', "g");
    _Translit_hash.insert(L'д', "d");
    _Translit_hash.insert(L'е', "e");
    _Translit_hash.insert(L'ё', "yo");
    _Translit_hash.insert(L'ж', "j");
    _Translit_hash.insert(L'з', "z");
    _Translit_hash.insert(L'и', "i");
    _Translit_hash.insert(L'й', "y");
    _Translit_hash.insert(L'к', "k");
    _Translit_hash.insert(L'л', "l");
    _Translit_hash.insert(L'м', "m");
    _Translit_hash.insert(L'н', "n");
    _Translit_hash.insert(L'о', "o");
    _Translit_hash.insert(L'п', "p");
    _Translit_hash.insert(L'р', "r");
    _Translit_hash.insert(L'с', "s");
    _Translit_hash.insert(L'т', "t");
    _Translit_hash.insert(L'у', "u");
    _Translit_hash.insert(L'ф', "f");
    _Translit_hash.insert(L'х', "h");
    _Translit_hash.insert(L'ц', "c");
    _Translit_hash.insert(L'ч', "ch");
    _Translit_hash.insert(L'ш', "sh");
    _Translit_hash.insert(L'щ', "csh");
    _Translit_hash.insert(L'ъ', "");
    _Translit_hash.insert(L'ы', "y");
    _Translit_hash.insert(L'ь', "");
    _Translit_hash.insert(L'э', "a");
    _Translit_hash.insert(L'ю', "yu");
    _Translit_hash.insert(L'я', "ya");
    _Translit_hash.insert(' ', "_");
    _Translit_hash.insert('\\', "_");
    _Translit_hash.insert('/', "_");

    _Widget_po->tableWidget->setRowCount(1);
    _Widget_po->tableWidget->setColumnCount(Columns_lst.size());
    for(int i = 0; i < Columns_lst.size(); ++i) {
      QTableWidgetItem * Item_po = new QTableWidgetItem(GetTranslit_str(Columns_lst.at(i).simplified()));
      Item_po->setToolTip(Columns_lst.at(i).simplified());
      _Widget_po->tableWidget->setHorizontalHeaderItem(i, Item_po);
      QComboBox * QComboBox_po = new QComboBox(this);
      QComboBox_po->addItems(SQLConstants::GetInstance_po()->GetSQLMAINTypes_lst());
      _Widget_po->tableWidget->setCellWidget(0, i, QComboBox_po);
    }
  }

  DCreateCSVTable::~DCreateCSVTable()
  {
    _Widget_po->tableWidget->clear();
    delete _Widget_po;
  }

  QString DCreateCSVTable::GetCSVHeaderWithTypes_str()
  {
    QString Query_str = "";
    for(int i = 0; i < _Widget_po->tableWidget->columnCount(); ++i) {
      QComboBox * QComboBox_po = dynamic_cast<QComboBox *>(_Widget_po->tableWidget->cellWidget(0, i));
      if(QComboBox_po) {
        Query_str += (QString(_Widget_po->tableWidget->horizontalHeaderItem(i)->text()) + " " + QComboBox_po->currentText());
        if(i != _Widget_po->tableWidget->columnCount() - 1) {
          Query_str += ",";
        }
      }
    }
    return Query_str;
  }

  QString DCreateCSVTable::GetTranslit_str(const QString & Text_str)
  {
    QString InputText_str = Text_str.toLower(), Result_str = "";
    for(int i = 0; i < InputText_str.size(); ++i) {
      QChar Char_o = InputText_str.at(i);
      if(_Translit_hash.contains(Char_o)) {
        Result_str += _Translit_hash[Char_o];
      }
      else {
        Result_str += Char_o;
      }
    }
    return Result_str;
  }
}
