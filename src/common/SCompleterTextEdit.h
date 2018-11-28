#ifndef SCOMPLETERTEXTEDIT_H
#define SCOMPLETERTEXTEDIT_H

#include <QTextEdit>

class QCompleter;

class SCompleterTextEdit : public QTextEdit
{
  Q_OBJECT

public:
  SCompleterTextEdit(QWidget *parent = 0);
  virtual ~SCompleterTextEdit();

  void SetCompleter_v(QCompleter * QCompleter_po);
  QCompleter *GetCompleter_po() const;

protected:
    void keyPressEvent(QKeyEvent * Event_po) override;
    void focusInEvent(QFocusEvent * Event_po) override;
    void mouseReleaseEvent(QMouseEvent *Event_po) override;

private slots:
    void InsertCompletion_slt(const QString &);

signals:
    void SelectedText_sig(const QString &);

private:
    QCompleter *_QCompleter_po;

    //------------------------------------------------------------
    QString TextUnderCursor_str() const;
};

#endif // SCOMPLETERTEXTEDIT_H
