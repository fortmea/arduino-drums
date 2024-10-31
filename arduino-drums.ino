//INSPIRED BY https://diyelectromusic.com/2021/06/22/arduino-mozzi-sample-drum-machine/
//PLEASE REFER TO THE README IN https://github.com/fortmea/arduino-drums BEFORE PROCEEDING.




#include <debounce.h>
#include <LiquidCrystal.h>
#include <MozziGuts.h>
#include <Sample.h>
#include <EEPROM.h>
#include "d_kit.h"
#define D_NUM 4
#define CONTROL_RATE 64
#define welcomeMenuIndex 0
#define kickMenuIndex 1
#define snareMenuIndex 2
#define ohatMenuIndex 3
#define chatMenuIndex 4
#define saveLoadMenuIndex 5
#define pitchMenuIndex 6
#define bpmPlayMenuIndex 7

bool play = true;
bool rendered = false;
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
const int lrnavigation = 10, tick = 8, playpause = 7, blank = 6;
const int listSize = 16;
const int numLists = 4;
const int numSlots = 3;
const int baseAddress = 0;
const int slotSize = listSize;
const int listSpacing = slotSize * numSlots;
const int bitDepth = 8;
const long interval = 300;
int screens = 7;
int screen = 0;
int pos = 0;
int slotNumber = 0;
int bpm = 120;
int currentStep = 0;
float pitch = 1.0;
unsigned long millitime;
unsigned long lastStepTime = 0;
unsigned long stepInterval = (60000 / (bpm * 4));
unsigned long previousMillis = 0;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
bool trig[D_NUM];
bool kick[16] = { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false };
bool snare[16] = { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false };
bool hat[16] = { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false };
bool clap[16] = { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false };


Sample<BD_NUM_CELLS, AUDIO_RATE> aBD(BD_DATA);
Sample<SD_NUM_CELLS, AUDIO_RATE> aSD(SD_DATA);
Sample<CH_NUM_CELLS, AUDIO_RATE> aCH(CH_DATA);
Sample<OH_NUM_CELLS, AUDIO_RATE> aOH(OH_DATA);

float drumPitch = (float)AUDIO_RATE / (float)BD_NUM_CELLS;
float snarePitch = (float)AUDIO_RATE / (float)SD_NUM_CELLS;
float chatPitch = (float)AUDIO_RATE / (float)CH_NUM_CELLS;
float ohatPitch = (float)AUDIO_RATE / (float)OH_NUM_CELLS;


byte seta_baixo[8] = {
  B00000,
  B00000,
  B00100,
  B00100,
  B00100,
  B11111,
  B01110,
  B00100
};

byte tick_off[8] = {
  B11111,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B11111
};

byte tick_on[8] = { B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111 };


void startDrum(int drum) {
  switch (drum) {
    case 0: aBD.start(); break;
    case 1: aSD.start(); break;
    case 2: aCH.start(); break;
    case 3: aOH.start(); break;
  }
}

void saveBoolList(bool list[], int listNumber) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("saving");
  int eepromAddress = baseAddress + listNumber * listSpacing + slotNumber * slotSize;
  for (int i = 0; i < listSize; i++) {
    lcd.setCursor(i, 1);
    lcd.print(".");
    EEPROM.update(eepromAddress + i, list[i] ? 1 : 0);
  }
}

void loadBoolList(bool list[], int listNumber) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("loading");
  int eepromAddress = baseAddress + listNumber * listSpacing + slotNumber * slotSize;
  for (int i = 0; i < listSize; i++) {
    lcd.setCursor(i, 1);
    lcd.print(".");
    list[i] = EEPROM.read(eepromAddress + i) == 1;
  }
}


static void navigationButtonHandler(uint8_t btnId, uint8_t btnState) {
  if (btnState == BTN_PRESSED) {
    screen++;
    pos = 0;
    rendered = false;
    lcd.clear();
  } else {
  }
}

