#ifndef BASSMATE_CONFIG_H
#define BASSMATE_CONFIG_H

#define LEN(N) (sizeof(N) / sizeof(N[0]))

#define SCK PB13
#define MISO PB14
#define MOSI PB15
#define SC_CS PB12
#define SC_DC PB3
#define SC_RESET PB5

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

#define VOLUME_ENCODER_PUSH_PIN PA10
#define TEMPO_ENCODER_PUSH_PIN PC15

#define DP_CS_PIN PB10
#define DP_SCK_PIN PA4
#define DP_SDI_PIN PA3

#define LCD_DMA_BUFFER_SIZE 32768

#define LEFT_TRELLIS_ADDRESS 0x2E
#define RIGHT_TRELLIS_ADDRESS 0x2F

#endif