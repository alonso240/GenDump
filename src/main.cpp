/*
  Version 0.001
  alonso240
  Luiti
*/

#include<Arduino.h>

// Versión del lector
String infoVersion = "GenDump v0.001";

// estructura GPIO
typedef struct
{
  volatile uint8_t *port; // puntero al registro PORTx
  volatile uint8_t *ddr;  // puntero al registro DDRx
  volatile uint8_t *pin;  // puntero al registro PINx
  uint8_t bitmask;        // mascara de bit para el pin
} PinInfo;

// Ya no necesitamos los arrays A[] y D[] si usamos PinInfo directamente en todas partes
uint8_t A[23] = {37, 33, 29, 3, 5, 27, 23, 8, 53, 51, 9, 25, 6, 4, 2, 31, 35, 49, 47, 45, 43, 41, 39};

//Marca si hemos cancelado el proceso de volcado.
bool cancelar = false;

//Pin que comprueba la existencia de un cartucho insertado.
PinInfo pinExistenciaCart = {&PORTF, &DDRF, &PINF, _BV(0)};

//Lista de pines de dirección.
PinInfo digitalPinsA[] = {
    {&PORTC, &DDRC, &PINC, _BV(0)}, // Pin 37  A1
    {&PORTC, &DDRC, &PINC, _BV(4)}, // Pin 33  A2
    {&PORTA, &DDRA, &PINA, _BV(7)}, // Pin 29  A3
    {&PORTE, &DDRE, &PINE, _BV(5)}, // Pin 3   A4
    {&PORTE, &DDRE, &PINE, _BV(3)}, // Pin 5   A5
    {&PORTA, &DDRA, &PINA, _BV(5)}, // Pin 27  A6
    {&PORTA, &DDRA, &PINA, _BV(1)}, // Pin 23  A7
    {&PORTH, &DDRH, &PINH, _BV(5)}, // Pin 8   A8
    {&PORTB, &DDRB, &PINB, _BV(0)}, // Pin 53  A9
    {&PORTB, &DDRB, &PINB, _BV(2)}, // Pin 51  A10
    {&PORTH, &DDRH, &PINH, _BV(6)}, // Pin 9   A11
    {&PORTA, &DDRA, &PINA, _BV(3)}, // Pin 25  A12
    {&PORTH, &DDRH, &PINH, _BV(3)}, // Pin 6   A13
    {&PORTG, &DDRG, &PING, _BV(5)}, // Pin 4   A14
    {&PORTE, &DDRE, &PINE, _BV(4)}, // Pin 2   A15
    {&PORTC, &DDRC, &PINC, _BV(6)}, // Pin 31  A16
    {&PORTC, &DDRC, &PINC, _BV(2)}, // Pin 35  A17
    {&PORTL, &DDRL, &PINL, _BV(0)}, // Pin 49  A18
    {&PORTL, &DDRL, &PINL, _BV(2)}, // Pin 47  A19
    {&PORTL, &DDRL, &PINL, _BV(4)}, // Pin 45  A20
    {&PORTL, &DDRL, &PINL, _BV(6)}, // Pin 43  A21
    {&PORTG, &DDRG, &PING, _BV(0)}, // Pin 41  A22
    {&PORTG, &DDRG, &PING, _BV(2)}  // Pin 39  A23
};

// data pins
//  uint8_t D[16] = { 38, 44, 30, 24, 26, 32, 42, 36, 40, 34, 28, 22, 52, 50, 48, 46 };

//Lista de pines de lectura.
PinInfo digitalPinsD[] = {
    {&PORTD, &DDRD, &PIND, _BV(7)}, // Pin 38  D0
    {&PORTL, &DDRL, &PINL, _BV(5)}, // Pin 44  D1
    {&PORTC, &DDRC, &PINC, _BV(7)}, // Pin 30  D2
    {&PORTA, &DDRA, &PINA, _BV(2)}, // Pin 24  D3
    {&PORTA, &DDRA, &PINA, _BV(4)}, // Pin 26  D4
    {&PORTC, &DDRC, &PINC, _BV(5)}, // Pin 32  D5
    {&PORTL, &DDRL, &PINL, _BV(7)}, // Pin 42  D6
    {&PORTC, &DDRC, &PINC, _BV(1)}, // Pin 36  D7
    {&PORTG, &DDRG, &PING, _BV(1)}, // Pin 40  D8
    {&PORTC, &DDRC, &PINC, _BV(3)}, // Pin 34  D9
    {&PORTA, &DDRA, &PINA, _BV(6)}, // Pin 28  D10
    {&PORTA, &DDRA, &PINA, _BV(0)}, // Pin 22  D11
    {&PORTB, &DDRB, &PINB, _BV(1)}, // Pin 52  D12
    {&PORTB, &DDRB, &PINB, _BV(3)}, // Pin 50  D13
    {&PORTL, &DDRL, &PINL, _BV(1)}, // Pin 48  D14
    {&PORTL, &DDRL, &PINL, _BV(3)}  // Pin 46  D15
};

