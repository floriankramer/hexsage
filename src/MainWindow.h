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

   Model &model();
   const Model &model() const;

private:
   ModelPtr _model;


   Gtk::Box _vbox;

   Gtk::Label _label;

   HexViewWidget _hex_view_widget;


};
}
