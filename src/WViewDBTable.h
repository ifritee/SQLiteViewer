#ifndef WVIEWDBTABLE_H
#define WVIEWDBTABLE_H

#include <QString>
#include <QGraphicsRectItem>
#include <QVector>

namespace sqliteviewer
{
  class WViewDBTable : public QGraphicsRectItem
  {
  public:
    /** @brief Конструктор */
    WViewDBTable(QGraphicsItem *parent = Q_NULLPTR);
    /** @brief Деструктор */
    virtual ~WViewDBTable();
    /** @brief Добавляет информацию по строке в таблицу*/
    void AddValueData_v(const QString & Name_str, const QString & Type_str, bool NotNull_b, const QString & Default_str, bool PrimaryKey_b);

  private:
    struct SValueData {
      QString _ValueName_str; ///< @brief Имя значения
      QString _ValueType_str; ///< @brief Тип значения
      bool _NotNull_b;         ///< @brief NN
      QString _Default_str;   ///< @brief Значение по умолчанию
      bool _PrimaryKey_b;      ///< @brief PK
    };
    QVector<SValueData *> _AllValues_vec; ///< @brief Все значения для таблицы
  };
}
#endif // WVIEWDBTABLE_H
