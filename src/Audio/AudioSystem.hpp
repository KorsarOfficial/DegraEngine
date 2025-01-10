#pragma once

#include <string>
#include <fmod.hpp>

class AudioSystem {
public:
    AudioSystem();
    ~AudioSystem();

    bool Initialize();
    void Update();
    void Shutdown();

    void PlaySound(const std::string& filename);
    void StopAllSounds();

private:
    FMOD::System* m_System;
    static const int MAX_CHANNELS = 32;
}; 