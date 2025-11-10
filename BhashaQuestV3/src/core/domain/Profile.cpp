#include "Profile.h"
#include <QJsonArray>
#include <QDebug>

Profile::Profile(const QString& username)
    : username(username), currentXP(0), streak(0), selectedLanguage("Nepali"),
      lastActivityDate(QDate::currentDate()) {
}

Profile::~Profile() {
    // Clean up skill progress objects
    for (auto* progress : skillProgress) {
        delete progress;
    }
    skillProgress.clear();
}

void Profile::addXP(int xp) {
    if (xp > 0) {
        currentXP += xp;
    }
}

void Profile::updateStreak() {
    QDate today = getCurrentDate();  // Use simulated or real date

    if (lastActivityDate.isValid()) {
        int daysSinceLastActivity = lastActivityDate.daysTo(today);

        if (daysSinceLastActivity == 0) {
            // Same day, no change to streak
            return;
        } else if (daysSinceLastActivity == 1) {
            // Consecutive day, increment streak
            streak++;
        } else {
            // Streak broken, reset
            streak = 1;
        }
    } else {
        // First activity
        streak = 1;
    }

    lastActivityDate = today;
}

void Profile::checkStreakValidity() {
    QDate today = getCurrentDate();  // Use simulated or real date
    if (lastActivityDate.isValid()) {
        int daysSinceLastActivity = lastActivityDate.daysTo(today);
        if (daysSinceLastActivity > 1) {
            // Streak broken
            streak = 0;
        }
    }
}

SkillProgress* Profile::getProgress(const QString& skillId) {
    if (skillProgress.contains(skillId)) {
        return skillProgress[skillId];
    }
    // Create new progress if it doesn't exist
    SkillProgress* newProgress = new SkillProgress(skillId);
    skillProgress[skillId] = newProgress;
    return newProgress;
}

void Profile::setProgress(const QString& skillId, SkillProgress* progress) {
    if (skillProgress.contains(skillId)) {
        delete skillProgress[skillId];
    }
    skillProgress[skillId] = progress;
}

QJsonObject Profile::toJson() const {
    QJsonObject json;
    json["username"] = username;
    json["currentXP"] = currentXP;
    json["streak"] = streak;
    json["selectedLanguage"] = selectedLanguage;
    json["lastActivityDate"] = lastActivityDate.toString(Qt::ISODate);

    // Serialize skill progress
    QJsonArray skillsArray;
    for (auto it = skillProgress.begin(); it != skillProgress.end(); ++it) {
        QJsonObject skillObj;
        skillObj["skillId"] = it.key();
        skillObj["progress"] = it.value()->toJson();
        skillsArray.append(skillObj);
    }
    json["skills"] = skillsArray;

    return json;
}

void Profile::fromJson(const QJsonObject& json) {
    username = json["username"].toString("Player");
    currentXP = json["currentXP"].toInt(0);
    streak = json["streak"].toInt(0);
    selectedLanguage = json["selectedLanguage"].toString("Nepali");
    lastActivityDate = QDate::fromString(json["lastActivityDate"].toString(), Qt::ISODate);

    // Deserialize skill progress
    QJsonArray skillsArray = json["skills"].toArray();
    for (const QJsonValue& value : skillsArray) {
        QJsonObject skillObj = value.toObject();
        QString skillId = skillObj["skillId"].toString();
        SkillProgress* progress = new SkillProgress(skillId);
        progress->fromJson(skillObj["progress"].toObject());
        skillProgress[skillId] = progress;
    }
}

// ========== Time Travel Methods ==========

void Profile::advanceSimulatedDate(int days) {
    if (!simulatedCurrentDate.isValid()) {
        // Initialize to current real date
        simulatedCurrentDate = QDate::currentDate();
    }
    simulatedCurrentDate = simulatedCurrentDate.addDays(days);
    qDebug() << "⏭️ Time Travel: Advanced to" << simulatedCurrentDate.toString(Qt::ISODate);
}

QDate Profile::getCurrentDate() const {
    return simulatedCurrentDate.isValid() ? simulatedCurrentDate : QDate::currentDate();
}

void Profile::resetSimulatedDate() {
    simulatedCurrentDate = QDate();  // Reset to invalid (use real date)
    qDebug() << "🔄 Time Travel: Reset to real date";
}
