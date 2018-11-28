#ifndef DADDTABLE_H
#define DADDTABLE_H

#include <QDialog>

namespace Ui {
  class DAddTable;
}

namespace sqliteviewer
{
  class DAddTable : public QDialog
  {
    Q_OBJECT

  public:
    /** @brief Конструктор */
    explicit DAddTable(QWidget *parent = 0);
    /** @brief Деструктор */
    virtual ~DAddTable();
    /** @brief Сформированная SQL строка */
    virtual QString GetSQLQuery_str();

  protected:
    Ui::DAddTable *_Widget_po;  ///< @brief Интерфейс пользователя
    qint32 _SelectedRow_i32;  ///< @brief Выбранная строка

    enum RowItems_En {
      _RIName_en,       ///< @brief Имя данного
      _RIType_en,       ///< @brief Тип данного
      _RINotNull_en,    ///< @brief Данное не може быть нулевым
      _RIDefault_en,    ///< @brief Значение по умолчанию
      _RIUnique_en,     ///< @brief Данное уникально
      _RIPrimaryKey_en, ///< @brief Основной ключ
      _RIForeignKey_en, ///< @brief Внешний ключ
      _RIReference_en   ///< @brief Ссылка для внешнего ключа
    };

  protected slots:
    /** @brief Слот добавления строки в таблицу */
    virtual void AddRow_slt();
    /** @brief Слот удаления строки из таблицы */
    virtual void DeleteRow_slt();
    /** @brief Выбор строки */
    virtual void SelectRow_slt(int, int);
    /** @brief Действия при положительном закрытии диалога */
    virtual void AcceptedDialog_slt();
  };

}

#endif // DADDTABLE_H
