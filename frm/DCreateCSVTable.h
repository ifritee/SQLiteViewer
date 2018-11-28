#ifndef DCREATECSVTABLE_H
#define DCREATECSVTABLE_H

#include <QDialog>
#include <QHash>

namespace Ui {
  class DCreateCSVTable;
}

namespace sqliteviewer
{

  class DCreateCSVTable : public QDialog
  {
    Q_OBJECT

  public:
    DCreateCSVTable(QStringList Columns_lst, QWidget *parent = nullptr);
    ~DCreateCSVTable();
    /** @brief Возвращает строку типа (ИМЯ ТИП, ИМЯ ТИП,... */
    QString GetCSVHeaderWithTypes_str();

  private:
    Ui::DCreateCSVTable *_Widget_po;
    QHash<QChar, QString> _Translit_hash;

    /** @brief Переводит строку в транслит */
    QString GetTranslit_str(const QString &);
  };

}

#endif // DCREATECSVTABLE_H
