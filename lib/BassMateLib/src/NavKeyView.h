#ifndef BM_NAVKEY_VIEW_H
#define BM_NAVKEY_VIEW_H

#include "IView.h"
#include "IController.h"
#include "Events/EventModel.h"

#include "Events/NavKeyWrappers/NavKeyEventWrapper.h"
#include "Events/NavKeyWrappers/Wrappings.h"

#include <Adafruit_GFX_NG.h> // Core graphics library

#include "Adafruit_ILI9341_NG.h"
#include "Core/BorderTool.h"
#include "Core/ColorsRGB16.h"
#include "DefaultFontRenderer_NG.h"
#include "Fonts/FreeSans9pt7b.h"
#include "HardwareSerial.h"
#include "NoteGrid.h"
#include "SimpleGUI.h"
#include "Widgets/ChannelVolumeWidget.h"
#include "Widgets/ContextWindow.h"
#include "Widgets/KeyboardWidget.h"
#include "Widgets/OptionSelectWidget.h"
#include "Widgets/PlayPauseStopWidget.h"
#include "Widgets/PresetPreviewWidget.h"
#include "Widgets/SelectWidget.h"
#include "Widgets/SelectWrappers.h"
#include "Widgets/StorageSelectWidget.h"
#include "Widgets/TabWidget.h"
#include "Widgets/TextWidget.h"
#include "Widgets/ValueWidget.h"
#include "Widgets/VolumeWidget.h"
#include "Widgets/WrapperSelectWidget.h"
#include "Events/NavKeyEventDispatcher.h"
#include "Events/NavKeyWrappers/Wrappings.h"
#include "Events/NavKeyWrappers/ChannelVolumeWrapper.h"

namespace bassmate
{

  using namespace simplegui;

  /*
   * Implementation of IView to use SimpleGUI with NavKey
   */
  class NavKeyView : public IView, public EventListener
  {
  public:
    const uint16_t BG = Color::fadeToBlack(Color::map(0x003399),
                                           80); // Color::fadeToBlack(WHITE, 25);
    const uint16_t FG = WHITE;
    const uint16_t HL = YELLOW;
    const uint16_t POP = BG;
    const GFXfont defaultFont = FreeSans9pt7b;
    const char *channelNames[4] = {"CH #1", "CH #2", "CH #3", "CH #4"};

    NavKeyView(Adafruit_ILI9341_NG *tft, NoteGrid &ng, Storage *storage)
        : _tft(tft), noteGrid(ng)
    {
      _storage = storage;
      fontRenderer = new DefaultFontRenderer_NG(&defaultFont, 240, 320);
      context = new GraphicsContext(tft, &defaultFont, fontRenderer);
      gui = new GUI(context);
      tft->setFontRenderer(fontRenderer);
      _controller = nullptr;
    }

    ~NavKeyView() {}

    /********************************************************************
     * Specific to NavKeyView
     ********************************************************************/
    void setController(IController *controller) {
      _controller = controller;
    }

    /**
     * Transition to the next state by installing a new event handler
     * Once installed, it is called with a null event to ensure focus
     * is set
     **/
    template <class T>
    void transition(void (T::*mptr)(Event e))
    {
      _fp.attach(this, mptr);
      _fp.call(Event::null());
    }
    /*********************************************************************
     * NavKey event handlers
     * Each handler is responsible for setting up its widgets' context
     * and for clearing the context (blurring at minimum) when
     * transitioning to a different context and hence event handler
     ********************************************************************/
    void volumeHandler(Event e)
    {
      // Home state is Sequencer context, events directed to the
      // master volume control
      volumeWidget->focus();
      switch (e.type)
      {
      case NAV_INC:
        volumeWidget->increment();
        break;

      case NAV_DEC:
        volumeWidget->decrement();
        break;

      case NAV_UP:
        volumeWidget->blur();
        transition(&NavKeyView::contextHandler);
        break;

      case NAV_DOWN:
        volumeWidget->blur();
        transition(&NavKeyView::channelsHandler);
        break;

      case NAV_RIGHT:
        volumeWidget->blur();
        transition(&NavKeyView::tempoHandler);
        break;

      default:
        break;
      }
    }

