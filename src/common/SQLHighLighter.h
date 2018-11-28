#ifndef SQLHIGHLIGHTER_H
#define SQLHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>

class QTextDocument;

namespace sqliteviewer
{

  class SQLHighLighter : public QSyntaxHighlighter
  {
    Q_OBJECT

  public:
    SQLHighLighter(QTextDocument * parent = 0);

  protected:
    void highlightBlock(const QString &text) Q_DECL_OVERRIDE;

  private:
    // Состояние подсветки, в которой находится текстовый блок на момент его закрытия
    enum EnTextBlockStates {
      _TBSNone_en,       // Без подсветки
      _TBSTag_en,        // Подсветка внутри тега
      _TBSComment_en,    // Внутри комментария
      _TBSQuote_en       // Внутри кавычек, которые внутри тега
    };

    class HighlightingRule
    {
    public:
      QRegExp _Pattern_o;
      QTextCharFormat _TextCharFormat_o;
    };
    QVector<HighlightingRule> _TagRules_o;      // Правила форматирования для тегов
    QVector<HighlightingRule> _TypeRules_o;     // Правила форматирования для типов данных

    QRegExp _CommentStartExpression_o;          // Регулярка начала комментария
    QRegExp _CommentEndExpression_o;            // Регулярка закрытия комментария
    QTextCharFormat _MultiLineCommentFormat_o;  // Форматирование текста внутри комментария

    QRegExp _Quotes_o;                          // Регулярное выражение для текста в кавычках внутри тега
    QTextCharFormat _QuotationFormat_o;         // Форматирование текста в кавычках внутри тега
    QTextCharFormat _TagsFormat_o;              // Форматирование самих тегов
    QTextCharFormat _TypeFormat_o;          // Форматирование типов данных
  };

}
#endif // SQLHIGHLIGHTER_H
