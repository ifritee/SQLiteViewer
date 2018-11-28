#include "DAbout.h"
#include "ui_DAbout.h"

namespace sqliteviewer
{
  DAbout::DAbout(QWidget *parent) : QDialog(parent),
    _Widget_po(new Ui::DAbout)
  {
    _Widget_po->setupUi(this);

    connect(_Widget_po->Close_pb, SIGNAL(clicked()), this, SLOT(close()));
  }

  DAbout::~DAbout()
  {
    delete _Widget_po;
  }

}
