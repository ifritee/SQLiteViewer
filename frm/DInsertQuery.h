#ifndef DINSERTQUERY_H
#define DINSERTQUERY_H

#include <QDialog>

class QStringList;

namespace Ui {
  class DInsertQuery;
}

namespace sqliteviewer
{
  class DInsertQuery : public QDialog
  {
    Q_OBJECT

  public:
    explicit DInsertQuery(QWidget *parent = 0);
    explicit DInsertQuery(const QStringList &, QWidget *parent = 0);
    virtual ~DInsertQuery();
    void SetStringList_v(const QStringList &);
    QString GetInsertQuery_str();

  private:
    Ui::DInsertQuery *_Widget_po; ///< @brief Интерфейс пользователя
  };
}
#endif // DINSERTQUERY_H
