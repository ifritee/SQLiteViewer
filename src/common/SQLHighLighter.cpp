#include <QString>

#include "SQLConstants.h"
#include "SQLHighLighter.h"

namespace sqliteviewer
{

  SQLHighLighter::SQLHighLighter(QTextDocument *parent) : QSyntaxHighlighter(parent)
  {
    //-----------------------------------------------------------------------
    HighlightingRule TagHighlightingRule_o;
    _TagsFormat_o.setForeground(Qt::darkBlue);
    _TagsFormat_o.setFontWeight(QFont::Bold);
    QStringList TagPatterns_lst = SQLConstants::GetInstance_po()->GetSQLTags_lst();
    for(int i = 0; i < TagPatterns_lst.size(); ++i) {
      TagPatterns_lst[i] = "\\b" + TagPatterns_lst[i].toUpper() + "\\b";
    }
    for (const QString &pattern : TagPatterns_lst) {
      TagHighlightingRule_o._Pattern_o = QRegExp(pattern);
      TagHighlightingRule_o._TextCharFormat_o = _TagsFormat_o;
      _TagRules_o.append(TagHighlightingRule_o);
    }
    //-----------------------------------------------------------------------
    _MultiLineCommentFormat_o.setForeground(Qt::darkGray);
    _CommentStartExpression_o = QRegExp("/\\*");
    _CommentEndExpression_o = QRegExp("\\*/");
    //-----------------------------------------------------------------------
    _QuotationFormat_o.setForeground(Qt::darkGreen);
    _Quotes_o = QRegExp("\"");

    //-----------------------------------------------------------------------
    HighlightingRule TypeHighlightingRule_o;
    _TypeFormat_o.setForeground(Qt::darkGreen);
    _TypeFormat_o.setFontWeight(QFont::Bold);
    QStringList TypePatterns_lst;
    TypePatterns_lst = TypePatterns_lst + SQLConstants::GetInstance_po()->GetSQLINTEGERTypes_lst() + SQLConstants::GetInstance_po()->GetSQLNONETypes_lst() +
        SQLConstants::GetInstance_po()->GetSQLNUMERICTypes_lst() + SQLConstants::GetInstance_po()->GetSQLREALTypes_lst() + SQLConstants::GetInstance_po()->GetSQLTEXTTypes_lst();
    for(int i = 0; i < TypePatterns_lst.size(); ++i) {
      TypePatterns_lst[i] = "\\b" + TypePatterns_lst[i].toUpper() + "\\b";
    }
    for (const QString &pattern : TypePatterns_lst) {
      TypeHighlightingRule_o._Pattern_o = QRegExp(pattern);
      TypeHighlightingRule_o._TextCharFormat_o = _TypeFormat_o;
      _TypeRules_o.append(TypeHighlightingRule_o);
    }
  }

  void SQLHighLighter::highlightBlock(const QString &Text_ro)
  {
    QString NewText_str = Text_ro.toUpper();
    setCurrentBlockState(SQLHighLighter::_TBSNone_en);

    // Обработка цвета тегов, то есть подсветка слов
    for (const HighlightingRule &rule : _TagRules_o) {
      QRegExp expression(rule._Pattern_o);
      int index = expression.indexIn(NewText_str);
      while (index >= 0) {
        int length = expression.matchedLength();
        setFormat(index, length, rule._TextCharFormat_o);
        index = expression.indexIn(NewText_str, index + length);
      }
    }

    // Обработка цвета типов, то есть подсветка слов
    for (const HighlightingRule &rule : _TypeRules_o) {
      QRegExp expression(rule._Pattern_o);
      int index = expression.indexIn(NewText_str);
      while (index >= 0) {
        int length = expression.matchedLength();
        setFormat(index, length, rule._TextCharFormat_o);
        index = expression.indexIn(NewText_str, index + length);
      }
    }

    // COMMENT
    int StartCommentIndex_i = 0;
    // Если предыдущее состояние тега не является комментарием
    if (previousBlockState() != SQLHighLighter::_TBSComment_en) {
      // то пытаемся найти начало комментария
      StartCommentIndex_i = _CommentStartExpression_o.indexIn(NewText_str);
    }

    // Если комментарий найден
    while (StartCommentIndex_i >= 0) {
      // Ищем конец комментария
      int EndCommentIndex_i = _CommentEndExpression_o.indexIn(NewText_str, StartCommentIndex_i);
      int CommentLength_i;

      // Если конец не найден
      if (EndCommentIndex_i == -1) {
        // То устанавливаем состяоние Comment
        // Принцип аналогичен, что и для обычных тегов
        setCurrentBlockState(SQLHighLighter::_TBSComment_en);
        CommentLength_i = NewText_str.length() - StartCommentIndex_i;
      }
      else {
        CommentLength_i = EndCommentIndex_i - StartCommentIndex_i + _CommentEndExpression_o.matchedLength();
      }
      setFormat(StartCommentIndex_i, CommentLength_i, _MultiLineCommentFormat_o);
      StartCommentIndex_i = _CommentStartExpression_o.indexIn(NewText_str, StartCommentIndex_i + CommentLength_i);
    }
  }
}
