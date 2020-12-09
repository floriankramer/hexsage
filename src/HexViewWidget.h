#pragma once

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

#include "Model.h"

#include <gtkmm/widget.h>

namespace hexsage {

class HexViewWidget : public Gtk::Widget {
public:
  HexViewWidget(ModelPtr model);
  virtual ~HexViewWidget();

  void setModel(ModelPtr model);

  virtual Gtk::SizeRequestMode get_request_mode_vfunc() const override;

  virtual void get_preferred_height_vfunc(int &minimum_height,
                                          int &natural_height) const override;
  virtual void get_preferred_width_vfunc(int &minimum_height,
                                         int &natural_height) const override;

  virtual void
  get_preferred_height_for_width_vfunc(int width, int &minimum_height,
                                       int &natural_height) const override;
  virtual void
  get_preferred_width_for_height_vfunc(int height, int &minimum_width,
                                       int &natural_width) const override;

  virtual void on_size_allocate(Gtk::Allocation &allocation) override;

  virtual void on_realize() override;
  virtual void on_unrealize() override;

  virtual bool on_draw(const Cairo::RefPtr<::Cairo::Context> &cr) override;

  virtual bool on_key_press_event(GdkEventKey *event) override;

private:
  ModelPtr _model;

  size_t _view_offset = 0;

  Glib::RefPtr<Gdk::Window> _window;

  size_t _bytes_per_line = 0;
};

} // namespace hexsage
