#include <QCompleter>
#include <QStringListModel>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QToolBar>
#include <QSpinBox>
#include <QAction>
#include <QSqlField>
#include <QFileInfo>
#include <QMessageBox>
#include <QLabel>
#include <QScrollBar>
#include <QFileDialog>
#include <QTextStream>
#include <QDebug>

#include "xlsxdocument.h"
#include "WDBInquiry.h"
#include "SQLHighLighter.h"
#include "SQLConstants.h"
#include "SLogger.h"
#include "DInsertQuery.h"

#include "ui_WDBInquiry.h"

namespace sqliteviewer
{

  const int PageSize_ci = 1024;  ///< @brief Размер страницы в таблице

  WDBInquiry::WDBInquiry(QWidget *parent) : QMainWindow(parent),
    _Widget_po(new Ui::WDBInquiry),
    _ToolBar_po(new QToolBar),
    _InsertRowNumber_po(new QSpinBox),
    _NextRow_po(new QAction(this)),
    _MaxRows_po(new QLabel(tr("Переход: ")))
  {
    _Widget_po->setupUi(this);

    _ToolBar_po->addWidget(_MaxRows_po);
    _InsertRowNumber_po->setFixedWidth(80);
    _ToolBar_po->addWidget(_InsertRowNumber_po);
    _NextRow_po->setIcon(QIcon(":/Next.png"));
    _ToolBar_po->addAction(_NextRow_po);

    _ToolBar_po->hide();
    _ToolBar_po->setEnabled(false);

    _SQLHighLighter_po = new SQLHighLighter(_Widget_po->Inquiry_te->document());
    _Widget_po->DockWidget_dw->setTitleBarWidget(new QWidget);

    //----- Добавим автодополнение кода -----
    _QCompleter_po = new QCompleter(this);
    QStringList QStringList_o;
    QStringList_o.append(SQLConstants::GetInstance_po()->GetSQLTags_lst());
    QStringList_o.append(SQLConstants::GetInstance_po()->GetSQLINTEGERTypes_lst());
    QStringList_o.append(SQLConstants::GetInstance_po()->GetSQLNONETypes_lst());
    QStringList_o.append(SQLConstants::GetInstance_po()->GetSQLNUMERICTypes_lst());
    QStringList_o.append(SQLConstants::GetInstance_po()->GetSQLREALTypes_lst());
    QStringList_o.append(SQLConstants::GetInstance_po()->GetSQLTEXTTypes_lst());
    qSort(QStringList_o.begin(), QStringList_o.end());
    QStringListModel *QStringListModel_po = new QStringListModel(QStringList_o);

    _QCompleter_po->setModel(QStringListModel_po);
    _QCompleter_po->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    _QCompleter_po->setCaseSensitivity(Qt::CaseInsensitive);
    _QCompleter_po->setCompletionMode(QCompleter::PopupCompletion);
    _QCompleter_po->setWrapAround(false);
    _Widget_po->Inquiry_te->SetCompleter_v(_QCompleter_po);

    _ErrorTabName_str = _Widget_po->tabWidget->tabText(_Widget_po->tabWidget->indexOf(_Widget_po->Errors_tab));

    connect(_Widget_po->SaveScript_pb,  SIGNAL(clicked()), this, SLOT(SaveScript_slt()));
    connect(_Widget_po->LoadScript_pb,  SIGNAL(clicked()), this, SLOT(LoadScript_slt()));
    connect(_Widget_po->RunQuery_pb,    SIGNAL(clicked()), this, SLOT(RunQuery_slt()));
    connect(_Widget_po->ClearQuery_pb,  SIGNAL(clicked()), this, SLOT(ClearQuery_slt()));
    connect(_Widget_po->ClearError_pb,  SIGNAL(clicked()), this, SLOT(ClearError_slt()));
    connect(_Widget_po->SQLQueryTable_tw->verticalScrollBar(), SIGNAL(sliderReleased()), this, SLOT(SQLTableSliderMoved_slt()));
    connect(_NextRow_po, SIGNAL(triggered()), this, SLOT(NextRow_slt()));
  }

  WDBInquiry::~WDBInquiry()
  {
    delete _Widget_po;
    delete _SQLHighLighter_po;
    delete _QCompleter_po;
    delete _QSqlQuery_po;
    delete _ToolBar_po;
  }

