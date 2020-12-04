#pragma once

#include <gtkmm/application.h>
#include "Model.h"

namespace hexsage {

class HexsageApplication : public Gtk::Application {
public:
  HexsageApplication(int argc, char **argv);
  virtual ~HexsageApplication();

  template <typename... Args>
  static Glib::RefPtr<HexsageApplication> create(Args... args) {
    return Glib::RefPtr<HexsageApplication>(new HexsageApplication(args...));
  }

  ModelPtr model() {
    return _model;
  }


protected:
  void on_startup() override;

  void on_open(const Gio::Application::type_vec_files& files,
    const Glib::ustring& hint) override;

private:
  void buildMenu();

  ModelPtr _model;
};
} // namespace hexsage
