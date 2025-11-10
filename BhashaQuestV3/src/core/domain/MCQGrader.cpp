#include "MCQGrader.h"

Result MCQGrader::grade(const QString& userAnswer, const Exercise* exercise) {
    // Cast to MCQExercise to access MCQ-specific methods
    const MCQExercise* mcq = dynamic_cast<const MCQExercise*>(exercise);

    if (!mcq) {
        return Result(false, 0, "Error: Invalid exercise type for MCQGrader");
    }

    // Convert user answer to integer index
    bool ok;
    int userIndex = userAnswer.toInt(&ok);

    if (!ok) {
        return Result(false, 0, "Error: Invalid answer format");
    }

    // Check if answer is correct
    int correctIndex = mcq->getCorrectIndex();
    bool correct = (userIndex == correctIndex);

    if (correct) {
        return Result(true, 10, QString("Correct! The answer is: %1")
                      .arg(mcq->getCorrectAnswer()));
    } else {
        return Result(false, 0, QString("Incorrect. The correct answer is: %1")
                      .arg(mcq->getCorrectAnswer()));
    }
}
