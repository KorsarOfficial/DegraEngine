#pragma once
#include <string>

#ifdef USE_FMOD
#include <fmod.hpp>
#endif

class AudioSystem {
public:
    AudioSystem();
    ~AudioSystem();

    bool Initialize();
    void Update();
    void Shutdown();

    // Базовые функции для работы со звуком
    bool LoadSound(const std::string& filename);
    void PlaySound(const std::string& filename);
    void StopSound(const std::string& filename);
    void SetVolume(float volume);

private:
    bool m_Initialized;
    float m_MasterVolume;

#ifdef USE_FMOD
    FMOD::System* m_System;
#endif
}; 