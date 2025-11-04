/**
 * Audio Engine Implementation
 */

#include "ogde/audio/AudioEngine.h"

namespace ogde {
namespace audio {

bool AudioEngine::initialize() {
    return true;
}

void AudioEngine::shutdown() {
}

void AudioEngine::playSound(const std::string& filename) {
}

void AudioEngine::playMusic(const std::string& filename, bool loop) {
}

void AudioEngine::stopAll() {
}

void AudioEngine::setMasterVolume(float volume) {
}

} // namespace audio
} // namespace ogde
