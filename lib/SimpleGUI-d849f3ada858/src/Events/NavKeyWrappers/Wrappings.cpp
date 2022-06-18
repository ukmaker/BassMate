#include "Events/NavKeyWrappers/Wrappings.h"
#include "Events/NavKeyWrappers/NavKeyEventWrapper.h"

namespace simplegui
{

  void Wrappings::focusWrapper(NavKeyEventWrapper *wrapper) { _focus = wrapper; }

  NavKeyEventWrapper *Wrappings::focus(Widget *w)
  {
    NavKeyEventWrapper *wrapper;
    _wrappers.reset();
    while (wrapper = _wrappers.next())
    {
      if (wrapper->wrapped() == w)
      {
        focus(wrapper->wrapped());
        return wrapper;
      }
    }
    return nullptr;
  }

  NavKeyEventWrapper *Wrappings::wrapperFor(Widget *w)
  {
    NavKeyEventWrapper *wrapper;
    _wrappers.reset();
    while (wrapper = _wrappers.next())
    {
      if (wrapper->wrapped() == w)
      {
        return wrapper;
      }
    }
    return nullptr;
  }

  void Wrappings::handleEvent(Event e)
  {
    if (_focus != nullptr)
    {
      NavKeyEventWrapper *next = _focus->handleEvent(e);
      if (next != _focus && next != nullptr)
      {
        _focus->wrapped()->unfocus();
        next->wrapped()->focus();
        _focus = next;
      }
    }
  }

  SelectEventWrapper *Wrappings::wrapSelect(SelectWidget *select)
  {
    SelectEventWrapper *w = new SelectEventWrapper(select);
    append(w);
    return w;
  }

  KeyboardEventWrapper *Wrappings::wrap(KeyboardWidget *keyboard)
  {
    KeyboardEventWrapper *w = new KeyboardEventWrapper(keyboard);
    append(w);
    return w;
  }

  ContextWindowEventWrapper *Wrappings::wrap(ContextWindow *window)
  {
    ContextWindowEventWrapper *w = new ContextWindowEventWrapper(window);
    append(w);
    return w;
  }

  TabWidgetEventWrapper *Wrappings::wrap(TabWidget *tabWidget)
  {
    TabWidgetEventWrapper *w = new TabWidgetEventWrapper(tabWidget);
    append(w);
    return w;
  }

  void Wrappings::append(NavKeyEventWrapper *wrapper)
  {
    _wrappers.append(wrapper);
    _focus = wrapper;
  }

} // namespace simplegui
