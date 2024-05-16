#include <TFT_eSPI.h>
#include "Mazda3.h"
#include "Wire.h"

// Minimálna a maximálna vzdialenosť meraná senzormi
const int MIN_DISTANCE = 20;
const int MAX_DISTANCE = 220;

// Maximálne hodnoty pre veľkú a malú mriežku
const int MAX_BIG_GRID_VALUE = 6;
const int MAX_SMALL_GRID_VALUE = 3;

// Minimálna hodnota mriežky
const int MIN_GRID_VALUE = -1;

// Šírka a medzera pre vykreslenie mriežky
const int WIDTH = 20;
const int GAP = 5;

// Inštancia LCD displeja
TFT_eSPI lcd = TFT_eSPI();

// Inštancie Sprite pre vozidlo, mriežku a indikátor
TFT_eSprite vehicleBack = TFT_eSprite(&lcd);
TFT_eSprite grid = TFT_eSprite(&lcd);
TFT_eSprite reck = TFT_eSprite(&lcd);

// Premenné pre riadenie senzorov
uint16_t writeByteBuffered = 0;
uint16_t writeMode = 0;
uint16_t readMode = 0;
static uint32_t _pulseBegin;
uint8_t _interruptPin = 35;

// Štruktúra pre senzory
typedef struct sensor {
  uint8_t trig;
  volatile long duration;
  volatile bool waiting;
  int num;
  TFT_eSprite text = TFT_eSprite(&lcd);
  sensor *next;
} sensor;

// Pole senzorov
sensor sensors[5];

// Ukazovateľ na aktuálny senzor
sensor *current;

// Premenné pre uchovanie posledných hodnôt na rôznych stranách vozidla
int DownLeft_Last;
int DownRight_Last;
int Left_Last;
int Right_Last;

/**
 * @brief Vytvorí obrázok vozidla na displeji.
 * 
 * Táto metóda vytvorí obrázok vozidla na displeji. Najprv vytvorí nový
 * sprajt s danými rozmermi, nastaví farbu na zelenú, nastaví swap bajtov
 * (swapBytes) na true, vloží obrázok vozidla na sprajt a potom sprajt
 * presunie na zadanú pozíciu (x, y). Nakoniec sa sprajt vymaže z pamäte.
 */
void createVehicleBack() {
  vehicleBack.createSprite(MAZDA_3_BK_BACK_WIDTH, MAZDA_3_BK_BACK_HEIGHT);
  vehicleBack.fillSprite(TFT_GREEN);
  vehicleBack.setSwapBytes(true);
  vehicleBack.pushImage(0, 0, MAZDA_3_BK_BACK_WIDTH, MAZDA_3_BK_BACK_HEIGHT, Mazda_3_BK_Back);
  vehicleBack.pushSprite(64, 0, 0x07E0);
  vehicleBack.deleteSprite();
}

/**
 * @brief Vytvorí mriežku na pozadí displeja.
 * 
 * Táto metóda vytvorí mriežku na pozadí displeja. Najprv vytvorí nový
 * sprajt s danými rozmermi, nastaví farbu na zelenú, vloží mriežku
 * na sprajt a potom sprajt presunie na zadanú pozíciu (x, y). Nakoniec
 * sa sprajt vymaže z pamäte.
 */
void createGrid_Back() {
  grid.createSprite(240, 164);
  grid.fillSprite(TFT_GREEN);
  grid.pushSprite(0, 100, 0x07E0);
  grid.deleteSprite();
}

/**
 * @brief Nastaví senzory.
 * 
 * Táto metóda inicializuje senzory. Pre každý senzor nastaví
 * potrebné parametre, ako je číslo senzora, jeho trvanie,
 * spúšťací pin atď. Nastaví tiež príslušné piny na výstupný
 * režim a inicializuje príznaky čakania. Nakoniec nastaví
 * ukazovateľ current na prvý senzor v poli senzorov.
 */