uint8_t pinExist = A0; // Este pin no es crítico para la velocidad, digitalRead es aceptable

//Marca si hay cartucho insertado.
uint8_t existenciaCartucho = 0;

//Límite máximo de direcciones enviadas a SNeptune.
uint32_t limiteEnvio = 64;

//Marca si estamos volcando
bool dumpeando = false;

//Texto recibido por el puerto serial
String receivedData;

// Configura un pin de dirección a LOW (activo)
void setAddressPinLow(uint8_t i)
{
  PinInfo p = digitalPinsA[i];
  noInterrupts(); // Deshabilitar interrupciones para operación atómica
  *(p.port) &= ~p.bitmask;
  interrupts();
}

// Configura un pin de dirección a HIGH (inactivo)
void setAddressPinHigh(uint8_t i)
{
  PinInfo p = digitalPinsA[i];
  noInterrupts(); // Deshabilitar interrupciones para operación atómica
  *(p.port) |= p.bitmask;
  interrupts();
}

// Lee el estado de un pin de datos
uint8_t readDataPin(uint8_t i)
{
  PinInfo p = digitalPinsD[i];
  return (*(p.pin) & p.bitmask) ? HIGH : LOW;
}

//Lee si existe cartucho
uint8_t readExistCart()
{
  // PinInfo p = pinExistenciaCart;
  return !(*(pinExistenciaCart.pin) & pinExistenciaCart.bitmask) ? HIGH : LOW;
}

//Reseteamos los pines de dirección.
void resetAddressBus()
{
  for (uint8_t i = 0; i < 23; i++)
  {
    setAddressPinLow(i);
  }
}

//Configuramos los pines de dirección
void writeAddress(uint32_t addr)
{
  uint32_t mask = 1;
  for (uint8_t i = 0; i < 23; i++)
  {
    uint32_t test = (mask & addr) >> i;
    if (test == 1)
    {
      setAddressPinHigh(i);
    }
    else
    {
      setAddressPinLow(i);
    }
    mask = mask << 1;
  }
}

//Leemos el estado de los pines de lectura.
word readData()
{
  word d = 0;
  word mask2 = 0b1;
  for (uint8_t i = 0; i < 16; i++)
  {
    uint8_t b = readDataPin(i);
    if (b == HIGH)
    {
      d = d | mask2;
    }
    mask2 = mask2 << 1;
  }
  return d;
}

// Leemos el código según la dirección...
String LeerDireccion(uint32_t direccion)
{
  String vl = "";

  resetAddressBus();

  uint32_t romstart = (direccion * 8);

  for (uint32_t i = romstart; i < (romstart + 8); i++)
  {
    writeAddress(i);

    word d = readData();

    byte d2 = d & 0xFF;
    byte d1 = (d & 0xFF00) >> 8;

    if (d1 < 0x10)
    {
      vl += "0";
    }
    vl += String(d1, HEX);
    if (d2 < 0x10)
    {
      vl += String("0");
    }
    vl += String(d2, HEX);
  }

  return vl;
}

//Leemos el código según la dirección
String DumpDireccion(uint32_t direccion)
{

  String vl = "";

  uint32_t romstart = (direccion * 8);

  for (uint32_t i = romstart; i < (romstart + 8); i++)
  {
    writeAddress(i);
    delayMicroseconds(50);
    word d = readData();

    byte d2 = d & 0xFF;
    byte d1 = (d & 0xFF00) >> 8;

    if (d1 < 0x10)
    {
      vl += "0";
    }
    vl += String(d1, HEX);
    if (d2 < 0x10)
    {
      vl += "0";
    }
    vl += String(d2, HEX);
  }

  return vl;
}

//Función de volcado del cartucho
void DumpCart()
{
  dumpeando = true;
  resetAddressBus();

  delay(5);
  // Configurar pines de datos como INPUT usando manipulación directa
  for (uint8_t i = 0; i < 16; i++)
  {
    PinInfo p = digitalPinsD[i];
    *(p.ddr) &= ~p.bitmask; // Set as INPUT
  }

  String vl = "";

  String vl2 = LeerDireccion(26);

  String dirM = vl2.substring(0, 8);
  String dirMax = vl2.substring(8, 16);

  uint32_t romstart = strtol(dirM.c_str(), NULL, 16);
  uint32_t romend = strtol(dirMax.c_str(), NULL, 16);

  // Max 65535
  uint32_t envio = 0;

  romend++;
  romend = romend / 16;

  for (uint32_t i = romstart; i < romend; i++)
  {

    vl += DumpDireccion(i);

    if (envio == limiteEnvio)
    {
      envio = 0;
      Serial.println(vl);
      // delay(100);
      vl = "";

      uint8_t existCart = readExistCart();

      if (existCart == 0)
      {
        cancelar = true;
        break;
      }
    }
    else
    {
      envio++;
    }
  }

  if (vl != "")
  {
    Serial.println(vl);
    vl = "";
  }

  dumpeando = false;
}

