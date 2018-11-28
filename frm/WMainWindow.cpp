#include <QMessageBox>
#include <QTextCodec>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QPoint>
#include <QTextStream>
#include <QDebug>

#include "SLogger.h"
#include "WMainWindow.h"
#include "DAbout.h"
#include "DCreateDB.h"
#include "DAddTable.h"
#include "DEditTable.h"
#include "CSVConverter.h"

#include "ui_WMainWindow.h"

namespace sqliteviewer
{

  WMainWindow::WMainWindow(QWidget *parent) : QMainWindow(parent),
    _Widget_po(new Ui::WMainWindow),
    _DAbout_po(new DAbout),
    _IsDBOpenned_b(false),
    _SelectMenuTreeItem_po(nullptr)
  {
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
  QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
#endif
    _Widget_po->setupUi(this);
    _TreeHeaderText_str = _Widget_po->TreeWidget_tw->headerItem()->text(0);
    _AllDBTables_lst.clear();
    _Widget_po->Inquiry_tab->SetMainToolBar_v(this);

    connect(_Widget_po->About_ac,       SIGNAL(triggered()),                        _DAbout_po, SLOT(exec()));
    connect(_Widget_po->Create_ac,      SIGNAL(triggered()),                              this, SLOT(CreateDB_slt()));
    connect(_Widget_po->Connect_ac,     SIGNAL(triggered()),                              this, SLOT(ConnectDB_slt()));
    connect(_Widget_po->DeleteTable_ac, SIGNAL(triggered()),                              this, SLOT(DeleteTable_slt()));
    connect(_Widget_po->TreeWidget_tw,  SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(SetTreeIndex_slt(QTreeWidgetItem*, int)));
    connect(_Widget_po->TreeWidget_tw,  SIGNAL(itemClicked(QTreeWidgetItem*, int)),       this, SLOT(ClickedTreeIndex_slt(QTreeWidgetItem*, int)));
    connect(_Widget_po->TreeWidget_tw,  SIGNAL(customContextMenuRequested(QPoint)),       this, SLOT(PrepareTreeContextMenu_slt(QPoint)));
    connect(_Widget_po->Disconnect_ac,  SIGNAL(triggered()),                              this, SLOT(CloseDB_slt()));
    connect(_Widget_po->AddTable_ac,    SIGNAL(triggered()),                              this, SLOT(AddTable_slt()));
    connect(_Widget_po->Inquiry_tab,    SIGNAL(FinishedQuery_sig()),                      this, SLOT(FinishedQuery_slt()));
    connect(_Widget_po->CreateDDS_ac,   SIGNAL(triggered()),                              this, SLOT(CreateDDS_slt()));
    connect(_Widget_po->SaveScript_ac,  SIGNAL(triggered()),           _Widget_po->Inquiry_tab, SLOT(SaveScript_slt()));
    connect(_Widget_po->LoadScript_ac,  SIGNAL(triggered()),           _Widget_po->Inquiry_tab, SLOT(LoadScript_slt()));
    connect(_Widget_po->ExcelReport_ac, SIGNAL(triggered()),           _Widget_po->Inquiry_tab, SLOT(CreateEXCELReport_slt()));
  }

  WMainWindow::~WMainWindow()
  {
    delete _Widget_po;
    delete _DAbout_po;
  }

  void WMainWindow::ConvertFileToBase_v(const QFile & ExtFileName_o)
  {
    QString Suffix_str = QFileInfo(ExtFileName_o).completeSuffix();
    if(Suffix_str.toUpper() == "CSV" ) {
      CSVConverter CSVConverter_o(ExtFileName_o, _DBFullName_str);
      if(CSVConverter_o.IsReady_b()) {
        if(!CSVConverter_o.CreateTable_b()) { // Если таблица не создана
          QMessageBox::warning(this, tr("Конвертация из CSV"), tr("Невозможно создать таблицу из CSV"), QMessageBox::Cancel);
          return;
        }
        if(!CSVConverter_o.ConvertData_b()) { // Если конвертация невозможна
          QMessageBox::warning(this, tr("Конвертация из CSV"), tr("Невозможно заполнить таблицу данными из CSV"), QMessageBox::Cancel);
          return;
        }
        OpenDB_b(_DBFullName_str);
      }
    }
  }

