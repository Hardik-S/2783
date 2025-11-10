#include "CharacterSelectionGrader.h"
#include "TranslateExercise.h"

Result CharacterSelectionGrader::grade(const QString& userAnswer, const Exercise* exercise) {
    // Cast to TranslateExercise to get correct answers
    const TranslateExercise* translateExercise = dynamic_cast<const TranslateExercise*>(exercise);
    if (!translateExercise) {
        return Result{false, 0, "Invalid exercise type for character selection."};
    }

    // Handle empty answer
    if (userAnswer.isEmpty()) {
        return Result{false, 0, "Please select all characters."};
    }

    // Parse user answer (remove semicolons)
    QString userSequence = parseCharacterSequence(userAnswer);

    // Get correct answers from exercise
    QStringList correctAnswers = translateExercise->getCorrectAnswers();

    // Check against all valid answers
    for (const QString& correctAnswer : correctAnswers) {
        if (userSequence == correctAnswer) {
            return Result{true, 10, "Correct!"};
        }
    }

    // No match found
    return Result{false, 0, "Incorrect sequence. Please try again."};
}

QString CharacterSelectionGrader::parseCharacterSequence(const QString& separatedSequence) const {
    // Split by semicolon and rejoin without separators
    QStringList characters = separatedSequence.split(";");
    return characters.join("");
}
