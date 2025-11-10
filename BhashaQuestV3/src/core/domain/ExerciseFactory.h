#ifndef EXERCISEFACTORY_H
#define EXERCISEFACTORY_H

#include "Exercise.h"
#include <QJsonObject>
#include <QString>

/**
 * ExerciseFactory - Factory Pattern implementation
 *
 * DESIGN PATTERN: Factory Pattern (Creational)
 * Purpose: Creates appropriate Exercise subclass instances based on type specification
 * Benefit: Encapsulates object creation logic, enabling extensibility for new exercise types
 *          without modifying client code
 *
 * Usage: ContentRepository calls createExercise() with JSON specification
 *        Factory determines type and instantiates appropriate concrete class
 */
class ExerciseFactory {
public:
    /**
     * Create an Exercise instance from JSON specification
     * @param type - Exercise type ("MCQ", "Translate", "TileOrder")
     * @param spec - JSON object containing exercise data
     * @return Pointer to created Exercise (caller owns memory)
     */
    static Exercise* createExercise(const QString& type, const QJsonObject& spec);

private:
    // Private helper methods for creating specific exercise types
    static Exercise* createMCQ(const QJsonObject& spec);
    static Exercise* createTranslate(const QJsonObject& spec);
    static Exercise* createTileOrder(const QJsonObject& spec);
};

#endif // EXERCISEFACTORY_H
