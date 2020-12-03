#pragma once

#include <gtkmm/application.h>


namespace hexsage {

class HexsageApplication : public Gtk::Application {
public:
  HexsageApplication();
  HexsageApplication(int argc, char **argv);
  virtual ~HexsageApplication();


  template <typename... Args>
  static Glib::RefPtr<HexsageApplication> create(Args... args) {
    return Glib::RefPtr<HexsageApplication>(new HexsageApplication(args...));
  }

  void on_startup() override;

private:
  void buildMenu();

};
} // namespace hexsage