static void tickButtonHandler(uint8_t btnId, uint8_t btnState) {
  if (btnState == BTN_PRESSED) {

    switch (screen) {
      case pitchMenuIndex:
        {
          pitch = pitch + 0.1;
          break;
        }
      case kickMenuIndex:
        {
          kick[pos] = true;
          break;
        }
      case snareMenuIndex:
        {
          snare[pos] = true;
          break;
        }
      case ohatMenuIndex:
        {
          hat[pos] = true;
          break;
        }
      case chatMenuIndex:
        {
          clap[pos] = true;
          break;
        }
      case bpmPlayMenuIndex:
        {
          bpm++;
          stepInterval = (60000 / (bpm * 4));
          break;
        }
      case saveLoadMenuIndex:
        {
          saveBoolList(kick, 0);
          saveBoolList(snare, 1);
          saveBoolList(hat, 2);
          saveBoolList(clap, 3);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("saved");
          break;
        }
    }
    pos++;
    rendered = false;
  } else {
  }
}

static void blankButtonHandler(uint8_t btnId, uint8_t btnState) {
  if (btnState == BTN_PRESSED) {

    switch (screen) {
      case pitchMenuIndex:
        {
          pitch = pitch - 0.1;
          break;
        }
      case kickMenuIndex:
        {
          kick[pos] = false;

          break;
        }
      case snareMenuIndex:
        {
          snare[pos] = false;
          break;
        }
      case ohatMenuIndex:
        {
          hat[pos] = false;
          break;
        }
      case chatMenuIndex:
        {
          clap[pos] = false;
          break;
        }
      case bpmPlayMenuIndex:
        {
          bpm--;
          stepInterval = (60000 / (bpm * 4));
          break;
        }
      case saveLoadMenuIndex:
        {
          loadBoolList(kick, 0);
          loadBoolList(snare, 1);
          loadBoolList(hat, 2);
          loadBoolList(clap, 3);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("loaded");

          break;
        }
    }
    rendered = false;
    pos++;
  } else {
  }
}
static void playPauseButtonHandler(uint8_t btnId, uint8_t btnState) {
  if (btnState == BTN_PRESSED) {
    play = !play;
  }
}


static void changePosHandler(uint8_t btnId, uint8_t btnState) {
  if (btnState == BTN_PRESSED) {
    switch (screen) {
      case bpmPlayMenuIndex:
        {
          play = !play;
          break;
        }
      case saveLoadMenuIndex:
        {
          slotNumber++;
          if (slotNumber > numSlots) { slotNumber = 0; };
          break;
        }
      default:
        {
          pos++;
          break;
        }
    }
    rendered = false;
  }
}


static Button navigationButton(0, navigationButtonHandler);
static Button tickButton(1, tickButtonHandler);
static Button blankButton(2, blankButtonHandler);
static Button playPauseButton(3, changePosHandler);  //playPauseButtonHandler);

void setup() {
  pinMode(lrnavigation, INPUT_PULLUP);
  pinMode(tick, INPUT_PULLUP);
  pinMode(playpause, INPUT_PULLUP);
  pinMode(blank, INPUT_PULLUP);
  //Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.createChar(0, seta_baixo);
  lcd.createChar(1, tick_on);
  lcd.createChar(2, tick_off);
  startMozzi();
  aBD.setFreq((float)D_SAMPLERATE / (float)BD_NUM_CELLS);
  aSD.setFreq((float)D_SAMPLERATE / (float)SD_NUM_CELLS);
  aCH.setFreq((float)D_SAMPLERATE / (float)CH_NUM_CELLS);
  aOH.setFreq((float)D_SAMPLERATE / (float)OH_NUM_CELLS);
  loadBoolList(kick, 0);
  loadBoolList(snare, 1);
  loadBoolList(hat, 2);
  loadBoolList(clap, 3);
}

