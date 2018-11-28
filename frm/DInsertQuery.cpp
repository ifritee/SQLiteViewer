#include <QStringList>

#include "DInsertQuery.h"
#include "ui_DInsertQuery.h"

namespace sqliteviewer
{
  DInsertQuery::DInsertQuery(QWidget *parent) : QDialog(parent),
    _Widget_po(new Ui::DInsertQuery)
  {
    _Widget_po->setupUi(this);
  }

  DInsertQuery::DInsertQuery(const QStringList &QStringList_lst, QWidget *parent) : QDialog(parent),
    _Widget_po(new Ui::DInsertQuery)
  {
    _Widget_po->setupUi(this);

    _Widget_po->QueryList_lw->addItems(QStringList_lst);
  }

  DInsertQuery::~DInsertQuery()
  {
    delete _Widget_po;
  }

  void DInsertQuery::SetStringList_v(const QStringList & QStringList_lst)
  {
    _Widget_po->QueryList_lw->addItems(QStringList_lst);
  }

  QString DInsertQuery::GetInsertQuery_str()
  {
    return _Widget_po->QueryList_lw->currentItem()->text();
  }
}