void setup()
{
  Serial.begin(115200); // Aumentar la velocidad del puerto serial

  // Primero limpia todos los registros relevantes
  DDRA = 0;
  PORTA = 0;
  DDRB = 0;
  PORTB = 0;
  DDRC = 0;
  PORTC = 0;
  DDRD = 0;
  PORTD = 0;
  DDRE = 0;
  PORTE = 0;
  DDRF = 0;
  PORTF = 0;
  DDRG = 0;
  PORTG = 0;
  DDRH = 0;
  PORTH = 0;
  DDRJ = 0;
  PORTJ = 0;
  DDRK = 0;
  PORTK = 0;
  DDRL = 0;
  PORTL = 0;

  pinMode(pinExist, INPUT_PULLUP); // digitalRead es ok para este pin

  // Configuración correcta de pines de dirección
  for (uint8_t i = 0; i < 23; i++)
  {
    PinInfo p = digitalPinsA[i];

    // Operación atómica para modificar solo el bit necesario
    *(p.ddr) |= p.bitmask;   // Configurar como SALIDA
    *(p.port) &= ~p.bitmask; // Poner en LOW inicialmente
  }

  // Configurar todos los pines de datos como ENTRADA usando manipulación directa
  for (uint8_t i = 0; i < 16; i++)
  {
    PinInfo p = digitalPinsD[i];
    *(p.ddr) &= ~p.bitmask; // Set as INPUT
                            // Si necesitas INPUT_PULLUP para los pines de datos, activa la siguiente línea:
    *(p.port) |= p.bitmask; // Activa la resistencia de pull-up interna
  }
}

void loop()
{

  if (!dumpeando)
  {
    uint8_t existCart = readExistCart();

    if (existenciaCartucho != existCart)
    {
      Serial.print("Cart_status:");
      Serial.println(existCart);
      existenciaCartucho = existCart;
    }
  }
  delay(500);
}

//Función que recibe comandos externos
void serialEvent()
{
  //Si el puerto está disponible...
  if (Serial.available() > 0)
  {
    //Pausamos 10 ms.
    delay(10);

    //Guardamos el texto
    receivedData = Serial.readStringUntil('\n');
    //Quitamos los espacios vacios al inicio y al final del texto
    receivedData.trim();

    //Si recibimos "#" comenzamos a volcar el cartucho...
   if (receivedData == "#")
    {
      //Si no hay cartucho...
      if (existenciaCartucho == 0)
      {
        //Salimos de la función
        return;
      }

      //Leemos el codigo
      DumpCart();

      //Si no se ha cancelado...
      if (!cancelar)
      {
        //Enviamos "_FINDUMP_" para avisar que ya esta todo el código leido
        Serial.println("_FINDUMP_");
      }
      //Si hemos cancelado...
      else
      {
        //Avisamos que hemos cancelado y el código no está completo.
        Serial.println("_CANCELAR_");
      }
    }

    //Si recibimos "A" devolvemos si hay cartucho insertado.
    else if (receivedData == "A")
    {
      //Enviamos el estado del cartucho
      Serial.print("Cart_status:");
      Serial.println(!digitalRead(pinExist));
    }

    //Si recibimos "@" devolvemos el código según la dirección recibida.
    else if (receivedData.indexOf("@") > -1)
    {
      //Si no hay cartucho insertado...
      if (existenciaCartucho == 0)
      {
        //Salimos de la función
        return;
      }
      //Guardamos el texto recibido y quitamos "@".
      String v = receivedData.substring(1);
      //Convertimos el texto en número.
      uint8_t dir = v.toInt();
      //Vamos a la dirección.
     Serial.print(DumpDireccion(dir));
      Serial.println("");
    }

    //Si recibimos "I" devolvemos la versión de GenDump.
    else if (receivedData == "I")
    {
      delay(500);
      Serial.print("Version:");

      Serial.println(infoVersion);
    }

    //Si recibimos "_C" devolvemos la cabecera del cartucho.
    else if (receivedData == "_C")
    {
      if (existenciaCartucho == 0)
      {
        return;
      }
      for (uint8_t a = 0; a < 16; a++)
      {
        Serial.print(DumpDireccion(16 + a));
      }
      Serial.println("_CABECERA_");
    }
  }
}