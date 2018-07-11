TARGET=	blink2

ARDUINO_BOARD= atmega328
AVRDUDE_PORT = /dev/cuaU0
AVRDUDE_SPEED = 115200

ARDUINO_MK_DIR=	/usr/local/arduino-bsd-mk

MAKEOBJDIRPREFIX=/usr/obj

ARDUINO_CFLAGS+= -Wall -Werror -Wstrict-prototypes
#ARDUINO_LIBS+= SparkFunDS3234RTC SPI
ARDUINO_LIBS+= SPI

.include "${ARDUINO_MK_DIR}/bsd.arduino.mk"
