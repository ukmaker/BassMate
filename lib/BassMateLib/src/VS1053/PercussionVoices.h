#ifndef BM_PERCUSSION_VOICES_H
#define BM_PERCUSSION_VOICES_H
#include "Voice.h"

namespace bassmate {

const Voice drumVoices[] = {{"Acoustic Bass Drum", 35},
                      {"Bass Drum 1", 36},
                      {"Acoustic Snare Drum", 38},
                      {"Electric Snare Drum", 40},
                      {"Low Flr Tom", 41},
                      {"High Flr Tom", 43},
                      {"Low Tom", 45},
                      {"Low-Mid Tom", 47},
                      {"High-Mid Tom", 48},
                      {"High Tom", 50},
                      {"High Bongo", 60},
                      {"Low Bongo", 61},
                      {"Hi Mute Conga", 62},
                      {"Open Hi Conga", 63},
                      {"Low Conga", 64},
                      {"High Timbale", 65},
                      {"Low Timbali", 66},
                      {"Mute Surdo", 86},
                      {"Open Surdo", 87}};

const Voice cymbalVoices[] = {{"Closed Hi-hat", 42},  {"Pedal Hi-hat", 44},
                        {"Open Hi-hat", 46},    {"Crash Cymbal 1", 49},
                        {"Crash Cymbal 2", 57}, {"Ride Cymbal 1", 51},
                        {"Ride Cymbal 2", 59},  {"China Cymbal", 52},
                        {"Ride Bell", 53},      {"Tamborine", 54}};

const Voice woodVoices[] = {
    {"Sticks", 31},          {"Square Click", 32},  {"Metronome Click", 33},
    {"Metronome Bell", 34},  {"Side Stick", 37},    {"Hand Clap", 39},
    {"High Agogo", 67},      {"Low Agogo", 68},     {"Claves", 75},
    {"High Wood Block", 76}, {"Low Wood Block", 77}};

const Voice specialVoices[] = {
    {"Slap", 28},          {"Scratch Push", 29}, {"Scratch Pull", 30},
    {"Splash", 55},        {"Cowbell", 56},      {"Vibra Slap", 58},
    {"Cabasa", 69},        {"Maracas", 70},      {"Short Whistle", 71},
    {"Long Whistle", 72},  {"Short Guiro", 73},  {"Long Guiro", 74},
    {"Mute Cuica", 78},    {"Open Cuica", 79},   {"Mute Triangle", 80},
    {"Open Triangle", 81}, {"Shaker", 82},       {"Jingle Bells", 83},
    {"Bell Tree", 84},     {"Castanets", 85}};

#define LEN(N) (sizeof(N) / sizeof(N[0]))

const Family drumsFamily = {"Drums", 0, LEN(drumVoices), drumVoices};
const Family cymbalsFamily = {"Cymbals", 1, LEN(cymbalVoices), cymbalVoices};
const Family woodsFamily = {"Wood", 2, LEN(woodVoices), woodVoices};
const Family specialsFamily = {"Special", 3, LEN(specialVoices), specialVoices};
const Family percussionFamilies[] = {drumsFamily, cymbalsFamily, woodsFamily, specialsFamily};

const Bank percussionBank = {"Percussion", 0x78, 4, percussionFamilies, false, 127};

}  // namespace bassmate
#endif