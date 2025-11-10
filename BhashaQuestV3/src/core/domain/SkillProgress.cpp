#include "SkillProgress.h"
#include <algorithm>

SkillProgress::SkillProgress(const QString& skillId)
    : skillId(skillId), masteryLevel(0), exercisesCompleted(0),
      totalExercises(0), correctAnswers(0), incorrectAnswers(0) {
}

void SkillProgress::recordResult(bool correct) {
    exercisesCompleted++;
    if (correct) {
        correctAnswers++;
        incrementMastery(5); // Increase mastery by 5% for correct answer
    } else {
        incorrectAnswers++;
        decrementMastery(2); // Decrease mastery by 2% for incorrect answer
    }
}

void SkillProgress::incrementMastery(int amount) {
    masteryLevel = std::min(100, masteryLevel + amount);
}

void SkillProgress::decrementMastery(int amount) {
    masteryLevel = std::max(0, masteryLevel - amount);
}

double SkillProgress::getAccuracy() const {
    int totalAttempts = correctAnswers + incorrectAnswers;
    if (totalAttempts == 0) {
        return 0.0;
    }
    return (static_cast<double>(correctAnswers) / totalAttempts) * 100.0;
}

QJsonObject SkillProgress::toJson() const {
    QJsonObject json;
    json["skillId"] = skillId;
    json["masteryLevel"] = masteryLevel;
    json["exercisesCompleted"] = exercisesCompleted;
    json["totalExercises"] = totalExercises;
    json["correctAnswers"] = correctAnswers;
    json["incorrectAnswers"] = incorrectAnswers;
    return json;
}

void SkillProgress::fromJson(const QJsonObject& json) {
    skillId = json["skillId"].toString();
    masteryLevel = json["masteryLevel"].toInt(0);
    exercisesCompleted = json["exercisesCompleted"].toInt(0);
    totalExercises = json["totalExercises"].toInt(0);
    correctAnswers = json["correctAnswers"].toInt(0);
    incorrectAnswers = json["incorrectAnswers"].toInt(0);
}
