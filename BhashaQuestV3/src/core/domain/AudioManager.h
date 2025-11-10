#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <QString>
#include <QMediaPlayer>
#include <QAudioOutput>

/**
 * AudioManager - Singleton pattern for audio playback
 *
 * DESIGN PATTERN: Singleton Pattern (Creational)
 * Purpose: Ensures single instance of audio resource manager
 * Benefit: Centralized audio control, prevents multiple QMediaPlayer instances
 *
 * Usage: AudioManager::getInstance().playSuccess()
 */
class AudioManager {
public:
    /**
     * Get the singleton instance
     * @return Reference to the single AudioManager instance
     */
    static AudioManager& getInstance();

    // Delete copy constructor and assignment operator (Singleton)
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    // Audio playback methods
    void playSuccess();
    void playError();
    void playAudio(const QString& filename);

private:
    // Private constructor (Singleton)
    AudioManager();
    ~AudioManager();

    // Singleton instance
    static AudioManager* instance;

    // Qt6 audio components
    QMediaPlayer* player;
    QAudioOutput* audioOutput;
};

#endif // AUDIOMANAGER_H
