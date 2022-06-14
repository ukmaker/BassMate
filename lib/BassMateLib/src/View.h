#ifndef BM_VIEW_H
#define BM_VIEW_H

#include <Adafruit_GFX_NG.h>  // Core graphics library

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

#define LEN(N) (sizeof(N) / sizeof(N[0]))

namespace bassmate {
using namespace simplegui;
using namespace bassmate;
/*
 * View encapsulates all the UI components
 * Activation of components is controlled by the presenter
 * which also forwards UI interactions as appropriate
 * **/
class View {
 public:
  const uint16_t BG = Color::fadeToBlack(Color::map(0x003399),
                                         80);  // Color::fadeToBlack(WHITE, 25);
  const uint16_t FG = WHITE;
  const uint16_t HL = YELLOW;
  const uint16_t POP = BG;
  const GFXfont defaultFont = FreeSans9pt7b;
  const char *ch[4] = {"CH #1", "CH #2", "CH #3", "CH #4"};

  View(Adafruit_ILI9341_NG *tft, NoteGrid &ng, Storage *storage)
      : _tft(tft), noteGrid(ng) {
    _storage = storage;
    fontRenderer = new DefaultFontRenderer_NG(&defaultFont, 240, 320);
    dispatcher = new EventDispatcher();
    context = new GraphicsContext(tft, dispatcher, &defaultFont, fontRenderer);
    gui = new GUI(context);
    tft->setFontRenderer(fontRenderer);
  }

  void begin() {
    gui->rootWindow()->setSize(context->display()->width(),
                               context->display()->height());
  }

