#ifndef SRSSCHEDULER_H
#define SRSSCHEDULER_H

#include <QString>
#include <QDate>
#include <QMap>
#include <QList>

/**
 * SRSScheduler - Spaced Repetition System scheduler
 *
 * Implements simplified spaced repetition algorithm for D2:
 * - Easy difficulty: +7 days
 * - Medium difficulty: +3 days
 * - Hard difficulty: +1 day
 *
 * Future (D3): Implement full SM-2 algorithm with ease factor
 */

enum class Difficulty {
    Hard = 1,
    Medium = 2,
    Easy = 3
};

struct ReviewData {
    QString exerciseId;
    QDate lastReviewDate;
    QDate nextReviewDate;
    int interval;               // Current interval in days
    Difficulty difficulty;      // User-rated difficulty
    int reviewCount;            // Number of times reviewed

    ReviewData()
        : interval(1), difficulty(Difficulty::Medium), reviewCount(0) {}
};

class SRSScheduler {
private:
    QMap<QString, ReviewData> reviewSchedule;  // exerciseId -> ReviewData

public:
    SRSScheduler();

    /**
     * Schedule next review for an exercise based on user-rated difficulty
     * @param exerciseId - ID of the exercise
     * @param difficulty - User's rating (Hard/Medium/Easy)
     */
    void scheduleNextReview(const QString& exerciseId, Difficulty difficulty);

    /**
     * Check if an exercise is due for review
     * @param exerciseId - ID of the exercise
     * @return true if next_review_date <= today
     */
    bool isDueForReview(const QString& exerciseId) const;

    /**
     * Get list of exercises due for review today
     * @return List of exercise IDs
     */
    QList<QString> getReviewQueue() const;

    /**
     * Get review data for an exercise
     */
    ReviewData getReviewData(const QString& exerciseId) const;

    /**
     * Set review data for an exercise
     */
    void setReviewData(const QString& exerciseId, const ReviewData& data);

    /**
     * Record that an exercise was completed (init review tracking)
     */
    void recordCompletion(const QString& exerciseId);

private:
    /**
     * Calculate next review interval based on difficulty
     */
    int calculateInterval(Difficulty difficulty, int currentInterval) const;
};

#endif // SRSSCHEDULER_H
