#include <QCompleter>
#include <QKeyEvent>
#include <QAbstractItemView>
#include <QApplication>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QScrollBar>
#include <QDebug>

#include "SCompleterTextEdit.h"

SCompleterTextEdit::SCompleterTextEdit(QWidget *parent) : QTextEdit(parent),
  _QCompleter_po(nullptr)
{

}

SCompleterTextEdit::~SCompleterTextEdit()
{

}

void SCompleterTextEdit::SetCompleter_v(QCompleter * QCompleter_po)
{
  if (_QCompleter_po) {
    QObject::disconnect(_QCompleter_po, 0, this, 0);
  }

  _QCompleter_po = QCompleter_po;

  if (!_QCompleter_po) {
    return;
  }

  _QCompleter_po->setWidget(this);
  _QCompleter_po->setCompletionMode(QCompleter::PopupCompletion);
  _QCompleter_po->setCaseSensitivity(Qt::CaseInsensitive);
  QObject::connect(_QCompleter_po, SIGNAL(activated(QString)), this, SLOT(InsertCompletion_slt(QString)));
}

QCompleter * SCompleterTextEdit::GetCompleter_po() const
{
  return _QCompleter_po;
}

void SCompleterTextEdit::InsertCompletion_slt(const QString & Completion_str)
{
  if (_QCompleter_po->widget() != this) {
    return;
  }
  QTextCursor QTextCursor_o = textCursor();
  QTextCursor_o.movePosition(QTextCursor::StartOfWord);
  QTextCursor_o.movePosition(QTextCursor::EndOfWord);
  QTextCursor_o.select(QTextCursor::WordUnderCursor);
  QTextCursor_o.removeSelectedText();
  QTextCursor_o.insertText(Completion_str);

  setTextCursor(QTextCursor_o);
}

QString SCompleterTextEdit::TextUnderCursor_str() const
{
  QTextCursor QTextCursor_o = textCursor();
  QTextCursor_o.select(QTextCursor::WordUnderCursor);
  return QTextCursor_o.selectedText();
}

void SCompleterTextEdit::focusInEvent(QFocusEvent * Event_po)
{
  if (_QCompleter_po) {
    _QCompleter_po->setWidget(this);
  }
  QTextEdit::focusInEvent(Event_po);
}

void SCompleterTextEdit::keyPressEvent(QKeyEvent * Event_po)
{
  if (_QCompleter_po && _QCompleter_po->popup()->isVisible()) {
    // The following keys are forwarded by the completer to the widget
    switch (Event_po->key()) {
    case Qt::Key_Enter:
    case Qt::Key_Return:
    case Qt::Key_Escape:
    case Qt::Key_Tab:
    case Qt::Key_Backtab:
      Event_po->ignore();
      return; // let the completer do default behavior
    default:
      break;
    }
  }

  bool IsShortcut_b = ((Event_po->modifiers() & Qt::ControlModifier) && Event_po->key() == Qt::Key_Space); // CTRL+Space
  if (!_QCompleter_po || !IsShortcut_b) // do not process the shortcut when we have a completer
    QTextEdit::keyPressEvent(Event_po);

  const bool CtrlOrShift_b = Event_po->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
  if (!_QCompleter_po || (CtrlOrShift_b && Event_po->text().isEmpty()))
    return;

  static QString EndOfWord_str("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
  bool HasModifier_b = (Event_po->modifiers() != Qt::NoModifier) && !CtrlOrShift_b;
  QString CompletionPrefix_str = TextUnderCursor_str();

  if (!IsShortcut_b && (HasModifier_b || Event_po->text().isEmpty()|| CompletionPrefix_str.length() < 3
                        || EndOfWord_str.contains(Event_po->text().right(1)))) {
    _QCompleter_po->popup()->hide();
    return;
  }

  if (CompletionPrefix_str != _QCompleter_po->completionPrefix()) {
    _QCompleter_po->setCompletionPrefix(CompletionPrefix_str);
    _QCompleter_po->popup()->setCurrentIndex(_QCompleter_po->completionModel()->index(0, 0));
  }
  QRect QRect_o = cursorRect();
  QRect_o.setWidth(_QCompleter_po->popup()->sizeHintForColumn(0) + _QCompleter_po->popup()->verticalScrollBar()->sizeHint().width());
  _QCompleter_po->complete(QRect_o); // popup it up!
}

void SCompleterTextEdit::mouseReleaseEvent(QMouseEvent *Event_po)
{
  if(!textCursor().selectedText().isEmpty()) {
    emit SelectedText_sig(textCursor().selectedText());
  }
  QTextEdit::mouseReleaseEvent(Event_po);
}
