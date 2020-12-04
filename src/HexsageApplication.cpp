#include "HexsageApplication.h"

#include <gtkmm.h>
#include <iostream>

namespace hexsage {

HexsageApplication::HexsageApplication(int argc, char **argv)
    : Gtk::Application(argc, argv, "hex.sage",
                       Gio::ApplicationFlags::APPLICATION_HANDLES_OPEN),
      _model(std::make_shared<Model>()) {}

HexsageApplication::~HexsageApplication() {}

void HexsageApplication::on_startup() {
  Gtk::Application::on_startup();
  buildMenu();
}

void HexsageApplication::on_open(const Gio::Application::type_vec_files &files,
                                 const Glib::ustring &hint) {
  if (files.size() > 0) {
    _model->load(files[0]->get_path());
  }
};

void HexsageApplication::buildMenu() {
  set_accel_for_action("win.open", "<Primary>o");
  set_accel_for_action("win.quit", "<Primary>q");

  auto test_menu = Gio::Menu::create();

  auto file_menu = Gio::Menu::create();

  file_menu->append("Open", "win.open");
  file_menu->append("Quit", "win.quit");

  test_menu->append_submenu("file", file_menu);

  set_menubar(test_menu);
  return;
}

} // namespace hexsage
