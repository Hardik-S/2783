#ifndef CONTENTREPOSITORY_H
#define CONTENTREPOSITORY_H

#include <QString>
#include <QList>
#include <QMap>
#include <QJsonObject>
#include "../domain/Exercise.h"

/**
 * ContentRepository - Data Layer for loading and managing exercise content
 *
 * ARCHITECTURE: Data Layer
 * Purpose: Load exercises from JSON file, manage content lifecycle
 *
 * Key Responsibilities:
 * 1. Load and parse content.json file
 * 2. Create Exercise instances via ExerciseFactory (Factory Pattern)
 * 3. Provide access to exercises by skill ID
 * 4. Cache loaded content for performance
 *
 * Data Flow:
 * 1. ContentRepository::loadContent() reads content.json
 * 2. For each exercise in JSON: calls ExerciseFactory::createExercise()
 * 3. Stores exercises organized by skill ID
 * 4. AppController requests exercises via getExercisesForSkill()
 *
 * Design Principles:
 * - Separation of concerns: Data loading isolated from business logic
 * - Uses Factory Pattern for exercise creation
 * - Caches content to avoid repeated file I/O
 */
class ContentRepository {
private:
    // Skill data structure
    struct Skill {
        QString id;
        QString name;
        QString language;
        QList<Exercise*> exercises;
    };

    // Content cache
    QMap<QString, Skill> skills;           // skillId -> Skill data
    bool contentLoaded;                     // Track if content.json has been loaded
    QString contentFilePath;                // Path to content.json

public:
    /**
     * Constructor
     * @param contentFileName - Name of content file (default: "content.json")
     *                         File will be resolved relative to application directory
     */
    explicit ContentRepository(const QString& contentFileName = "content.json");

    /**
     * Destructor - cleans up all loaded exercises
     */
    ~ContentRepository();

    /**
     * Load content from JSON file
     * Parses JSON and creates Exercise instances via ExerciseFactory
     * @return true if loading succeeded, false on error
     */
    bool loadContent();

    /**
     * Get all exercises for a specific skill
     * @param skillId - The skill identifier
     * @return List of Exercise pointers (empty list if skill not found)
     * NOTE: Caller does NOT own the returned pointers - repository manages memory
     */
    QList<Exercise*> getExercisesForSkill(const QString& skillId) const;

    /**
     * Get all available skill IDs
     * @return List of skill identifiers
     */
    QList<QString> getAvailableSkills() const;

    /**
     * Get skill information
     * @param skillId - The skill identifier
     * @return Skill name (empty string if not found)
     */
    QString getSkillName(const QString& skillId) const;

    /**
     * Get skill language
     * @param skillId - The skill identifier
     * @return Language name (empty string if not found)
     */
    QString getSkillLanguage(const QString& skillId) const;

    /**
     * Check if content has been loaded
     * @return true if loadContent() has been called successfully
     */
    bool isLoaded() const;

    /**
     * Reload content from file
     * Clears existing content and reloads from JSON
     * @return true if reload succeeded
     */
    bool reload();

    /**
     * Get total number of exercises across all skills
     * @return Total exercise count
     */
    int getTotalExerciseCount() const;

private:
    /**
     * Parse a skill object from JSON
     * @param skillJson - JSON object representing a skill
     * @return Skill structure (exercises list may be empty on parse errors)
     */
    Skill parseSkill(const QJsonObject& skillJson);

    /**
     * Clear all loaded content
     * Deletes all Exercise instances and clears cache
     */
    void clearContent();
};

#endif // CONTENTREPOSITORY_H
