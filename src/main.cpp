#include <Wire.h>

#include "Model.h"
#include "NoteGrid.h"
#include "Presenter.h"
#include "STMDMA.h"
#include "View.h"

#include "Events/NavKeyEventDispatcher.h"
#include "Events/NavKeyWrappers/Wrappings.h"

#define LEN(N) (sizeof(N) / sizeof(N[0]))

#define SCK PB13
#define MISO PB14
#define MOSI PB15
#define SC_CS PB12
#define SC_DC PA8
#define SC_RESET PB4

#define SCL PB6
#define SDA PB7
#define NK_INT PB8
#define NT_INT PB9

#define VS_MOSI PA7
#define VS_MISO PA6
#define VS_SCK PA5
#define VS_CS PB1
#define VS_DCS PB10
#define VS_RESET PB0

using namespace bassmate;

SPIClass spi2(MOSI, MISO, SCK, SC_CS);
Adafruit_ILI9341_NG tft = Adafruit_ILI9341_NG(&spi2, SC_DC, SC_CS, SC_RESET);

volatile bool spiDmaTransferComplete = true;
STMDMA stmdma(&tft, 32768, SPI2, DMA1_Stream4);

extern "C" {
void DMA1_Stream4_IRQHandler() {
  HAL_DMA_IRQHandler(&stmdma._dma);
  // spiDmaTransferComplete = true;
}
}

extern "C" {
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
  spiDmaTransferComplete = true;
}
}

/*
 * Hardware interfaces
 */
SPIClass spi1(VS_MOSI, VS_MISO, VS_SCK);
Adafruit_NeoTrellis t_array[1][2] = {

    {Adafruit_NeoTrellis(0x2E), Adafruit_NeoTrellis(0x2F)}

};
Adafruit_MultiTrellis trellis((Adafruit_NeoTrellis *)t_array, 1, 2);
NoteGrid noteGrid(trellis, NT_INT);

/*
 * Model
 */
i2cNavKey navkey(0b0010000); /* Default address when no jumper are soldered */
VS1053 midi(VS_CS, VS_DCS, VS_RESET, spi1, Serial2);
Sequencer sequencer;
Storage *storage = new Storage();
uint8_t instrumentGroupMap[4];
Model model(navkey, NK_INT, midi, sequencer, storage);

/*
 * View
 */
View view(&tft, noteGrid, storage);

Presenter presenter(model, view, noteGrid);

/**
 * Global event wiring
 */
/*
void UP_Button_Pressed(i2cNavKey *p) { view.navKeyEventDispatcher->UP_Button_Pressed(p); }
void DOWN_Button_Pressed(i2cNavKey *p) { view.navKeyEventDispatcher->DOWN_Button_Pressed(p); }
void LEFT_Button_Pressed(i2cNavKey *p) { view.navKeyEventDispatcher->LEFT_Button_Pressed(p); }
void RIGHT_Button_Pressed(i2cNavKey *p) { view.navKeyEventDispatcher->RIGHT_Button_Pressed(p); }

void Encoder_Increment(i2cNavKey *p) { view.navKeyEventDispatcher->Encoder_Increment(p); }
void Encoder_Decrement(i2cNavKey *p) { view.navKeyEventDispatcher->Encoder_Decrement(p); }
void Encoder_Push(i2cNavKey *p) { view.navKeyEventDispatcher->Encoder_Push(p); }
void Encoder_Release(i2cNavKey *p) { view.navKeyEventDispatcher->Encoder_Release(p); }
*/

void UP_Button_Pressed(i2cNavKey *p) { presenter.handleUpPress(); }
void DOWN_Button_Pressed(i2cNavKey *p) { presenter.handleDownPress(); }
void LEFT_Button_Pressed(i2cNavKey *p) { presenter.handleLeftPress(); }
void RIGHT_Button_Pressed(i2cNavKey *p) { presenter.handleRightPress(); }

void Encoder_Increment(i2cNavKey *p) { presenter.handleIncrement(); }
void Encoder_Decrement(i2cNavKey *p) { presenter.handleDecrement(); }
void Encoder_Push(i2cNavKey *p) { presenter.handleSelectStart(); }
void Encoder_Release(i2cNavKey *p) { presenter.handleSelect(); }




