#pragma once

#include <gtkmm/application.h>
#include "MainWindow.h"

namespace hexsage {

class HexsageApplication : public Gtk::Application {
public:
  HexsageApplication(int argc, char **argv);
  virtual ~HexsageApplication();

  template <typename... Args>
  static Glib::RefPtr<HexsageApplication> create(Args... args) {
    return Glib::RefPtr<HexsageApplication>(new HexsageApplication(args...));
  }


protected:
  void on_startup() override;

  void on_activate() override;

  void on_open(const Gio::Application::type_vec_files& files,
    const Glib::ustring& hint) override;

  void on_hide_window(Gtk::Window *window);

private:
  MainWindow *createMainWindow();

  void buildMenu();
};
} // namespace hexsage
