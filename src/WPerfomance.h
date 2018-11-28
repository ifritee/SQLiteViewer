#ifndef WPERFOMANCE_H
#define WPERFOMANCE_H

#include <QGraphicsView>
#include <QMap>

class QGraphicsScene;


namespace sqliteviewer
{
  class WViewDBTable;

  class WPerfomance : public QGraphicsView
  {
  public:
    /** @brief Конструктор */
    WPerfomance(QWidget *parent = Q_NULLPTR);
    /** @brief Конструктор с параметром сцены */
    WPerfomance(QGraphicsScene *scene, QWidget *parent = Q_NULLPTR);
    /** @brief Деструктор */
    virtual ~WPerfomance();
    /** @brief Зададим имена всех таблиц */
    void SetAllDBTableNames_v(const QStringList & AllDBTables_lst, const QString &);

  private:
    QGraphicsScene * _Scene_po; ///< @brief Основноая сцена
    QMap<QString, WViewDBTable *> _AllDBTables_map; ///< @brief Все доступные таблицы
  };

}

#endif // WPERFOMANCE_H