  void WDBInquiry::SetAllDBTableNames_v(const QStringList & AllDBTables_lst)
  {
    QStringList Tables_lst;
    foreach (const QString & Text_str, AllDBTables_lst) {
      Tables_lst.push_back(Text_str.toUpper());
    }

    QStringList QStringList_o;
    QStringList_o.append(QFileInfo(_DataBaseName_str).baseName().toUpper());
    QStringList_o.append(SQLConstants::GetInstance_po()->GetSQLTags_lst());
    QStringList_o.append(SQLConstants::GetInstance_po()->GetSQLINTEGERTypes_lst());
    QStringList_o.append(SQLConstants::GetInstance_po()->GetSQLNONETypes_lst());
    QStringList_o.append(SQLConstants::GetInstance_po()->GetSQLNUMERICTypes_lst());
    QStringList_o.append(SQLConstants::GetInstance_po()->GetSQLREALTypes_lst());
    QStringList_o.append(SQLConstants::GetInstance_po()->GetSQLTEXTTypes_lst());
    QStringList_o.append(Tables_lst);
    qSort(QStringList_o.begin(), QStringList_o.end());
    QStringListModel * QStringListModel_po = qobject_cast<QStringListModel *>(_QCompleter_po->model());
    log_assert(QStringListModel_po);
    QStringListModel_po->setStringList(QStringList_o);
    _QCompleter_po->setModel(QStringListModel_po);
  }

  void WDBInquiry::SetDBName_v(const QString & DBFullName_str)
  {
    _DataBaseName_str = DBFullName_str;
    QStringListModel * QStringListModel_po = qobject_cast<QStringListModel *>(_QCompleter_po->model());
    log_assert(QStringListModel_po);
    QStringList QStringList_o;
    QStringList_o.append(QFileInfo(_DataBaseName_str).baseName().toUpper());
    QStringList_o.append(QStringListModel_po->stringList());
    qSort(QStringList_o.begin(), QStringList_o.end());

    QStringListModel_po->setStringList(QStringList_o);
    _QCompleter_po->setModel(QStringListModel_po);
  }

  void WDBInquiry::ShowTableInformation_v()
  {
    _Widget_po->SQLQueryTable_tw->clear();
    const QSqlRecord SqlRecord_o = _QSqlQuery_po->record();
    _Widget_po->SQLQueryTable_tw->setColumnCount(SqlRecord_o.count());
    for(int i = 0; i < SqlRecord_o.count(); ++i) {
      _Widget_po->SQLQueryTable_tw->setHorizontalHeaderItem(i, new QTableWidgetItem(SqlRecord_o.fieldName(i)));
    }
    quint32 RowNumber_u32 = 0;
    while (_QSqlQuery_po->next()) {
      _Widget_po->SQLQueryTable_tw->setRowCount(RowNumber_u32 + 1);
      for(int i = 0; i < SqlRecord_o.count(); ++i) {
        _Widget_po->SQLQueryTable_tw->setItem(RowNumber_u32, i, new QTableWidgetItem((_QSqlQuery_po->value(i).type() == QVariant::Invalid) ? "" : _QSqlQuery_po->value(i).toString()));
      }
      _Widget_po->SQLQueryTable_tw->setVerticalHeaderItem(RowNumber_u32, new QTableWidgetItem(QString::number(PageSize_ci * _PageQueryNumber_u32 / 2 + RowNumber_u32 + _SeekRowNumber_u32) + "  "));
      if(++RowNumber_u32 == PageSize_ci) {
        break;
      }
    }
  }

  void WDBInquiry::Disconnected_v()
  {
    _Widget_po->Inquiry_te->clear();
    _Widget_po->Errors_lw->clear();
    _Widget_po->SQLQueryTable_tw->setRowCount(0);
    _Widget_po->SQLQueryTable_tw->setColumnCount(0);
    _Widget_po->SQLQueryTable_tw->clear();
    _ToolBar_po->setEnabled(false);
    if(!_DataBaseName_str.isEmpty()) {
      QSqlDatabase QSqlDatabase_o = QSqlDatabase::database();
      QSqlDatabase_o.setDatabaseName(_DataBaseName_str);
      QSqlDatabase_o.close();
    }
  }

  void WDBInquiry::SetMainToolBar_v(QMainWindow * Parent_po)
  {
    Parent_po->addToolBar(Qt::TopToolBarArea, _ToolBar_po);
  }

  void WDBInquiry::showEvent(QShowEvent * Event_po)
  {
    _ToolBar_po->show();
    QMainWindow::showEvent(Event_po);
  }

  void WDBInquiry::hideEvent(QHideEvent * Event_po)
  {
    _ToolBar_po->hide();
    QMainWindow::hideEvent(Event_po);
  }

