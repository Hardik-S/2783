#ifndef PROFILE_H
#define PROFILE_H

#include <QString>
#include <QMap>
#include <QJsonObject>
#include <QDate>
#include "SkillProgress.h"

/**
 * Profile - User profile and progress data
 *
 * Tracks user information, XP, streak, and skill progress
 */
class Profile {
private:
    QString username;
    int currentXP;
    int streak;                             // Current streak in days
    QString selectedLanguage;
    QDate lastActivityDate;                 // Track for streak calculation
    QDate simulatedCurrentDate;             // For testing time travel (if null, use real date)
    QMap<QString, SkillProgress*> skillProgress;  // skillId -> progress

public:
    Profile(const QString& username = "Player");
    ~Profile();

    // XP management
    void addXP(int xp);
    int getCurrentXP() const { return currentXP; }

    // Streak management
    void updateStreak();
    int getStreak() const { return streak; }
    void checkStreakValidity();             // Check if streak should be maintained

    // Skill progress
    SkillProgress* getProgress(const QString& skillId);
    void setProgress(const QString& skillId, SkillProgress* progress);
    QMap<QString, SkillProgress*> getAllProgress() const { return skillProgress; }

    // Getters and setters
    QString getUsername() const { return username; }
    void setUsername(const QString& name) { username = name; }

    QString getSelectedLanguage() const { return selectedLanguage; }
    void setSelectedLanguage(const QString& language) { selectedLanguage = language; }

    QDate getLastActivityDate() const { return lastActivityDate; }
    void setLastActivityDate(const QDate& date) { lastActivityDate = date; }

    // Time travel for testing
    /**
     * Advance simulated date by N days (for testing streak feature)
     * @param days - Number of days to advance (default: 1)
     */
    void advanceSimulatedDate(int days = 1);

    /**
     * Get current date (simulated if time travel active, otherwise real date)
     * @return Current or simulated date
     */
    QDate getCurrentDate() const;

    /**
     * Reset time travel (go back to real date)
     */
    void resetSimulatedDate();

    // Serialization (for future JSON persistence)
    QJsonObject toJson() const;
    void fromJson(const QJsonObject& json);
};

#endif // PROFILE_H
