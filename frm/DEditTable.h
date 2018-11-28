#ifndef DEDITTABLE_H
#define DEDITTABLE_H

#include "DAddTable.h"

namespace sqliteviewer
{
  class DEditTable : public DAddTable
  {
    Q_OBJECT

  public:
    /** @brief Конструктор */
    explicit DEditTable(const QString & DBName_ro, QWidget *parent = 0);
    /** @brief Деструктор */
    virtual ~DEditTable();
    /** @brief Устанавливаем имя таблицы*/
    void SetTableName_v(const QString &);
    /** @brief Сформированная SQL строка */
    virtual QString GetSQLQuery_str() override final;
    /** @brief Сформированные SQL строки */
    QStringList GetSQLQueries_lst() { return _Queries_lst; }

  private:
    QString _DataBaseName_str;  ///< @brief Имя базы данных
    QString _LastTableName_str; ///< @brief Заданное имя таблицы
    QStringList _Queries_lst;   ///< @brief Комплект действий по данной таблице
    qint32 _OldRowCounts_i32;  ///< @brief Счетчик старых записей

    //----------------------------------------------------------
    /** @brief Показывает загруженную таблицу */
    void ShowTableInfo_v();
    /** @brief Добавляем строку по полученным данным */
    void AddRow_v(const QString & Name_str, const QString & Type_str, bool IsNotNull_b = false,
                  const QString & Default_str = QString(), bool IsUnique_b = false,
                  bool IsPrimaryKey_b = false, bool IsForeignKey_b = false,
                  const QString & Reference_str = QString());
    /** @brief Формирование списка действий над БД */
    void QueriesFormation_v();

  protected slots:
    /** @brief Слот добавления строки в таблицу */
    virtual void AddRow_slt() override final;
    /** @brief Слот удаления строки из таблицы */
    virtual void DeleteRow_slt() override final;
    /** @brief Выбор строки */
    virtual void SelectRow_slt(int, int) override final;
    /** @brief Действия при положительном закрытии диалога */
    virtual void AcceptedDialog_slt() override final;
  };
}

#endif // DEDITTABLE_H
