#ifndef DCREATEDDS_H
#define DCREATEDDS_H

#include <QDialog>

namespace Ui {
  class DCreateDDS;
}

class DCreateDDS : public QDialog
{
  Q_OBJECT

public:
  explicit DCreateDDS(QWidget *parent = 0);
  ~DCreateDDS();

private:
  Ui::DCreateDDS *ui;
};

#endif // DCREATEDDS_H
