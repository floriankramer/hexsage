#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace hexsage {

class Model {
public:
  typedef std::function<void()> OnCursorChangeHandler;
  typedef std::function<void()> OnDataChangeHandler;


  void load(const std::string &path);

  const std::vector<uint8_t> &data() const;

  size_t cursorPos() const;
  void setCursorPos(size_t pos);

  void moveCursorPos(ssize_t by);

  size_t cursorByte() const;
  void setCursorByte(size_t byte);

  void addOnCursorChangedHandler(void *owner, OnCursorChangeHandler handler);
  void removeOnCursorChangedHandlers(void *owner);

  void addOnDataChangedHandler(void *owner, OnDataChangeHandler handler);
  void removeOnDataChangedHandlers(void *owner);

private:
  void emitCursorChanged();
  void emitDataChanged();

  std::vector<uint8_t> _data;

  // The position of the cursor in 4 bit increments (position 4 is the third byte)
  size_t _cursor_pos;

  std::vector<std::pair<void *, OnCursorChangeHandler>> _on_cursor_change_handlers;
  std::vector<std::pair<void *, OnDataChangeHandler>> _on_data_change_handlers;
};

typedef std::shared_ptr<Model> ModelPtr;

} // namespace hexsage