  void makeUI() {
    def(gui->rootWindow(), "ROOT");
    gui->rootWindow()->setBorderWidth(8);
    gui->rootWindow()->setBackground(BG);
    gui->rootWindow()->setBorderColor(FG);
    gui->rootWindow()->setPadding(2);
    gui->rootWindow()->setBevel(true);
    gui->rootWindow()->setLayout(Window::FIXED);

    contextWindow = new ContextWindow(context);
    def(contextWindow, "topWindow");
    contextWindow->setLayout(Window::FIXED);
    contextWindow->setSize(300, 220);
    contextWindow->setTitleBarBackground(HL);
    contextWindow->setTitleBarForeground(BG);
    contextWindow->setTitleBarHeight(30);

    sequencerWindow = new Window(context);
    def(sequencerWindow, "Sequencer");
    sequencerWindow->setLayout(Window::FIXED);
    sequencerWindow->setLocation(0, 32);
    sequencerWindow->setSize(300, 178);

    presetsWindow = new Window(context);
    def(presetsWindow, "Presets");
    presetsWindow->setLayout(Window::FIXED);
    presetsWindow->setLocation(0, 32);
    presetsWindow->setSize(300, 178);
    presetsWindow->setBackground(BG);

    volume = new VolumeWidget(context);
    def(volume, "Volume");
    volume->setPercent(75);
    volume->setSize(60, 16);
    volume->setLocation(6, 6);
    volume->setStep(1);
    volume->setMode(VolumeWidget::ANTILOG);

    volumeLabel = new TextWidget(context);
    def(volumeLabel, "");
    volumeLabel->setSize(100, 28);
    volumeLabel->setText("Vol");
    volumeLabel->setLocation(68, 0);

    tempoWidget = new ValueWidget(context);
    def(tempoWidget, "");
    tempoWidget->setMax(300);
    tempoWidget->setMin(20);
    tempoWidget->setStep(1);
    tempoWidget->setFont(&defaultFont);
    tempoWidget->setSize(60, 28);
    tempoWidget->setLocation(200, 0);

    pps = new PlayPauseStopWidget(context);
    def(pps, "");
    pps->setLocation(270, 0);
    pps->setSize(20, 20);

    channelTabs = new TabWidget(context);
    def(channelTabs, "channelTabs");
    channelTabs->setLocation(0, 30);

    VoiceWrapper *wrapper = new VoiceWrapper();

    voiceSelect = new WrapperSelectWidget<Voice>(context, wrapper, nullptr, 0);
    def(voiceSelect, "Voice");
    voiceSelect->setLocation(0, 0);
    voiceSelect->setSize(290, 160);
    voiceSelect->setHighlightColor(HL);

    FamilyWrapper *familyWrapper = new FamilyWrapper();
    familySelect =
        new WrapperSelectWidget<Family>(context, familyWrapper, nullptr, 0);
    def(familySelect, "Family");
    familySelect->setLocation(0, 0);
    familySelect->setSize(290, 160);
    familySelect->setHighlightColor(HL);

    channelVols = new ChannelVolumeWidget<4>(context);
    def(channelVols, "Vol");
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
    channelVols->setSize(290, 120);
    channelVols->setFont(&defaultFont);
    channelVols->setTextRight();
    channelVols->setHighlightColor(HL);
    channelVols->setStep(5);

    storageSelect = new StorageSelectWidget<8>(context, _storage);
    def(storageSelect, "Load");
    storageSelect->setSize(280, 170);

    presetPreviewWidget = new PresetPreviewWidget(context);
    def(presetPreviewWidget, "Presets");
    presetPreviewWidget->setSize(280, 200);

    keyboard = new KeyboardWidget(context, textBuffer, 9);
    def(keyboard, "Save");
    keyboard->setSize(280, 200);
    keyboard->setHighlightForegroundColor(BLACK);
    keyboard->setHighlightBackgroundColor(YELLOW);
    keyboard->setText("hello world", 11);

    savingDialog = new TextWidget(context);
    def(savingDialog, "");
    savingDialog->setText("Saving..");
    savingDialog->setSize(300, 190);
    savingDialog->setLocation(0, 30);
    savingDialog->setHAlign(HAlign::CENTER);
    savingDialog->setVAlign(VAlign::MIDDLE);
    savingDialog->hide();

    presetsTabs = new TabWidget(context);
    def(presetsTabs, "Presets");
    presetsTabs->setLocation(0, 30);
    presetsTabs->setSize(299, 190);
    presetsTabs->attach(storageSelect);
    presetsTabs->attach(keyboard);
    presetsTabs->attach(presetPreviewWidget);

    sequencerWindow->attach(volume);
    sequencerWindow->attach(volumeLabel);
    sequencerWindow->attach(tempoWidget);
    sequencerWindow->attach(pps);
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

  void def(Widget *w, const char *label) {
    w->setForeground(FG);
    w->setBackground(BG);
    w->setLabel(label);
  }

  void def(TabWidget *w, const char *label) {
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

  void testFillScreen() {
    _tft->fillScreen(ILI9341_BLACK);
    _tft->fillScreen(ILI9341_RED);
    _tft->fillScreen(ILI9341_GREEN);
    _tft->fillScreen(ILI9341_BLUE);
  }

  void selectContextSequencer() {
    contextWindow->focus();
    contextWindow->select(sequencerWindow);
    channelTabs->selectTab(channelVols);
    channelVols->setHighlight(-1);
    volume->setForeground(FG);
    volumeLabel->setForeground(FG);
    tempoWidget->setForeground(FG);
    pps->setForeground(FG);
  }

  void selectContextPresets() {
    contextWindow->focus();
    contextWindow->select(presetsTabs);
    unselectTabs(presetsTabs);
  }

  void unselectTabs(TabWidget *w) {
    w->setTabSelectedFontColor(WHITE);
    w->setTabUnselectedFontColor(WHITE);
    w->setTabSelectedColor(BG);
    w->setTabUnselectedColor(BG);
    w->selectTab(nullptr);
  }

  void selectTabByIndex(TabWidget *w, uint8_t idx) {
    w->setTabSelectedFontColor(BG);
    w->setTabUnselectedFontColor(WHITE);
    w->setTabSelectedColor(HL);
    w->setTabUnselectedColor(BG);
    w->selectTabByIndex(idx);
  }

  void selectTab(TabWidget *w, Widget *t) {
    w->setTabSelectedFontColor(BG);
    w->setTabUnselectedFontColor(WHITE);
    w->setTabSelectedColor(HL);
    w->setTabUnselectedColor(BG);
    w->selectTab(t);
  }

  void selectPresetsSaveTab() {
    contextWindow->unfocus();
    contextWindow->select(presetsTabs);
    selectTab(presetsTabs, keyboard);
  }

  void selectPresetsLoadTab() {
    contextWindow->unfocus();
    contextWindow->select(presetsTabs);
    selectTab(presetsTabs, storageSelect);
    storageSelect->load();
  }

  void selectPresetsSaveUI() {
    contextWindow->select(presetsTabs);
    selectTab(presetsTabs, keyboard);
    keyboard->focus();
    contextWindow->unfocus();
  }

  void selectPresetsLoadUI() {
    contextWindow->select(presetsTabs);
    selectTab(presetsTabs, storageSelect);
    contextWindow->unfocus();
    storageSelect->load();
  }

  void selectTop() {
    contextWindow->select(sequencerWindow);
    contextWindow->unfocus();

    channelTabs->selectTab(channelVols);
    channelVols->setHighlight(-1);
  }

  void selectTopVolume() {
    selectTop();
    volume->setForeground(HL);
    volumeLabel->setForeground(HL);
    tempoWidget->setForeground(FG);
    pps->setForeground(FG);
    channelTabs->setTabSelectedFontColor(FG);
    channelTabs->setTabUnselectedFontColor(FG);
    channelTabs->setTabSelectedColor(BG);
    channelTabs->setTabUnselectedColor(BG);
  }

  void selectTopTempo() {
    selectTop();
    volume->setForeground(FG);
    volumeLabel->setForeground(FG);
    tempoWidget->setForeground(HL);
    pps->setForeground(HL);
  }

  void selectChannel(uint8_t c) {
    channelTabs->setTabSelectedFontColor(BG);
    channelTabs->setTabUnselectedFontColor(WHITE);
    channelTabs->setTabSelectedColor(HL);
    channelTabs->setTabUnselectedColor(BG);
    channelTabs->selectTab(channelVols);
    volume->setForeground(FG);
    volumeLabel->setForeground(FG);
    channelVols->setHighlight(c);
    channelVols->setLabel("Vol");
    familySelect->setLabel("Family");
  }

  void selectChannelInstrument(uint8_t c, const Family instruments,
                               const Voice selectedInstrument) {
    voiceSelect->setValues(instruments.voices, instruments.numVoices);
    voiceSelect->setSelectedValue(selectedInstrument);
    channelTabs->selectTab(voiceSelect);
    channelVols->setLabel(ch[c]);
    familySelect->setLabel(instruments.name);
  }

  void selectChannelInstrumentGroup(uint8_t channel, const Bank *bank,
                                    const Family selectedFamily) {
    familySelect->setValues(bank->families, bank->numFamilies);
    familySelect->setSelectedValue(selectedFamily);
    familySelect->setLabel(selectedFamily.name);
    // force redraw to take account of the family name change
    channelTabs->noteDirty();
    channelTabs->selectTab(familySelect);
  }

  void showSaving() {
    savingDialog->show();
    contextWindow->hide();
  }

  void hideSaving() {
    savingDialog->hide();
    contextWindow->show();
  }

  void run() {
    gui->run();
    noteGrid.tick();
  }

 public:
  EventDispatcher *dispatcher;
  DefaultFontRenderer_NG *fontRenderer;
  GraphicsContext *context;
  GUI *gui;
  Storage *_storage;

  ContextWindow *contextWindow;

  Window *sequencerWindow, *presetsWindow;
  VolumeWidget *volume;
  TextWidget *volumeLabel, *savingDialog, *selectionTextWidget;
  ValueWidget *tempoWidget;
  PlayPauseStopWidget *pps;

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

 protected:
  Adafruit_ILI9341_NG *_tft;
};
}  // namespace bassmate
#endif