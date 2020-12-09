/**
 * Copyright 2020 Florian Kramer
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "HexViewWidget.h"
#include <cstring>
#include <iostream>

#include <gdk/gdk.h>
#include <gdkmm/event.h>
#include <gtkmm.h>

namespace hexsage {

const char CHAR_LOOKUP[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                            '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

HexViewWidget::HexViewWidget(ModelPtr model)
    : Glib::ObjectBase("HexViewWidget"), Gtk::Widget(), _model(model) {
  set_has_window();
  set_name("hex-view-widget");
  set_can_focus();
  set_focus_on_click();

  _model->addOnDataChangedHandler(this, [this]() { queue_draw(); });
  _model->addOnCursorChangedHandler(this, [this]() { queue_draw(); });

  add_events(Gdk::KEY_PRESS_MASK);
}

HexViewWidget::~HexViewWidget() {
  _model->removeOnCursorChangedHandlers(this);
  _model->removeOnDataChangedHandlers(this);
}

bool HexViewWidget::on_key_press_event(GdkEventKey *event) {
  if (event->keyval == GDK_KEY_Right) {
    if (event->state & GDK_SHIFT_MASK) {
      _model->moveCursorPos(8);
    } else {
      _model->moveCursorPos(1);
    }
    return true;
  } else if (event->keyval == GDK_KEY_Left) {
    if (event->state & GDK_SHIFT_MASK) {
      _model->moveCursorPos(-8);
    } else {
      _model->moveCursorPos(-1);
    }
    return true;
  } else if (event->keyval == GDK_KEY_Up) {
    if (event->state & GDK_SHIFT_MASK) {
      _model->moveCursorPos(-_bytes_per_line * 2 * 10);
    } else {
      _model->moveCursorPos(-_bytes_per_line * 2);
    }
    return true;
  } else if (event->keyval == GDK_KEY_Down) {
    if (event->state & GDK_SHIFT_MASK) {
      _model->moveCursorPos(_bytes_per_line * 2 * 10);
    } else {
      _model->moveCursorPos(_bytes_per_line * 2);
    }
    return true;
  } else if (event->keyval == GDK_KEY_g) {
    _model->setCursorPos(0);
  } else if (event->keyval == GDK_KEY_G) {
    _model->setCursorByte(_model->data().size());
  }

  return Widget::on_key_press_event(event);
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

  constexpr size_t GROUPING_SIZE = 4;
  const Gtk::Allocation allocation = get_allocation();

  // Fill the background
  Gdk::Cairo::set_source_rgba(cr, Gdk::RGBA("#333"));
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

  double character_width = extends.x_advance;
  double character_height = extends.height * 1.5;
  // The amount of free space above each character.
  double character_head_space = extends.height * 0.5;

  // three characters per bytes
  _bytes_per_line = allocation.get_width() / (character_width * 3);
  if (_bytes_per_line == 0) {
    return true;
  }
  // one additional space per four bytes
  _bytes_per_line -= (_bytes_per_line / GROUPING_SIZE);
  // Only show full blocks
  _bytes_per_line = size_t(_bytes_per_line / GROUPING_SIZE) * GROUPING_SIZE;
  size_t lines_on_page = allocation.get_height() / character_height;
  if (lines_on_page == 0) {
    return true;
  }

  size_t cursor_byte = _model->cursorByte();

  // Ensure the cursor is visible
  size_t max_visible_byte = _view_offset + (_bytes_per_line * lines_on_page);
  if (cursor_byte < _view_offset) {
    _view_offset = (cursor_byte / _bytes_per_line) * _bytes_per_line;
  } else if (cursor_byte >= max_visible_byte) {
    _view_offset = (cursor_byte / _bytes_per_line) * _bytes_per_line;
    _view_offset -= _bytes_per_line * (lines_on_page - 1);
  }

  // draw the cursor
  Gdk::Cairo::set_source_rgba(cr, Gdk::RGBA("#daa"));
  size_t cursor_pos = _model->cursorPos();
  size_t cursor_col = (cursor_pos - _view_offset * 2) % (_bytes_per_line * 2);
  // Add the spaces between bytes and between groups
  cursor_col += cursor_col / 2 + cursor_col / (GROUPING_SIZE * 2);
  size_t cursor_row = (cursor_pos - _view_offset * 2) / (_bytes_per_line * 2);
  cr->move_to(cursor_col * character_width,
              cursor_row * character_height + character_head_space);
  cr->line_to((cursor_col + 1) * character_width,
              cursor_row * character_height + character_head_space);
  cr->line_to((cursor_col + 1) * character_width,
              (cursor_row + 1) * character_height);
  cr->line_to(cursor_col * character_width,
              (cursor_row + 1) * character_height);
  cr->fill();

  // Draw the data
  Gdk::Cairo::set_source_rgba(cr, Gdk::RGBA("#d5d5d5"));
  std::string line(allocation.get_width() / character_width, ' ');
  for (size_t line_idx = 0; line_idx < lines_on_page; ++line_idx) {
    // assemble the line
    for (size_t i = 0; i < line.size(); ++i) {
      line[i] = ' ';
    }
    size_t byte_offset = _view_offset + line_idx * _bytes_per_line;
    if (byte_offset >= _model->data().size()) {
      break;
    }
    for (size_t b_idx = 0; b_idx < _bytes_per_line; ++b_idx) {
      if (b_idx + byte_offset >= _model->data().size()) {
        break;
      }
      uint8_t byte = _model->data()[byte_offset + b_idx];
      line[b_idx * 3 + 0 + b_idx / GROUPING_SIZE] =
          CHAR_LOOKUP[(byte >> 4) & 15];
      line[b_idx * 3 + 1 + b_idx / GROUPING_SIZE] = CHAR_LOOKUP[byte & 15];
    }

    cr->move_to(0, (line_idx + 1) * character_height);
    cr->show_text(line);
  }

  return true;
}

} // namespace hexsage
