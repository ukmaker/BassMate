#ifndef SIMPLEGUI_OPTION_H
#define SIMPLEGUI_OPTION_H


template <typename T>
struct Option {
  char *text;
  T value;
};

template <typename T>
class Opt {
 public:
  Opt(T v) : value(v) {}
  virtual char *getLabel() = 0;
  T getValue();

  T value;
};

#endif