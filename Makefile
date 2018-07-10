# @(#) $Id: Makefile 46 2017-11-29 03:29:58Z leres $ (XSE)

TARGET=	blink2

ARDUINO_BOARD= atmega328
AVRDUDE_PORT = /dev/cuaU0
#PORT = /dev/cuaU0
AVRDUDE_SPEED = 115200
#UPLOAD_RATE = 115200

ARDUINO_MK_DIR=	/usr/local/arduino-bsd-mk

MAKEOBJDIRPREFIX=/usr/obj

ARDUINO_CFLAGS+= -Wall -Werror -Wstrict-prototypes

.include "${ARDUINO_MK_DIR}/bsd.arduino.mk"