void setup_sensors() {
  for (int i = 0; i < sizeof(sensors) / sizeof(sensors[0]); ++i) {
    sensors[i].num = i;
    sensors[i].duration = MIN_DISTANCE;
    sensors[i].trig = i * 2;
    sensors[i].waiting = false;

    sensors[i].text.createSprite(20, 10);
    sensors[i].text.fillScreen(TFT_BLACK);
    sensors[i].text.setTextColor(TFT_WHITE);
    sensors[i].text.setTextDatum(MC_DATUM);

    Mode(sensors[i].trig, OUTPUT);
    Mode(sensors[i].trig + 1, OUTPUT);
    Write(sensors[i].trig + 1, HIGH);
    sensors[i].waiting = false;

    if (i < sizeof(sensors) / sizeof(sensors[0]) - 1) {
      sensors[i].next = &sensors[i + 1];
    } else {
      sensors[i].next = &sensors[0];
    }
  }
  current = &sensors[0];
  pinMode(_interruptPin, INPUT_PULLUP);
}

/**
 * @brief Inicializuje zariadenie.
 * 
 * Táto metóda inicializuje zariadenie. Nastaví sériovú komunikáciu,
 * inicializuje LCD displej, vytvorí vozidlo na displeji, inicializuje
 * senzory a nastaví prerušovací pin.
 */
void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.fillScreen(TFT_BLACK);
  lcd.setRotation(3);
  createVehicleBack();
  Wire.begin();
  setup_sensors();
}

/**
 * @brief Hlavná metóda loop.
 * 
 * Táto metóda sa volá neustále v hlavnom cykle programu. Ak aktuálny senzor
 * nie je v stave čakania, metóda aktualizuje zobrazenie aktuálnej vzdialenosti
 * na displeji pre daný senzor. Následne pre každý senzor volá metódu doSonar(),
 * ktorá vykonáva sonarovanie.
 */
void loop() {
  if (!current->waiting) {                                                                        // Ak senzor nie je v stave čakania
    current->text.fillSprite(TFT_BLACK);                                                          // Vyčistí obrazovku pre aktuálny senzor
    int distance = current->duration;                                                             // Získa aktuálnu vzdialenosť od senzora
    current->text.drawNumber(distance, current->text.width() >> 1, current->text.height() >> 1);  // Zobrazí vzdialenosť na displeji
    int x;
    int y;
    // Zistí súradnice zobrazenia na displeji pre aktuálny senzor
    switch (current->num) {
      case 0:
        x = 50;
        y = 10;
        // Aktualizuje zobrazenie na displeji pre senzor 0
        update(0, map(distance, MIN_DISTANCE, MAX_DISTANCE, MAX_SMALL_GRID_VALUE, MIN_GRID_VALUE));
        break;
      case 1:
        x = 100;
        y = 80;
        // Aktualizuje zobrazenie na displeji pre senzor 1 (vľavo dole)
        if (distance <= sensors[3].duration) {
          update(1, map(distance, MIN_DISTANCE, MAX_DISTANCE, MAX_BIG_GRID_VALUE, MIN_GRID_VALUE));
        }
        break;
      case 2:
        x = 200;
        y = 80;
        // Aktualizuje zobrazenie na displeji pre senzor 2 (vpravo dole)
        // Serial.print("DIS: ");
        // Serial.println(distance);
        // Serial.println();
        // Serial.print("DIS3: ");
        // Serial.println(sensors[3].duration);
        if (distance <= sensors[3].duration) {
          update(2, map(distance, MIN_DISTANCE, MAX_DISTANCE, MAX_BIG_GRID_VALUE, MIN_GRID_VALUE));
        }
        break;
      case 3:
        x = 150;
        y = 90;
        // Aktualizuje zobrazenie na displeji pre senzor 1 (vľavo dole)
        if (distance <= sensors[1].duration) {
          update(1, map(distance, MIN_DISTANCE, MAX_DISTANCE, MAX_BIG_GRID_VALUE, MIN_GRID_VALUE));
        }
        // Aktualizuje zobrazenie na displeji pre senzor 2 (vpravo dole)
        if (distance <= sensors[2].duration) {
          update(2, map(distance, MIN_DISTANCE, MAX_DISTANCE, MAX_BIG_GRID_VALUE, MIN_GRID_VALUE));
        }
        break;
      case 4:
        x = 250;
        y = 10;
        // Aktualizuje zobrazenie na displeji pre senzor 3
        update(3, map(distance, MIN_DISTANCE, MAX_DISTANCE, MAX_SMALL_GRID_VALUE, MIN_GRID_VALUE));
        break;
    }
    current->text.pushSprite(x, y);  // Zobrazí aktualizovaný sprite senzora na displeji
    doSonar();                       // Vykoná sonarovanie pre aktuálny senzor
  }
  delay(5);  // Krátka pauza pred ďalším prechodom hlavného cyklu
}

