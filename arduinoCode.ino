// Pinos PWM
const int PIN_R = 9;
const int PIN_G = 10;
const int PIN_B = 11;

// Estados e valores (0–100%)
int pctR = 0, pctG = 0, pctB = 0;        // último valor ajustado por porcentagem
bool onR = false, onG = false, onB = false; // estado ON/OFF de cada LED

// Converte 0–100% para 0–255 PWM
int pctToPwm(int pct) {
  if (pct < 0) pct = 0;
  if (pct > 100) pct = 100;
  return (int)round(pct * 255.0 / 100.0);
}

// Aplica estados aos pinos
void applyOutputs() {
  analogWrite(PIN_R, onR ? pctToPwm(pctR) : 0);
  analogWrite(PIN_G, onG ? pctToPwm(pctG) : 0);
  analogWrite(PIN_B, onB ? pctToPwm(pctB) : 0);
}

void setup() {
  pinMode(PIN_R, OUTPUT);
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_B, OUTPUT);
  Serial.begin(9600);
  // estado inicial: todos OFF
  applyOutputs();
  Serial.println("Pronto. Use comandos: R50, GON, BOFF, ALLON, ALLOFF. Separe por ; se quiser.");
}

void handleToken(String tok) {
  tok.trim();
  tok.toUpperCase();
  if (tok.length() == 0) return;

  // Comandos globais
  if (tok == "ALLON") { onR = onG = onB = true; applyOutputs(); return; }
  if (tok == "ALLOFF") { onR = onG = onB = false; applyOutputs(); return; }

  // LED específico por porcentagem: Rxx, Gxx, Bxx (0–100)
  char c = tok.charAt(0);
  if (c == 'R' || c == 'G' || c == 'B') {
    String rest = tok.substring(1);
    // ON/OFF específicos
    if (rest == "ON") {
      if (c == 'R') onR = true;
      if (c == 'G') onG = true;
      if (c == 'B') onB = true;
      applyOutputs();
      return;
    }
    if (rest == "OFF") {
      if (c == 'R') onR = false;
      if (c == 'G') onG = false;
      if (c == 'B') onB = false;
      applyOutputs();
      return;
    }
    // Percentual
    int val = rest.toInt(); // lida com "50", "100", "0"
    if (val >= 0 && val <= 100) {
      if (c == 'R') { pctR = val; onR = (val > 0) ? true : onR; }
      if (c == 'G') { pctG = val; onG = (val > 0) ? true : onG; }
      if (c == 'B') { pctB = val; onB = (val > 0) ? true : onB; }
      // Nota: se val=0, não força OFF do estado; quem decide é o onX. Para garantir apagado, use XOFF ou ALLOFF.
      applyOutputs();
      return;
    }
  }

  // Se chegou aqui, comando inválido
  Serial.print("Comando invalido: ");
  Serial.println(tok);
}

void loop() {
  // Lê linha até \n e permite múltiplos comandos separados por ;
  if (Serial.available()) {
    String line = Serial.readStringUntil('\n');
    line.trim();
    if (line.length() == 0) return;
    // Divide por ';'
    int start = 0;
    while (start < line.length()) {
      int sep = line.indexOf(';', start);
      String tok;
      if (sep == -1) {
        tok = line.substring(start);
        start = line.length();
      } else {
        tok = line.substring(start, sep);
        start = sep + 1;
      }
      handleToken(tok);
    }
  }
}
