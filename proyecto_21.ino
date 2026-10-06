// C++ code
//
// Configuración del LCD: RS=8, EN=9, D4=4, D5=5, D6=6, D7=7
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); 
// Asignación de P
const int pinPedir = 2;
// Botón Pedir Carta [P] 
const int pinPlantar = 3; 
// Botón Plantarse [S] 
const int pinBuzzer = 10;
// Zumbador 
const int pinReset = 11;
// Botón Reiniciar [R] 
//Variables de Control de Tiempo y Flujo 
  unsigned long tiempoInicial; 
const unsigned long tiempoLimiteTotal = 45000;
// 45 segundos para las 3 etapas
int etapaActual = 1; 
// Nivel de 1 a 3 
bool juegoActivo = true;
// Variables de Cartas de Blackjack
int puntosJugador = 0;
int puntosCrupier = 0;
bool turnoJugador = true;
void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  // Entradas con Pull-Up interno (el botón se activa al ir a GND) 
  pinMode(pinPedir, INPUT\_PULLUP);
  pinMode(pinPlantar, INPUT\_PULLUP);
  pinMode(pinReset, INPUT\_PULLUP);
  pinMode(pinBuzzer, OUTPUT); 
  // Semilla de azar basada en ruido analógico
  randomSeed(analogRead(A5));
  // Mensaje inicial en LCD
  lcd.setCursor(0, 0);
  lcd.print(" BLACKJACK BOMB ");
  lcd.setCursor(0, 1);
  lcd.print(" DEFUSE PROTOCOL");
  delay(2000);
  lcd.clear();
  tiempoInicial = millis();
  iniciarEtapa();
}
void loop() {
  // 1\. Verificación del Botón de Reinicio (Disponible siempre)
  if (digitalRead(pinReset) == LOW)
  {
    delay(200);
    // Anti-rebote
    reiniciarJuego();
    return; 
  } if (!juegoActivo) return;
  // Si la bomba explotó o se desactivó, se detiene el flujo 
  unsigned long tiempoActual = millis();
  unsigned long transcurrido = tiempoActual - tiempoInicial;
  // 2\. Verificación de Tiempo Agotado
  if (transcurrido &gt;= tiempoLimiteTotal) {
    explotarBomba("TIEMPO AGOTADO!");
    return; }
  int segundosRestantes = (tiempoLimiteTotal - transcurrido) / 1000;
  // 3\. Tic-Tac sonoro y actualización de reloj cada 1 segundo 
  static unsigned long ultimoTic = 0;
  if (tiempoActual - ultimoTic &gt;= 1000) 
  {
    tone(pinBuzzer, 1000, 40);
    ultimoTic = tiempoActual;
    if (turnoJugador) 
    {
      mostrarEstadoJugador(segundosRestantes);
    }
  } // 4\. Lectura de Botones de Jugador
  if (turnoJugador) 
  {
    // BOTÓN PEDIR CARTA (Hit)
    if (digitalRead(pinPedir) == LOW)
    {
      delay(200);
      int nuevaCarta = random(2, 12);
      puntosJugador += nuevaCarta;
      tone(pinBuzzer, 1500, 80);
      if (puntosJugador &gt; 21) 
      {
        explotarBomba("PASASTE DE 21!");
      }
      else { mostrarEstadoJugador(segundosRestantes);
           }
    }
    // BOTÓN PLANTARSE (Stand)
    if (digitalRead(pinPlantar) == LOW)
    {
      delay(200); 
      turnoJugador = false;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("TURNO CRUPIER...");
      delay(1000);
      jugarCrupier();
    }
  }
}
void iniciarEtapa() {
  puntosJugador = random(2, 12) + random(2, 12);
  // 2 cartas iniciales
  puntosCrupier = random(2, 12);
  // 1 carta visible 
  turnoJugador = true;
  int segundosRestantes = (tiempoLimiteTotal - (millis() - tiempoInicial)) / 1000;
  mostrarEstadoJugador(segundosRestantes);
}
void jugarCrupier() {
  // El Crupier pide cartas automáticamente hasta sumar al menos 17 
  while (puntosCrupier &lt;
         { 
           puntosCrupier += random(2, 12);
         } 
         lcd.clear();
         lcd.setCursor(0, 0);
         lcd.print("CRUPIER TOTAL:");
         lcd.setCursor(0, 1);
         lcd.print("Puntos: ");
         lcd.print(puntosCrupier);
         delay(1800);
 // Evaluación de Ganador de la Etapa
         if (puntosCrupier >21 || puntosJugador > puntosCrupier)
         {
           tone(pinBuzzer, 1800, 200);
           lcd.clear();
           lcd.setCursor(0, 0);
           lcd.print("¡ETAPA ");
           lcd.print(etapaActual);
           lcd.print(" GANADA!");
           if (etapaActual &lt; 3)
           {
             lcd.setCursor(0, 1);
             lcd.print("Siguiente nivel..");
             etapaActual++;
             delay(2000);
             iniciarEtapa();
           }
           else 
           { desactivarBomba();
           }
         }
         else 
         { 
           explotarBomba("CRUPIER TE GANO!");
         }
       }
 void mostrarEstadoJugador(int segRestantes) 
         { 
           // Fila 0: Etapa, Tiempo restante y Carta del Crupier 
           lcd.setCursor(0, 0);
           lcd.print("E:");
           lcd.print(etapaActual);
           lcd.print(" T:");
           if (segRestantes < 10) lcd.print("0");
           lcd.print(segRestantes);
           lcd.print("s Cr:");
           lcd.print(puntosCrupier);
           if (puntosCrupier < 10) lcd.print(" ");
           // Fila 1: Puntos del Jugador y guía rápida de botones
           lcd.setCursor(0, 1);
           lcd.print("Jug:");
           lcd.print(puntosJugador);
           if (puntosJugador < 10) lcd.print(" ");
           lcd.print(" [P][S][R]");
         }
         void explotarBomba(String motivo)
         {
           juegoActivo = false;
           lcd.clear();
           lcd.setCursor(0, 0);
           lcd.print("¡BOOM! EXPLOTO");
           lcd.setCursor(0, 1);
           lcd.print(motivo);
           tone(pinBuzzer, 150, 2000);
           // Sonido grave de explosión
         } 
         void desactivarBomba() 
         { juegoActivo = false;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("¡BOMBA DESACTIV!");
          lcd.setCursor(0, 1);
          lcd.print("HAS GANADO 3/3!");
          // Melodía de victoria
          for (int i = 0; i &lt; 8; i++)
          {
            tone(pinBuzzer, 600 + (i \* 200), 120);
            delay(120);
          }
         }
         void reiniciarJuego() 
         { 
           etapaActual = 1;
           juegoActivo = true;
           tiempoInicial = millis();
           tone(pinBuzzer, 2000, 150);
           lcd.clear();
           lcd.setCursor(0, 0);
           lcd.print(" REINICIANDO ");
           lcd.setCursor(0, 1);
           lcd.print(" PARTIDA... ");
           delay(1200);
           lcd.clear();
           iniciarEtapa();
         }