    void tempoHandler(Event e)
    {
      tempoWidget->focus();
      playPauseStopWidget->focus();
      switch (e.type)
      {
      case NAV_INC:
        tempoWidget->increment();
        break;

      case NAV_DEC:
        tempoWidget->decrement();
        break;

      case NAV_SELECT:
        playPauseStopWidget->toggle();
        break;

      case NAV_UP:
        tempoWidget->blur();
        playPauseStopWidget->blur();
        transition(&NavKeyView::contextHandler);
        break;

      case NAV_DOWN:
        tempoWidget->blur();
        playPauseStopWidget->blur();
        transition(&NavKeyView::channelsHandler);
        break;

      case NAV_LEFT:
        tempoWidget->blur();
        playPauseStopWidget->blur();
        transition(&NavKeyView::volumeHandler);
        break;
      }
    }

    void contextHandler(Event e)
    {
      contextWindow->focus();
      switch (e.type)
      {
      case NAV_RIGHT:
        contextWindow->right();
        break;

      case NAV_LEFT:
        contextWindow->left();
        break;

      case NAV_DOWN:
      {
        contextWindow->blur();
        Widget *target = contextWindow->focusChild();
        if (target == channelTabs)
        {
          transition(&NavKeyView::volumeHandler);
        }
        else
        {
          transition(&NavKeyView::presetsHandler);
        }
      }
      break;

      case NAV_UP:
      case NAV_INC:
      case NAV_DEC:
      default:
        break;
      }
    }

    void channelsHandler(Event e)
    {
      // switch focus to the channel tabs widget
      // initially select the volumes tab
      // then transition again so to channelVolumes can handle the events
      channelTabs->focus();
      transition(&NavKeyView::channelVolsHandler);
    }

    void channelVolsHandler(Event e)
    {

      channelVols->focus();

      switch (e.type)
      {
      case NAV_INC:
        channelVols->increment();
        break;

      case NAV_DEC:
        channelVols->decrement();
        break;

      case NAV_UP:
        if (channelVols->getSelected() == 0)
        {
          channelVols->blur();
          transition(&NavKeyView::volumeHandler);
        }
        else
        {
          channelVols->up();
        }
        break;

      case NAV_DOWN:
        channelVols->down();
        break;

      case NAV_RIGHT:
        channelVols->blur();
        transition(&NavKeyView::channelInstrumentsHandler);
        break;

      default:
        break;
      }
    }

    void channelInstrumentsHandler(Event e)
    {
      voiceSelect->focus();
    }

    void presetsHandler(Event e)
    {
      presetsTabs->focus();
      switch (e.type)
      {
      case NAV_LEFT:
        presetsTabs->left();
        break;

      case NAV_RIGHT:
        presetsTabs->right();
        break;

      case NAV_UP:
        presetsTabs->blur();
        transition(&NavKeyView::contextHandler);
        break;

      case NAV_DOWN:
      {
        presetsTabs->blur();
        Widget *target = presetsTabs->focusChild();
        if (target == presetsWindow)
        {
          transition(&NavKeyView::selectPresetHandler);
        }
        else
        {
          transition(&NavKeyView::savePresetHandler);
        }
      }
      break;

      default:
        break;
      }
    }

    void selectPresetHandler(Event e)
    {
    }

    void loadPresetsHandler(Event e)
    {
    }

    void savePresetHandler(Event e)
    {
    }

    void contextWindowOnFocus(Widget *w)
    {
      // nothing to do
    }

    void contextWindowOnBlur(Widget *w)
    {
      // nothing to do
    }

    void channelTabsOnFocus(Widget *w)
    {
      channelTabs->setTabSelectedFontColor(BG);
      channelTabs->setTabUnselectedFontColor(WHITE);
      channelTabs->setTabSelectedColor(HL);
      channelTabs->setTabUnselectedColor(BG);
      channelTabs->selectTab(channelVols);
      channelVols->setLabel("Vol");
      familySelect->setLabel("Family");
    }

    void channelTabsOnBlur(Widget *w)
    {
      channelTabs->setTabSelectedFontColor(FG);
      channelTabs->setTabUnselectedFontColor(FG);
      channelTabs->setTabSelectedColor(BG);
      channelTabs->setTabUnselectedColor(BG);
      channelTabs->selectTab(channelVols);
      channelVols->setSelected(-1);
    }

    void masterVolumeOnFocus(Widget *w)
    {
      volumeWidget->setForeground(HL);
      volumeLabel->setForeground(HL);
    }

    void masterVolumeOnBlur(Widget *w)
    {
      volumeWidget->setForeground(FG);
      volumeLabel->setForeground(FG);
    }

