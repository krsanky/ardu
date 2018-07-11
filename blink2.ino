#include <SPI.h>

/*
 * 1 - Setup the clock such that we get a SQW output 2 - Set the time of the
 * clock 3 - Read the time of the clock
 */

int		chipSelect = 8;
int		intFreq = 4;

#define WRITE_CONTROL_REG 0x8E
#define READ_CONTROL_REG 0x0E
#define WRITE_TIME_REG 0x80
#define READ_TIME_REG 0x00

typedef struct {
	uint8_t		ss;
	uint8_t		mm;
	uint8_t		hh;
	uint8_t		dy;
	uint8_t		d;
	uint8_t		m;
	uint8_t		y;
}		timeParameters;


void 
RTC_init(int chipSelect, int intFreq)
{
	/*
	 * The DS3234 offers four output frequencies: Options: 1:     1
	 * Hz 2:     1.024 kHz 3:     4.096 kHz 4:     8.192 kHz 5:     OFF
	 */

	pinMode(chipSelect, OUTPUT);

	digitalWrite(chipSelect, LOW);
	SPI.transfer(READ_CONTROL_REG);
	byte		originalConfig = SPI.transfer(0x00);
	digitalWrite(chipSelect, HIGH);
	delay(10);

	byte		configModifier;
	byte		newConfig;

	if (intFreq == 5) {
		configModifier = 0 b10111111;
		newConfig = configModifier & originalConfig;
	} else if (intFreq < 5) {
		uint8_t		freqOption = intFreq - 1;
		configModifier = (freqOption << 3) | 0 b01000000;
		newConfig = configModifier | (originalConfig & 0 b11100011);
	} else {
		newConfig = 0 b11100011;
	}

	digitalWrite(chipSelect, LOW);
	SPI.transfer(WRITE_CONTROL_REG);
	SPI.transfer(newConfig);
	digitalWrite(chipSelect, HIGH);
	delay(10);
}

static uint8_t 
convertValueIN(uint8_t value)
{
	uint8_t		convertedVal = value - 6 * (value >> 4);
	return convertedVal;
}

static uint8_t 
convertValueOUT(uint8_t value)
{
	uint8_t		convertedVal = value + 6 * (value / 10);
	return convertedVal;
}

void 
setTime(int chipSelect, timeParameters * timeVals)
{
	pinMode(chipSelect, OUTPUT);

	digitalWrite(chipSelect, LOW);
	SPI.transfer(WRITE_TIME_REG);
	SPI.transfer(convertValueOUT(timeVals->ss));
	SPI.transfer(convertValueOUT(timeVals->mm));
	SPI.transfer(convertValueOUT(timeVals->hh));
	SPI.transfer(convertValueOUT(timeVals->dy));
	SPI.transfer(convertValueOUT(timeVals->d));
	SPI.transfer(convertValueOUT(timeVals->m));
	SPI.transfer(convertValueOUT(timeVals->y));
	digitalWrite(chipSelect, HIGH);
	delay(10);

}

void 
readTime(int chipSelect, timeParameters * timeVals)
{
	pinMode(chipSelect, OUTPUT);

	digitalWrite(chipSelect, LOW);
	SPI.transfer(READ_TIME_REG);
	timeVals->ss = convertValueIN(SPI.transfer(0x00));
	timeVals->mm = convertValueIN(SPI.transfer(0x00));
	timeVals->hh = convertValueIN(SPI.transfer(0x00));
	timeVals->dy = convertValueIN(SPI.transfer(0x00));
	timeVals->d = convertValueIN(SPI.transfer(0x00));
	timeVals->m = convertValueIN(SPI.transfer(0x00));
	timeVals->y = convertValueIN(SPI.transfer(0x00));
	digitalWrite(chipSelect, HIGH);
	delay(10);
}

timeParameters	example_vals = {
	50,
	59,
	23,
	1,
	31,
	12,
	10
};

uint32_t	timeTrigs = 0;

void 
timeFunc()
{
	timeTrigs += 1;
}

void 
setup()
{
	SPI.begin();
	SPI.setBitOrder(MSBFIRST);
	SPI.setDataMode(SPI_MODE1);
	Serial.begin(9600);
	delay(10);

	RTC_init(chipSelect, intFreq);
	setTime(chipSelect, &example_vals);
	attachInterrupt(0, timeFunc, RISING);

}

void 
loop()
{
	delay(1000);
	readTime(chipSelect, &example_vals);

	Serial.print("Time: ");
	Serial.print(example_vals.hh);
	Serial.print(":");
	Serial.print(example_vals.mm);
	Serial.print(":");
	Serial.println(example_vals.ss);

	Serial.print("Date: ");
	Serial.print(example_vals.y);
	Serial.print("/");
	Serial.print(example_vals.m);
	Serial.print("/");
	Serial.println(example_vals.d);

	Serial.print("Time Triggers: ");
	Serial.println(timeTrigs);
	timeTrigs = 0;
}
