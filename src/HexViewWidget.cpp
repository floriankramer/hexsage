#include "HexViewWidget.h"
#include <cstring>
#include <iostream>

#include <gtkmm.h>

namespace hexsage {

const char CHAR_LOOKUP[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                            '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

HexViewWidget::HexViewWidget(ModelPtr model)
    : Glib::ObjectBase("HexViewWidget"), Gtk::Widget(), _model(model) {
  set_has_window();
  set_name("hex-view-widget");

  _model->addOnDataChangedHandler(this, [this]() { queue_draw(); });
  _model->addOnCursorChangedHandler(this, [this]() { queue_draw(); });
}

HexViewWidget::~HexViewWidget() {
  _model->removeOnCursorChangedHandlers(this);
  _model->removeOnDataChangedHandlers(this);
}

void HexViewWidget::setModel(ModelPtr model) {
  if (_model) {
    _model->removeOnCursorChangedHandlers(this);
    _model->removeOnDataChangedHandlers(this);
  }
  _model = model;
  _model->addOnDataChangedHandler(this, [this]() { queue_draw(); });
  _model->addOnCursorChangedHandler(this, [this]() { queue_draw(); });
  queue_draw();
}

Gtk::SizeRequestMode HexViewWidget::get_request_mode_vfunc() const {
  return Gtk::Widget::get_request_mode_vfunc();
};

void HexViewWidget::get_preferred_height_vfunc(int &minimum_height,
                                               int &natural_height) const {
  minimum_height = 100;
  natural_height = 100;
}

void HexViewWidget::get_preferred_width_vfunc(int &minimum_width,
                                              int &natural_width) const {
  minimum_width = 100;
  natural_width = 100;
}

void HexViewWidget::get_preferred_height_for_width_vfunc(
    int width, int &minimum_height, int &natural_height) const {
  get_preferred_height_vfunc(minimum_height, natural_height);
}

void HexViewWidget::get_preferred_width_for_height_vfunc(
    int height, int &minimum_width, int &natural_width) const {
  get_preferred_width(minimum_width, natural_width);
}

void HexViewWidget::on_size_allocate(Gtk::Allocation &allocation) {
  set_allocation(allocation);

  if (_window) {
    _window->move_resize(allocation.get_x(), allocation.get_y(),
                         allocation.get_width(), allocation.get_height());
  }
}

void HexViewWidget::on_realize() {
  set_realized();
  if (!_window) {
    // We need to create the window

    GdkWindowAttr attributes;
    std::memset(&attributes, 0, sizeof(GdkWindowAttr));

    Gtk::Allocation alloc = get_allocation();
    attributes.x = alloc.get_x();
    attributes.y = alloc.get_y();
    attributes.width = alloc.get_width();
    attributes.height = alloc.get_height();

    attributes.event_mask = get_events() | Gdk::EXPOSURE_MASK;
    attributes.window_type = GDK_WINDOW_CHILD;
    attributes.wclass = GDK_INPUT_OUTPUT;

    _window = Gdk::Window::create(get_parent_window(), &attributes,
                                  GDK_WA_X | GDK_WA_Y);

    set_window(_window);

    // Apparently required for expose events
    _window->set_user_data(gobj());
  }
}

void HexViewWidget::on_unrealize() {
  _window.reset();
  Gtk::Widget::on_unrealize();
}

bool HexViewWidget::on_draw(const Cairo::RefPtr<::Cairo::Context> &cr) {
  const Gtk::Allocation allocation = get_allocation();

  // Fill the background
  Gdk::Cairo::set_source_rgba(cr, Gdk::RGBA("#363b4e"));
  cr->move_to(0, 0);
  cr->line_to(allocation.get_width(), 0);
  cr->line_to(allocation.get_width(), allocation.get_height());
  cr->line_to(0, allocation.get_height());
  cr->fill();

  // Draw in a monospace font
  cr->select_font_face("monospace", Cairo::FontSlant::FONT_SLANT_NORMAL,
                       Cairo::FontWeight::FONT_WEIGHT_NORMAL);
  cr->set_font_size(16);

  Cairo::TextExtents extends;
  cr->get_text_extents("0", extends);

  double width = extends.width;
  double height = extends.height * 1.5;

  // three characters per bytes
  size_t bytes_per_line = allocation.get_width() / (width * 3);
  // one additional space per four bytes
  bytes_per_line -= (bytes_per_line / 4);
  // Only show full blocks
  bytes_per_line = size_t(bytes_per_line / 4) * 4;
  size_t lines_on_page = allocation.get_height() / height;

  // Draw the data
  Gdk::Cairo::set_source_rgba(cr, Gdk::RGBA("#c4bbf0"));

  std::string line(allocation.get_width() / width, ' ');
  for (size_t line_idx = 0; line_idx < lines_on_page; ++line_idx) {
    // assemble the line
    for (size_t i = 0; i < line.size(); ++i) {
      line[i] = ' ';
    }
    size_t byte_offset = line_idx * bytes_per_line;
    if (byte_offset >= _model->data().size()) {
      break;
    }
    for (size_t b_idx = 0; b_idx < bytes_per_line; ++b_idx) {
      if (b_idx >= _model->data().size()) {
        break;
      }
      uint8_t byte = _model->data()[byte_offset + b_idx];
      line[b_idx * 3 + 0 + b_idx / 4] = CHAR_LOOKUP[(byte >> 4) & 15];
      line[b_idx * 3 + 1 + b_idx / 4] = CHAR_LOOKUP[byte & 15];
    }

    cr->move_to(0, (line_idx + 1) * height);
    cr->show_text(line);
  }

  return true;
}

} // namespace hexsage
