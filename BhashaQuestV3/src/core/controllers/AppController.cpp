#include "AppController.h"
#include "../domain/MCQGrader.h"
#include "../domain/TranslateGrader.h"
#include "../domain/TranslateExercise.h"
#include "../domain/TileOrderGrader.h"
#include "../domain/CharacterSelectionGrader.h"
#include <QDate>
#include <QDebug>

/**
 * AppController Implementation - FIXED VERSION
 *
 * This file implements the controller layer orchestrating the lesson flow:
 * Load → Display → Grade → Update Profile → Next
 */

AppController::AppController(QObject* parent)
    : QObject(parent)
    , currentExercise(nullptr)
    , activeSequence(nullptr)
    , sequenceIterator()
    , sequenceEnd()
    , exercisesServed(0)
    , sessionActive(false)
    , userProfile(new Profile())
    , srsScheduler(new SRSScheduler())
    , currentGrader(nullptr)
    , currentSkillId("")
    , sessionsCompletedToday(0)
    , sessionXP(0)  // Track XP for current session
{
}

AppController::~AppController() {
    cleanupCurrentExercise();

    activeSequence.clear();

    // Clean up domain objects
    delete userProfile;
    delete srsScheduler;
}

// ========== Session Management ==========

void AppController::startLesson(const QString& skillId, const QSharedPointer<ExerciseSequence>& sequence) {
    // Clean up any existing session
    endLesson();

    if (!sequence || sequence->isEmpty()) {
        qDebug() << "Warning: Cannot start lesson - sequence empty";
        return;
    }

    currentSkillId = skillId;
    activeSequence = sequence;
    sequenceIterator = activeSequence->begin();
    sequenceEnd = activeSequence->end();
    exercisesServed = 0;
    sessionXP = 0;  // Reset session XP
    sessionActive = true;

    // Load first exercise
    loadNextExercise();
}

void AppController::endLesson() {
    // Calculate session stats
    int totalXP = sessionXP;  // Use accumulated session XP
    int completed = exercisesServed;

    // Only emit completion if we actually completed exercises
    if (sessionActive && completed > 0) {
        // Update user's total session count
        sessionsCompletedToday++;

        // Emit lesson completion signal with actual XP earned
        emit lessonCompleted(totalXP, completed);
    }

    // Clean up session state
    cleanupCurrentExercise();
    activeSequence.clear();
    sequenceIterator = ExerciseSequence::Iterator();
    sequenceEnd = ExerciseSequence::Iterator();
    currentSkillId = "";
    sessionXP = 0;
    exercisesServed = 0;
    sessionActive = false;
}

Exercise* AppController::getCurrentExercise() const {
    return currentExercise;
}

bool AppController::hasNextExercise() const {
    return activeSequence && sequenceIterator != sequenceEnd;
}

bool AppController::loadNextExercise() {
    if (!activeSequence || sequenceIterator == sequenceEnd) {
        endLesson();
        return false;
    }

    // Clean up previous exercise and grader
    cleanupCurrentExercise();

    currentExercise = *sequenceIterator;

    if (!currentExercise) {
        qDebug() << "Error: Found null exercise in sequence";
        return false;
    }

    ++sequenceIterator;
    exercisesServed = sequenceIterator.position();

    // Create appropriate grader for this exercise
    currentGrader = createGraderForExercise(currentExercise);

    if (!currentGrader) {
        qDebug() << "Error: Failed to create grader for exercise type" << currentExercise->getType();
        return false;
    }

    // Update progress display (show current as N of Total)
    emit progressUpdated(exercisesServed, activeSequence->size());

    // Emit signal to update UI
    emit exerciseChanged(currentExercise);

    return true;
}

// ========== Answer Submission and Grading ==========

void AppController::submitAnswer(const QString& userAnswer) {
    if (!currentExercise || !currentGrader) {
        qDebug() << "Error: Cannot submit answer - no active exercise or grader";
        return;
    }

    // Grade the answer using Strategy Pattern
    Result result = gradeAnswer(userAnswer);

    // Calculate and add XP for this exercise
    int xpEarned = calculateXP(result);
    sessionXP += xpEarned;  // Accumulate session XP

    // Update profile based on result
    updateProfile(result);

    // Record completion for SRS tracking
    srsScheduler->recordCompletion(currentExercise->getId());

    // Add XP earned to result feedback
    if (result.correct) {
        result.feedback += QString(" (+%1 XP)").arg(xpEarned);
    }

    // Emit result signal to UI
    emit answerGraded(result);
}

