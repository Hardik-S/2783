#ifndef CHARACTERSELECTIONGRADER_H
#define CHARACTERSELECTIONGRADER_H

#include "StrategyGrader.h"

/**
 * CharacterSelectionGrader - Grading strategy for Character Selection exercises
 *
 * DESIGN PATTERN: Strategy Pattern (Concrete Strategy)
 * Grading logic: Exact sequence matching
 *
 * Purpose:
 * Grades character selection exercises where users click characters in correct order
 * to form the correct translation.
 *
 * Input format:
 * - User answer: Semicolon-separated characters (e.g., "न;म;स")
 * - Correct answer: Direct string without separators (e.g., "नमस")
 *
 * Process:
 * 1. Parse user answer by splitting on semicolons
 * 2. Reconstruct user sequence (remove separators)
 * 3. Compare against correct answer
 * 4. Return exact match result
 *
 * Note: This grader is simpler than TranslateGrader because users cannot
 * make typos when selecting from a character bank.
 */
class CharacterSelectionGrader : public StrategyGrader {
public:
    /**
     * Grade character selection answer
     *
     * @param userAnswer - Semicolon-separated selected characters (e.g., "न;म;स")
     * @param exercise - The exercise being graded (must be TranslateExercise)
     * @return Result with correct flag, score, and feedback
     *
     * Process:
     * 1. Remove semicolon separators from user answer
     * 2. Get correct answer from exercise
     * 3. Compare strings for exact match
     * 4. Return result with 10 points if correct, 0 if incorrect
     */
    Result grade(const QString& userAnswer, const Exercise* exercise) override;

private:
    /**
     * Parse character sequence from semicolon-separated format
     *
     * @param separatedSequence Input like "न;म;स"
     * @return Reconstructed string like "नमस"
     */
    QString parseCharacterSequence(const QString& separatedSequence) const;
};

#endif // CHARACTERSELECTIONGRADER_H
