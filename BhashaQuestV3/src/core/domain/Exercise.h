#ifndef EXERCISE_H
#define EXERCISE_H

#include <QString>
#include <QStringList>
#include <QWidget>

/**
 * Exercise - Abstract base class for all exercise types
 *
 * This class defines the interface that all concrete exercise types must implement.
 * Derived classes: MCQExercise, TranslateExercise, TileOrderExercise
 */
class Exercise {
protected:
    QString id;                 // Unique identifier for this exercise
    QString type;               // Exercise type ("MCQ", "Translate", "TileOrder")
    QString prompt;             // Question or instruction text
    QString skillId;            // Associated skill ID
    int difficulty;             // Difficulty level (1-5)
    QString audioFile;          // Optional audio file for pronunciation

public:
    virtual ~Exercise() = default;

    // Pure virtual methods - must be implemented by derived classes

    /**
     * Get the exercise prompt/question
     */
    virtual QString getPrompt() const = 0;

    /**
     * Get the correct answer as a string
     */
    virtual QString getCorrectAnswer() const = 0;

    /**
     * Get the exercise type identifier
     */
    virtual QString getType() const = 0;

    /**
     * Render UI elements for this exercise type
     * Returns a widget containing the exercise UI
     */
    virtual QWidget* renderUI(QWidget* parent = nullptr) = 0;

    // Getters
    QString getId() const { return id; }
    QString getSkillId() const { return skillId; }
    int getDifficulty() const { return difficulty; }
    QString getAudioFile() const { return audioFile; }

    // Setters
    void setId(const QString& newId) { id = newId; }
    void setSkillId(const QString& newSkillId) { skillId = newSkillId; }
    void setDifficulty(int newDifficulty) { difficulty = newDifficulty; }
    void setAudioFile(const QString& newAudioFile) { audioFile = newAudioFile; }
};

#endif // EXERCISE_H
