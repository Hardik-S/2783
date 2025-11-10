#include "ContentRepository.h"
#include "../domain/ExerciseFactory.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QCoreApplication>
#include <QDir>

/**
 * ContentRepository Implementation
 *
 * This file implements the data layer for loading exercises from JSON.
 * Uses ExerciseFactory (Factory Pattern) to create Exercise instances.
 */

ContentRepository::ContentRepository(const QString& contentFileName)
    : contentLoaded(false)
{
    // KISS Principle: Simple path resolution using Qt's built-in functionality
    // Build absolute path to content file in application directory
    QString appDir = QCoreApplication::applicationDirPath();
    contentFilePath = QDir(appDir).filePath(contentFileName);

    qDebug() << "ContentRepository: Resolved content path to" << contentFilePath;
}

ContentRepository::~ContentRepository() {
    clearContent();
}

// ========== Content Loading ==========

bool ContentRepository::loadContent() {
    // Clear any existing content first
    clearContent();

    // Open the JSON file
    QFile file(contentFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "ContentRepository: Failed to open" << contentFilePath;
        return false;
    }

    // Read and parse JSON
    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (doc.isNull() || !doc.isObject()) {
        qWarning() << "ContentRepository: Invalid JSON format in" << contentFilePath;
        return false;
    }

    QJsonObject root = doc.object();

    // Parse "skills" array
    if (!root.contains("skills") || !root["skills"].isArray()) {
        qWarning() << "ContentRepository: Missing or invalid 'skills' array";
        return false;
    }

    QJsonArray skillsArray = root["skills"].toArray();

    // Parse each skill
    for (const QJsonValue& skillValue : skillsArray) {
        if (!skillValue.isObject()) {
            qWarning() << "ContentRepository: Skipping invalid skill entry";
            continue;
        }

        Skill skill = parseSkill(skillValue.toObject());

        // Store skill if it has valid exercises
        if (!skill.id.isEmpty() && !skill.exercises.isEmpty()) {
            skills[skill.id] = skill;
            qDebug() << "ContentRepository: Loaded skill" << skill.id
                     << "with" << skill.exercises.size() << "exercises";
        }
    }

    contentLoaded = !skills.isEmpty();

    if (contentLoaded) {
        qDebug() << "ContentRepository: Successfully loaded" << skills.size()
                 << "skills with" << getTotalExerciseCount() << "total exercises";
    } else {
        qWarning() << "ContentRepository: No valid skills loaded";
    }

    return contentLoaded;
}

bool ContentRepository::reload() {
    return loadContent();
}

// ========== Content Access ==========

QList<Exercise*> ContentRepository::getExercisesForSkill(const QString& skillId) const {
    if (!skills.contains(skillId)) {
        return QList<Exercise*>();
    }

    // Return a copy of the exercise list
    // Note: Caller does NOT own these pointers - repository manages memory
    return skills[skillId].exercises;
}

QList<QString> ContentRepository::getAvailableSkills() const {
    return skills.keys();
}

QString ContentRepository::getSkillName(const QString& skillId) const {
    if (!skills.contains(skillId)) {
        return QString();
    }
    return skills[skillId].name;
}

QString ContentRepository::getSkillLanguage(const QString& skillId) const {
    if (!skills.contains(skillId)) {
        return QString();
    }
    return skills[skillId].language;
}

bool ContentRepository::isLoaded() const {
    return contentLoaded;
}

int ContentRepository::getTotalExerciseCount() const {
    int total = 0;
    for (const Skill& skill : skills) {
        total += skill.exercises.size();
    }
    return total;
}

// ========== Private Helper Methods ==========

ContentRepository::Skill ContentRepository::parseSkill(const QJsonObject& skillJson) {
    Skill skill;

    // Parse skill metadata
    skill.id = skillJson["id"].toString();
    skill.name = skillJson["name"].toString();
    skill.language = skillJson["language"].toString();

    // Validate required fields
    if (skill.id.isEmpty()) {
        qWarning() << "ContentRepository: Skill missing 'id' field";
        return skill;
    }

    // Parse exercises array
    if (!skillJson.contains("exercises") || !skillJson["exercises"].isArray()) {
        qWarning() << "ContentRepository: Skill" << skill.id << "missing 'exercises' array";
        return skill;
    }

    QJsonArray exercisesArray = skillJson["exercises"].toArray();

    // Parse each exercise
    for (const QJsonValue& exerciseValue : exercisesArray) {
        if (!exerciseValue.isObject()) {
            qWarning() << "ContentRepository: Skipping invalid exercise in skill" << skill.id;
            continue;
        }

        QJsonObject exerciseJson = exerciseValue.toObject();

        // Extract exercise type
        QString type = exerciseJson["type"].toString();
        if (type.isEmpty()) {
            qWarning() << "ContentRepository: Exercise missing 'type' field in skill" << skill.id;
            continue;
        }

        // DESIGN PATTERN: Factory Pattern
        // Delegate exercise creation to ExerciseFactory
        // The repository doesn't need to know the specifics of each exercise type
        Exercise* exercise = ExerciseFactory::createExercise(type, exerciseJson);

        if (exercise) {
            skill.exercises.append(exercise);
            qDebug() << "ContentRepository: Created" << type << "exercise"
                     << exercise->getId() << "for skill" << skill.id;
        } else {
            qWarning() << "ContentRepository: Failed to create" << type
                       << "exercise in skill" << skill.id;
        }
    }

    return skill;
}

void ContentRepository::clearContent() {
    // Delete all Exercise instances
    for (Skill& skill : skills) {
        qDeleteAll(skill.exercises);
        skill.exercises.clear();
    }

    // Clear the skills map
    skills.clear();
    contentLoaded = false;
}
