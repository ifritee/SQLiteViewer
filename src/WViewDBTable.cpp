#include "WViewDBTable.h"

namespace sqliteviewer
{
  WViewDBTable::WViewDBTable(QGraphicsItem *parent) : QGraphicsRectItem(parent)
  {

  }

  WViewDBTable::~WViewDBTable()
  {
    foreach(SValueData * SValueData_po, _AllValues_vec) {
      delete SValueData_po;
    }
    _AllValues_vec.clear();
  }

  void WViewDBTable::AddValueData_v(const QString & Name_str, const QString & Type_str, bool NotNull_b, const QString & Default_str, bool PrimaryKey_b)
  {
    SValueData * SValueData_po = new  SValueData;
    SValueData_po->_ValueName_str = Name_str;
    SValueData_po->_ValueType_str = Type_str;
    SValueData_po->_NotNull_b = NotNull_b;
    SValueData_po->_Default_str = Default_str;
    SValueData_po->_PrimaryKey_b = PrimaryKey_b;
    _AllValues_vec.push_back(SValueData_po);
  }
}
