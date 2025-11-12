#ifndef CONTENTREPOSITORY_H
#define CONTENTREPOSITORY_H

#include <QString>
#include <QStringList>
#include <QList>
#include <QMap>
#include <QJsonObject>
#include <QSharedPointer>
#include "../domain/Exercise.h"
#include "../domain/ExerciseSequence.h"

using ExerciseSequencePtr = QSharedPointer<ExerciseSequence>;

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
 */
class ContentRepository {
private:
    struct Skill {
        QString id;
        QString name;
        QString language;
        QStringList characterSet;
        QList<Exercise*> exercises;
    };

    QMap<QString, Skill> skills;
    bool contentLoaded;
    QString contentFilePath;

public:
    explicit ContentRepository(const QString& contentFileName = "content.json");
    ~ContentRepository();

    bool loadContent();
    QList<Exercise*> getExercisesForSkill(const QString& skillId) const;
    QList<QString> getAvailableSkills() const;
    QString getSkillName(const QString& skillId) const;
    QString getSkillLanguage(const QString& skillId) const;
    bool isLoaded() const;
    bool reload();
    int getTotalExerciseCount() const;
    ExerciseSequencePtr createSequenceForSkill(const QString& skillId) const;
    ExerciseSequencePtr createSequenceForReview(const QList<QString>& exerciseIds) const;
    QStringList getSkillCharacterSet(const QString& skillId) const;

private:
    Exercise* findExerciseById(const QString& exerciseId) const;
    Skill parseSkill(const QJsonObject& skillJson);
    void clearContent();
};

#endif // CONTENTREPOSITORY_H
