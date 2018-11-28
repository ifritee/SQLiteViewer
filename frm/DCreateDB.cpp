#include <QDir>
#include <QFileDialog>

#include "DCreateDB.h"
#include "ui_DCreateDB.h"

namespace sqliteviewer
{

  DCreateDB::DCreateDB(QWidget *parent) : QDialog(parent),
    _Widget_po(new Ui::DCreateDB)
  {
    _Widget_po->setupUi(this);
    _Widget_po->DBPath_lb->setText(QDir::homePath());
    _Widget_po->DBName_le->setText("NewDataBase");
    _Widget_po->label_3->setVisible(false);
    _Widget_po->ExtFileName_lb->setVisible(false);

    connect(_Widget_po->SetDirectory_pb, SIGNAL(clicked()), this, SLOT(SetDirectory_slt()));
    connect(_Widget_po->SetExtFile_pb, SIGNAL(clicked()), this, SLOT(SetExtFile_slt()));
  }

  DCreateDB::~DCreateDB()
  {
    delete _Widget_po;
  }

  QString DCreateDB::GetDataBase_str()
  {
    return _Widget_po->DBPath_lb->text() + "/" + _Widget_po->DBName_le->text() + ".sqlite";
  }

  QString DCreateDB::GetExtFile_str()
  {
    QString ExtFileName_str;
    if(_Widget_po->SetExFile_cb->isChecked() && QFile(_Widget_po->ExtFileName_lb->text()).exists()) {
      ExtFileName_str = _Widget_po->ExtFileName_lb->text();
    }
    return ExtFileName_str;
  }

  void DCreateDB::SetDirectory_slt()
  {
    QString Directory_str = QFileDialog::getExistingDirectory(this, tr("Open directory"), QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(!Directory_str.isEmpty()) {
      _Widget_po->DBPath_lb->setText(Directory_str);
    }
  }

  void DCreateDB::SetExtFile_slt()
  {
    QString FileType_str = _Widget_po->InsertFile_cb->currentText();
    QString ExtFileName_str = QFileDialog::getOpenFileName(this, tr("Open ") + FileType_str + tr(" file"), QDir::homePath(),
                                                         FileType_str + " file ( *." + FileType_str.toUpper() +
                                                         " *." + FileType_str.toLower() + ")");
    if(!ExtFileName_str.isEmpty()) {
      _Widget_po->ExtFileName_lb->setText(ExtFileName_str);
    }
  }

}
