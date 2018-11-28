#ifndef WDBINGUIRY_H
#define WDBINGUIRY_H

#include <QMainWindow>

class QCompleter;
class QStringListModel;
class QSqlQuery;
class QToolBar;
class QSpinBox;
class QAction;
class QLabel;

namespace Ui {
  class WDBInquiry;
}

namespace sqliteviewer
{

  class SQLHighLighter;

  class WDBInquiry : public QMainWindow
  {
    Q_OBJECT

  public:
    /** @brief Конструктор */
    explicit WDBInquiry(QWidget *parent = 0);
    /** @brief Деструктор */
    virtual ~WDBInquiry();
    /** @brief Зададим имена всех таблиц */
    void SetAllDBTableNames_v(const QStringList & AllDBTables_lst);
    /** @brief Зададим имя БД */
    void SetDBName_v(const QString & DBFullName_str);
    /** @brief Отображение информации в табличном виде */
    void ShowTableInformation_v();
    /** @brief При обрыве соединения с БД */
    void Disconnected_v();
    /** @brief Задание основного тулбара */
    void SetMainToolBar_v(QMainWindow *Parent_po);

  private:
    Ui::WDBInquiry *_Widget_po;           ///< @brief Интерфейс пользователя
    SQLHighLighter * _SQLHighLighter_po;  ///< @brief Подсветка синтаксиса
    QCompleter *_QCompleter_po;           ///< @brief Автокомплит
    QString _DataBaseName_str;            ///< @brief Имя БД
    QString _ErrorTabName_str;            ///< @brief Имя для виджета ошибок
    QSqlQuery * _QSqlQuery_po = nullptr;  ///< @brief Основной запросник
    quint32 _PageQueryNumber_u32 = 0;     ///< @brief Номер страницы для дальнейшего отображения
    quint32 _SeekRowNumber_u32 = 0;       ///< @brief Номер строки для дальнейшего отображения
    QToolBar * _ToolBar_po;               ///< @brief Панель для навигации по страницам
    QSpinBox * _InsertRowNumber_po;       ///< @brief Ввод номера строки
    QAction * _NextRow_po;                ///< @brief Переход к строке
    QLabel * _MaxRows_po;                 ///< @brief Максимальное количество строк

  protected:
    /** @brief Метод при отображении окна */
    virtual void showEvent(QShowEvent *) override;
    /** @brief Метод при скрытии окна */
    virtual void hideEvent(QHideEvent *) override;

    //-------------------------------------------------------------------------------
    /** @brief Возвращает количество строк в запросе */
    qint32 GetQuerySize_i32(const QString & QSqlQuery_str);

  private slots:
    /** @brief Очистка запроса */
    void ClearQuery_slt();
    /** @brief Очистка ошибок */
    void ClearError_slt();
    /** @brief Действия при отпускании слайдера */
    void SQLTableSliderMoved_slt();

  public slots:
    /** @brief Исполнение запроса */
    void RunQuery_slt(const QString & ExtQuery_str = QString());
    /** @brief Сохранить скрипт */
    void SaveScript_slt();
    /** @brief Загрузить скрипт */
    void LoadScript_slt();
    /** @brief Составление отчета в excel */
    void CreateEXCELReport_slt();
    /** @brief Переход к определенной строке */
    void NextRow_slt();

  signals:
    /** @brief Сигнал об окончании выполнения запроса */
    void FinishedQuery_sig();
  };

}

#endif // WDBINGUIRY_H
