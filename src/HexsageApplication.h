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

#include <gtkmm/application.h>
#include "MainWindow.h"

namespace hexsage {

class HexsageApplication : public Gtk::Application {
public:
  HexsageApplication(int argc, char **argv);
  virtual ~HexsageApplication();

  template <typename... Args>
  static Glib::RefPtr<HexsageApplication> create(Args... args) {
    return Glib::RefPtr<HexsageApplication>(new HexsageApplication(args...));
  }


protected:
  void on_startup() override;

  void on_activate() override;

  void on_open(const Gio::Application::type_vec_files& files,
    const Glib::ustring& hint) override;

  void on_hide_window(Gtk::Window *window);

private:
  MainWindow *createMainWindow();

  void buildMenu();
};
} // namespace hexsage
