#include "TileOrderGrader.h"
#include <QDebug>

Result TileOrderGrader::grade(const QString& userAnswer, const Exercise* exercise) {
    // Cast to TileOrderExercise to access TileOrder-specific methods
    const TileOrderExercise* tileEx = dynamic_cast<const TileOrderExercise*>(exercise);

    if (!tileEx) {
        return Result(false, 0, "Error: Invalid exercise type for TileOrderGrader");
    }

    QString trimmedAnswer = userAnswer.trimmed();

    if (trimmedAnswer.isEmpty()) {
        return Result(false, 0, "Please arrange the tiles to form your answer");
    }

    // Convert user answer from semicolon-separated to space-separated
    QStringList userTiles = trimmedAnswer.split(";", Qt::SkipEmptyParts);
    QString userOrderSpaced = userTiles.join(" ");

    // Get correct order as space-separated string
    QString correctAnswer = tileEx->getCorrectAnswer();

    // Compare user answer with correct answer
    bool correct = (userOrderSpaced == correctAnswer);

    if (correct) {
        return Result(true, 10, "Perfect! You arranged the words correctly!");
    } else {
        // Provide helpful feedback showing the correct order
        QStringList correctTiles = correctAnswer.split(" ");
        QString formattedCorrect = correctTiles.join(" → ");
        return Result(false, 0, QString("Not quite right. Correct order: %1").arg(formattedCorrect));
    }
}