/**
 * @brief Aktualizuje zobrazenie na displeji pre daný mriežkový senzor.
 * 
 * Táto metóda aktualizuje zobrazenie mriežky na displeji na základe aktuálnej
 * úrovne signálu z mriežkového senzora a identifikátora mriežky.
 * 
 * @param grid Identifikátor mriežkového senzora.
 * @param level Aktuálna úroveň signálu z mriežkového senzora.
 */
void update(int grid, int level) {
  int start = 155;  // Začiatočná pozícia pre kreslenie mriežky
  switch (grid) {
    case 0:

      // Aktualizuje zobrazenie pre mriežkový senzor 0 (vľavo hore)
      if (level >= 0 && Left_Last != level) {
        ++start;
        reck.createSprite(70, 120);  // Vytvorí sprite pre zobrazenie mriežky
        reck.fillSprite(TFT_GREEN);  // Vyplní sprite zelenou farbou

        // Kreslí oblúky pre jednotlivé úrovne signálu
        for (int i = 0; i < MAX_SMALL_GRID_VALUE; i++) {
          int arcColor = TFT_BLACK;  // Farba oblúku
          if (level >= (MAX_SMALL_GRID_VALUE - i)) {
            if (i == 0) arcColor = TFT_RED;  // Najbližšia úroveň (červená)
            else arcColor = TFT_ORANGE;      // Ostatné úrovne (oranžová)
          }
          int arcStart = start + (i * WIDTH) + (i * GAP);                                                   // Začiatočný uhol oblúka
          int arcEnd = start + ((i + 1) * WIDTH) + (i * GAP);                                               // Koncový uhol oblúka
          reck.drawSmoothArc(200, 0, arcEnd, arcStart, 65 + ((3 - i) * 3), 120, arcColor, arcColor, true);  // Kreslí oblúk
        }
        reck.pushSprite(0, 0, 0x07E0);  // Zobrazí sprite mriežky na displeji
        Left_Last = level;              // Uloží aktuálnu úroveň signálu pre ďalšie použitie
      }
      break;
    case 1:
      // Aktualizuje zobrazenie pre mriežkový senzor 1 (vľavo dole)
      if (level >= 0 && DownLeft_Last != level) {
        reck.createSprite(154, 164);  // Vytvorí sprite pre zobrazenie mriežky
        reck.fillScreen(TFT_GREEN);   // Vyplní sprite zelenou farbou

        // Kreslí oblúky pre jednotlivé úrovne signálu
        for (int i = 0; i < MAX_BIG_GRID_VALUE; i++) {
          int arcColor = TFT_BLACK;  // Farba oblúku
          if (level >= (MAX_BIG_GRID_VALUE - i)) {
            if (i == 0) {
              arcColor = TFT_RED;                    // Najbližšia úroveň (červená)
            } else if (i >= 3) arcColor = TFT_BLUE;  // Vzdialené úrovne (modrá)
            else arcColor = TFT_ORANGE;              // Ostatné úrovne (oranžová)
          }
          int arcStart = start + (i * WIDTH) + (i * GAP);                                    // Začiatočný uhol oblúka
          int arcEnd = start + ((i + 1) * WIDTH) + (i * GAP);                                // Koncový uhol oblúka
          reck.drawSmoothArc(160, -120, arcEnd, arcStart, 0, 35, arcColor, arcColor, true);  // Kreslí oblúk
        }
        reck.pushSprite(0, 80, 0x07E0);  // Zobrazí sprite mriežky na displeji
        DownLeft_Last = level;           // Uloží aktuálnu úroveň signálu pre ďalšie použitie
      }
      break;
    case 2:
      // Aktualizuje zobrazenie pre mriežkový senzor 2 (vpravo dole)
      // Serial.print("CASE");
      // Serial.println(level);
      // Serial.println();

      if (level >= 0 && DownRight_Last != level) {
        reck.createSprite(154, 164);  // Vytvorí sprite pre zobrazenie mriežky
        reck.fillScreen(TFT_GREEN);   // Vyplní sprite zelenou farbou

        // Kreslí oblúky pre jednotlivé úrovne signálu
        for (int i = 0; i < MAX_BIG_GRID_VALUE; i++) {
          int arcColor = TFT_BLACK;  // Farba oblúku
          if (level >= (MAX_BIG_GRID_VALUE - i)) {
            if (i == 0) {
              arcColor = TFT_RED;                    // Najbližšia úroveň (červená)
            } else if (i >= 3) arcColor = TFT_BLUE;  // Vzdialené úrovne (modrá)
            else arcColor = TFT_ORANGE;              // Ostatné úrovne (oranžová)
          }
          int arcStart = start + (i * WIDTH) + (i * GAP);                                    // Začiatočný uhol oblúka
          int arcEnd = start + ((i + 1) * WIDTH) + (i * GAP);                                // Koncový uhol oblúka
          reck.drawSmoothArc(-5, -120, arcEnd, arcStart, 325, 0, arcColor, arcColor, true);  // Kreslí oblúk
        }
        reck.pushSprite(165, 80, 0x07E0);  // Zobrazí sprite mriežky na displeji
        DownRight_Last = level;            // Uloží aktuálnu úroveň signálu pre ďalšie použitie
      }
      break;
    case 3:
      // Aktualizuje zobrazenie pre mriežkový senzor 3 (vpravo hore)
      if (level >= 0 && Right_Last != level) {
        ++start;
        reck.createSprite(70, 120);  // Vytvorí sprite pre zobrazenie mriežky
        reck.fillSprite(TFT_GREEN);  // Vyplní sprite zelenou farbou

        // Kreslí oblúky pre jednotlivé úrovne signálu
        for (int i = 0; i < MAX_SMALL_GRID_VALUE; i++) {
          int arcColor = TFT_BLACK;  // Farba oblúku
          if (level >= (MAX_SMALL_GRID_VALUE - i)) {
            if (i == 0) arcColor = TFT_RED;  // Najbližšia úroveň (červená)
            else arcColor = TFT_ORANGE;      // Ostatné úrovne (oranžová)
          }
          int arcStart = start + (i * WIDTH) + (i * GAP);                                                     // Začiatočný uhol oblúka
          int arcEnd = start + ((i + 1) * WIDTH) + (i * GAP);                                                 // Koncový uhol oblúka
          reck.drawSmoothArc(-130, 0, arcEnd, arcStart, 240, 300 - ((3 - i) * 3), arcColor, arcColor, true);  // Kreslí oblúk
        }
        reck.pushSprite(250, 0, 0x07E0);  // Zobrazí sprite mriežky na displeji
        Right_Last = level;               // Uloží aktuálnu úroveň signálu pre ďalšie použitie
      }
      break;
  }
  reck.deleteSprite();  // Odstráni sprite po použití
}