  qint32 WDBInquiry::GetQuerySize_i32(const QString & QSqlQuery_str)
  {
    QString NewQUERY_str = QSqlQuery_str.toUpper();
    if(!NewQUERY_str.contains("SELECT")) {
      return -1;
    }
    NewQUERY_str.replace("SELECT", "SELECT COUNT(*),");
    QSqlQuery QSqlQuery_o;
    if(!QSqlQuery_o.exec(NewQUERY_str)) {
      return -1;
    }
    QSqlQuery_o.next();
    int Count_i = (QSqlQuery_o.value(0).type() == QVariant::Invalid) ? -1 : QSqlQuery_o.value(0).toInt();
    return Count_i;
  }

  void WDBInquiry::RunQuery_slt(const QString & ExtQuery_str)
  {
    _Widget_po->Errors_lw->clear();
    _Widget_po->SQLQueryTable_tw->setRowCount(0);
    _Widget_po->SQLQueryTable_tw->setColumnCount(0);
    _Widget_po->SQLQueryTable_tw->clear();
    log_assert(!_DataBaseName_str.isEmpty());
    QString AllText_str = ExtQuery_str.isEmpty() ? _Widget_po->Inquiry_te->toPlainText() : ExtQuery_str;
    AllText_str.replace("\n", " ");
    QStringList AllQueries_lst = AllText_str.split(";");
    for(int i = 0; i < AllQueries_lst.size(); ++i) {
      AllQueries_lst[i] = AllQueries_lst[i].simplified();
      if(AllQueries_lst[i].isEmpty()) {
        AllQueries_lst.removeAt(i);
        --i;
      }
      else {
        AllQueries_lst[i] += ";";
      }
    }

    if(AllQueries_lst.size() > 1) {
      DInsertQuery DInsertQuery_o(AllQueries_lst);
      if(DInsertQuery_o.exec() == QDialog::Accepted) {
        AllText_str = DInsertQuery_o.GetInsertQuery_str();
      }
    }

    QSqlDatabase QSqlDatabase_o = QSqlDatabase::database();
    QSqlDatabase_o.setDatabaseName(_DataBaseName_str);
    QSqlDatabase_o.close();
    if(!QSqlDatabase_o.open()) {
      slogger->error(QSqlDatabase_o.lastError().text().toStdString());
      QSqlDatabase_o.close();
      return;
    }
    quint32 Errors_u32 = 0;
    delete _QSqlQuery_po;
    _QSqlQuery_po = new QSqlQuery(QSqlDatabase_o);
    QSqlDatabase_o.transaction();

    qint32 QuerySize_i32 = GetQuerySize_i32(AllText_str);
    QuerySize_i32 = (QuerySize_i32 < 0) ? 0 : QuerySize_i32;

    if(!_QSqlQuery_po->exec(AllText_str)) {
      slogger->error(_QSqlQuery_po->lastError().text().toStdString());
      _Widget_po->Errors_lw->addItem(_QSqlQuery_po->lastError().text());
      ++Errors_u32;
      QSqlDatabase_o.close();
      _Widget_po->tabWidget->setTabText(_Widget_po->tabWidget->indexOf(_Widget_po->Errors_tab), Errors_u32 == 0 ?
                                          _ErrorTabName_str : _ErrorTabName_str + "(" + QString::number(Errors_u32) + ")");
      return;
    }

    _InsertRowNumber_po->setMaximum(QuerySize_i32);
    _InsertRowNumber_po->setToolTip(tr("Максимум: ") + QString::number(QuerySize_i32));
    _PageQueryNumber_u32 = 0;
    _SeekRowNumber_u32 = 0;
    ShowTableInformation_v();
    QSqlDatabase_o.commit();
    _ToolBar_po->setEnabled(true);
    _Widget_po->tabWidget->setTabText(_Widget_po->tabWidget->indexOf(_Widget_po->Errors_tab), Errors_u32 == 0 ?
                                        _ErrorTabName_str : _ErrorTabName_str + "(" + QString::number(Errors_u32) + ")");
    if(AllText_str.indexOf("INSERT") != -1 || AllText_str.indexOf("CREATE") != -1 || AllText_str.indexOf("DROP") != -1) {
      emit FinishedQuery_sig();
    }
  }

  void WDBInquiry::SaveScript_slt()
  {
    QString FileName_str = QFileDialog::getSaveFileName(this, tr("Сохранить файл скрипта на диске"), QDir::homePath(), tr("SCRIPT (*.script);;ALL (*.*)"));
    if(!FileName_str.isEmpty()) {
      QFile ScriptFile_o(FileName_str);
      if(!ScriptFile_o.open(QIODevice::WriteOnly)) {
        slogger->error(MORE_DATA("Don\'t save script to file ").append(FileName_str.toStdString()));
        return;
      }
      QTextStream TextStream(&ScriptFile_o);
      TextStream<<_Widget_po->Inquiry_te->toPlainText();
      ScriptFile_o.close();
    }
  }

