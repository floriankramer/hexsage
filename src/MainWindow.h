#pragma once

#include <gtkmm/applicationwindow.h>
#include <gtkmm/label.h>

namespace hexsage {
class MainWindow : public Gtk::ApplicationWindow {
public:
   MainWindow();
   virtual ~MainWindow();

private:
   void createMenu();

   Gtk::Label _label;

};
}
