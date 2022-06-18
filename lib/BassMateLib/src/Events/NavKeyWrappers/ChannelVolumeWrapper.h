#ifndef BASSMATE_CHANNEL_VOLUME_WRAPPER_H
#define BASSMATE_CHANNEL_VOLUME_WRAPPER_H

#include "Events/NavKeyWrappers/NavKeyEventWrapper.h"
#include "Widgets/ChannelVolumeWidget.h"

namespace bassmate
{

    using namespace simplegui;

    template <int T>
    class ChannelVolumeWrapper : public NavKeyEventWrapper
    {

    public:
        ChannelVolumeWrapper(ChannelVolumeWidget<T> *widget)
            : NavKeyEventWrapper(widget) {}
        ~ChannelVolumeWrapper() {}

        ChannelVolumeWidget<T> *wrappedWidget()
        {
            return static_cast<ChannelVolumeWidget<T> *>(this->_wrapped);
        }

        virtual NavKeyEventWrapper *handleEvent(Event e)
        {

            NavKeyEventWrapper *to = this;

            switch (e.type)
            {
            case NAV_DEC:
                wrappedWidget()->decrement();
                break;

            case NAV_LEFT:
                to = _moveLeft(true);
                break;

            case NAV_INC:
                wrappedWidget()->increment();
                break;

            case NAV_RIGHT:
                to = _moveRight(true);
                break;

            case NAV_DOWN:
                wrappedWidget()->down();
                break;

            case NAV_UP:
                if (wrappedWidget()->getSelected() == 0)
                {
                    to = _moveUp(true);
                }
                else
                {
                    wrappedWidget()->up();
                }
                break;

            default:
                _onEvent.call(e);
                break;
            }

            return to;
        };
    };

}
#endif