
#include "Arduino.h"

#define S0 4
#define S1 5
#define S2 6
#define E_A 17
#define E_B 20
#define E_C 21

void portSetup()
{
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(E_A, INPUT);
    pinMode(E_B, INPUT);
    pinMode(E_C, INPUT);
}

void sendBits(byte i)
{
    //4, 5, 6
    // digitalWrite(4, LOW);
    // digitalWrite(5, LOW);
    // digitalWrite(6, LOW);
    digitalWrite(S0, i & 0b001);
    digitalWrite(S1, i & 0b010);
    digitalWrite(S2, i & 0b100);
}

namespace std
{
void __throw_bad_alloc()
{
    Serial.println("Unable to allocate memory");
}

void __throw_length_error(char const *e)
{
    Serial.print("Length Error :");
    Serial.println(e);
}

void __throw_bad_function_call()
{
    Serial.println("Bad function call!");
}
} // namespace std

void print_cpu_memory_status(void)
{
    Serial.print("CPU: ");
    Serial.print(AudioProcessorUsage());
    Serial.print(",");
    Serial.print(AudioProcessorUsageMax());
    Serial.print("    ");
    Serial.print("Memory: ");
    Serial.print(AudioMemoryUsage());
    Serial.print(",");

    Serial.print(AudioMemoryUsageMax());
    Serial.println();
}

void print_i2c_status(void)
{
    switch (Wire1.status())
    {
    case I2C_WAITING:
        // Serial.print("I2C waiting, no errors\n");
        break;
    case I2C_ADDR_NAK:
        Serial.print("Slave addr not acknowledged\n");
        break;
    case I2C_DATA_NAK:
        Serial.print("Slave data not acknowledged\n");
        break;
    case I2C_ARB_LOST:
        Serial.print("Bus Error: Arbitration Lost\n");
        break;
    case I2C_TIMEOUT:
        Serial.print("I2C timeout\n");
        break;
    case I2C_BUF_OVF:
        Serial.print("I2C buffer overflow\n");
        break;
    case I2C_NOT_ACQ:
        Serial.print("I2C: Not acq\n");
        break;
    case I2C_DMA_ERR:
        Serial.print("DMA error\b");
        break;
    case I2C_SENDING:
        Serial.println("i2c sending");
        break;
    case I2C_SEND_ADDR:
        Serial.println("i2c send addr");
        break;
    case I2C_RECEIVING:
        Serial.println("i2c recieving");
        break;
    default:
        Serial.print("I2C busy\n");
        break;
    }
}