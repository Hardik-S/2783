#ifndef TRANSLATEEXERCISE_H
#define TRANSLATEEXERCISE_H

#include "Exercise.h"
#include <QStringList>

/**
 * TranslateExercise - Translation exercise type
 *
 * Presents an English phrase and expects translation to target language
 * Accepts multiple valid answers (e.g., both native script and romanization)
 * Example: "Translate: water" → "पानी" or "pani"
 */
class TranslateExercise : public Exercise {
private:
    QString englishPhrase;              // Phrase to translate
    QStringList correctAnswers;         // Multiple valid answers (native + romanization)
    QString targetLanguage;             // Target language name
    QStringList characterSet;           // Available characters for script entry
    bool characterSelectionEnabled;     // Flag to show character picker

public:
    TranslateExercise();
    TranslateExercise(const QString& id, const QString& englishPhrase,
                      const QStringList& correctAnswers, const QString& targetLanguage);

    // Override pure virtual methods from Exercise
    QString getPrompt() const override;
    QString getCorrectAnswer() const override;
    QString getType() const override;
    QWidget* renderUI(QWidget* parent = nullptr) override;

    // Translate-specific methods
    QString getEnglishPhrase() const { return englishPhrase; }
    QStringList getCorrectAnswers() const { return correctAnswers; }
    QString getTargetLanguage() const { return targetLanguage; }

    void setEnglishPhrase(const QString& phrase) { englishPhrase = phrase; }
    void setCorrectAnswers(const QStringList& answers) { correctAnswers = answers; }
    void setTargetLanguage(const QString& language) { targetLanguage = language; }
    void setPrompt(const QString& newPrompt) { prompt = newPrompt; }
    void setCharacterSet(const QStringList& characters);
    QStringList getCharacterSet() const { return characterSet; }
    void enableCharacterSelection(bool enabled);
    bool usesCharacterSelection() const { return characterSelectionEnabled; }
};

#endif // TRANSLATEEXERCISE_H
