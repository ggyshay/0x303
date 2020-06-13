#include "DisplayDriver.h"
#include "Audio.h"
static const char font_bmap[18 * 47] PROGMEM =
    {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xf0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x2, 0x2, 0x2, 0x2, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x30, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x80, 0xe0, 0x10, 0x0, 0x0, 0x0, 0x0, 0x30, 0xc, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc0, 0x60, 0x10, 0x10, 0x10, 0x60, 0xc0, 0x0, 0x0, 0xf, 0x18, 0x20, 0x23, 0x20, 0x18, 0x7, 0x0, 0x0, 0x0, 0x20, 0x10, 0xf0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x20, 0x20, 0x3f, 0x20, 0x20, 0x0, 0x0, 0x0, 0x0, 0x30, 0x10, 0x10, 0x30, 0xe0, 0x0, 0x0, 0x0, 0x0, 0x30, 0x28, 0x24, 0x23, 0x21, 0x0, 0x0, 0x0, 0x0, 0x20, 0x10, 0x10, 0xa0, 0xe0, 0x0, 0x0, 0x0, 0x0, 0x20, 0x21, 0x21, 0x13, 0x1e, 0x0, 0x0, 0x0, 0x0, 0x0, 0x80, 0x60, 0xf0, 0xf0, 0x0, 0x0, 0x0, 0xc, 0xb, 0x8, 0x8, 0x3f, 0x3f, 0x8, 0x0, 0x0, 0x0, 0xf0, 0x10, 0x10, 0x10, 0x10, 0x0, 0x0, 0x0, 0x0, 0x31, 0x21, 0x21, 0x33, 0x1e, 0x0, 0x0, 0x0, 0x0, 0xe0, 0x30, 0x10, 0x10, 0x10, 0x0, 0x0, 0x0, 0x3, 0x1f, 0x31, 0x21, 0x21, 0x1a, 0xc, 0x0, 0x0, 0x10, 0x10, 0x10, 0x10, 0x90, 0x70, 0x10, 0x0, 0x0, 0x0, 0x20, 0x38, 0xe, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0xe0, 0x10, 0x10, 0x10, 0xe0, 0x0, 0x0, 0x0, 0xc, 0x1e, 0x21, 0x21, 0x21, 0x1e, 0xc, 0x0, 0x0, 0xc0, 0xe0, 0x10, 0x10, 0x30, 0xe0, 0x80, 0x0, 0x0, 0x0, 0x21, 0x22, 0x22, 0x32, 0x1f, 0x7, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x30, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xf0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x80, 0x80, 0x40, 0x20, 0x0, 0x0, 0x0, 0x0, 0x1, 0x2, 0x6, 0x4, 0x8, 0x0, 0x0, 0x0, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x0, 0x0, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x0, 0x0, 0x0, 0x20, 0x40, 0x40, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x8, 0x4, 0x4, 0x2, 0x1, 0x0, 0x0, 0x0, 0x0, 0x20, 0x10, 0x10, 0xb0, 0xe0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x36, 0x3, 0x1, 0x0, 0x0, 0x0, 0x0, 0xc0, 0x20, 0x10, 0x90, 0x90, 0x20, 0xc0, 0x0, 0x0, 0x3f, 0x40, 0x9b, 0x90, 0x9f, 0x90, 0xc, 0x0, 0x0, 0x0, 0x0, 0xe0, 0x30, 0xe0, 0x0, 0x0, 0x0, 0x0, 0x38, 0xf, 0x5, 0x4, 0x5, 0xf, 0x38, 0x0, 0x0, 0x0, 0xf0, 0x10, 0x10, 0x10, 0xe0, 0x40, 0x0, 0x0, 0x0, 0x3f, 0x21, 0x21, 0x21, 0x12, 0xc, 0x0, 0x0, 0xc0, 0x60, 0x10, 0x10, 0x10, 0x10, 0x0, 0x0, 0x0, 0xf, 0x18, 0x20, 0x20, 0x20, 0x20, 0x10, 0x0, 0x0, 0x0, 0xf0, 0x10, 0x10, 0x30, 0x60, 0xc0, 0x0, 0x0, 0x0, 0x3f, 0x20, 0x20, 0x30, 0x18, 0xf, 0x0, 0x0, 0x0, 0xf0, 0x10, 0x10, 0x10, 0x10, 0x0, 0x0, 0x0, 0x0, 0x3f, 0x21, 0x21, 0x21, 0x21, 0x0, 0x0, 0x0, 0x0, 0xf0, 0x10, 0x10, 0x10, 0x10, 0x0, 0x0, 0x0, 0x0, 0x3f, 0x1, 0x1, 0x1, 0x1, 0x0, 0x0, 0x0, 0xc0, 0x60, 0x10, 0x10, 0x10, 0x10, 0x0, 0x0, 0x0, 0xf, 0x18, 0x20, 0x20, 0x22, 0x32, 0x1e, 0x0, 0x0, 0x0, 0xf0, 0x0, 0x0, 0x0, 0xf0, 0x0, 0x0, 0x0, 0x0, 0x3f, 0x1, 0x1, 0x1, 0x3f, 0x0, 0x0, 0x0, 0x0, 0x10, 0x10, 0xf0, 0x10, 0x10, 0x0, 0x0, 0x0, 0x0, 0x20, 0x20, 0x3f, 0x20, 0x20, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xf0, 0x0, 0x0, 0x0, 0x0, 0x20, 0x20, 0x20, 0x20, 0x1f, 0x0, 0x0, 0x0, 0x0, 0x0, 0xf0, 0x0, 0x80, 0x40, 0x20, 0x10, 0x0, 0x0, 0x0, 0x3f, 0x1, 0x3, 0x6, 0x18, 0x30, 0x0, 0x0, 0x0, 0xf0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3f, 0x20, 0x20, 0x20, 0x20, 0x0, 0x0, 0x0, 0xf0, 0x70, 0xc0, 0x0, 0xc0, 0x30, 0xf0, 0x0, 0x0, 0x3f, 0x0, 0x0, 0x3, 0x1, 0x0, 0x3f, 0x0, 0x0, 0x0, 0xf0, 0xc0, 0x0, 0x0, 0xf0, 0x0, 0x0, 0x0, 0x0, 0x3f, 0x0, 0x3, 0xc, 0x3f, 0x0, 0x0, 0x0, 0xc0, 0x60, 0x10, 0x10, 0x10, 0x60, 0xc0, 0x0, 0x0, 0xf, 0x18, 0x20, 0x20, 0x20, 0x18, 0xf, 0x0, 0x0, 0x0, 0xf0, 0x10, 0x10, 0x10, 0xf0, 0x40, 0x0, 0x0, 0x0, 0x3f, 0x1, 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0xc0, 0x60, 0x10, 0x10, 0x10, 0x60, 0xc0, 0x0, 0x0, 0xf, 0x18, 0x20, 0x20, 0x60, 0x98, 0x8f, 0x0, 0x0, 0x0, 0xf0, 0x10, 0x10, 0x10, 0xe0, 0x40, 0x0, 0x0, 0x0, 0x3f, 0x1, 0x3, 0x7, 0x18, 0x30, 0x0, 0x0, 0x0, 0xe0, 0x10, 0x10, 0x10, 0x20, 0x0, 0x0, 0x0, 0x0, 0x30, 0x21, 0x21, 0x22, 0x1e, 0x0, 0x0, 0x0, 0x10, 0x10, 0x10, 0xf0, 0x10, 0x10, 0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3f, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xf0, 0x0, 0x0, 0x0, 0xf0, 0x0, 0x0, 0x0, 0x0, 0x1f, 0x20, 0x20, 0x20, 0x1f, 0x0, 0x0, 0x0, 0x70, 0x80, 0x0, 0x0, 0x0, 0x80, 0x70, 0x0, 0x0, 0x0, 0x3, 0x1c, 0x30, 0x1c, 0x3, 0x0, 0x0, 0x10, 0xf0, 0x0, 0x0, 0x80, 0x0, 0x0, 0xf0, 0x10, 0x0, 0x1f, 0x38, 0xf, 0x3, 0xf, 0x38, 0x1f, 0x0, 0x0, 0x10, 0x60, 0xc0, 0x0, 0x80, 0x60, 0x10, 0x0, 0x0, 0x20, 0x18, 0xc, 0x3, 0x6, 0x18, 0x30, 0x0, 0x0, 0x10, 0x60, 0xc0, 0x0, 0xc0, 0x60, 0x10, 0x0, 0x0, 0x0, 0x0, 0x1, 0x3f, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x10, 0x10, 0x10, 0xd0, 0x70, 0x10, 0x0, 0x0, 0x20, 0x38, 0x2c, 0x23, 0x20, 0x20, 0x20, 0x0};
