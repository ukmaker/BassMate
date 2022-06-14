#ifndef SIMPLEGUI_OPTARRAYSELECT_WIDGET_H
#define SIMPLEGUI_OPTARRAYSELECT_WIDGET_H

#include "Widgets/AbstractSelectWidget.h"
#include "Widgets/Option.h"


/**
* a select widget allows the user to choose one of a set of choices
* Choices are presented as scrolling text, with the selected choice centred and bolded
* Choices are mapped to a backing value
**/
template<typename T>
class OptArraySelectWidget : public AbstractSelectWidget<T>
{

public:

    OptArraySelectWidget(GraphicsContext *context, const GFXfont* font, Array<Opt<T>> options) : 
        AbstractSelectWidget<T>(context),
        _options(options)
    {
        this->setFont(font);
    }

    const Option<T> getSelectedOption() {
        return _options[this->getSelectedIndex()];
    }

    const Option<T> getViewOption() {
        return _options[this->getViewIndex()];
    }

    const char* getSelectedText() {
        if(!this->isSelected()) {
            return nullptr;
        }

        return _options[this->getSelectedIndex()].getLabel();
    }

    const T getSelectedValue() {
        if(!this->isSelected()) {
            return 0;
        }

        return _options[this->getSelectedIndex()].getValue();
    }
    const char* getText(uint16_t idx) {
        return _options[idx].getLabel();
    }

    const T getValue(uint16_t idx) {
        return _options[idx].getValue();
    }

    uint16_t getNumChoices() {
        return _options.size;
    }

    void setOptions(const Array<Opt<T>> options) {
        _options = options;
        this->unselect();
        this->setViewIndex(0);
    }

private:
    const Array<Opt<T>>  _options;
};

#endif