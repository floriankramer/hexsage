#pragma once

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

private:
  ModelPtr _model;

  Glib::RefPtr<Gdk::Window> _window;
};

} // namespace hexsage
