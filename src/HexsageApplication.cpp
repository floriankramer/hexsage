#include "HexsageApplication.h"

#include <gtkmm.h>
#include <iostream>

namespace hexsage {

HexsageApplication::HexsageApplication() : Gtk::Application("hex.sage") {}

HexsageApplication::HexsageApplication(int argc, char **argv)
    : Gtk::Application(argc, argv, "hex.sage") {}

HexsageApplication::~HexsageApplication() {}

void HexsageApplication::on_startup() {
  Gtk::Application::on_startup();
  buildMenu();
}

void HexsageApplication::buildMenu() {

  auto test_menu = Gio::Menu::create();

  auto file_menu = Gio::Menu::create();
  file_menu->append("Open", "app.open");
  file_menu->append("Quit", "app.quit");

  test_menu->append_submenu("file", file_menu);

  set_menubar(test_menu);
  return;
}

} // namespace hexsage
