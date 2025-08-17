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



### Ejemplo de uso en consola

![Consola Serial](images/consola_serial.png)

## Comandos disponibles

- `#` : Inicia el volcado completo del cartucho
- `A` : Consulta si hay cartucho insertado
- `@N` : Lee la dirección N del cartucho
- `I` : Muestra la versión del programa
- `_C` : Muestra la cabecera del cartucho

## Requisitos

- Arduino Mega 2560
- Cableado según el esquema de pines del proyecto
- [PlatformIO](https://platformio.org/) o Arduino IDE

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
