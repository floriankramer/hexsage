#include "HexsageApplication.h"
#include "MainWindow.h"

int main(int argc, char **argv) {
  Glib::RefPtr<hexsage::HexsageApplication> app =
      hexsage::HexsageApplication::create(argc, argv);

  hexsage::MainWindow main_window;
  return app->run(main_window);
}