    void tempoOnFocus(Widget *w)
    {
      tempoWidget->setForeground(HL);
      playPauseStopWidget->setForeground(HL);
    }

    void tempoOnBlur(Widget *w)
    {
      tempoWidget->setForeground(FG);
      playPauseStopWidget->setForeground(FG);
    }

    void voiceSelectOnFocus(Widget *w)
    {
      // get updated values
      uint8_t channel = static_cast<WrapperSelectWidget<Voice> *>(w)->getSelectedIndex();
      _controller->channelRefresh(channel);
      // set the labels on the tabs
      channelTabs->selectTab(voiceSelect);
      channelVols->setLabel(channelNames[channel]);
      familySelect->setLabel(familySelect->getSelectedValue().name);
    }

    void begin()
    {
      gui->rootWindow()->setSize(context->display()->width(),
                                 context->display()->height());
      makeUI();
      attachHandlers();
      _fp.attach(this, &NavKeyView::volumeHandler);
    }

    void attachHandlers()
    {
      contextWindow->onFocus(this, &NavKeyView::contextWindowOnFocus);
      contextWindow->onBlur(this, &NavKeyView::contextWindowOnFocus);
      channelTabs->onFocus(this, &NavKeyView::channelTabsOnFocus);
      channelTabs->onBlur(this, &NavKeyView::channelTabsOnBlur);
      volumeWidget->onFocus(this, &NavKeyView::masterVolumeOnFocus);
      volumeWidget->onBlur(this, &NavKeyView::masterVolumeOnBlur);
      tempoWidget->onFocus(this, &NavKeyView::tempoOnFocus);
      tempoWidget->onBlur(this, &NavKeyView::tempoOnBlur);
    }

