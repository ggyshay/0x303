#include <vector>
#include "ValueInterface.h"
#include "AudioEngine.h"

class AudioInfra
{
public:
    void begin()
    {
        createParamLists();
        audioEngine.begin();
        updateWaveform1Params();
        updateWaveform2Params();
        updateFilterParams();
        updateFilterEvelopeParams();
        updateAmpEvelopeParams();
        updateEffectsParams();
        audioEngine.noteOn(1, 20, 127);
    }

    std::vector<Value> *getParamList(byte index)
    {
        return &(paramLists[index]);
    }

    void updateWaveform1Params(char note = 0)
    {
        audioEngine.updateWaveform1Params(note, paramLists[0][0].value, paramLists[0][1].value, paramLists[0][2].value);
    }
    void updateWaveform2Params(char note = 0)
    {
        audioEngine.updateWaveform2Params(note, paramLists[1][0].value, paramLists[1][1].value, paramLists[1][2].value, paramLists[1][3].value);
    }
    void updateFilterParams()
    {
        audioEngine.updateFilterParams(paramLists[2][0].value, paramLists[2][1].value, paramLists[2][2].value, paramLists[2][3].value);
    }
    void updateFilterEvelopeParams()
    {
        audioEngine.updateFilterEvelopeParams(paramLists[3][0].value, paramLists[3][1].value, paramLists[3][2].value, paramLists[3][3].value);
    }
    void updateAmpEvelopeParams()
    {
        audioEngine.updateAmpEvelopeParams(paramLists[4][0].value, paramLists[4][1].value, paramLists[4][2].value, paramLists[4][3].value);
    }
    void updateEffectsParams()
    {
        audioEngine.updateEffectsParams(paramLists[5][0].value, paramLists[5][1].value, paramLists[5][2].value, paramLists[5][3].value);
    }

    void updateLFOParams()
    {
        audioEngine.updateLFOParams(paramLists[6][0].value, paramLists[6][1].value, paramLists[6][2].value, paramLists[6][3].value);
    }

    void updateInfraParams()
    {
        audioEngine.updateInfraParams(paramLists[7][0].value);
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
        case 6:
            updateLFOParams();
            break;
        case 7:
            updateInfraParams();
            break;
        default:
            break;
        }
    }

    std::vector<Value> paramLists[8]; // make it private

    void noteOn(byte channel, byte note, byte velocity)
    {
        audioEngine.noteOn(channel, note, velocity);
        updateWaveform1Params(note);
        updateWaveform2Params(note);
    }
    void noteOff(byte channel, byte note, byte velocity)
    {
        audioEngine.noteOff();
    }

private:
    AudioEngine audioEngine;
    void createParamLists()
    {
        //WAVEFORM1 -----------------------------------------------------
        Value wf1SemitoneValue(-48.0, 48.0, 0.0, "SEMITONE", 96);
        paramLists[0].push_back(wf1SemitoneValue);

        Value wf1Detune(-1.0, 1.0, 0.0, "DETUNE", 24);
        paramLists[0].push_back(wf1Detune);

        Value wf1Waveform(0.0, 3.0, 0.0, "WAVEFORM", 3);
        paramLists[0].push_back(wf1Waveform);

        //WAVEFORM2 -----------------------------------------------------
        Value wf2SemitoneValue(-48.0, 48.0, 0.0, "SEMITONE", 96);
        paramLists[1].push_back(wf2SemitoneValue);

        Value wf2Detune(-1.0, 1.0, 0.0, "DETUNE", 24);
        paramLists[1].push_back(wf2Detune);

        Value wf2Waveform(0.0, 3.0, 0.0, "WAVEFORM", 3);
        paramLists[1].push_back(wf2Waveform);

        Value wf2Level(0.0, 1.0, 0.0, "LEVEL", 15);
        paramLists[1].push_back(wf2Level);

        //FILTER -----------------------------------------------------
        Value fltFrequencyValue(55.0, 7040.0, 7000.0, "FREQUENCY", 144, true);
        paramLists[2].push_back(fltFrequencyValue);

        Value fltTypeValue(0.0, 2.0, 0.0, "TYPE", 2);
        paramLists[2].push_back(fltTypeValue);

        Value fltQValue(0.707, 5.0, 0.707, "RESONANCE", 20);
        paramLists[2].push_back(fltQValue);

        Value fltEnvModValue(0.0, 1.0, 0.5, "ENVMOD", 25);
        paramLists[2].push_back(fltEnvModValue);

        // FILTER ENVELOPE ----------------------------------------------
        Value fltEnvAttackValue(1.0, 200.0, 1.0, "ATTACK", 30, true);
        paramLists[3].push_back(fltEnvAttackValue);

        Value fltEnvDecayValue(1.0, 3000.0, 100.0, "DECAY", 70, true);
        paramLists[3].push_back(fltEnvDecayValue);

        Value fltEnvSustainValue(0.001, .99, 0.7, "SUSTAIN", 80);
        paramLists[3].push_back(fltEnvSustainValue);

        Value fltEnvReleaseValue(1.0, 3000.0, 100.0, "RELEASE", 70, true);
        paramLists[3].push_back(fltEnvReleaseValue);

        // AMP ENVELOPE --------------------------------------------------
        Value ampEnvAttackValue(1.0, 200.0, 1.0, "ATTACK", 30, true);
        paramLists[4].push_back(ampEnvAttackValue);

        Value ampEnvDecayValue(1.0, 3000.0, 100.0, "DECAY", 70, true);
        paramLists[4].push_back(ampEnvDecayValue);

        Value ampEnvSustainValue(0.001, 0.99, 0.7, "SUSTAIN", 80);
        paramLists[4].push_back(ampEnvSustainValue);

        Value ampEnvReleaseValue(1.0, 3000.0, 100.0, "RELEASE", 70, true);
        paramLists[4].push_back(ampEnvReleaseValue);

        //FX -------------------------------------------------------------
        Value fxDist(0, 100.0, 0.0, "DISTORTION", 100);
        paramLists[5].push_back(fxDist);

        Value fxDelayTime(0, 500.0, 120.0, "DELAY TIME", 100);
        paramLists[5].push_back(fxDelayTime);

        Value fxDelayDryWet(0, 1.0, 0.0, "DEL DRYWET", 100);
        paramLists[5].push_back(fxDelayDryWet);

        Value fxDelayFeedback(0, 1.0, 0.65, "DEL FEEDBACK", 100);
        paramLists[5].push_back(fxDelayFeedback);

        //LFO ------------------------------------------------------------
        Value lfoFrequency(1000.0, 1200.0, 1000.0, "AM FREQUENCY", 100, true);
        paramLists[6].push_back(lfoFrequency);

        Value lfoAmount(0.0, 1.0, 0.0, "AM AMOUNT", 100);
        paramLists[6].push_back(lfoAmount);

        Value lfo2Frequency(0.001, 200.0, 1.0, "FLT LFO", 100, true);
        paramLists[6].push_back(lfo2Frequency);

        Value lfo2Amount(0.0, 2.0, 0.0, "F LFO AMOUNT", 100);
        paramLists[6].push_back(lfo2Amount);

        //GLOBAL -------------------------------------------
        Value globalVolume(0, 1.0, 0.3, "VOLUME", 100);
        paramLists[7].push_back(globalVolume);
    }
};
