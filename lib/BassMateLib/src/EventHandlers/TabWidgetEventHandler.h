#ifndef BM_TABWIDGET_EVENT_HANDLER_H
#define BM_TABWIDGET_EVENT_HANDLER_H

#include "Widgets/TabWidget.h"
#include "WidgetEventHandler.h"

namespace bassmate {

    /*
    * Class to use NavKey events to drive a TabWidget
    **/

    class TabWidgetEventHandler : WidgetEventHandler<TabWidget> {

        public:
        TabWidgetEventHandler(TabWidget *w) : _w(w) {

        }

        virtual void handleUp() {}
        virtual void handleDown() {}
        virtual void handleIncrement() {}
        virtual void handleDecrement() {}
        virtual void handleLeft() {}
        virtual void handleRight() {}
        virtual void handleSelect() {}


        protected:
        TabWidget *_w;

    };
}
#endif