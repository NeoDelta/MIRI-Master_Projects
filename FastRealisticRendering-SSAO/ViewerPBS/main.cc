// Author: Marc Comino 2020

#include <QApplication>
#include <QGLFormat>
#include "./main_window.h"

int main(int argc, char *argv[]) {
  //QGLFormat fmt;
  //fmt.setVersion(4, 3);
 // fmt.setProfile(QGLFormat::CoreProfile);
 // QGLFormat::setDefaultFormat(fmt);

  QSurfaceFormat fmt;
  fmt.setVersion(4, 3);
  fmt.setProfile(QSurfaceFormat::CoreProfile);
  QSurfaceFormat::setDefaultFormat(fmt);

  QApplication a(argc, argv);
  a.setAttribute(Qt::AA_UseDesktopOpenGL);
  gui::MainWindow w;
  w.show();

  return a.exec();
}
