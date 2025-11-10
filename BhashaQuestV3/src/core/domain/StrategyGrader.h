#ifndef STRATEGYGRADER_H
#define STRATEGYGRADER_H

#include "Result.h"
#include "Exercise.h"
#include <QString>

/**
 * StrategyGrader - Abstract interface for the Strategy Pattern
 *
 * This interface defines the grading contract that all concrete graders must implement.
 * Different exercise types use different grading strategies:
 * - MCQGrader: Exact index matching
 * - TranslateGrader: Fuzzy string matching with multiple valid answers
 * - TileOrderGrader: Sequence order matching
 *
 * DESIGN PATTERN: Strategy Pattern (Behavioral)
 * Purpose: Allows pluggable grading algorithms per exercise type
 */
class StrategyGrader {
public:
    virtual ~StrategyGrader() = default;

    /**
     * Grade a user's answer against the exercise
     * @param userAnswer - The user's submitted answer
     * @param exercise - The exercise being graded
     * @return Result struct with correct flag, score, and feedback
     */
    virtual Result grade(const QString& userAnswer, const Exercise* exercise) = 0;
};

#endif // STRATEGYGRADER_H
