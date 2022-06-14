#ifndef BM_STORAGE_SELECT_WIDGET_H
#define BM_STORAGE_SELECT_WIDGET_H

#include "Storage.h"
#include "Widgets/AbstractSelectWidget.h"

namespace bassmate {

template <uint8_t N>
class StorageSelectWidget : public AbstractSelectWidget<Storage::Preset> {
 public:
  StorageSelectWidget(GraphicsContext *context, Storage *storage)
      : AbstractSelectWidget<Storage::Preset>(context) {
        _storage = storage;
      }

  void load() {
    _storage->open();
    for (uint8_t i = 0; i < N; i++) {
      _values[i] = _storage->get(i);
    }
    noteDirty();
  }

  // concrete subclasses must implement these methods
  const char *getSelectedText() {
    if (isSelected()) {
      return _values[getSelectedIndex()].name;
    }
    return "";
  }

  const char *getText(uint16_t idx) {
    if (idx < _storage->getNumPresets()) {
      return _values[idx].name;
    }
    return "";
  }

  const Storage::Preset getSelectedValue() {
    if (isSelected()) {
      return _values[getSelectedIndex()];
    }
    Storage::Preset h;
    h.setName("---", 3);
    return h;
  }

  const Storage::Preset getValue(uint16_t idx) {
    if (idx < _storage->getNumPresets()) {
      return _values[idx];
    }
    Storage::Preset h;
    h.setName("---", 3);
    return h;
  }

  uint16_t getNumChoices() { return _storage->getNumPresets(); }

  bool equals(Storage::Preset a, Storage::Preset b) {
    if (strcmp(a.name, b.name) != 0) {
      return false;
    }
    return true;
  }

 protected:
  Storage::Preset _values[N];
  Storage *_storage;
};
}  // namespace bassmate
#endif