  bool WMainWindow::OpenDB_b(const QString & DBName_str)
  {
    QSqlDatabase QSqlDatabase_o;
    if(QSqlDatabase::contains(QSqlDatabase::defaultConnection))
      QSqlDatabase_o = QSqlDatabase::database();
    else
      QSqlDatabase_o = QSqlDatabase::addDatabase("QSQLITE");
    QSqlDatabase_o.setDatabaseName(DBName_str);
    if(!QSqlDatabase_o.open()) {
      slogger->error(QSqlDatabase_o.lastError().text().toStdString());
      QSqlDatabase_o.close();
      return false;
    }
    ShowAllTables_v();
    QSqlDatabase_o.close();
    return true;
  }

  void WMainWindow::ShowAllTables_v()
  {
    _Widget_po->TreeWidget_tw->clear();
    _AllDBTables_lst.clear();
    QFileInfo QFileInfo_o(_DBFullName_str);
    _Widget_po->TreeWidget_tw->headerItem()->setText(0, QFileInfo_o.baseName());
    QSqlQuery QSqlQuery_o;
    if(!QSqlQuery_o.exec("SELECT * FROM sqlite_master WHERE type = \'table\'")) {
      slogger->error(MORE_DATA("Query exec error! ").append(QSqlQuery_o.lastError().text().toStdString()));
      return;
    }
    while(QSqlQuery_o.next()) {
      QString TableName_str = QSqlQuery_o.value(1).toString();
      _AllDBTables_lst.push_back(TableName_str);
    }
    for(int i = 0; i < _AllDBTables_lst.size(); ++i) {
      if(!QSqlQuery_o.exec("SELECT COUNT(*) FROM \'" + _AllDBTables_lst[i] + "\';")) {
        slogger->error(MORE_DATA("Query exec error! ").append(QSqlQuery_o.lastError().text().toStdString()));
        return;
      }
      QSqlQuery_o.next();
      QString TableName_str = (QSqlQuery_o.value(0).type() != QVariant::Invalid) ? _AllDBTables_lst[i] + " (" + QSqlQuery_o.value(0).toString() + ")" : _AllDBTables_lst[i];
      QTreeWidgetItem * Item_po = new QTreeWidgetItem(QStringList() << TableName_str);
      Item_po->addChild(new QTreeWidgetItem(QStringList()<<tr("Столбцы")));
      Item_po->addChild(new QTreeWidgetItem(QStringList()<<tr("Ключи")));
      _Widget_po->TreeWidget_tw->addTopLevelItem(Item_po);
    }
    _Widget_po->Inquiry_tab->SetAllDBTableNames_v(_AllDBTables_lst);
    _Widget_po->Main_gv->SetAllDBTableNames_v(_AllDBTables_lst, _DBFullName_str);
  }