void loop() {
  if (pitch < 0) {
    pitch = 0;
  }
  if (pos > 16) {
    pos = 0;
  }
  if (screen > screens) {
    screen = 0;
    rendered = false;
  }
  if (rendered == false) {
    //Serial.println(stepInterval);
    lcd.clear();
    if (screen == pitchMenuIndex) {
      pitchMenu();
    } else if (screen == kickMenuIndex) {
      kickMenu();
    } else if (screen == snareMenuIndex) {
      snareMenu();
    } else if (screen == ohatMenuIndex) {
      hatMenu();
    } else if (screen == chatMenuIndex) {
      clapMenu();
    } else if (screen == bpmPlayMenuIndex) {
      bpmPlayMenu();
    } else if (screen == saveLoadMenuIndex) {
      saveLoadMenu();
    } else if (screen == welcomeMenuIndex) {
      welcomeMenu();
    }
    rendered = true;
  }
  pollButtons();
  audioHook();
}

void welcomeMenu() {
  lcd.setCursor(0, 0);
  lcd.print("Ardubeats!");
  lcd.setCursor(0, 1);
  lcd.print("https://github.com/fortmea/arduino-drums");
  //lcd.scrollDisplayLeft();
}

static void pollButtons() {
  // update() will call buttonHandler() if PIN transitions to a new state and stays there
  // for multiple reads over 25+ ms.
  navigationButton.update(digitalRead(lrnavigation));
  tickButton.update(digitalRead(tick));
  blankButton.update(digitalRead(blank));
  playPauseButton.update(digitalRead(playpause));
}

void pitchMenu() {

  lcd.setCursor(0, 0);
  lcd.print("pitch multiplier");
  lcd.setCursor(0, 1);
  lcd.print(pitch);
}

void kickMenu() {
  lcd.setCursor(0, 0);
  lcd.print("kick");
  lcd.setCursor(pos, 0);
  lcd.write(byte(0));
  lcd.setCursor(0, 1);
  for (int i = 0; i < 16; i++) {
    lcd.write(kick[i] ? 1 : 2);
  }
}

void snareMenu() {
  lcd.setCursor(0, 0);
  lcd.print("snare");
  lcd.setCursor(pos, 0);
  lcd.write(byte(0));
  lcd.setCursor(0, 1);
  for (int i = 0; i < 16; i++) {
    lcd.write(snare[i] ? 1 : 2);
  }
}

void clapMenu() {
  lcd.setCursor(0, 0);
  lcd.print("hi hat");
  lcd.setCursor(pos, 0);
  lcd.write(byte(0));
  lcd.setCursor(0, 1);
  for (int i = 0; i < 16; i++) {
    lcd.write(clap[i] ? 1 : 2);
  }
}

void hatMenu() {
  lcd.setCursor(0, 0);
  lcd.print("open hat");
  lcd.setCursor(pos, 0);
  lcd.write(byte(0));
  lcd.setCursor(0, 1);
  for (int i = 0; i < 16; i++) {
    lcd.write(hat[i] ? 1 : 2);
  }
}

void bpmPlayMenu() {
  lcd.setCursor(0, 0);
  lcd.print("bpm / " + String(play ? "playing" : "stopped"));
  lcd.setCursor(0, 1);
  lcd.print(bpm);
}

void saveLoadMenu() {
  lcd.setCursor(0, 0);
  lcd.print("save/load slot " + String(slotNumber));
}


void updateControl() {
  unsigned long currentMillis = millis();



  if ((millis() - lastStepTime >= stepInterval) && play) {
    if ((currentMillis - previousMillis >= interval) && (screen == welcomeMenuIndex)) {
      previousMillis = currentMillis;
      lcd.scrollDisplayLeft();
    }
    lastStepTime = millis();
    aBD.setFreq(drumPitch * pitch);
    aSD.setFreq(snarePitch * pitch);
    aCH.setFreq(chatPitch * pitch);
    aOH.setFreq(ohatPitch * pitch);
    // Tocar os samples se a etapa atual for marcada como true
    if (kick[currentStep]) {
      startDrum(0);
    }
    if (snare[currentStep]) {
      startDrum(1);
    }
    if (hat[currentStep]) {
      startDrum(2);
    }
    if (clap[currentStep]) {
      startDrum(3);
    }

    // Avançar para o próximo step
    currentStep = (currentStep + 1) % 16;
  }
}
AudioOutput_t updateAudio() {


  int16_t d_sample = aBD.next() + aSD.next() + aCH.next() + aOH.next();
  return MonoOutput::fromNBit(9, d_sample);
}
