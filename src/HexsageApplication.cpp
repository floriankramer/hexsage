#include "HexsageApplication.h"

#include <gtkmm.h>
#include <iostream>

namespace hexsage {

HexsageApplication::HexsageApplication(int argc, char **argv)
    : Gtk::Application(argc, argv, "hex.sage",
                       Gio::ApplicationFlags::APPLICATION_HANDLES_OPEN) {}

HexsageApplication::~HexsageApplication() {}

void HexsageApplication::on_startup() {
  Gtk::Application::on_startup();
  buildMenu();
}

void HexsageApplication::on_activate() { createMainWindow()->present(); }

void HexsageApplication::on_open(const Gio::Application::type_vec_files &files,
                                 const Glib::ustring &hint) {
  for (const Glib::RefPtr<Gio::File> &file : files) {
    MainWindow *window = createMainWindow();
    window->model().load(file->get_path());
    window->present();
  }
};

void HexsageApplication::on_hide_window(Gtk::Window *window) { delete window; }

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

MainWindow *HexsageApplication::createMainWindow() {
  auto window = new MainWindow();
  add_window(*window);
  // ensure we free the windows memory again
  window->signal_hide().connect(sigc::bind<Gtk::Window *>(
      sigc::mem_fun(*this, &HexsageApplication::on_hide_window), window));
  return window;
}
} // namespace hexsage
