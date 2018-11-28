#include "DCreateDDS.h"
#include "ui_DCreateDDS.h"

DCreateDDS::DCreateDDS(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::DCreateDDS)
{
  ui->setupUi(this);
}

DCreateDDS::~DCreateDDS()
{
  delete ui;
}
