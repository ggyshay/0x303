#include <vector>
#include <Audio.h> // tirar esse import generico e importar só o que precisa
#include "Envelope2.h"
#include "ValueInterface.h"
#include "Constants.h"
#include "waveform.h"
#define SPECIAL_WF 100

void setupWaveShaper(float *curve, int buffer_length, float amount)
{
    float deg = PI / 180.0f;
    float x;
    for (int i = 0; i < buffer_length; ++i)
    {
        x = (float)i * 2.0f / buffer_length - 1.0f;
        curve[i] = (3.0 + amount) * x * 20.0 * deg / (PI + amount * abs(x));
    }
}

class AudioEngine
{
public:
    void begin()
    {
        AudioMemory(60);
        sgtl5000_1.enable();
        sgtl5000_1.volume(0.3);
        patchCord1 = new AudioConnection(waveform1, 0, mixer1, 0);
        patchCord2 = new AudioConnection(waveform2, 0, mixer1, 1);

        patchCord3 = new AudioConnection(mixer1, 0, filter1, 0);

        patchCord4 = new AudioConnection(filter1, 0, mixer2, 0);
        patchCord5 = new AudioConnection(filter1, 1, mixer2, 1);
        patchCord6 = new AudioConnection(filter1, 2, mixer2, 2);

        patchCord7 = new AudioConnection(mixer2, 0, multiply1, 0);
        patchCord8 = new AudioConnection(ampEnv, 0, multiply1, 1);

        patchCord9 = new AudioConnection(multiply1, distortion);
        patchCord10 = new AudioConnection(distortion, outputGain);

        patchCord10 = new AudioConnection(outputGain, 0, i2s1, 0);
        patchCord11 = new AudioConnection(outputGain, 0, i2s1, 1);

        patchCord12 = new AudioConnection(outputGain, 0, usbOut, 0);
        patchCord13 = new AudioConnection(outputGain, 0, usbOut, 1);

        waveform1.begin(1.0, 440, WAVEFORM_SAWTOOTH);

        waveform2.begin(0, 440, WAVEFORM_SAWTOOTH);
        filter1.frequency(100);
        filter1.resonance(1.6);
        filter1.octaveControl(5);

        outputGain.gain(0.6);
        changeFilterType(0);
        createParamLists();

        updateAmpEvelopeParams();
        updateFilterEvelopeParams();
        updateWaveform1Params();
        updateWaveform2Params();
        updateFilterParams();
    }

    std::vector<Value> *getParamList(byte index)
    {
        return &(paramLists[index]);
    }

    void setChanged(byte index)
    {
        switch (index)
        {
        case 0:
            updateWaveform1Params();
            break;
        case 1:
            updateWaveform2Params();
            break;
        case 2:
            updateFilterParams();
            break;
        case 3:
            updateFilterEvelopeParams();
            break;
        case 4:
            updateAmpEvelopeParams();
            break;
        case 5:
            updateEffectsParams();
            break;
        default:
            break;
        }
    }

    Envelope ampEnv;
    Envelope fltEnv;

    void changeWaveform1Frequency(byte note)
    {

        float frequency = pow(2.0, (note - 69.0) / 12.0) * 440.0 * pow(NOTE_INTERVAL, paramLists[0][0].value + paramLists[0][1].value);
        waveform1.frequency(frequency);
    }

    void changeWaveform2Frequency(byte note) // pelo amor de deus faz ser uma função só essa e a de cima
    {
        float frequency = pow(2.0, (note - 69.0) / 12.0) * 440.0 * pow(NOTE_INTERVAL, paramLists[1][0].value + paramLists[1][1].value);
        waveform2.frequency(frequency);
    }

    void noteOn(byte channel, byte note, byte velocity)
    {
        AudioNoInterrupts();
        changeWaveform1Frequency(note);
        changeWaveform2Frequency(note);

        ampEnv.noteOn();
        waveform1.reset_phase();
        waveform2.reset_phase();

        fltEnv.noteOn();
        AudioInterrupts();
    }
    void noteOff(byte channel, byte note, byte velocity)
    {
        ampEnv.noteOff();
        fltEnv.noteOff();
    }

private:
    SynthWaveform waveform1;
    SynthWaveform waveform2;
    AudioMixer4 mixer1;
    AudioFilterStateVariable filter1;
    AudioEffectMultiply multiply1;
    AudioMixer4 mixer2;
    AudioOutputI2S i2s1;
    AudioOutputUSB usbOut;
    AudioAmplifier outputGain;
    AudioEffectWaveshaper distortion;
    float distCurve[129];
    int lastNote = 69;

    AudioConnection *patchCord1;
    AudioConnection *patchCord2;
    AudioConnection *patchCord3;
    AudioConnection *patchCord4;
    AudioConnection *patchCord5;
    AudioConnection *patchCord6;
    AudioConnection *patchCord7;
    AudioConnection *patchCord8;
    AudioConnection *patchCord9;
    AudioConnection *patchCord10;
    AudioConnection *patchCord11;
    AudioConnection *patchCord12;
    AudioConnection *patchCord13;
    AudioConnection *patchCord14;
    AudioControlSGTL5000 sgtl5000_1;

    void updateWaveform1Params()
    {
        byte i = (char)(paramLists[0][2].value);
        switch (i)
        {
        case 0:
        {
            waveform1.begin(WAVEFORM_TRIANGLE);
            break;
        }
        case 1:
        {
            waveform1.begin(WAVEFORM_SAWTOOTH);
            break;
        }
        case 2:
        {
            waveform1.begin(WAVEFORM_SQUARE);
            break;
        }
        case 3:
        {
            waveform1.begin(SPECIAL_WF);
            break;
        }
        default:
            break;
        }
        changeWaveform1Frequency(lastNote);
    }

    void updateWaveform2Params(char wf)
    {
        switch (wf)
        {
        case 0:
            waveform2.begin(WAVEFORM_TRIANGLE);
            break;
        case 1:
            waveform2.begin(WAVEFORM_SAWTOOTH);
            break;
        case 2:
            waveform2.begin(WAVEFORM_SQUARE);
            break;
        case 3:
            waveform2.begin(SPECIAL_WF);
        default:
            break;
        }
        changeWaveform2Frequency(lastNote);
        Serial.println(paramLists[1][3].value);
        waveform2.amplitude(paramLists[1][3].value);
    }

    void changeFilterType(byte t)
    {
        mixer2.gain(0, (float)(t == 0));
        mixer2.gain(1, (float)(t == 1));
        mixer2.gain(2, (float)(t == 2));
    }

    void updateFilterParams()
    {
        filter1.frequency(paramLists[2][0].value);

        byte t = (char)(paramLists[2][1].value);
        changeFilterType(t);

        filter1.resonance(paramLists[2][2].value);
        filter1.octaveControl(paramLists[2][3].value);
    }

    void updateFilterEvelopeParams()
    {
        fltEnv.setCoefficients(paramLists[3][1].value, 0.001, paramLists[3][0].value, paramLists[3][2].value, paramLists[3][3].value);
    }

    void updateAmpEvelopeParams()
    {
        ampEnv.setCoefficients(paramLists[4][1].value, 0.001, paramLists[4][0].value, paramLists[4][2].value, paramLists[4][3].value);
    }

    void updateEffectsParams()
    {
        setupWaveShaper(distCurve, 129, paramLists[5][0].value);
        distortion.shape(distCurve, 129);
    }
};
