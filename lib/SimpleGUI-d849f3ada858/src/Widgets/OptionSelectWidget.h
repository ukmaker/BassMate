#ifndef SIMPLEGUI_OPTIONSELECT_WIDGET_H
#define SIMPLEGUI_OPTIONSELECT_WIDGET_H

#include "Widgets/AbstractSelectWidget.h"
#include "Widgets/Option.h"


/**
* a select widget allows the user to choose one of a set of choices
* Choices are presented as scrolling text, with the selected choice centred and bolded
* Choices are mapped to a backing value
**/
template<typename T>
class OptionSelectWidget : public AbstractSelectWidget<T>
{

public:

    OptionSelectWidget(GraphicsContext *context, const GFXfont* font, const Option<T> *options, const uint16_t numChoices) : 
        AbstractSelectWidget<T>(context),
        _options(options),
        _numChoices(numChoices)
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

        return _options[this->getSelectedIndex()].text;
    }

    const T getSelectedValue() {
        if(!this->isSelected()) {
            return 0;
        }

        return _options[this->getSelectedIndex()].value;
    }
    const char* getText(uint16_t idx) {
        return _options[idx].text;
    }

    const T getValue(uint16_t idx) {
        return _options[idx].value;
    }

    uint16_t getNumChoices() {
        return _numChoices;
    }

    void setOptions(const Option<T> *options, uint16_t numChoices) {
        _options = options;
        _numChoices = numChoices;
        this->unselect();
        this->setViewIndex(0);
    }

private:
    const Option<T> *_options;
    uint16_t _numChoices;
};

#endif