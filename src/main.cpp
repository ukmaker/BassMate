#include "Controller.h"
#include "Events/NavKeyEventDispatcher.h"
#include "Hardware/STM_SPIDMA.h"
#include "STM32/STM32F411_Timer1Encoder.h"
#include "STM32/STM32F411_Timer2Encoder.h"
#include "Model.h"
#include "NavKeyView.h"
#include "NoteGrid.h"
#include <Wire.h>
#include "configuration.h"


using namespace bassmate;

SPIClass spi2(MOSI, MISO, SCK, SC_CS);
Adafruit_ILI9341_DMA tft = Adafruit_ILI9341_DMA(&spi2, SC_DC, SC_CS, SC_RESET);

volatile bool spiDmaTransferComplete = true;
STM_SPIDMA stmdma(&tft, LCD_DMA_BUFFER_SIZE, SPI2, DMA1_Stream4);

STM32F411_Timer1Encoder volumeEncoder(VOLUME_ENCODER_PUSH_PIN);
STM32F411_Timer2Encoder tempoEncoder(TEMPO_ENCODER_PUSH_PIN);

AD5204 digipot(DP_CS_PIN, DP_SCK_PIN, DP_SDI_PIN);

extern "C" {
void DMA1_Stream4_IRQHandler()
{
    HAL_DMA_IRQHandler(&stmdma._dma);
    // spiDmaTransferComplete = true;
}
}

extern "C" {
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef* hspi)
{
    spiDmaTransferComplete = true;
}
}

/*
 * Hardware interfaces
 */
SPIClass spi1(VS_MOSI, VS_MISO, VS_SCK);
Adafruit_NeoTrellis t_array[1][2] = {

    { Adafruit_NeoTrellis(0x2E), Adafruit_NeoTrellis(0x2F) }

};
Adafruit_MultiTrellis trellis((Adafruit_NeoTrellis*)t_array, 1, 2);
NoteGrid noteGrid(trellis, NT_INT);

/*
 * Model
 */
i2cNavKey navkey(0b0010000); /* Default address when no jumper are soldered */
VS1053 midi(VS_CS, VS_DCS, VS_RESET, spi1, Serial2);
Sequencer sequencer;
Storage* storage = new Storage();
uint8_t instrumentGroupMap[4];
Model model(navkey, NK_INT, midi, sequencer, storage, digipot);

/*
 * View
 */
NavKeyView view(&tft, noteGrid, storage);
NavKeyEventDispatcher navKeyEventDispatcher(&view);
Controller controller(model, view, noteGrid);
void UP_Button_Pressed(i2cNavKey* p)
{
    navKeyEventDispatcher.UP_Button_Pressed();
}
void DOWN_Button_Pressed(i2cNavKey* p) { navKeyEventDispatcher.DOWN_Button_Pressed(); }
void LEFT_Button_Pressed(i2cNavKey* p) { navKeyEventDispatcher.LEFT_Button_Pressed(); }
void RIGHT_Button_Pressed(i2cNavKey* p) { navKeyEventDispatcher.RIGHT_Button_Pressed(); }

void Encoder_Increment(i2cNavKey* p) { navKeyEventDispatcher.Encoder_Increment(); }
void Encoder_Decrement(i2cNavKey* p) { navKeyEventDispatcher.Encoder_Decrement(); }
void Encoder_Push(i2cNavKey* p) { navKeyEventDispatcher.Encoder_Push(); }
void Encoder_Release(i2cNavKey* p) { navKeyEventDispatcher.Encoder_Release(); }

void beatCallback(uint8_t beat) { controller.handleBeat(beat); }
void noteOnCallback(uint8_t beat, MIDINote note)
{
    controller.handleNoteOn(beat, note);
}
void noteOffCallback(uint8_t beat, MIDINote note)
{
    controller.handleNoteOff(beat, note);
}
void noteGridEventHandler(keyEvent e) { controller.handleNoteGridEvent(e); }
void noteGridKeyPressCallback(uint8_t x, uint8_t y)
{
    controller.handleNoteGridKeyPress(x, y);
}

void noteGridKeyLongPressCallback(uint8_t x, uint8_t y)
{
}

void setupNavKey()
{
    navkey.begin(i2cNavKey::FLOAT_DATA | i2cNavKey::WRAP_ENABLE | i2cNavKey::DIRE_RIGHT | i2cNavKey::IPUP_ENABLE);

    navkey.writeCounter((float)0); /* Reset the counter value */
    navkey.writeMax((float)0.95); /* Set the maximum threshold*/
    navkey.writeMin((float)0.05); /* Set the minimum threshold */
    navkey.writeStep((float)0.05); /* Set the step to 1*/

    navkey.writeDoublePushPeriod(5); // not used yet..
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

void DMACallback(DMA_HandleTypeDef* _hdma) { spiDmaTransferComplete = true; }

void setup()
{

    noteGrid.attachEventHandler(noteGridEventHandler);
    noteGrid.attachKeyPressCallback(noteGridKeyPressCallback);
    noteGrid.attachKeyLongPressCallback(noteGridKeyLongPressCallback);
    sequencer.attachNoteOnCallback(noteOnCallback);
    sequencer.attachNoteOffCallback(noteOffCallback);
    sequencer.attachBeatCallback(beatCallback);

    volumeEncoder.onDecrement(&view, &NavKeyView::volumeUp);
    volumeEncoder.onIncrement(&view, &NavKeyView::volumeDown);
    volumeEncoder.onClick(&view, &NavKeyView::toggleMute);

    tempoEncoder.onIncrement(&view, &NavKeyView::faster);
    tempoEncoder.onDecrement(&view, &NavKeyView::slower);
    tempoEncoder.onClick(&view, &NavKeyView::togglePause);

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


    volumeEncoder.setupIO(true);
    tempoEncoder.setupIO(true);

    Wire.begin((uint32_t)SDA, (uint32_t)SCL);
    Wire.setClock(400000);
    Serial2.begin(31250);

    setupNavKey();

    tft.setDMA(&stmdma);
    view.fontRenderer->setDMA(&stmdma);
    HAL_DMA_RegisterCallback(&stmdma._dma, HAL_DMA_XFER_CPLT_CB_ID, DMACallback);
    spiDmaTransferComplete = true;
    stmdma.begin();

    tft.begin(48000000);
    tft.setRotation(3);

    view.setController(&controller);

    view.begin();

    trellis.begin();
    noteGrid.setFlashBeat(true);
    noteGrid.begin();
    sequencer.setTempo(70);

    model.begin();
    controller.begin();
    volumeEncoder.begin();
    tempoEncoder.begin();
    pinMode(DP_SDI_PIN, OUTPUT);
}

void loop()
{
    volumeEncoder.tick();
    tempoEncoder.tick();
    view.run();
    model.run();
}
