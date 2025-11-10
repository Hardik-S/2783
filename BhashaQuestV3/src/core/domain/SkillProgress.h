#ifndef SKILLPROGRESS_H
#define SKILLPROGRESS_H

#include <QString>
#include <QJsonObject>

/**
 * SkillProgress - Tracks user progress for a specific skill
 *
 * Maintains mastery level, completion stats, and performance metrics
 */
class SkillProgress {
private:
    QString skillId;
    int masteryLevel;               // 0-100 percentage
    int exercisesCompleted;
    int totalExercises;
    int correctAnswers;
    int incorrectAnswers;

public:
    SkillProgress(const QString& skillId = "");

    // Progress tracking
    void recordResult(bool correct);
    void incrementMastery(int amount);
    void decrementMastery(int amount);

    // Getters
    QString getSkillId() const { return skillId; }
    int getMasteryLevel() const { return masteryLevel; }
    int getExercisesCompleted() const { return exercisesCompleted; }
    int getTotalExercises() const { return totalExercises; }
    int getCorrectAnswers() const { return correctAnswers; }
    int getIncorrectAnswers() const { return incorrectAnswers; }
    double getAccuracy() const;

    // Setters
    void setSkillId(const QString& id) { skillId = id; }
    void setTotalExercises(int total) { totalExercises = total; }

    // Serialization
    QJsonObject toJson() const;
    void fromJson(const QJsonObject& json);
};

#endif // SKILLPROGRESS_H
