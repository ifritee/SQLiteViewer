#ifndef CSVCONVERTER_H
#define CSVCONVERTER_H

#include <QObject>

class QFile;
class QString;

namespace sqliteviewer
{
  class CSVConverter : public QObject
  {
    Q_OBJECT
  public:
    /** @brief Конструктор */
    explicit CSVConverter(const QFile & CSVFile_rc, const QString & DatabaseName_str, QObject *parent = nullptr);
    /** @brief Проверяет возможность обработки файла и БД */
    bool IsReady_b();
    /** @brief Создает таблицу в БД */
    bool CreateTable_b();
    /** @brief Конвертирует CSV в созданную таблицу, где имя таблицы - базовое имя файла CSV */
    bool ConvertData_b();

  private:
    QFile * _CSVFile_po = nullptr;
    QString * _DBFileName_po = nullptr;

  signals:

  public slots:
  };
}
#endif // CSVCONVERTER_H