  void WMainWindow::CreateDB_slt()
  {
    if(_IsDBOpenned_b) {  // Если уже открыта БД
      switch(QMessageBox::warning(this, tr("Сохранить изменения?"), tr("База данных была изменена.\n" "Вы хотите сохранить изменения?"),
                                  QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save)) {
      case QMessageBox::Save:
        CloseDB_slt();
      break;

      case QMessageBox::Discard:
        return;
      break;

      case QMessageBox::Cancel:
        CloseDB_slt();
      break;

      default:
        break;
      }
    }
    DCreateDB DCreateDB_o;
    if(DCreateDB_o.exec() == QDialog::Accepted) {
      if(QFile::exists(DCreateDB_o.GetDataBase_str())) {  // Если файл уже существует
        QMessageBox::StandardButton StandardButton_o = QMessageBox::warning(this, tr("БД уже существует"), tr("Удалить старую БД?"),
                                                                            QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Cancel);
        if(StandardButton_o == QMessageBox::Cancel) {
          return;
        }
        else if(StandardButton_o == QMessageBox::Yes) {
          QFile DBFile_o(DCreateDB_o.GetDataBase_str());
          if(!DBFile_o.remove()) {  // Если не смог удалить
            slogger->error(DBFile_o.errorString().toStdString());
            return;
          }
        }
      }
      _DBFullName_str = DCreateDB_o.GetDataBase_str();
      if(OpenDB_b(_DBFullName_str) == true) {
        _Widget_po->Disconnect_ac->setEnabled(true);
        _Widget_po->AddTable_ac->setEnabled(true);
        _Widget_po->CentralWidget_tw->setEnabled(true);
        _Widget_po->CreateDDS_ac->setEnabled(true);
        _Widget_po->Inquiry_tab->SetDBName_v(_DBFullName_str);
        _Widget_po->SaveScript_ac->setEnabled(true);
        _Widget_po->LoadScript_ac->setEnabled(true);
        _Widget_po->ExcelReport_ac->setEnabled(true);
      }
      _IsDBOpenned_b = true;
      if(!DCreateDB_o.GetExtFile_str().isEmpty()) { // Если есть файл преобразования
        ConvertFileToBase_v(QFile(DCreateDB_o.GetExtFile_str()));
      }
    }
  }

  void WMainWindow::ConnectDB_slt(const QString & DBFileName_str)
  {
    QString FileName_str = DBFileName_str.isEmpty() ? QFileDialog::getOpenFileName(this, tr("Выбрать SQLite БД..."), QDir::homePath(), tr("SQLite (*.sqlite *.db)")) : DBFileName_str;
    if(!FileName_str.isEmpty()) {
      _DBFullName_str = FileName_str;
      if(OpenDB_b(FileName_str) == true) {
        slogger->info(std::string("Connect database ").append(FileName_str.toStdString()));
        _Widget_po->Disconnect_ac->setEnabled(true);
        _Widget_po->AddTable_ac->setEnabled(true);
        _Widget_po->CentralWidget_tw->setEnabled(true);
        _Widget_po->CreateDDS_ac->setEnabled(true);
        _Widget_po->Inquiry_tab->SetDBName_v(_DBFullName_str);
        _Widget_po->SaveScript_ac->setEnabled(true);
        _Widget_po->LoadScript_ac->setEnabled(true);
        _Widget_po->ExcelReport_ac->setEnabled(true);
      }
      _IsDBOpenned_b = true;
    }
  }

  void WMainWindow::CloseDB_slt()
  {
    QSqlDatabase QSqlDatabase_o = QSqlDatabase::database();
    QSqlDatabase_o.setDatabaseName(_DBFullName_str);
    QSqlDatabase_o.close();
    _Widget_po->TreeWidget_tw->clear();
    _Widget_po->TreeWidget_tw->headerItem()->setText(0, _TreeHeaderText_str);
    _Widget_po->Disconnect_ac->setEnabled(false);
    _Widget_po->AddTable_ac->setEnabled(false);
    _Widget_po->CentralWidget_tw->setEnabled(false);
    _Widget_po->CreateDDS_ac->setEnabled(false);
    _Widget_po->Inquiry_tab->Disconnected_v();
    _Widget_po->SaveScript_ac->setEnabled(false);
    _Widget_po->LoadScript_ac->setEnabled(false);
    _Widget_po->ExcelReport_ac->setEnabled(false);
    _IsDBOpenned_b = false;
    slogger->info(std::string("Disconnect database ").append(_DBFullName_str.toStdString()));
  }

  void WMainWindow::SetTreeIndex_slt(QTreeWidgetItem * QTreeWidgetItem_po, int)
  {
    if(QTreeWidgetItem_po->parent()) {
//----- CHILD -----
      _Widget_po->DeleteTable_ac->setEnabled(false);
      _SelectMenuTreeItem_po = nullptr;
    }
    else {
//----- MAIN -----
      _Widget_po->DeleteTable_ac->setEnabled(true);
      _SelectMenuTreeItem_po = QTreeWidgetItem_po;
      _Widget_po->Inquiry_tab->RunQuery_slt("SELECT * FROM " + QTreeWidgetItem_po->text(0).split(" ")[0] + ";");
    }
  }

