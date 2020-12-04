#pragma once

#include <gtkmm/applicationwindow.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>

#include "HexViewWidget.h"

#include "Model.h"

namespace hexsage {
class MainWindow : public Gtk::ApplicationWindow {
public:
   MainWindow(ModelPtr model = std::make_shared<Model>());
   virtual ~MainWindow();

private:
   ModelPtr _model;


   Gtk::Box _vbox;

   Gtk::Label _label;

   HexViewWidget _hex_view_widget;


};
}
