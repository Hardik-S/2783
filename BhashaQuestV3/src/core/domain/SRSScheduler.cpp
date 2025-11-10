#include "SRSScheduler.h"

SRSScheduler::SRSScheduler() {
}

void SRSScheduler::scheduleNextReview(const QString& exerciseId, Difficulty difficulty) {
    ReviewData data;

    // Check if exercise already has review data
    if (reviewSchedule.contains(exerciseId)) {
        data = reviewSchedule[exerciseId];
    } else {
        data.exerciseId = exerciseId;
        data.lastReviewDate = QDate::currentDate();
    }

    // Calculate new interval based on difficulty
    data.interval = calculateInterval(difficulty, data.interval);
    data.difficulty = difficulty;
    data.reviewCount++;
    data.lastReviewDate = QDate::currentDate();
    data.nextReviewDate = data.lastReviewDate.addDays(data.interval);

    // Update schedule
    reviewSchedule[exerciseId] = data;
}

bool SRSScheduler::isDueForReview(const QString& exerciseId) const {
    if (!reviewSchedule.contains(exerciseId)) {
        return false;
    }

    const ReviewData& data = reviewSchedule[exerciseId];
    return data.nextReviewDate <= QDate::currentDate();
}

QList<QString> SRSScheduler::getReviewQueue() const {
    QList<QString> queue;
    QDate today = QDate::currentDate();

    for (auto it = reviewSchedule.begin(); it != reviewSchedule.end(); ++it) {
        if (it.value().nextReviewDate <= today) {
            queue.append(it.key());
        }
    }

    return queue;
}

ReviewData SRSScheduler::getReviewData(const QString& exerciseId) const {
    if (reviewSchedule.contains(exerciseId)) {
        return reviewSchedule[exerciseId];
    }
    return ReviewData();
}

void SRSScheduler::setReviewData(const QString& exerciseId, const ReviewData& data) {
    reviewSchedule[exerciseId] = data;
}

void SRSScheduler::recordCompletion(const QString& exerciseId) {
    if (!reviewSchedule.contains(exerciseId)) {
        ReviewData data;
        data.exerciseId = exerciseId;
        data.lastReviewDate = QDate::currentDate();
        data.nextReviewDate = data.lastReviewDate.addDays(1);
        data.interval = 1;
        data.reviewCount = 1;
        reviewSchedule[exerciseId] = data;
    }
}

int SRSScheduler::calculateInterval(Difficulty difficulty, int currentInterval) const {
    // Simplified SRS algorithm for D2
    switch (difficulty) {
        case Difficulty::Easy:
            // Easy: increase interval significantly (7 days or 2x current)
            return std::max(7, currentInterval * 2);

        case Difficulty::Medium:
            // Medium: moderate increase (3 days or 1.5x current)
            return std::max(3, static_cast<int>(currentInterval * 1.5));

        case Difficulty::Hard:
            // Hard: minimal increase (1 day or keep same)
            return std::max(1, currentInterval);

        default:
            return 1;
    }
}
