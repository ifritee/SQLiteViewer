#ifndef DCREATEDB_H
#define DCREATEDB_H

#include <QDialog>
#include <QString>

namespace Ui {
  class DCreateDB;
}

namespace sqliteviewer
{

  class DCreateDB : public QDialog
  {
    Q_OBJECT

  public:
    /** @brief Конструктор */
    explicit DCreateDB(QWidget *parent = 0);
    /** @brief Деструктор */
    virtual ~DCreateDB();
    /** @brief Возвращает имя БД с польным путем */
    QString GetDataBase_str();
    /** @brief Возвращает имя преобразуемого файла или "" */
    QString GetExtFile_str();

  private:
    Ui::DCreateDB *_Widget_po;  ///< @brief Интерфейс пользователя

  private slots:
    /** @brief Установка папки*/
    void SetDirectory_slt();
    /** @brief Установка папки*/
    void SetExtFile_slt();
  };

}

#endif // DCREATEDB_H
