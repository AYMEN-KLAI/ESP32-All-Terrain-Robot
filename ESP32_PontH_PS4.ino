/*
 * ============================================================
 * ESP32 + Pont H — Contrôle Robot via Manette PS4
 * ============================================================
 * Librairie requise : PS4-esp32 (par aed3)
 *   Installer via Arduino IDE → Gérer les bibliothèques → "PS4Controller"
 *   ou : https://github.com/aed3/PS4-esp32
 *
 * Câblage Pont H (2x L298N ou équivalent) :
 *   Moteur Droit  IN1 → GPIO 14  (rm1)
 *                 IN2 → GPIO 27  (rm2)
 *   Moteur Gauche IN1 → GPIO 12  (lm1)
 *                 IN2 → GPIO 13  (lm2)
 *   Enable Droit  EN  → GPIO 25  (v1)  — PWM vitesse
 *   Enable Gauche EN  → GPIO 26  (v2)  — PWM vitesse
 *
 * Adresse MAC Bluetooth : modifier MPU_MAC ci-dessous
 * ============================================================ */

#include <PS4Controller.h>

/* ── Pins moteurs ────────────────────────────────────────── */
#define rm1  14   /* Moteur droit  — direction A  */
#define rm2  27   /* Moteur droit  — direction B  */
#define lm1  12   /* Moteur gauche — direction A  */
#define lm2  13   /* Moteur gauche — direction B  */
#define v1   25   /* Enable / vitesse moteur droit  (PWM) */
#define v2   26   /* Enable / vitesse moteur gauche (PWM) */

/* ── Adresse MAC Bluetooth de l'ESP32 ───────────────────── */
/* Remplacer par l'adresse MAC réelle de votre module        */
#define MPU_MAC  "e8:61:7e:58:16:ce"

/* ── Vitesse maximale (0-255) ───────────────────────────── */
#define SPEED_MAX  255

/* ===========================================================
   FONCTIONS MOUVEMENT
   =========================================================== */

void AVANT_MAX() {
    digitalWrite(rm1, HIGH);
    digitalWrite(rm2, LOW);
    digitalWrite(lm1, HIGH);
    digitalWrite(lm2, LOW);
    analogWrite(v1, SPEED_MAX);
    analogWrite(v2, SPEED_MAX);
}

void ARRIERE_MAX() {
    digitalWrite(rm1, LOW);
    digitalWrite(rm2, HIGH);
    digitalWrite(lm1, LOW);
    digitalWrite(lm2, HIGH);
    analogWrite(v1, SPEED_MAX);
    analogWrite(v2, SPEED_MAX);
}

/* Bouton Droite manette → tourne à droite (moteur G avant, moteur D arrière) */
void DROITE_MAX() {
    digitalWrite(rm1, HIGH);
    digitalWrite(rm2, LOW);
    digitalWrite(lm1, LOW);
    digitalWrite(lm2, HIGH);
    analogWrite(v1, SPEED_MAX);
    analogWrite(v2, SPEED_MAX);
}

/* Bouton Gauche manette → tourne à gauche (moteur D avant, moteur G arrière) */
void GAUCHE_MAX() {
    digitalWrite(rm1, LOW);
    digitalWrite(rm2, HIGH);
    digitalWrite(lm1, HIGH);
    digitalWrite(lm2, LOW);
    analogWrite(v1, SPEED_MAX);
    analogWrite(v2, SPEED_MAX);
}

void STOP() {
    digitalWrite(rm1, LOW);
    digitalWrite(rm2, LOW);
    digitalWrite(lm1, LOW);
    digitalWrite(lm2, LOW);
    analogWrite(v1, 0);
    analogWrite(v2, 0);
}

/* ===========================================================
   SETUP
   =========================================================== */
void setup() {
    pinMode(rm1, OUTPUT);
    pinMode(rm2, OUTPUT);
    pinMode(lm1, OUTPUT);
    pinMode(lm2, OUTPUT);
    pinMode(v1,  OUTPUT);
    pinMode(v2,  OUTPUT);

    STOP();   /* sécurité — moteurs arrêtés au démarrage */

    Serial.begin(115200);
    PS4.begin(MPU_MAC);
    Serial.println("Pret. En attente de la manette PS4...");
}

/* ===========================================================
   LOOP
   =========================================================== */
void loop() {
    if (PS4.isConnected()) {

        if (PS4.Up()) {
            AVANT_MAX();
            Serial.println("avant");
        }
        else if (PS4.Down()) {
            ARRIERE_MAX();
            Serial.println("arriere");
        }
        else if (PS4.Left()) {
            DROITE_MAX();
            Serial.println("droite");
        }
        else if (PS4.Right()) {
            GAUCHE_MAX();
            Serial.println("gauche");
        }
        else {
            STOP();
            Serial.println("stop");
        }

    } else {
        /* Manette non connectée — moteurs à l'arrêt par sécurité */
        STOP();
    }
}
