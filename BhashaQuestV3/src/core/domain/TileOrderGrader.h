#ifndef TILEORDERGRADER_H
#define TILEORDERGRADER_H

#include "StrategyGrader.h"
#include "TileOrderExercise.h"

/**
 * TileOrderGrader - Grading strategy for Tile Ordering exercises
 *
 * DESIGN PATTERN: Strategy Pattern (Concrete Strategy)
 * Grading logic: Sequence matching (exact order required)
 */
class TileOrderGrader : public StrategyGrader {
public:
    /**
     * Grade tile order answer
     * @param userAnswer - User's tile sequence as space-separated string
     * @param exercise - The TileOrder exercise being graded
     * @return Result with correct flag, score, and feedback
     */
    Result grade(const QString& userAnswer, const Exercise* exercise) override;
};

#endif // TILEORDERGRADER_H
