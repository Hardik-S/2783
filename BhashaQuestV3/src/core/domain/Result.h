#ifndef RESULT_H
#define RESULT_H

#include <QString>

/**
 * Result struct - Holds the outcome of grading an exercise
 * Used by all StrategyGrader implementations
 */
struct Result {
    bool correct;           // Whether the answer was correct
    int score;              // Points earned (typically 0 or 10)
    QString feedback;       // Feedback message to display to user

    // Constructor for convenience
    Result(bool correct = false, int score = 0, const QString& feedback = "")
        : correct(correct), score(score), feedback(feedback) {}
};

#endif // RESULT_H
