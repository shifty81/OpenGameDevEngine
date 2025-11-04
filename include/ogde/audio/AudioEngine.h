/**
 * @file AudioEngine.h
 * @brief Audio playback and management system
 */

#ifndef OGDE_AUDIO_AUDIOENGINE_H
#define OGDE_AUDIO_AUDIOENGINE_H

#include <string>

namespace ogde {
namespace audio {

/**
 * @class AudioEngine
 * @brief Main audio system for sound and music
 */
class AudioEngine {
public:
    /**
     * @brief Initialize the audio engine
     * @return true if initialization was successful
     */
    bool initialize();

    /**
     * @brief Shutdown the audio engine
     */
    void shutdown();

    /**
     * @brief Play a sound effect
     * @param filename Path to the sound file
     */
    void playSound(const std::string& filename);

    /**
     * @brief Play music
     * @param filename Path to the music file
     * @param loop Whether to loop the music
     */
    void playMusic(const std::string& filename, bool loop = true);

    /**
     * @brief Stop all audio
     */
    void stopAll();

    /**
     * @brief Set master volume
     * @param volume Volume level (0.0 to 1.0)
     */
    void setMasterVolume(float volume);
};

} // namespace audio
} // namespace ogde

#endif // OGDE_AUDIO_AUDIOENGINE_H
