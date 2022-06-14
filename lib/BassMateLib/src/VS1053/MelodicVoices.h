#ifndef BM_MELODIC_const VoiceS_H
#define BM_MELODIC_const VoiceS_H
#include "Voice.h"

namespace bassmate {
const Voice keys[] = {{"Grand Piano", 0},      {"Bright Piano", 1},
                {"Elec Grand Piano", 2}, {"Honky-Tonk Piano", 3},
                {"Electric Piano 1", 4}, {"Electric Piano 2", 5},
                {"Harpsicord", 6},       {"Clavi", 7},
                {"Celesta", 8}};

const Voice struck[] = {{"Glockenspiel", 9}, {"Music Box", 10}, {"Vibraphone", 11},
                  {"Marimba", 12},     {"Xylophone", 13}, {"Tubular Bells", 14},
                  {"Dulcimer", 15}};

const Voice organs[] = {{"Drawbar Organ", 16}, {"Percussive Organ", 17},
                  {"Rock Organ", 18},    {"Church Organ", 19},
                  {"Reed Organ", 20},    {"Accordian", 21},
                  {"Harmonica", 22},     {"Tango Accordian", 23}};

const Voice strings[] = {
    {"Acous Gtr (Nylon)", 24},
    {"Acous Gtr (Steel)", 25},
    {"Elec Gtr (Jazz)", 26},
    {"Elec Gtr (Clean)", 27},
    {"Elec Gtr (Muted)", 28},
    {"Overdriven Gtr", 29},
    {"Distorted Gtr", 30},
    {"Gtr Harmonics", 31},
    {"Acoustic Bass", 32},
    {"Elec Bass (Finger)", 33},
    {"Elec Bass (Pick)", 34},
    {"Fretless Bass", 35},
    {"Slap Bass 1", 36},
    {"Slap Bass 2", 37},
    {"Synth Bass 1", 38},
    {"Synth Bass 2", 39},
    {"Violin", 40},
    {"Viola", 41},
    {"Cello", 42},
    {"Contrabass", 43},
    {"Tremelo Strings", 44},
    {"Pizzicato Strings", 45},
    {"Orchestral Harp", 46},
    {"String Ensemb 1", 48},
    {"String Ensemb 2", 49},
    {"Synth Strings 1", 50},
    {"Synth Strings 2", 51},
};

const Voice voxhumana[] = {
    {"Choir Aahs", 52}, {"const Voice Oohs", 53}, {"Synth const Voice", 54}};

const Voice brass[] = {
    {"Trumpet", 56},       {"Trombone", 57},      {"Tuba", 58},
    {"Muted Trumpet", 59}, {"French Horn", 60},   {"Brass Section", 61},
    {"Synth Brass 1", 62}, {"Synth Brass 2", 63}, {"Soprano Sax", 64},
    {"Alto Sax", 65},      {"Tenor Sax", 66},     {"Baritone Sax", 67}};

const Voice woodwind[] = {
    {"Oboe", 68},       {"English Horn", 69}, {"Bassoon", 70},
    {"Clarinet", 71},   {"Piccolo", 72},      {"Flute", 73},
    {"Recorder", 74},   {"Pan Flute", 75},    {"Blown Bottle", 76},
    {"Shakuhachi", 77}, {"Whistle", 78},      {"Ocarina", 79}};

const Voice lead[] = {{"Square Lead", 80},   {"Saw Lead", 81},
                {"Calliope Lead", 82}, {"Chiff Lead", 83},
                {"Charang Lead", 84},  {"const Voice Lead", 85},
                {"Fifths Lead", 86},   {"Bass & Lead", 87}};

const Voice pad[] = {{"New Age Pad", 88}, {"Warm Pad", 89},  {"Polysynth Pad", 90},
               {"Choir Pad", 91},   {"Bowed Pad", 92}, {"Metallic Pad", 93},
               {"Halo Pad", 94},    {"Sweep Pad", 95}};

const Voice effects[] = {{"Rain FX", 96},        {"Sound Track FX", 97},
                   {"Crystal FX", 98},     {"Atmosphere FX", 99},
                   {"Brightness FX", 100}, {"Goblins FX", 101},
                   {"Echoes FX", 102},     {"Sci-Fi FX", 103}};

const Voice traditional[] = {{"Sitar", 104},  {"Banjo", 105},   {"Shamisen", 106},
                       {"Koto", 107},   {"Kalimba", 108}, {"Bag Pipe", 109},
                       {"Fiddle", 110}, {"Shanai", 111}};

const Voice percussion[] = {{"Timpani", 47},        {"Tinkle Bell", 112},
                      {"Agogo", 113},         {"Pitched Percussion", 114},
                      {"Wood Block", 115},    {"Taiko Drum", 116},
                      {"Melodic Drum", 117},  {"Synth Drum", 118},
                      {"Reverse Cymbal", 119}};

const Voice various[] = {
    {"Orchestra Hit", 55}, {"Guitar Fret Noise", 120}, {"Breath Noise", 121},
    {"Seashore", 122},     {"Bird Tweet", 123},        {"Telephone Ring", 124},
    {"Helicopter", 125},   {"Applause", 126},          {"Gunshot", 127}};

const Voice instrumentGroups[] = {
    {"Keys", 0},       {"Struck", 1},  {"Organs", 2},       {"Strings", 3},
    {"Vox Humana", 4}, {"Brass", 5},   {"Woodwind", 6},     {"Lead", 7},
    {"Pad", 8},        {"Effects", 9}, {"Traditional", 10}, {"Percussion", 11},
    {"Various", 12}};

#define LEN(N) (sizeof(N) / sizeof(N[0]))

const Family m0 = {"Keys", 0, LEN(keys), keys};
const Family m1 = {"Struck", 1, LEN(struck), struck};
const Family m2 = {"Organs", 2, LEN(organs), organs};
const Family m3 = {"Strings", 3, LEN(strings), strings};
const Family m4 = {"Vox Humana", 4, LEN(voxhumana), voxhumana};
const Family m5 = {"Brass", 5, LEN(brass), brass};
const Family m6 = {"Woodwind", 6, LEN(woodwind), woodwind};
const Family m7 = {"Lead", 7, LEN(lead), lead};
const Family m8 = {"Pad", 8, LEN(pad), pad};
const Family m9 = {"Effects", 9, LEN(effects), effects};
const Family m10 = {"Traditional", 10, LEN(traditional), traditional};
const Family m11 = {"Percussion", 11, LEN(percussion), percussion};
const Family m12 = {"Various", 12, LEN(various), various};

const Family melodicFamilies[] = {m0,m1,m2,m3,m4,m5,m6,m7,m8,m9,m10,m11,m12};
const Bank melodicBank = {"Melodic", 0x79, 4, melodicFamilies, false, 127};

}  // namespace bassmate
#endif