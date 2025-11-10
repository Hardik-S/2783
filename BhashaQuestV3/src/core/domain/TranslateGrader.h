#ifndef TRANSLATEGRADER_H
#define TRANSLATEGRADER_H

#include "StrategyGrader.h"
#include "TranslateExercise.h"

/**
 * TranslateGrader - Grading strategy for Translation exercises
 *
 * DESIGN PATTERN: Strategy Pattern (Concrete Strategy)
 * Grading logic: Fuzzy matching with multiple valid answers
 * - Accepts native script or romanization
 * - Case-insensitive for romanization
 * - Allows minor typos (Levenshtein distance <= 2)
 */
class TranslateGrader : public StrategyGrader {
public:
    /**
     * Grade translation answer
     * @param userAnswer - User's translation
     * @param exercise - The Translate exercise being graded
     * @return Result with correct flag, score, and feedback
     */
    Result grade(const QString& userAnswer, const Exercise* exercise) override;

private:
    /**
     * Calculate Levenshtein distance between two strings
     * Used for fuzzy matching to allow minor typos
     */
    int levenshteinDistance(const QString& s1, const QString& s2) const;

    /**
     * Check if user answer matches any correct answer (with fuzzy matching)
     */
    bool fuzzyMatch(const QString& userAnswer, const QStringList& correctAnswers) const;
};

#endif // TRANSLATEGRADER_H