void beatCallback(uint8_t beat) { presenter.handleBeat(beat); }
void noteOnCallback(uint8_t beat, MIDINote note) {
  presenter.handleNoteOn(beat, note);
}
void noteOffCallback(uint8_t beat, MIDINote note) {
  presenter.handleNoteOff(beat, note);
}
void noteGridEventHandler(keyEvent e) { presenter.handleNoteGridEvent(e); }
void noteGridKeyPressCallback(uint8_t x, uint8_t y) {
  presenter.handleNoteGridKeyPress(x, y);
}
void noteGridKeyLongPressCallback(uint8_t x, uint8_t y) {}

void setupNavKey() {
  navkey.begin(i2cNavKey::FLOAT_DATA | i2cNavKey::WRAP_ENABLE |
               i2cNavKey::DIRE_RIGHT | i2cNavKey::IPUP_ENABLE);

  navkey.writeCounter((float)0); /* Reset the counter value */
  navkey.writeMax((float)0.95);  /* Set the maximum threshold*/
  navkey.writeMin((float)0.05);  /* Set the minimum threshold */
  navkey.writeStep((float)0.05); /* Set the step to 1*/

  navkey.writeDoublePushPeriod(5);  // not used yet..
  navkey.onUpPush = UP_Button_Pressed;
  navkey.onDownPush = DOWN_Button_Pressed;
  navkey.onLeftPush = LEFT_Button_Pressed;
  navkey.onRightPush = RIGHT_Button_Pressed;
  navkey.onIncrement = Encoder_Increment;
  navkey.onDecrement = Encoder_Decrement;
  navkey.onCentralPush = Encoder_Push;
  navkey.onCentralRelease = Encoder_Release;
  navkey.autoconfigInterrupt();
}

void DMACallback(DMA_HandleTypeDef *_hdma) { spiDmaTransferComplete = true; }

void setup() {
  Serial.begin(9600);
  spi1.begin(48000000);
  spi2.begin(48000000);

  pinMode(VS_MOSI, OUTPUT);
  pinMode(VS_MISO, INPUT);
  pinMode(VS_SCK, OUTPUT);
  pinMode(VS_CS, OUTPUT);
  pinMode(VS_DCS, OUTPUT);
  pinMode(VS_RESET, OUTPUT);
  digitalWrite(VS_MOSI, HIGH);
  digitalWrite(VS_SCK, HIGH);
  digitalWrite(VS_CS, HIGH);
  digitalWrite(VS_DCS, HIGH);
  digitalWrite(VS_RESET, HIGH);
  pinMode(PA2, OUTPUT);

  pinMode(NK_INT, INPUT_PULLUP);
  pinMode(NT_INT, INPUT_PULLUP);
  pinMode(SC_RESET, OUTPUT);
  digitalWrite(SC_RESET, 0);

  pinMode(SCL, OUTPUT);
  pinMode(SDA, OUTPUT);
  digitalWrite(SCL, 1);
  digitalWrite(SDA, 1);

  Wire.begin((uint32_t)SDA, (uint32_t)SCL);
  Wire.setClock(400000);
  Serial2.begin(31250);

  setupNavKey();

  tft.setSTMDMA(&stmdma);
  view.fontRenderer->setBlitter(&stmdma);
  HAL_DMA_RegisterCallback(&stmdma._dma, HAL_DMA_XFER_CPLT_CB_ID, DMACallback);
  spiDmaTransferComplete = true;
  stmdma.begin();

  tft.begin(48000000);
  tft.setRotation(1);

  view.begin();
  view.makeUI();
  view.wrapHomeWidgets();

  trellis.begin();
  noteGrid.attachEventHandler(noteGridEventHandler);
  noteGrid.attachKeyPressCallback(noteGridKeyPressCallback);
  noteGrid.attachKeyLongPressCallback(noteGridKeyLongPressCallback);
  noteGrid.setFlashBeat(true);
  noteGrid.begin();

  sequencer.attachNoteOnCallback(noteOnCallback);
  sequencer.attachNoteOffCallback(noteOffCallback);
  sequencer.attachBeatCallback(beatCallback);
  sequencer.setTempo(70);

  model.begin();
  presenter.begin();
  view.testFillScreen();
}

void loop() {
  presenter.tick();
  view.run();
  model.run();
}
