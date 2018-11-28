#include "SQLConstants.h"

namespace sqliteviewer
{

  SQLConstants * SQLConstants::_SQLConstants_po = 0;
  SQLConstantsDestroyer SQLConstants::_SQLConstantsDestroyer_o;

  SQLConstantsDestroyer::~SQLConstantsDestroyer()
  {
    delete _SQLConstants_po;
  }

  void SQLConstantsDestroyer::InitDestoyer_v(SQLConstants * SQLConstants_po)
  {
    _SQLConstants_po = SQLConstants_po;
  }

  SQLConstants::SQLConstants()
  {
    _SQLTags_lst << "ABORT" << "ACTION" << "ADD" << "AFTER" << "ALL" << "ALTER" << "ANALYZE" << "AND" << "AS" << "ASC" << "ATTACH" << "AUTOINCREMENT" << "BEFORE" <<
                    "BEGIN" << "BETWEEN" << "BY" << "CASCADE" << "CASE" << "CAST" << "CHECK" << "COLLATE" << "COLUMN" << "COMMIT" << "CONFLICT" << "CONSTRAINT" <<
                    "CREATE" << "CROSS" << "CURRENT_DATE" << "CURRENT_TIME" << "CURRENT_TIMESTAMP" << "DATABASE" << "DEFAULT" << "DEFERRABLE" << "DEFERRED" <<
                    "DELETE" << "DESC" << "DETACH" << "DISTINCT" << "DROP" << "EACH" << "ELSE" << "END" << "ESCAPE" << "EXCEPT" << "EXCLUSIVE" << "EXISTSEXPLAIN" <<
                    "FAIL" << "FOR" << "FOREIGN" << "FROM" << "FULL" << "GLOB" << "GROUP" << "HAVING" << "IF" << "IGNORE" << "IMMEDIATE" << "IN" << "INDEX" <<
                    "INDEXED" << "INITIALLY" << "INNER" << "INSERT" << "INSTEAD" << "INTERSECT" << "INTO" << "IS" << "ISNULL" << "JOIN" << "KEY" << "LEFT" << "LIKE" <<
                    "LIMIT" << "MATCH" << "NATURAL" << "NO" << "NOT" << "NOTNULL" << "NULL" << "OF" << "OFFSET" << "ON" << "OR" << "ORDER" << "OUTER" << "PLAN" <<
                    "PRAGMA" << "PRIMARY" << "QUERY" << "RAISE" << "RECURSIVEREFERENCES" << "REGEXP" << "REINDEX" << "RELEASE" << "RENAME" << "REPLACE" << "RESTRICT" <<
                    "RIGHT" << "ROLLBACK" << "ROW" << "SAVEPOINT" << "SELECT" << "SET" << "TABLE" << "TEMP" << "TEMPORARY" << "THEN" << "TO" << "TRANSACTION" <<
                    "TRIGGER" << "UNION" << "UNIQUE" << "UPDATE" << "USING" << "VACUUM" << "VALUES" << "VIEW" << "VIRTUAL" << "WHEN" << "WHERE" << "WITH" << "WITHOUT";

    _SQLINTEGERTypes_lst<<"INT"<<"INTEGER"<<"TINYINT"<<"SMALLINT"<<"MEDIUMINT"<<"BIGINT"<<"UNSIGNED BIG INT"<<"INT2"<<"INT8";
    _SQLTEXTTypes_lst<<"CHARACTER"<<"VARCHAR"<<"VARYING CHARACTER"<<"NCHAR"<<"NATIVE CHARACTER"<<"NVARCHAR"<<"TEXT"<<"CLOB";
    _SQLNONETypes_lst<<"BLOB"<<"<<NULL>>";
    _SQLREALTypes_lst<<"REAL"<<"DOUBLE"<<"DOUBLE PRECISION"<<"FLOATNONE";
    _SQLNUMERICTypes_lst<<"NUMERIC"<<"DECIMAL"<<"BOOLEAN"<<"DATE"<<"DATETIME";
    _SQLMAINTypes_lst<<"TEXT"<<"INTEGER"<<"REAL";
  }

  SQLConstants::~SQLConstants()
  {

  }

  SQLConstants* SQLConstants::GetInstance_po()
  {
    if(!_SQLConstants_po){
      _SQLConstants_po = new SQLConstants();
      _SQLConstantsDestroyer_o.InitDestoyer_v(_SQLConstants_po);
    }
    return _SQLConstants_po;
  }

}
