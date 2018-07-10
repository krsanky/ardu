/* @(#) $Id: blink2.ino 34 2017-03-19 17:51:19Z leres $ (XSE) */

/*
 * This is a example sketch that blinks the LED like a heartbeat
 */

#ifndef PIN_LED_OUT
#define PIN_LED_OUT LED_BUILTIN
#endif

#ifndef LED_ON
#define LED_ON HIGH
#endif

#ifndef LED_OFF
#define LED_OFF LOW
#endif

void
setup()
{
	pinMode(PIN_LED_OUT, OUTPUT);
}

void
loop()
{
	digitalWrite(PIN_LED_OUT, LED_ON);
	delay(25);

	digitalWrite(PIN_LED_OUT, LED_OFF);
	delay(50);

	digitalWrite(PIN_LED_OUT, LED_ON);
	delay(25);

	digitalWrite(PIN_LED_OUT, LED_OFF);
	delay(900);
}
