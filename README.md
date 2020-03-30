# Cemco-sirio

# Construido con
* [Visual Studio Code](https://code.visualstudio.com/) - El editor de codigo usado.

* [PlatformIO](https://platformio.org/) - Extensión para sistemas embebidos. 

# Librerias
En el desarrollo del proyecto se implementaron liberías de terceros para la comunicación con los periféricos.

* [ArduinoJson](https://github.com/bblanchon/ArduinoJson) - Libería para procesar json de los request.

* [EDB](https://github.com/jwhiddon/EDB) - Libería para gestionar las base de datos de forma local en el arduino.

* [Ethernet](https://github.com/arduino-libraries/Ethernet) - Libería del shield de ethernet.

* [Hashmap](https://github.com/vlast3k/Arduino-libraries/tree/master/HashMap) - Libería para la matriz de dispositivos.

* [Time](https://github.com/PaulStoffregen/Time) - Libería para el manejo del tiempo

* [VC0706 Serial camera](https://github.com/adafruit/Adafruit-VC0706-Serial-Camera-Library) - Libería para comunicarse con la camara serial.

* [Wiegand](https://github.com/monkeyboard/Wiegand-Protocol-Library-for-Arduino) - Libería del protocolo usado para comincarse con el keypad.

Para la instalación de las liberías se puede dirigir al gestor de liberías de platformio y buscar cada una de las liberías.

![Example_install_library](Example.png "Example")

Otra opción para instalar las liberías en cualquier versión del OS Windows es descargar el código source y moverlo a la ubicación en disco:
```
C:\Users\<YourUserName>\.platformio\packages\framework-arduino-avr\libraries
``` 