    void makeUI()
    {
      def(gui->rootWindow(), "ROOT", 320, 240);
      {
        gui->rootWindow()->setBorderWidth(8);
        gui->rootWindow()->setBackground(BG);
        gui->rootWindow()->setBorderColor(FG);
        gui->rootWindow()->setPadding(2);
        gui->rootWindow()->setBevel(true);
        gui->rootWindow()->setLayout(Window::FIXED);
      }

      contextWindow = new ContextWindow(context);
      def(contextWindow, "topWindow", 300, 220);
      {
        contextWindow->setLayout(Window::FIXED);
        contextWindow->setTitleBarBackground(HL);
        contextWindow->setTitleBarForeground(BG);
        contextWindow->setTitleBarHeight(30);
      }

      sequencerWindow = new Window(context);
      def(sequencerWindow, "Sequencer", 300, 178);
      {
        sequencerWindow->setLayout(Window::FIXED);
        sequencerWindow->setLocation(0, 32);
      }

      presetsWindow = new Window(context);
      def(presetsWindow, "Presets", 300, 178);
      {
        presetsWindow->setLayout(Window::FIXED);
        presetsWindow->setLocation(0, 32);
        presetsWindow->setBackground(BG);
      }

      volumeWidget = new VolumeWidget(context);
      def(volumeWidget, "Volume", 60, 16);
      {
        volumeWidget->setPercent(75);
        volumeWidget->setLocation(6, 6);
        volumeWidget->setStep(1);
        volumeWidget->setMode(VolumeWidget::ANTILOG);
        volumeWidget->onChange(this, &NavKeyView::onVolumeChange);
      }

      volumeLabel = new TextWidget(context);
      def(volumeLabel, "", 100, 28);
      {
        volumeLabel->setText("Vol");
        volumeLabel->setLocation(68, 0);
      }

      tempoWidget = new ValueWidget(context);
      def(tempoWidget, "", 60, 28);
      {
        tempoWidget->setMax(300);
        tempoWidget->setMin(20);
        tempoWidget->setStep(1);
        tempoWidget->setFont(&defaultFont);
        tempoWidget->setLocation(200, 0);
      }

      playPauseStopWidget = new PlayPauseStopWidget(context);
      def(playPauseStopWidget, "", 20, 20);
      {
        playPauseStopWidget->setLocation(270, 0);
        playPauseStopWidget->onChange(this, &NavKeyView::onPlayPauseStop);
      }

      channelTabs = new TabWidget(context);
      defTab(channelTabs, "channelTabs");
      {
        channelTabs->setLocation(0, 30);
      }

      VoiceWrapper *wrapper = new VoiceWrapper();
      voiceSelect = new WrapperSelectWidget<Voice>(context, wrapper, nullptr, 0);
      def(voiceSelect, "Voice", 290, 160);
      {
        voiceSelect->setLocation(0, 0);
        voiceSelect->setHighlightColor(HL);
      }

      FamilyWrapper *familyWrapper = new FamilyWrapper();
      familySelect =
          new WrapperSelectWidget<Family>(context, familyWrapper, nullptr, 0);
      def(familySelect, "Family", 290, 160);
      {
        familySelect->setLocation(0, 0);
        familySelect->setHighlightColor(HL);
      }

      channelVols = new ChannelVolumeWidget<4>(context);
      def(channelVols, "Vol", 290, 120);
      {
        channelVols->setPercent(0, 75);
        channelVols->setPercent(1, 60);
        channelVols->setPercent(2, 45);
        channelVols->setPercent(3, 30);
        channelVols->setChannelLabel(0, "--");
        channelVols->setChannelLabel(1, "--");
        channelVols->setChannelLabel(2, "--");
        channelVols->setChannelLabel(3, "--");
        channelVols->setLocation(0, 0);
        channelVols->setTextMargin(4);
        channelVols->setTextWidth(195);
        channelVols->setFont(&defaultFont);
        channelVols->setTextRight();
        channelVols->setSelectedColor(HL);
        channelVols->setStep(5);
      }

      storageSelect = new StorageSelectWidget<8>(context, _storage);
      def(storageSelect, "Load", 280, 170);

      presetPreviewWidget = new PresetPreviewWidget(context);
      def(presetPreviewWidget, "Presets", 280, 200);

      keyboard = new KeyboardWidget(context, textBuffer, 9);
      def(keyboard, "Save", 280, 200);
      {
        keyboard->setHighlightForegroundColor(BLACK);
        keyboard->setHighlightBackgroundColor(YELLOW);
        keyboard->setText("hello world", 11);
      }

      savingDialog = new TextWidget(context);
      def(savingDialog, "", 300, 190);
      {
        savingDialog->setText("Saving..");
        savingDialog->setLocation(0, 30);
        savingDialog->setHAlign(HAlign::CENTER);
        savingDialog->setVAlign(VAlign::MIDDLE);
        savingDialog->hide();
      }

      presetsTabs = new TabWidget(context);
      defTab(presetsTabs, "Presets");
      {
        presetsTabs->setLocation(0, 30);
        presetsTabs->setSize(299, 190);
        presetsTabs->attach(storageSelect);
        presetsTabs->attach(keyboard);
        presetsTabs->attach(presetPreviewWidget);
      }

      sequencerWindow->attach(volumeWidget);
      sequencerWindow->attach(volumeLabel);
      sequencerWindow->attach(tempoWidget);
      sequencerWindow->attach(playPauseStopWidget);
      channelTabs->attach(channelVols);
      channelTabs->attach(voiceSelect);
      channelTabs->attach(familySelect);

      sequencerWindow->attach(channelTabs);

      contextWindow->attach(sequencerWindow);
      contextWindow->attach(presetsTabs);
      gui->rootWindow()->attach(contextWindow);
      gui->rootWindow()->attach(savingDialog);
      savingDialog->hide();
    }

    void def(Widget *w, const char *label, int width, int height)
    {
      w->setForeground(FG);
      w->setBackground(BG);
      w->setLabel(label);
      w->setSize(width, height);
    }

    void defTab(TabWidget *w, const char *label)
    {
      w->setForeground(FG);
      w->setBackground(BG);
      w->setBackground(BG);
      w->setTabWidth(99);
      w->setTabHeight(32);
      w->setBorderWidth(2);
      w->setBorderColor(WHITE);
      w->setTabBorderColor(WHITE);
      w->setTabBorderWidth(2);
      w->setTabPadding(2);
      w->setTabSelectedFontColor(BG);
      w->setTabUnselectedFontColor(WHITE);
      w->setTabSelectedColor(HL);
      w->setTabUnselectedColor(BG);
      w->setLayout(Window::FIXED);
      w->setLabel(label);
      w->setSize(299, 160);
    }

    /********************************************************************
     * Implementation of EventListener
     ********************************************************************/
    void handleEvent(Event e)
    {
      _fp.call(e);
    }

    /********************************************************************
     * Implementation of IView
     ********************************************************************/
    void run()
    {
      gui->run();
      noteGrid.tick();
    }

