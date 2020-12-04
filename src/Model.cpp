#include "Model.h"

#include <fstream>
#include <iostream>

namespace hexsage {

void Model::load(const std::string &path) {
  std::ifstream in(path);
  if (!in.is_open()) {
    throw std::runtime_error("Unable to open " + path);
  }

  in.seekg(std::ios::end);
  size_t file_size_min = in.tellg();
  in.seekg(std::ios::beg);

  _data.reserve(file_size_min);
  _data.clear();

  uint8_t buffer[4096];
  while (in) {
    in.read((char *)buffer, 4096);
    _data.insert(_data.end(), buffer, buffer + in.gcount());
  }

  std::cout << "[INFO] Loaded " << _data.size() << " bytes" << std::endl;

  emitDataChanged();
  _cursor_pos = 0;
  emitCursorChanged();
}

size_t Model::cursorPos() const { return _cursor_pos; }
const std::vector<uint8_t> &Model::data() const { return _data; }

void Model::setCursorPos(size_t pos) {
  _cursor_pos = pos;
  emitCursorChanged();
}

void Model::addOnCursorChangedHandler(void *owner,
                                      OnCursorChangeHandler handler) {
  _on_cursor_change_handlers.emplace_back(owner, handler);
}
void Model::removeOnCursorChangedHandlers(void *owner) {
  size_t i = 0;
  while (i < _on_cursor_change_handlers.size()) {
    if (_on_cursor_change_handlers[i].first == owner) {
      std::iter_swap(_on_cursor_change_handlers.begin() + i,
                     _on_cursor_change_handlers.end() - 1);
      _on_cursor_change_handlers.erase(_on_cursor_change_handlers.end() - 1);
    } else {
      i++;
    }
  }
}

void Model::addOnDataChangedHandler(void *owner, OnDataChangeHandler handler) {
  _on_data_change_handlers.emplace_back(owner, handler);
}

void Model::removeOnDataChangedHandlers(void *owner) {
  size_t i = 0;
  while (i < _on_data_change_handlers.size()) {
    if (_on_data_change_handlers[i].first == owner) {
      std::iter_swap(_on_data_change_handlers.begin() + i,
                     _on_data_change_handlers.end() - 1);
      _on_data_change_handlers.erase(_on_data_change_handlers.end() - 1);
    } else {
      i++;
    }
  }
}

void Model::emitCursorChanged() {
  for (std::pair<void *, OnCursorChangeHandler> &handler :
       _on_cursor_change_handlers) {
    handler.second();
  }
}

void Model::emitDataChanged() {

  for (std::pair<void *, OnDataChangeHandler> &handler :
       _on_data_change_handlers) {
    handler.second();
  }
}

} // namespace hexsage