#include "HexsageApplication.h"
#include "MainWindow.h"

int main(int argc, char **argv) {
  Glib::RefPtr<hexsage::HexsageApplication> app =
      hexsage::HexsageApplication::create(argc, argv);
  return app->run();
}