    /********************************************************************
     * Methods to supply values to the UI
     ********************************************************************/
    void setVolume(uint8_t volume)
    {
      volumeWidget->setPercent(volume);
    }

    void setTempo(uint8_t tempo)
    {
      tempoWidget->setValue(tempo);
    }

    void setPlayPauseStop(PlayPauseStop pps)
    {
      playPauseStopWidget->setState(pps);
    }

    void setChannelVolume(uint8_t channel, uint8_t volume)
    {
      channelVols->setPercent(channel, volume);
    }

    void setChannelInstruments(uint8_t channel, const Family instruments,
                               const Voice selectedInstrument)
    {
      voiceSelect->setValues(instruments.voices, instruments.numVoices);
      voiceSelect->setSelectedValue(selectedInstrument);
    }

    void setChannelFamily(uint8_t channel, const Bank *bank,
                          const Family selectedFamily)
    {
      familySelect->setValues(bank->families, bank->numFamilies);
      familySelect->setSelectedValue(selectedFamily);
    }

    /********************************************************************
     * The following callbacks are to communicate a change in the value
     * of a component
     ********************************************************************/
    void onVolumeChange(Widget *w)
    {
      _controller->volumeChange(static_cast<VolumeWidget*>(w)->getValue());
    }

    void onChannelVolumeChange(Widget *w)
    {
      _controller->channelVolumeChange(channelVols->getSelected(), channelVols->getSelectedPercent());
    }

    void onTempoChange(Widget *w)
    {
      _controller->tempoChange(static_cast<ValueWidget*>(w)->getValue());
    }
    void onPlayPauseStop(Widget *w)
    {
      _controller->playPauseStopChange(static_cast<PlayPauseStopWidget*>(w)->getState());
    }

    void onVoiceChange(Widget *w)
    {
      _controller->voiceChange(channelVols->getSelected(), voiceSelect->getSelectedValue().id);
    }

    void onVoicePreview(Widget *w)
    {
      _controller->voicePreviewChange(channelVols->getSelected(), voiceSelect->getViewValue().id);
    }

    void onFamilyChange(Widget *w)
    {
      _controller->familyChange(channelVols->getSelected(), familySelect->getSelectedValue().id);
    }

    void onLoadPreset(Widget *w)
    {
      _controller->loadPreset(storageSelect->getSelectedIndex());
    }

    void onDeletePreset(Widget *w)
    {
      _controller->deletePreset(storageSelect->getSelectedIndex());
    }

    void onSavePreset(Widget *w)
    {
      _controller->savePreset(keyboard->getText(), keyboard->getTextLen());
    }

    void onClearPresets(Widget *w)
    {
      _controller->clearPresets();
    }

    /********************************************************************
     * This group of callbacks is to ask for a component's state to be
     * refreshed e.g. because it has focus
     ********************************************************************/

    /*
     * @brief Called when the channel dialog is show to ask for the correct
     * set of instrument choices
     */
    void onChannelRefresh(uint8_t channel)
    {
      _controller->channelRefresh(channel);
    }

    /*
     * @brief Called when the load dialog is displayed to refresh the list
     * of saved presets
     */
    void onLoadRefresh()
    {
      _controller->loadRefresh();
    }

  protected:
    FunctionPointerArg1<void, Event> _fp;
    IController *_controller;

  public:
    DefaultFontRenderer_NG *fontRenderer;
    GraphicsContext *context;
    GUI *gui;
    Storage *_storage;
    Adafruit_ILI9341_NG *_tft;

    ContextWindow *contextWindow;

    Window *sequencerWindow, *presetsWindow;
    VolumeWidget *volumeWidget;
    TextWidget *volumeLabel, *savingDialog, *selectionTextWidget;
    ValueWidget *tempoWidget;
    PlayPauseStopWidget *playPauseStopWidget;

    Window *functionsContainer;
    TabWidget *presetsTabs;
    StorageSelectWidget<8> *storageSelect;
    PresetPreviewWidget *presetPreviewWidget;
    KeyboardWidget *keyboard;
    char textBuffer[9];

    TabWidget *channelTabs;
    ChannelVolumeWidget<4> *channelVols;
    WrapperSelectWidget<Voice> *voiceSelect;
    WrapperSelectWidget<Family> *familySelect;

    NoteGrid &noteGrid;
  };
} // namespace bassmate
#endif