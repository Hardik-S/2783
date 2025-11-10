#ifndef MCQGRADER_H
#define MCQGRADER_H

#include "StrategyGrader.h"
#include "MCQExercise.h"

/**
 * MCQGrader - Grading strategy for Multiple Choice Questions
 *
 * DESIGN PATTERN: Strategy Pattern (Concrete Strategy)
 * Grading logic: Exact index match (strict comparison)
 */
class MCQGrader : public StrategyGrader {
public:
    /**
     * Grade MCQ answer
     * @param userAnswer - Index of selected option as string (e.g., "0", "1", "2", "3")
     * @param exercise - The MCQ exercise being graded
     * @return Result with correct flag, score (10 if correct, 0 otherwise), and feedback
     */
    Result grade(const QString& userAnswer, const Exercise* exercise) override;
};

#endif // MCQGRADER_H
