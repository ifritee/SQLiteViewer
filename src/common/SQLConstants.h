#ifndef SQLCONSTANTS_H
#define SQLCONSTANTS_H

#include <QString>
#include <QStringList>

namespace sqliteviewer
{

  class SQLConstants;

  /**
 * @class SQLConstantsDestroyer
 * @brief Класс-деструктор для Logger
 */
  class SQLConstantsDestroyer
  {
  private:
    SQLConstants * _SQLConstants_po;
  public:
    virtual ~SQLConstantsDestroyer();
    void InitDestoyer_v(SQLConstants * SQLConstants_po);
  };

  class SQLConstants
  {
  private:
    static SQLConstants * _SQLConstants_po; ///< @brief Объект класса, с которым идет работа
    static SQLConstantsDestroyer _SQLConstantsDestroyer_o; ///< @brief Объект класса-деструктора

  protected:
    SQLConstants();
    ~SQLConstants();
    friend class SQLConstantsDestroyer;

  public:
    /** @brief Метод для получения объекта _APIInternalData_po */
    static SQLConstants * GetInstance_po();

    QStringList GetSQLTags_lst() { return _SQLTags_lst; }

    QStringList GetSQLINTEGERTypes_lst() { return _SQLINTEGERTypes_lst; }
    QStringList GetSQLTEXTTypes_lst() { return _SQLTEXTTypes_lst; }
    QStringList GetSQLNONETypes_lst() { return _SQLNONETypes_lst; }
    QStringList GetSQLREALTypes_lst() { return _SQLREALTypes_lst; }
    QStringList GetSQLNUMERICTypes_lst() { return _SQLNUMERICTypes_lst; }
    QStringList GetSQLMAINTypes_lst() { return _SQLMAINTypes_lst; }

  private:
    QStringList _SQLTags_lst;

    QStringList _SQLINTEGERTypes_lst;
    QStringList _SQLTEXTTypes_lst;
    QStringList _SQLNONETypes_lst;
    QStringList _SQLREALTypes_lst;
    QStringList _SQLNUMERICTypes_lst;
    QStringList _SQLMAINTypes_lst;
  };

}

#endif // SQLCONSTANTS_H
