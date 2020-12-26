#include "AudioStream.h"

#ifndef DCRemover_h
#define DCRemover_h

class DCRemover : public AudioStream
{
public:
    DCRemover() : AudioStream(1, inputQueueArray) {}

    void update()
    {
        audio_block_t *block;
        block = receiveWritable();
        if (!block)
            return;
        uint32_t *p, *end;
        p = (uint32_t *)(block->data);
        // uint32_t tmp;
        end = p + AUDIO_BLOCK_SAMPLES / 2;
        while (p < end)
        {
            lastY = (*p + multiply_32x32_rshift32(2145336164, lastY)) - lastX; // 0.999
            lastX = *p;

            *p++ = (int16_t)(lastY / (1 << (16)));
        }
        transmit(block);
        release(block);
    }

private:
    audio_block_t *inputQueueArray[1];
    uint32_t lastY = 0, lastX = 0;
};
#endif