char DisplayDriver::nextStrings[2][14] = {"", ""};

void DisplayDriver::sendCmd(unsigned char cmd)
{
    Wire.beginTransmission(0x3c);
    Wire.write(0x00);
    Wire.write(cmd);
    Wire.sendTransmission();
    Wire.finish();
}

void DisplayDriver::init()
{
    // pinMode(37, OUTPUT); // set SDA + SCL pins as output
    // pinMode(38, OUTPUT);
    Wire.begin(I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_EXT, 400000);
    Wire.setOpMode(I2C_OP_MODE_DMA);
    delay(100);

    sendCmd(0x20); // modo de endereçamento
    sendCmd(0x01); // horizontal, 02 é page

    sendCmd(0x21); // range da coluna
    sendCmd(0x00); // inicial é 0
    sendCmd(0x7f); // final é 7f

    sendCmd(0x22); // range da pagina
    sendCmd(0x00); // inicial é 0
    sendCmd(0x01); // final é 7

    sendCmd(0x40); // start line 0

    sendCmd(0xc0); // com mode normal
    sendCmd(0xa0); // remap normal

    sendCmd(0x81); // contraste, acho que não precisa dele
    sendCmd(0xFF);

    sendCmd(0xAF);

    sendCmd(0x8D); //--set vcomh
    sendCmd(0x14); //0x20,0.77xVcc
    delay(50);
    Wire.onTransmitDone(triggerTransmission);

    Wire.beginTransmission(0x3c);
    Wire.write(0x00);
    Wire.write(0x20);
    Wire.write(0x00);
    Wire.write(0x21);
    Wire.write(0x00);
    Wire.write(0x7f);
    Wire.write(0x22);
    Wire.write(0);
    Wire.write(0x07);
    Wire.sendTransmission();
    Wire.finish();
}