/**
 * @brief Vykoná sonarovanie.
 * 
 * Táto metóda vykoná sonarovanie, ale iba ak je pin pre prerušenie
 * aktívny (LOW). Ak je pin aktívny, metóda volá metódu send_ping().
 * 
 * @note Ak je pin aktívny, metóda send_ping() sa zavolá asynchrónne
 * a sonarovanie sa vykonáva v rámci prerušenia.
 */
void doSonar() {
  if (digitalRead(_interruptPin) == LOW) return;  // Prerušovací pin je aktívny - preskoč
  send_ping();                                    // Vykonaj sonarovanie
}

/**
 * @brief Pošle sonarovací signál.
 * 
 * Táto metóda pošle sonarovací signál a nastaví príznak čakania.
 * Následne pridá prerušenie, aby zaznamenalo začiatok impulzu,
 * nastaví príslušný pin na HIGH, počká 10 mikrosekúnd, a potom
 * nastaví príslušný pin na LOW.
 */
void send_ping() {
  _pulseBegin = 0;                                                             // Vynuluj začiatok impulzu (pulse)
  current->waiting = true;                                                     // Nastav príznak čakania
  attachInterrupt(digitalPinToInterrupt(_interruptPin), startPulse, FALLING);  // Pridaj prerušenie pre zachytenie impulzu
  Write(current->trig, HIGH);                                                  // Nastav pin na HIGH
  delayMicroseconds(10);                                                       // Počkaj 10 mikrosekúnd
  Write(current->trig, LOW);                                                   // Nastav pin na LOW
}