  void WMainWindow::ClickedTreeIndex_slt(QTreeWidgetItem * QTreeWidgetItem_po, int)
  {
    if(QTreeWidgetItem_po->parent()) {
//----- CHILD -----
      _Widget_po->DeleteTable_ac->setEnabled(false);
      _SelectMenuTreeItem_po = nullptr;
    }
    else {
//----- MAIN -----
      _Widget_po->DeleteTable_ac->setEnabled(true);
      _SelectMenuTreeItem_po = QTreeWidgetItem_po;
    }
  }

  void WMainWindow::PrepareTreeContextMenu_slt(const QPoint & Position_ro)
  {
    if(!_IsDBOpenned_b) {
      return;
    }
    _SelectMenuTreeItem_po = _Widget_po->TreeWidget_tw->itemAt( Position_ro );
    QMenu Menu_o(this);
    if(_SelectMenuTreeItem_po) {  // Только если ткнули на QAction
      if(!_SelectMenuTreeItem_po->parent()) {
        _Widget_po->DeleteTable_ac->setEnabled(true);
        QAction * DeleteAction_po = new QAction(QIcon(":/Delete.png"), tr("Удалить"), this);
        connect(DeleteAction_po, SIGNAL(triggered()), this, SLOT(DeleteTable_slt()));
        QAction * EditAction_po = new QAction(QIcon(":/Edit.png"), tr("Редактировать"), this);
        connect(EditAction_po, SIGNAL(triggered()), this, SLOT(EditTable_slt()));
        Menu_o.addAction(DeleteAction_po);
        Menu_o.addAction(EditAction_po);
      }
      else {
        _Widget_po->DeleteTable_ac->setEnabled(false);
        QAction * EditAction_po = new QAction(QIcon(":/Edit.png"), tr("Редактировать"), this);
        connect(EditAction_po, SIGNAL(triggered()), this, SLOT(EditTable_slt()));
        Menu_o.addAction(EditAction_po);
      }
    }
    else {
      QAction * AddTableAction_po = new QAction(QIcon(":/AddTable.png"), tr("Добавить таблицу"), this);
      Menu_o.addAction(AddTableAction_po);
      connect(AddTableAction_po, SIGNAL(triggered()), this, SLOT(AddTable_slt()));
    }
    Menu_o.exec(_Widget_po->TreeWidget_tw->mapToGlobal(Position_ro));
  }

  void WMainWindow::DeleteTable_slt()
  {
    if(QMessageBox::warning(this, tr("Удаление..."), tr("Удалить таблицу из БД?"), QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Cancel) == QMessageBox::Yes) {
      QSqlDatabase QSqlDatabase_o = QSqlDatabase::database();
      QSqlDatabase_o.setDatabaseName(_DBFullName_str);
      if(!QSqlDatabase_o.open()) {
        slogger->error(QSqlDatabase_o.lastError().text().toStdString());
        QSqlDatabase_o.close();
        return;
      }
      QSqlQuery QSqlQuery_o(QSqlDatabase_o);
      if(!QSqlQuery_o.exec("DROP TABLE " + _SelectMenuTreeItem_po->text(0).split(" ")[0])) {
        slogger->error(QSqlQuery_o.lastError().text().toStdString());
      }
      ShowAllTables_v();
      QSqlDatabase_o.close();
    }
    _SelectMenuTreeItem_po = 0;
  }

  void WMainWindow::AddTable_slt()
  {
    DAddTable DAddTable_o(this);
    if(DAddTable_o.exec() == QDialog::Accepted) {
      QSqlDatabase QSqlDatabase_o = QSqlDatabase::database();
      QSqlDatabase_o.setDatabaseName(_DBFullName_str);
      if(!QSqlDatabase_o.open()) {
        slogger->error(QSqlDatabase_o.lastError().text().toStdString());
        QSqlDatabase_o.close();
        return;
      }
      QSqlQuery QSqlQuery_o(QSqlDatabase_o);
      if(!QSqlQuery_o.exec(DAddTable_o.GetSQLQuery_str())) {
        slogger->error(QSqlQuery_o.lastError().text().toStdString());
      }
      ShowAllTables_v();
      QSqlDatabase_o.close();
    }
  }