void DisplayDriver::cls(char b)
{
    Wire.beginTransmission(0x3c);
    Wire.write(0x00);
    Wire.write(0x21);
    Wire.write(0x00);
    Wire.write(0x7f);

    Wire.write(0x22);
    Wire.write(0);
    Wire.write(0x07);

    Wire.sendTransmission();
    Wire.finish();
    Wire.beginTransmission(0x3c);
    Wire.write(0x40);
    for (unsigned int n = 0; n < 1024; n++)
    {
        Wire.write(0x0);
    }
    Wire.sendTransmission();
    Wire.finish();
}

void DisplayDriver::putScreen(const char *str1, const char *str2)
{
    strcpy(nextStrings[0], str1);
    strcpy(nextStrings[1], str2);
    triggerTransmission();
}

void DisplayDriver::putScreen(const char *str1, float str2)
{
    char f[9];
    snprintf(f, 9, "%.2f", str2);
    strcpy(nextStrings[0], str1);
    strcpy(nextStrings[1], f);
    triggerTransmission();
}

void DisplayDriver::blackLine()
{
    byte col = 0;
    while (col < 128)
    {
        Wire.write(0x0);
        ++col;
    }
}

void DisplayDriver::writeLine(char *s)
{
    for (byte page = 0; page < 2; ++page)
    {
        byte col = 0;
        for (; col < 9; ++col)
        {
            Wire.write(0x0); // 9 columns padding
        }

        byte index = 0;
        char c;
        while (*(s + index) != 0x00 && index < 15)
        {
            c = (*(s + index) - 0x2c);
            if (*(s + index) == 0x20)
            {
                for (char i = 0; i < 9; i++)
                {
                    Wire.write(0x0);
                    ++col;
                }
            }
            else
            {
                for (char i = 0; i < 9; i++)
                {
                    Wire.write(pgm_read_byte(&(font_bmap[18 * c + i + page * 9])));
                    ++col;
                }
            }
            ++index;
        }
        while (col < 128)
        {
            Wire.write(0x0);
            ++col;
        }
    }
}

unsigned int DisplayDriver::lastTransmit = 0;

void DisplayDriver::triggerTransmission()
{
    if (nextStrings[0][0] != 0 && nextStrings[1][0] != 0 && Wire.done())
    {
        transmitScreen();

        nextStrings[0][0] = 0;
        nextStrings[1][0] = 0;
    }
    else if (!Wire.done() && micros() - lastTransmit > 125000)
    {
        Serial.printf("stuck for %d micros\n", micros() - lastTransmit);
        Wire.resetBus();
    }
}

void DisplayDriver::transmitScreen()
{
    Wire.beginTransmission(0x3c);
    Wire.write(0x40);
    blackLine();
    writeLine(nextStrings[0]);
    blackLine();
    writeLine(nextStrings[1]);
    blackLine();
    blackLine();
    lastTransmit = micros();
    Wire.sendTransmission();
}
