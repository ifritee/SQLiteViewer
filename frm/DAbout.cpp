#include "DAbout.h"
#include "ui_DAbout.h"

namespace sqliteviewer
{
  DAbout::DAbout(QWidget *parent) : QDialog(parent),
    _Widget_po(new Ui::DAbout)
  {
    _Widget_po->setupUi(this);
    QString Version_str = VERSION;
    _Widget_po->textBrowser->setHtml("<h1>Программа SQLiteViewer</h1>"
                                     "<p><b>Версия: </b>" + Version_str + "</p>"
                                     "<p><b>Автор: </b>Никишин Е.В.</p>"
                                     "<h2>Описание</h2>"
                                     "<p>Эта программа служит для отображения БД SQLite</p>");

    connect(_Widget_po->Close_pb, SIGNAL(clicked()), this, SLOT(close()));
  }

  DAbout::~DAbout()
  {
    delete _Widget_po;
  }

}
