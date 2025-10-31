const int FILAS = 4;
const int COLUMNAS = 4;

const int Pinesfilas[FILAS] = { 13, 12, 14, 27 };
const int Pinescolumnas[COLUMNAS] = { 26, 25, 33, 32 };

const int VERDE = 4;
const int ROJO = 2;

char Teclas[FILAS][COLUMNAS] = { { '1', '2', '3', 'A' }, { '4', '5', '6', 'B' }, { '7', '8', '9', 'C' }, { '*', '0', '#', 'D' } };

const char Clave[5] = "1234";

char leerTecla() {
  for (int F = 0; F < FILAS; F++) {
    digitalWrite(Pinesfilas[F], LOW);
    delay(5);

    for (int C = 0; C < COLUMNAS; C++) {
      if (digitalRead(Pinescolumnas[C]) == LOW) {
        unsigned long t0 = millis();
        while (digitalRead(Pinescolumnas[C]) == LOW) {
          if (millis() - t0 > 1000) break;
        }
        delay(50);

        digitalWrite(Pinesfilas[F], HIGH);

        return Teclas[F][C];
      }
    }
    digitalWrite(Pinesfilas[F], HIGH);
  }
  return '\0';
}

void setup() {
  Serial.begin(9600);
  configurarEntradasSalidas();
  Serial.println("Ingrese la clave de 4 dígitos en el teclado");
}

void loop() {
  static char buffer[5] = { '\0', '\0', '\0', '\0', '\0' };
  static int index = 0;

  char Lectura = leerTecla();
  if (Lectura != '\0') {
    if ((Lectura >= '0' && Lectura <= '9')) {
      buffer[index] = Lectura;
      index++;
      Serial.print(Lectura);
      Serial.print(" ");
      delay(120);

      if (index >= 4) {
        buffer[4] = '\0';
        Serial.println();
        Serial.print("Clave ingresada: ");
        Serial.println(buffer);
        verificarClave(buffer);

        index = 0;
        for (int i = 0; i < 5; i++) buffer[i] = '\0';
        Serial.println("Listo para otro intento.");
      }
    } else {
      Serial.print("Tecla no numérica presionada: ");
      Serial.println(Lectura);
      delay(150);
    }
  }

  delay(10);
}

void configurarEntradasSalidas() {

  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH);
  pinMode(14, OUTPUT);
  digitalWrite(14, HIGH);
  pinMode(27, OUTPUT);
  digitalWrite(27, HIGH);

  pinMode(26, INPUT_PULLUP);
  pinMode(25, INPUT_PULLUP);
  pinMode(33, INPUT_PULLUP);
  pinMode(32, INPUT_PULLUP);

  pinMode(VERDE, OUTPUT);
  pinMode(ROJO, OUTPUT);
  digitalWrite(VERDE, LOW);
  digitalWrite(ROJO, LOW);
}

void verificarClave(const char* ingresada) {

  bool correcta = true;
  for (int i = 0; i < 4; i++) {
    if (ingresada[i] != Clave[i]) {
      correcta = false;
      break;
    }
  }

  if (correcta) {
    Serial.println("Clave correcta. Acceso concedido.");
    digitalWrite(VERDE, HIGH);
    digitalWrite(ROJO, LOW);
  } else {
    Serial.println("Clave incorrecta");
    digitalWrite(ROJO, HIGH);
    digitalWrite(VERDE, LOW);
  }

  delay(3000);
  digitalWrite(VERDE, LOW);
  digitalWrite(ROJO, LOW);
}
