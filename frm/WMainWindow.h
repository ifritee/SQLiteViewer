#ifndef WMAINWINDOW_H
#define WMAINWINDOW_H

#include <QMainWindow>

class QTreeWidgetItem;
class QPoint;
class QFile;

namespace Ui {
  class WMainWindow;
}

namespace sqliteviewer
{

  class DAbout;

  /**
   * @class WMainWindow
   * @brief Класс основного окна приложения
   */
  class WMainWindow : public QMainWindow
  {
    Q_OBJECT

  public:
    /** @brief Конструктор */
    explicit WMainWindow(QWidget *parent = 0);
    /** @brief Деструктор */
    virtual ~WMainWindow();
    /** @brief Конвертирование файла заданного типа */
    void ConvertFileToBase_v(const QFile &);

  private:
    Ui::WMainWindow *_Widget_po;  ///< @brief Интерфейс пользователя
    DAbout *_DAbout_po;           ///< @brief Диалог "О программе..."
    bool _IsDBOpenned_b;          ///< @brief Флаг открытой БД
    QString _DBFullName_str;      ///< @brief Полное имя БД
    QTreeWidgetItem * _SelectMenuTreeItem_po; ///< @brief Последний итем выбранный в меню
    QString _TreeHeaderText_str; ///< @brief Текст хедера дерева
    QStringList _AllDBTables_lst; ///< @brief Все таблицы в этой БД

    //---------------------------------------------------------------
    /** @brief Открытие БД*/
    bool OpenDB_b(const QString &);
    /** @brief Отображение всех таблиц*/
    void ShowAllTables_v();

  public slots:
    /** @brief Соединение с существующей БД */
    void ConnectDB_slt(const QString & DBFileName_str = QString());

  private slots:
    /** @brief Создание БД */
    void CreateDB_slt();
    /** @brief Закрытие БД */
    void CloseDB_slt();
    /** @brief Выбор индекса в дереве */
    void SetTreeIndex_slt(QTreeWidgetItem *, int);
    /** @brief Выбор индекса в дереве */
    void ClickedTreeIndex_slt(QTreeWidgetItem *, int);
    /** @brief Вызов меню ПКМ на TreeWidget */
    void PrepareTreeContextMenu_slt(const QPoint &);
    /** @brief Удаление таблицы */
    void DeleteTable_slt();
    /** @brief Добавление таблицы */
    void AddTable_slt();
    /** @brief Редактирование таблицы */
    void EditTable_slt();
    /** @brief Окончание запроса */
    void FinishedQuery_slt();
    /** @brief Формирование DDS скрипта */
    void CreateDDS_slt();
  };
}

#endif // WMAINWINDOW_H
