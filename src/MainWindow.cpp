#include "MainWindow.h"

#include <giomm/simpleactiongroup.h>
#include <gtkmm/action.h>
#include <gtkmm/menu.h>
#include <gtkmm/uimanager.h>

#include <gtkmm/filechooserdialog.h>

#include <iostream>

namespace hexsage {
MainWindow::MainWindow(ModelPtr model)
    : _model(model), _hex_view_widget(_model) {
  set_title("hexsage");
  set_border_width(10);

  add(_vbox);

  _vbox.pack_start(_hex_view_widget, Gtk::PACK_EXPAND_WIDGET);

  show_all_children();

  add_action("open", [this]() {
    std::cout << "open" << std::endl;
    Gtk::FileChooserDialog dialog("Select the file to open",
                                  Gtk::FILE_CHOOSER_ACTION_OPEN);
    dialog.set_transient_for(*this);
    dialog.add_button("Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("Open", Gtk::RESPONSE_OK);
    int result = dialog.run();
    if (result == Gtk::RESPONSE_OK) {
      std::string file = dialog.get_filename();
      _model->load(file);
    }
  });

  add_action("quit", [this]() { hide(); });
}

MainWindow::~MainWindow() {}

} // namespace hexsage
