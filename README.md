# GenDump

GenDump es un proyecto para volcar la memoria de cartuchos de Sega Genesis (Mega Drive) y Sega 32X (Mega Drive 32X) usando Arduino Mega.  
Permite leer y guardar el contenido del cartucho a través de comandos seriales.

## Características

- Comunicación por puerto serie
- Detección automática de cartucho insertado
- Comandos para volcado, lectura de dirección y cabecera

### Mejoras pendientes

- Más velocidad de volcado.
- Lectura y escritura de partidas guardadas.
- Lectura de cartuchos combinados (Sonic & Knuckles)

## Hardware

El hardware está basado en el proyecto [Mega Mega Dumper](https://www.sodnpoo.com/posts.xml/mega_mega_dumper.xml)

### Partes

- ARDUINO MEGA

![Arduino Mega](Images/Parts/Arduino_Mega.jpg)

- SHIELD PARA ARDUINO MEGA

![SHIELD](Images/Parts/PCB_NEPTUNECD.png)

[Descargar Gerber GenDump](Shield/Gerber%20GenDump.zip)

- SLOT DE SEGA GENESIS (MEGA DRIVE)

![SLOT](Images/Parts/Slot_Megadrive.jpg)

- 1 RESISTENCIA DE 4K7

![RESISTENCIA 4K7](Images/Parts/Res4k7.jpg)

- 47 PINES

![47 PINES](Images/Parts/pines_macho.jpg)

## Comunicación Serial

### Configuración

- Tasa de baudios: 115200

### Comandos para comunicarse con GenDump

- `#` : Inicia el volcado completo del cartucho
- `A` : Consulta si hay cartucho insertado
- `@NÚMERO` : Lee la dirección "NÚMERO" del cartucho
- `I` : Muestra la versión del programa
- `_C` : Muestra la cabecera del cartucho

## Funcionamiento

### Pines de los cartuchos

Debemos saber el nombre de los pines de los cartuchos y que función tiene cada uno.

![PINES CARTUCHO](Images/Conection/Genesis-cart-pins.jpg)

#### Funciones

| PIN TRASERO | FUNCION  | PIN DELANTERO | FUNCION |
|-------------|----------|---------------|---------|
| A1          | GND      | B1            | SL1     |
| A2          | VCC      | B2            | MRES    |
| A3          | VA8      | B3            | SR1     |
| A4          | VA11     | B4            | VA9     |
| A5          | VA7      | B5            | VA10    |
| A6          | VA12     | B6            | VA18    |
| A7          | VA6      | B7            | VA19    |
| A8          | VA13     | B8            | VA20    |
| A9          | VA5      | B9            | VA21    |
| A10         | VA14     | B10           | VA22    |
| A11         | VA4      | B11           | VA23    |
| A12         | VA15     | B12           | /YS     |
| A13         | VA3      | B13           | /VSYNC  |
| A14         | VA16     | B14           | /HSYNC  |
| A15         | VA2      | B15           | EDCLK   |
| A16         | VA17     | B16           | /CAS0   |
| A17         | VA1      | B17           | /CE_0   |
| A18         | GND      | B18           | /AS     |
| A19         | VD7      | B19           | VCLK    |
| A20         | VD0      | B20           | /DTACK  |
| A21         | VD8      | B21           | /CAS2   |
| A22         | VD6      | B22           | VD15    |
| A23         | VD1      | B23           | VD14    |
| A24         | VD9      | B24           | VD13    |
| A25         | VD5      | B25           | VD12    |
| A26         | VD2      | B26           | /ASEL   |
| A27         | VD10     | B27           | /VRES   |
| A28         | VD4      | B28           | /LWR    |
| A29         | VD3      | B29           | /UWR    |
| A30         | VD11     | B30           | /M3     |
| A31         | VCC      | B31           | /TIME   |
| A32         | GND      | B32           | /CART   |


## Instalación

1. Clona el repositorio:
   ```bash
   git clone https://github.com/alonso240/GenDump.git
   ```
2. Abre el proyecto en PlatformIO o Arduino IDE.
3. Compila y sube el código al Arduino Mega.

## Créditos

- alonso240
- Luiti

---

> **Nota:**  
> Para mostrar imágenes, colócalas en la carpeta `images` dentro del repositorio y usa la sintaxis `![Texto alternativo](images/nombre_imagen.png)`.  
> Si usas enlaces externos, reemplaza la ruta por la URL directa.
