#include "MainWindow.h"

#include <gtkmm/menu.h>
#include <gtkmm/action.h>
#include <gtkmm/uimanager.h>
#include <giomm/simpleactiongroup.h>

namespace hexsage {
MainWindow::MainWindow() {
  set_title("hexsage");
  set_border_width(10);


  //_label.set_text("Hello World!");
  //add(_label);
  //_label.show();

  createMenu();

  show_all_children();

}
MainWindow::~MainWindow() {

}

void MainWindow::createMenu() {

}
}
