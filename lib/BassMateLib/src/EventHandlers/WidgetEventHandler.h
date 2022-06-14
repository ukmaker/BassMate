#ifndef BM_WIDGET_EVENT_HANDLER_H
#define BM_WIDGET_EVENT_HANDLER_H

#include "Widgets/Widget.h"

namespace bassmate {

    /*
    * Class to use NavKey events to drive a TabWidget
    **/
    template<class T>
    class WidgetEventHandler {

        public:

        virtual void handleUp() {}
        virtual void handleDown() {}
        virtual void handleIncrement() {}
        virtual void handleDecrement() {}
        virtual void handleLeft() {}
        virtual void handleRight() {}
        virtual void handleSelect() {}


        protected:
        T *_w;

    };
}
#endif