Result AppController::gradeAnswer(const QString& userAnswer) {
    if (!currentExercise || !currentGrader) {
        return Result(false, 0, "No active exercise");
    }

    // Delegate grading to appropriate strategy
    // This is the Strategy Pattern in action
    return currentGrader->grade(userAnswer, currentExercise);
}

// ========== Profile and Progress Management ==========

void AppController::updateProfile(const Result& result) {
    if (!userProfile) {
        qDebug() << "Error: No user profile to update";
        return;
    }

    // Calculate XP based on result
    int xpEarned = calculateXP(result);

    // Add XP to profile
    if (xpEarned > 0) {
        userProfile->addXP(xpEarned);
    }

    // Update streak (check if user is active today)
    userProfile->updateStreak();

    // Update skill progress for current skill
    if (!currentSkillId.isEmpty()) {
        SkillProgress* progress = userProfile->getProgress(currentSkillId);
        if (!progress) {
            progress = new SkillProgress(currentSkillId);
            userProfile->setProgress(currentSkillId, progress);
        }

        // Record the result (correct or incorrect)
        progress->recordResult(result.correct);

        // Update mastery based on performance
        if (result.correct) {
            progress->incrementMastery(5);  // +5% mastery for correct answer
        } else {
            progress->decrementMastery(2);  // -2% mastery for incorrect answer
        }
    }

    // Emit profile update signal
    emit profileUpdated(userProfile->getCurrentXP(), userProfile->getStreak());
}

Profile* AppController::getProfile() const {
    return userProfile;
}

void AppController::setProfile(Profile* profile) {
    if (userProfile && userProfile != profile) {
        delete userProfile;
    }
    userProfile = profile;
}

// ========== SRS Integration ==========

SRSScheduler* AppController::getSRSScheduler() const {
    return srsScheduler;
}

void AppController::scheduleReview(Difficulty difficulty) {
    if (!currentExercise || !srsScheduler) {
        return;
    }

    srsScheduler->scheduleNextReview(currentExercise->getId(), difficulty);
}

// ========== Utility Methods ==========

QPair<int, int> AppController::getSessionProgress() const {
    int completed = exercisesServed;
    int total = activeSequence ? activeSequence->size() : 0;
    return QPair<int, int>(completed, total);
}

void AppController::reset() {
    endLesson();

    // Reset profile (optional - could keep progress)
    if (userProfile) {
        delete userProfile;
        userProfile = new Profile();
    }

    sessionsCompletedToday = 0;
}

// ========== Private Helper Methods ==========

StrategyGrader* AppController::createGraderForExercise(Exercise* exercise) {
    if (!exercise) {
        qDebug() << "Error: Cannot create grader for null exercise";
        return nullptr;
    }

    // DESIGN PATTERN: Strategy Pattern
    // Create appropriate grader based on exercise type
    QString type = exercise->getType();

    if (type == "MCQ") {
        return new MCQGrader();
    } else if (type == "Translate") {
        TranslateExercise* translateExercise = dynamic_cast<TranslateExercise*>(exercise);
        if (translateExercise && translateExercise->usesCharacterSelection()) {
            return new CharacterSelectionGrader();
        }
        return new TranslateGrader();
    } else if (type == "TileOrder") {
        return new TileOrderGrader();
    }

    // Log unknown exercise type
    qDebug() << "Warning: Unknown exercise type:" << type;
    return nullptr;
}

int AppController::calculateXP(const Result& result) const {
    if (!result.correct || !currentExercise) {
        return 0;
    }

    // Base XP from result
    int baseXP = result.score;

    // If base XP is 0, use default value
    if (baseXP == 0) {
        baseXP = 10;  // Default base XP
    }

    // Bonus XP based on difficulty
    int difficultyMultiplier = currentExercise->getDifficulty();

    // Ensure difficulty is valid (1-3)
    if (difficultyMultiplier < 1) difficultyMultiplier = 1;
    if (difficultyMultiplier > 3) difficultyMultiplier = 3;

    // Calculate total XP: base * difficulty
    // Example: score=10, difficulty=3 → 30 XP
    int totalXP = baseXP * difficultyMultiplier;

    return totalXP;
}

void AppController::cleanupCurrentExercise() {
    // Note: We don't delete currentExercise because it's owned by ContentRepository
    // We just clear the pointer
    currentExercise = nullptr;

    // Delete the current grader (we own this)
    if (currentGrader) {
        delete currentGrader;
        currentGrader = nullptr;
    }
}
