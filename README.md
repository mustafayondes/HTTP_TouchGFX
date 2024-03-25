This system consists of 2 microprocessors. STM32F469 is main microprocessors which is proggrammed in TOUCHGFX Designer. ESP32 is used to connect wifi and get data through HTTP protocol.
These 2 microprocessors communicate with UART. ESP32 Send request to HTTP server to get data. After that these datas taken from HTTP , are sent from ESP32 to STM32.
STM32 showes datas on Touch Screen .

