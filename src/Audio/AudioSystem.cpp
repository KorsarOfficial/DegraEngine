#include "AudioSystem.hpp"
#include <stdexcept>

AudioSystem::AudioSystem() : m_System(nullptr) {}

AudioSystem::~AudioSystem() {
    Shutdown();
}

bool AudioSystem::Initialize() {
    FMOD_RESULT result;
    result = FMOD::System_Create(&m_System);
    if (result != FMOD_OK) {
        return false;
    }

    result = m_System->init(MAX_CHANNELS, FMOD_INIT_NORMAL, nullptr);
    if (result != FMOD_OK) {
        return false;
    }

    return true;
}

void AudioSystem::Update() {
    if (m_System) {
        m_System->update();
    }
}

void AudioSystem::Shutdown() {
    if (m_System) {
        m_System->release();
        m_System = nullptr;
    }
}

void AudioSystem::PlaySound(const std::string& filename) {
    if (!m_System) return;

    FMOD::Sound* sound = nullptr;
    FMOD::Channel* channel = nullptr;
    
    FMOD_RESULT result = m_System->createSound(filename.c_str(), FMOD_DEFAULT, nullptr, &sound);
    if (result == FMOD_OK) {
        m_System->playSound(sound, nullptr, false, &channel);
        sound->release(); // FMOD будет управлять памятью сам
    }
}

void AudioSystem::StopAllSounds() {
    if (!m_System) return;
    
    FMOD::Channel* channel = nullptr;
    for (int i = 0; i < MAX_CHANNELS; i++) {
        if (m_System->getChannel(i, &channel) == FMOD_OK && channel) {
            channel->stop();
        }
    }
} 