/**
 * @brief Spustí meranie dĺžky impulzu.
 * 
 * Táto metóda nastaví začiatok impulzu (pulse) na aktuálny čas
 * pomocou funkcie micros(). Následne pridá prerušenie pre
 * zachytenie konca impulzu.
 */
void startPulse() {
  _pulseBegin = micros();                                                   // Nastav začiatok impulzu na aktuálny čas
  attachInterrupt(digitalPinToInterrupt(_interruptPin), endPulse, RISING);  // Pridaj prerušenie pre zachytenie konca impulzu
}

/**
 * @brief Ukáže koniec merania dĺžky impulzu.
 * 
 * Táto metóda vypočíta dĺžku impulzu a nastaví ju ako aktuálnu
 * dĺžku trvania. Následne zruší prerušenie a prejde na ďalší senzor.
 */
void endPulse() {
  uint32_t now = micros();                                // Zisti aktuálny čas
  detachInterrupt(digitalPinToInterrupt(_interruptPin));  // Odstráň prerušenie
  // Ignoruj nezmyselné hodnoty
  if (now < _pulseBegin) {
    current->waiting = false;  // Zruš príznak čakania
    return;                    // Ak sme začali v budúcnosti, preskoč (pravdepodobne pretečenie micros())
  }

  // Vypočítaj dĺžku impulzu a obmedz ju na maximálnu hodnotu
  current->duration = (((now - _pulseBegin) / 2) / 29.1) > 200 ? 200 : (((now - _pulseBegin) / 2) / 29.1);
  current->waiting = false;  // Zruš príznak čakania
  current = current->next;   // Prejdi na ďalší senzor
}

/**
 * @brief Nastaví režim pinu mikrokontroléra.
 * 
 * Táto funkcia slúži na nastavenie režimu pinu mikrokontroléra
 * na vstupný alebo výstupný podľa zadaného parametra.
 * 
 * @param pin Číslo pinu, ktorý sa má nastaviť.
 * @param mode Požadovaný režim pre pin (INPUT alebo OUTPUT).
 */
void Mode(uint8_t pin, uint8_t mode) {
  // Ak je požadovaný režim OUTPUT
  if (mode == OUTPUT) {
    writeMode = writeMode | bit(pin);  // Nastaví príslušný bit v writeMode na 1
    readMode = readMode & ~bit(pin);   // Vynuluje príslušný bit v readMode

    // Ak je požadovaný režim INPUT
  } else if (mode == INPUT) {
    writeMode = writeMode & ~bit(pin);  // Vynuluje príslušný bit v writeMode
    readMode = readMode | bit(pin);     // Nastaví príslušný bit v readMode na 1
  }
}

/**
 * @brief Zapíše hodnotu na určitý pin.
 * 
 * Táto funkcia zapíše zadanú hodnotu na určitý pin mikrokontroléra.
 * Hodnota HIGH nastaví príslušný pin na logickú jednotku (1),
 * hodnota LOW nastaví pin na logickú nulu (0).
 * 
 * @param pin Číslo pinu, na ktorý sa má zapísať hodnota.
 * @param value Hodnota, ktorá sa má zapísať na pin (HIGH alebo LOW).
 */
void Write(uint8_t pin, uint8_t value) {
  Wire.beginTransmission(0x20);  // Začni prenos na PCF8575
  // Ak je hodnota HIGH
  if (value == HIGH) {
    writeByteBuffered = writeByteBuffered | bit(pin);  // Nastav príslušný bit v writeByteBuffered na 1
  } else {
    writeByteBuffered = writeByteBuffered & ~bit(pin);  // Vynuluj príslušný bit v writeByteBuffered
  }

  // Aplikuj masku writeMode na writeByteBuffered
  writeByteBuffered = writeByteBuffered & writeMode;
  // Zapíš writeByteBuffered na PCF8575 (najprv nižšie bajty, potom vyššie)
  Wire.write((uint8_t)writeByteBuffered);
  Wire.write((uint8_t)(writeByteBuffered >> 8));
  Wire.endTransmission();  // Ukonči prenos
}
