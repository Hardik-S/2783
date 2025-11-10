#include "CharacterUtils.h"
#include <QRandomGenerator>
#include <QDebug>
#include <algorithm>
#include <stdexcept>

QStringList CharacterUtils::extractAllCharacters(const QString& text) {
    QStringList characters;

    // Iterate through text and extract each character
    for (const QChar& ch : text) {
        // Skip spaces and special formatting characters
        if (!ch.isSpace()) {
            characters.append(QString(ch));
        }
    }

    return characters;
}

QStringList CharacterUtils::extractUniqueCharacters(const QString& text) {
    QStringList allChars = extractAllCharacters(text);
    QStringList uniqueChars;

    // Add each character only if not already in the list
    for (const QString& ch : allChars) {
        if (!uniqueChars.contains(ch)) {
            uniqueChars.append(ch);
        }
    }

    return uniqueChars;
}

QStringList CharacterUtils::getRandomDistractors(
    const QString& sourceText,
    const QStringList& languageCharacterSet,
    int count) {

    QStringList sourceCharacters = extractUniqueCharacters(sourceText);
    QStringList availableDistractors;

    // Build list of characters NOT in source text
    for (const QString& ch : languageCharacterSet) {
        if (!sourceCharacters.contains(ch)) {
            availableDistractors.append(ch);
        }
    }

    QStringList selectedDistractors;
    QRandomGenerator generator = QRandomGenerator::securelySeeded();

    // Select random distractor characters
    for (int i = 0; i < count && availableDistractors.length() > 0; ++i) {
        int randomIndex = generator.bounded(availableDistractors.length());
        QString distractor = availableDistractors.takeAt(randomIndex);
        selectedDistractors.append(distractor);
    }

    return selectedDistractors;
}

QStringList CharacterUtils::generateCharacterBank(
    const QString& correctAnswer,
    const QStringList& languageCharacterSet) {

    // Validate inputs
    if (correctAnswer.isEmpty() || languageCharacterSet.isEmpty()) {
        qWarning() << "CharacterUtils: Invalid inputs to generateCharacterBank"
                  << "correctAnswer empty:" << correctAnswer.isEmpty()
                  << "characterSet empty:" << languageCharacterSet.isEmpty();
        return QStringList();
    }

    // Extract unique characters from correct answer
    QStringList correctCharacters = extractUniqueCharacters(correctAnswer);
    if (correctCharacters.isEmpty()) {
        qWarning() << "CharacterUtils: No characters extracted from correct answer:" << correctAnswer;
        return QStringList();
    }

    int n = correctCharacters.length();

    // Get 3 random distractor characters
    QStringList distractors = getRandomDistractors(
        correctAnswer,
        languageCharacterSet,
        3
    );

    // Combine correct characters and distractors
    QStringList characterBank = correctCharacters + distractors;

    if (characterBank.isEmpty()) {
        qWarning() << "CharacterUtils: Character bank is empty after combining characters";
        return QStringList();
    }

    // Shuffle the character bank
    try {
        std::shuffle(characterBank.begin(), characterBank.end(),
                     QRandomGenerator::securelySeeded());
    } catch (const std::exception& e) {
        qWarning() << "CharacterUtils: Exception during shuffle:" << QString::fromStdString(e.what());
        return characterBank; // Return unshuffled but valid bank
    }

    return characterBank;
}
