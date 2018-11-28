#ifndef DABOUT_H
#define DABOUT_H

#include <QDialog>

namespace Ui {
  class DAbout;
}

namespace sqliteviewer
{
  class DAbout : public QDialog
  {
    Q_OBJECT

  public:
    explicit DAbout(QWidget *parent = 0);
    virtual ~DAbout();

  private:
    Ui::DAbout *_Widget_po;
  };

}

#endif // DABOUT_H
