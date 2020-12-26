#include <vector>
#include <Audio.h> // tirar esse import generico e importar só o que precisa
#include "Envelope.h"
#include "ValueInterface.h"
#include "Constants.h"
#include "waveform.h"
#include "DCRemover.h"
#define SPECIAL_WF 100

struct OscillatorState_t
{
    float semitone;
    float detune;
    char note;
    float amplitude;
    float wf;

    void setState(char _note, float _semitone, float _detune, float _wf, float _amplitude = 0)
    {
        note = _note;
        semitone = _semitone;
        detune = _detune;
        amplitude = _amplitude;
        wf = _wf;
    }
};

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
        AudioMemory(130);
        sgtl5000_1.enable();
        sgtl5000_1.volume(0.6);
        patchCord1 = new AudioConnection(waveform1, 0, mixer1, 0);
        patchCord2 = new AudioConnection(waveform2, 0, mixer1, 1);

        patchCord3 = new AudioConnection(mixer1, 0, filter1, 0);
        patchCord4 = new AudioConnection(fltEnv, 0, filterModulation, 0);
        patchCord4 = new AudioConnection(LFO2, 0, filterModulation, 1);
        patchCord5 = new AudioConnection(filterModulation, 0, filter1, 1);

        patchCord6 = new AudioConnection(filter1, 0, mixer2, 0);
        patchCord7 = new AudioConnection(filter1, 1, mixer2, 1);
        patchCord8 = new AudioConnection(filter1, 2, mixer2, 2);

        patchCord9 = new AudioConnection(mixer2, 0, multiply1, 0);
        patchCord10 = new AudioConnection(ampEnv, 0, multiply1, 1);

        patchCord11 = new AudioConnection(multiply1, distortion);
        patchCord12 = new AudioConnection(distortion, 0, delayMixer, 0);

        patchCord13 = new AudioConnection(distortion, 0, delayDryWetMixer, 0);
        patchCord14 = new AudioConnection(delayMixer, delayFx);
        patchCord15 = new AudioConnection(delayFx, 0, delayMixer, 1);
        patchCord16 = new AudioConnection(delayFx, 0, delayDryWetMixer, 1);

        patchCord17 = new AudioConnection(delayDryWetMixer, dcRemover);
        patchCord17 = new AudioConnection(dcRemover, 0, amFx, 0);
        patchCord18 = new AudioConnection(LFO, 0, amFx, 1);

        patchCord18 = new AudioConnection(amFx, 0, AMMixer, 0);
        patchCord18 = new AudioConnection(delayDryWetMixer, 0, AMMixer, 1);

        patchCord18 = new AudioConnection(AMMixer, outputGain);

        patchCord19 = new AudioConnection(outputGain, 0, i2s1, 0);
        patchCord20 = new AudioConnection(outputGain, 0, i2s1, 1);

        patchCord21 = new AudioConnection(outputGain, 0, usbOut, 0);
        patchCord22 = new AudioConnection(outputGain, 0, usbOut, 1);

        waveform1.begin(1.0, 440, WAVEFORM_SAWTOOTH);

        waveform2.begin(0, 440, WAVEFORM_SAWTOOTH);
        filter1.frequency(100);
        filter1.resonance(1.6);
        filter1.octaveControl(3);

        outputGain.gain(0.6);
        changeFilterType(0);

        delayFx.delay(0, 200);
        delayMixer.gain(1, 0.7);
        LFO.frequency(1720);

        setAMDryWet(0);
        filterModulation.gain(1, 0);
    }

    Envelope ampEnv;
    Envelope fltEnv;

    void changeWaveform1Frequency(byte note, float semitones, float detune)
    {

        float frequency = pow(2.0, (note - 69.0) / 12.0) * 440.0 * pow(NOTE_INTERVAL, semitones + detune);
        waveform1.frequency(frequency);
    }

    void changeWaveform2Frequency(byte note, float semitones, float detune) // pelo amor de deus faz ser uma função só essa e a de cima
    {
        float frequency = pow(2.0, (note - 69.0) / 12.0) * 440.0 * pow(NOTE_INTERVAL, semitones + detune);
        waveform2.frequency(frequency);
    }

    void noteOn(byte channel, byte note, byte velocity)
    {
        // AudioNoInterrupts();
        changeWaveform1Frequency(note, wf1State.semitone, wf1State.detune);
        changeWaveform2Frequency(note, wf2State.semitone, wf2State.detune);

        ampEnv.noteOn();
        waveform1.reset_phase();
        waveform2.reset_phase();

        // Serial.println("filter note on");
        fltEnv.noteOn();
        // AudioInterrupts();
    }
    void noteOff()
    {
        ampEnv.noteOff();
        fltEnv.noteOff();
    }
    void updateWaveform1Params(char note, float semitones, float detune, char wf)
    {
        // Serial.printf("note: %d, semitones: %f, detune: %f, wf: %f\n", note, semitones, detune, wf);
        wf1State.setState(note, semitones, detune, wf, 1);
        switch (wf)
        {
        case 0:
            waveform1.begin(WAVEFORM_TRIANGLE);
            break;
        case 1:
            waveform1.begin(WAVEFORM_SAWTOOTH);
            break;
        case 2:
            waveform1.begin(WAVEFORM_SQUARE);
            break;
        case 3:
            waveform1.begin(SPECIAL_WF);
            break;
        default:
            break;
        }
        lastNote = note == 0 ? lastNote : note;
        changeWaveform1Frequency(lastNote, semitones, detune);
    }

    void updateWaveform2Params(char note, float semitones, float detune, char wf, float amplitude)
    {
        wf2State.setState(note, semitones, detune, wf, amplitude);
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
        changeWaveform2Frequency(lastNote, semitones, detune);
        waveform2.amplitude(amplitude);
    }

    void changeFilterType(byte t)
    {
        mixer2.gain(0, (float)(t == 0));
        mixer2.gain(1, (float)(t == 1));
        mixer2.gain(2, (float)(t == 2));
    }

    void setDelayDryWet(float amount)
    {
        delayDryWetMixer.gain(0, 1 - amount);
        delayDryWetMixer.gain(1, amount);
    }

    void setAMDryWet(float amount)
    {
        AMMixer.gain(0, amount);
        AMMixer.gain(1, 1 - amount);
    }

    void updateFilterParams(float frequency, float type, float resonance, float modAmount)
    {
        // Serial.printf("frequency: %f, type %f, resonance: %f, modAmount: %f\n", frequency, type, resonance, modAmount);
        filter1.frequency(frequency);

        byte t = (char)(type);
        changeFilterType(t);

        filter1.resonance(resonance);
        filterModulation.gain(0, modAmount);
        // filter1.octaveControl(modAmount);
    }

    void updateFilterEvelopeParams(float _attack, float _decay, float _sustain, float _release)
    {
        fltEnv.setCoefficients(_decay, 0.001, _attack, _sustain, _release);
    }

    void updateAmpEvelopeParams(float _attack, float _decay, float _sustain, float _release)
    {
        ampEnv.setCoefficients(_decay, 0.001, _attack, _sustain, _release);
    }

    void updateEffectsParams(float dist, float delayTime, float delayDryWet, float delayFeedback)
    {
        setupWaveShaper(distCurve, 129, dist); // coisas da distorção
        distortion.shape(distCurve, 129);

        setDelayDryWet(delayDryWet);       // dry wet
        delayFx.delay(0, (int)delayTime);  // tempo de delay (tamanho do buffer de delay)
        delayMixer.gain(1, delayFeedback); // multiplicador do sinal de feedback
    }

    void updateLFOParams(float frequency, float amount, float frequency2, float amount2)
    {
        // Serial.printf("f = %f; amount = %f\n", frequency, amount);
        LFO.frequency(frequency);
        LFO2.frequency(frequency2);
        filterModulation.gain(1, amount2);
        setAMDryWet(amount);
    }

    void updateInfraParams(float volume)
    {
        // Serial.println(volume);
        outputGain.gain(volume);
    }

private:
    OscillatorState_t wf1State;
    OscillatorState_t wf2State;
    SynthWaveform waveform1;
    SynthWaveform waveform2;
    AudioMixer4 mixer1;
    AudioMixer4 delayMixer;
    AudioFilterStateVariable filter1;
    AudioEffectMultiply multiply1;
    AudioMixer4 mixer2;
    AudioMixer4 delayDryWetMixer;
    AudioOutputI2S i2s1;
    AudioOutputUSB usbOut;
    AudioAmplifier outputGain;
    DCRemover dcRemover;
    AudioEffectWaveshaper distortion;
    AudioEffectDelay delayFx;
    AudioEffectMultiply amFx;
    AudioSynthWaveformSine LFO;
    AudioSynthWaveformSine LFO2;
    AudioMixer4 filterModulation;
    AudioMixer4 AMMixer;
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
    AudioConnection *patchCord15;
    AudioConnection *patchCord16;
    AudioConnection *patchCord17;
    AudioConnection *patchCord18;
    AudioConnection *patchCord19;
    AudioConnection *patchCord20;
    AudioConnection *patchCord21;
    AudioConnection *patchCord22;
    AudioControlSGTL5000 sgtl5000_1;
};