  void WDBInquiry::LoadScript_slt()
  {
    QString FileName_str = QFileDialog::getOpenFileName(this, tr("Выбрать скрипт ..."), QDir::homePath(), tr("SCRIPT (*.script);;ALL (*.*)"));
    if(!FileName_str.isEmpty()) {
      QFile ScriptFile_o(FileName_str);
      if(!ScriptFile_o.open(QIODevice::ReadOnly)) {
        slogger->error(MORE_DATA("Don\'t load script to file ").append(FileName_str.toStdString()));
        return;
      }
      QTextStream TextStream(&ScriptFile_o);
      _Widget_po->Inquiry_te->setPlainText(TextStream.readAll());
      ScriptFile_o.close();
    }
  }

  void WDBInquiry::CreateEXCELReport_slt()
  {
    QString FileName_str = QFileDialog::getSaveFileName(this, tr("Сохранить файл скрипта на диске"), QDir::homePath(), tr("EXCEL (*.xlsx)"));
    if( FileName_str.isEmpty() || _Widget_po->SQLQueryTable_tw->columnCount() == 0 ) {
      return;
    }
    if(FileName_str.right(5).toUpper() != ".XLSX") {
      FileName_str += ".xlsx";
    }
    //----- Удалим файл целиком -----
    QFile File_o(FileName_str);
    File_o.remove();
    //----- Создадим отчет -----
    QXlsx::Document ExcelDocument_o(FileName_str);
    QXlsx::Format Format_o;
    Format_o.setFontBold(true);
    //----- Заголовок -----
    ExcelDocument_o.setDocumentProperty("title", _QSqlQuery_po->executedQuery());
    for(int i = 0; i < _Widget_po->SQLQueryTable_tw->columnCount(); ++i) {
      ExcelDocument_o.write(1, i + 1, _Widget_po->SQLQueryTable_tw->horizontalHeaderItem(i)->text());
      ExcelDocument_o.setColumnWidth(i + 1, 15.0);
    }
    ExcelDocument_o.setRowFormat(1, Format_o);
    Format_o.setFontBold(false);
    _QSqlQuery_po->first();
    quint32 RowNumber_u = 1;
    //----- Тело -----
    while(_QSqlQuery_po->next()) {
      ExcelDocument_o.setRowFormat(++RowNumber_u, Format_o);
      for(int i = 0; i < _Widget_po->SQLQueryTable_tw->columnCount(); ++i) {
        ExcelDocument_o.write(RowNumber_u, i + 1, _QSqlQuery_po->value(i));
      }
    }
    ExcelDocument_o.save();
  }

  void WDBInquiry::NextRow_slt()
  {
    _QSqlQuery_po->first();
    _QSqlQuery_po->seek(_InsertRowNumber_po->value() - 1);
    _SeekRowNumber_u32 = _InsertRowNumber_po->value();
    ShowTableInformation_v();
  }

  void WDBInquiry::ClearQuery_slt()
  {
    _Widget_po->Inquiry_te->clear();
    ClearError_slt();
  }

  void WDBInquiry::ClearError_slt()
  {
    _Widget_po->Errors_lw->clear();
    _Widget_po->tabWidget->setTabText(_Widget_po->tabWidget->indexOf(_Widget_po->Errors_tab), _ErrorTabName_str);
  }

  void WDBInquiry::SQLTableSliderMoved_slt()
  {
    QScrollBar * ScrollBar_po = _Widget_po->SQLQueryTable_tw->verticalScrollBar();
    if( ScrollBar_po->sliderPosition() == ScrollBar_po->maximum() && _Widget_po->SQLQueryTable_tw->rowCount() == PageSize_ci) {
      ++_PageQueryNumber_u32;
    }
    else if ( ScrollBar_po->sliderPosition() == ScrollBar_po->minimum() && _PageQueryNumber_u32 > 0) {
      --_PageQueryNumber_u32;
    }
    else {
      return;
    }
    _QSqlQuery_po->first();
    _QSqlQuery_po->seek(_PageQueryNumber_u32 * (PageSize_ci / 2) - 1);
    _SeekRowNumber_u32 = 0;
    ShowTableInformation_v();
    _Widget_po->SQLQueryTable_tw->verticalScrollBar()->setValue(_PageQueryNumber_u32 > 0 ? _Widget_po->SQLQueryTable_tw->verticalScrollBar()->maximum() / 2 : 0);
  }
}