  void WMainWindow::EditTable_slt()
  {
    QTreeWidgetItem * MainTreeItem_po = _SelectMenuTreeItem_po;
    if(_SelectMenuTreeItem_po->parent()) {
      MainTreeItem_po = _SelectMenuTreeItem_po->parent();
    }
    DEditTable DEditTable_o(_DBFullName_str);
    DEditTable_o.SetTableName_v(MainTreeItem_po->text(0).split(" ")[0]);
    if(DEditTable_o.exec() == QDialog::Accepted) {
      QSqlDatabase QSqlDatabase_o = QSqlDatabase::database();
      QSqlDatabase_o.setDatabaseName(_DBFullName_str);
      if(!QSqlDatabase_o.open()) {
        slogger->error(QSqlDatabase_o.lastError().text().toStdString());
        QSqlDatabase_o.close();
        return;
      }
      QSqlQuery QSqlQuery_o(QSqlDatabase_o);
      foreach (QString Query_str, DEditTable_o.GetSQLQueries_lst()) {
        if(!QSqlQuery_o.exec(Query_str)) {
          slogger->error(QSqlQuery_o.lastError().text().toStdString());
        }
      }
      ShowAllTables_v();
      QSqlDatabase_o.close();
    }
  }

  void WMainWindow::FinishedQuery_slt()
  {
    QSqlDatabase QSqlDatabase_o = QSqlDatabase::database();
    QSqlDatabase_o.setDatabaseName(_DBFullName_str);
    if(!QSqlDatabase_o.open()) {
      slogger->error(QSqlDatabase_o.lastError().text().toStdString());
      QSqlDatabase_o.close();
      return;
    }
    ShowAllTables_v();
    QSqlDatabase_o.close();
  }

  void WMainWindow::CreateDDS_slt()
  {
    QString FileName_str = QFileDialog::getSaveFileName(this, tr("Сохранить файл DDS на диске"), QDir::homePath(), tr("DDS (*.dds);;TEXT (*.txt)"));
    if(!FileName_str.isEmpty()) {
      QSqlDatabase QSqlDatabase_o = QSqlDatabase::database();
      QSqlDatabase_o.setDatabaseName(_DBFullName_str);
      if(!QSqlDatabase_o.open()) {
        slogger->error(QSqlDatabase_o.lastError().text().toStdString());
        QSqlDatabase_o.close();
        return;
      }
      QSqlQuery QSqlQuery_o(QSqlDatabase_o);
      if(!QSqlQuery_o.exec("SELECT * FROM sqlite_master;")) {
        slogger->error(QSqlQuery_o.lastError().text().toStdString());
        QSqlDatabase_o.close();
        return;
      }
      QString DDS_str;
      while(QSqlQuery_o.next()) { // Тут происходит отбор SQL для скрипта
        QString Type_str = (QSqlQuery_o.value("type").type() == QVariant::Invalid) ? "" : QSqlQuery_o.value("type").toString();
        QString SQL_str = (QSqlQuery_o.value("sql").type() == QVariant::Invalid) ? "" : QSqlQuery_o.value("sql").toString();
        if(Type_str.toUpper() == "TABLE") {
          DDS_str = DDS_str + SQL_str + ";\n";
        }
      }
      QFile DDSFile_o(FileName_str);
      if(!DDSFile_o.open(QIODevice::WriteOnly)) {
        slogger->error(QString("Not open file! " + FileName_str).toStdString());
        qWarning()<<"Not open file! "<<FileName_str;
        QSqlDatabase_o.close();
        return;
      }
      QTextStream TextStream_o(&DDSFile_o);
      TextStream_o << DDS_str;
      DDSFile_o.close();
      QSqlDatabase_o.close();
    }
  }

}
