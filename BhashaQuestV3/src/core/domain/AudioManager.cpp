#include "AudioManager.h"
#include <QUrl>
#include <QDebug>
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>

// Initialize static instance to nullptr
AudioManager* AudioManager::instance = nullptr;

AudioManager::AudioManager() {
    // Initialize Qt6 audio components
    player = new QMediaPlayer();
    audioOutput = new QAudioOutput();
    player->setAudioOutput(audioOutput);

    // Set default volume
    audioOutput->setVolume(0.5);
}

AudioManager::~AudioManager() {
    delete player;
    delete audioOutput;
}

AudioManager& AudioManager::getInstance() {
    // Singleton Pattern: Create instance on first access
    if (instance == nullptr) {
        instance = new AudioManager();
    }
    return *instance;
}

void AudioManager::playSuccess() {
    playAudio("assets/audio/success.wav");
}

void AudioManager::playError() {
    playAudio("assets/audio/error.wav");
}

void AudioManager::playAudio(const QString& filename) {
    // For D2: Log audio playback (actual audio files optional)
    qDebug() << "AudioManager: Playing" << filename;

    // Resolve file path relative to application directory
    // This ensures audio files work both in development and after deployment
    QString resolvedPath;

    // First, try relative to application directory (works after deployment)
    QString appDirPath = QCoreApplication::applicationDirPath() + "/" + filename;
    if (QFileInfo::exists(appDirPath)) {
        resolvedPath = appDirPath;
    } else {
        // Fallback: try relative to working directory (development mode)
        QString workingDirPath = QDir::current().absoluteFilePath(filename);
        if (QFileInfo::exists(workingDirPath)) {
            resolvedPath = workingDirPath;
        } else {
            qDebug() << "AudioManager: Warning - Audio file not found at" << appDirPath << "or" << workingDirPath;
            resolvedPath = filename; // Use original path as last resort
        }
    }

    // Attempt to play audio file
    QUrl audioUrl = QUrl::fromLocalFile(resolvedPath);
    qDebug() << "AudioManager: Resolved path:" << resolvedPath;
    player->setSource(audioUrl);
    player->play();

    // Note: In Qt6, QMediaPlayer::play() is asynchronous
    // For production, should connect to mediaStatusChanged signal to handle errors
}
