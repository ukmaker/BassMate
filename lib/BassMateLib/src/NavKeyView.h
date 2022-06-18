#ifndef BM_NAVKEY_VIEW_H
#define BM_NAVKEY_VIEW_H

#include "IView.h"
#include "Events/EventModel.h"

#include "Events/NavKeyWrappers/NavKeyEventWrapper.h"

namespace bassmate
{

  using namespace simplegui;

  /*
   * Implementation of IView to use SimpleGUI with NavKey
   */
  class NavKeyView : public IView, EventListener
  {
  public:
    /********************************************************************
     * Specific to NavKeyView
     ********************************************************************/

    /********************************************************************
     * Implementation of EventListener
     ********************************************************************/
    virtual void handleEvent(Event e) {
      if(_focus != nullptr) {
        NavKeyEventWrapper *next = _focus->handleEvent(e);
      }
    }

    NavKeyEventWrapper *_focus;

    virtual void setEventHandler(EventHandler *handler) {}

    virtual void unsetEventHandler(EventHandler *handler) {}

    /********************************************************************
     * Implementation of IView
     ********************************************************************/
    void run()
    {
    }

    
    /********************************************************************
     * Methods to supply values to the UI
     ********************************************************************/
    void setVolume(uint8_t volume);
    void setTempo(uint8_t tempo);
    void setPlayPauseStop(PlayPauseStop pps);
    void setChannelVolume(uint8_t channel, uint8_t volume);
    void setChannelInstruments(uint8_t channel, const Family instruments,
                               const Voice selectedInstrument);
    void setChannelFamily(uint8_t channel, const Bank *bank,
                          const Family selectedFamily);

  };
} // namespace bassmate
#endif