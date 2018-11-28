#include <QApplication>
#include <QDebug>

#include "SLogger.h"
#include "WMainWindow.h"

int main(int argc, char *argv[])
{
  slogger->info("Start program...");
  Q_INIT_RESOURCE(Icons);
  slogger->info("Initial resources...");
  QApplication Application_o(argc, argv);
  slogger->info("Run GUI...");
  try {
    sqliteviewer::WMainWindow MainWindow_o;
    MainWindow_o.show();
    if(argc > 1) {
      slogger->info(std::string("Load file ").append(argv[1]));
      MainWindow_o.ConnectDB_slt(QString(argv[1]));
    }
    int ReturnCode_i = Application_o.exec();
    return ReturnCode_i;
  }
  catch(...) {
    return -1;
  }
  return 0;
}
