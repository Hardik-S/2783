#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QObject>
#include <QString>
#include <QSharedPointer>
#include "../domain/Exercise.h"
#include "../domain/StrategyGrader.h"
#include "../domain/Profile.h"
#include "../domain/SRSScheduler.h"
#include "../domain/Result.h"
#include "../domain/ExerciseSequence.h"

/**
 * AppController - Central controller orchestrating the application logic
 *
 * ARCHITECTURE: Controller Layer
 * Purpose: Mediates between UI Layer and Domain Layer, orchestrating the lesson flow
 *
 * Key Responsibilities:
 * 1. Manage current lesson/exercise session
 * 2. Coordinate exercise loading, grading, and progression
 * 3. Update user profile (XP, streak) based on exercise results
 * 4. Integrate with SRS scheduler for review queue
 * 5. Provide signals/slots for UI communication
 *
 * Data Flow (Exercise Completion):
 * 1. UI emits signal → AppController::submitAnswer()
 * 2. AppController uses StrategyGrader::grade() (Strategy Pattern)
 * 3. AppController updates Profile and SRSScheduler
 * 4. AppController emits signals to update UI
 * 5. AppController loads next exercise via ExerciseFactory (Factory Pattern)
 *
 * Design Principles:
 * - UI components only interact with AppController, never directly with domain objects
 * - AppController maintains session state but delegates business logic to domain layer
 * - Uses Qt signals/slots for loose coupling with UI
 */
class AppController : public QObject {
    Q_OBJECT

private:
    // Session state
    Exercise* currentExercise;                                      // Current exercise being displayed
    QSharedPointer<ExerciseSequence> activeSequence;                 // Sequence driving the current session
    ExerciseSequence::Iterator sequenceIterator;                    // Iterator for the next exercise
    ExerciseSequence::Iterator sequenceEnd;                         // End marker for the sequence
    int exercisesServed;                                           // Exercises that have been presented already
    bool sessionActive;                                            // Tracks whether a session is in progress

    // Domain objects
    Profile* userProfile;                   // User profile and progress
    SRSScheduler* srsScheduler;             // Spaced repetition scheduler
    StrategyGrader* currentGrader;          // Grader for current exercise type

    // Session metadata
    QString currentSkillId;                 // Current skill being practiced
    int sessionsCompletedToday;             // Track daily activity
    int sessionXP;                          // Track XP for current session

public:
    explicit AppController(QObject* parent = nullptr);
    ~AppController();

    // Session management

    /**
     * Start a new lesson session for a specific skill using an ExerciseSequence
     * The sequence holds iterator-aware metadata to drive traversal without exposing QList indices.
     * @param skillId - The skill to practice
     * @param sequence - Shared pointer to the ExerciseSequence for this session
     */
    void startLesson(const QString& skillId, const QSharedPointer<ExerciseSequence>& sequence);

    /**
     * End the current lesson session
     * Cleans up resources, updates final profile state
     */
    void endLesson();

    /**
     * Get the current exercise
     * @return Pointer to current exercise (nullptr if no active session)
     */
    Exercise* getCurrentExercise() const;

    /**
     * Check if there are more exercises in the queue
     * @return true if more exercises available
     */
    bool hasNextExercise() const;

    /**
     * Load the next exercise in the queue
     * Emits exerciseChanged signal
     * @return true if next exercise loaded, false if queue exhausted
     */
    bool loadNextExercise();

    // Answer submission and grading

    /**
     * Submit user's answer for grading
     * Orchestrates: grade → update profile → update SRS → emit result signal
     * @param userAnswer - The user's submitted answer
     */
    void submitAnswer(const QString& userAnswer);

    /**
     * Grade the current exercise with the provided answer
     * Uses appropriate StrategyGrader based on exercise type
     * @param userAnswer - The user's answer
     * @return Result struct with correct flag, score, and feedback
     */
    Result gradeAnswer(const QString& userAnswer);

    // Profile and progress management

    /**
     * Update user profile based on exercise result
     * Adds XP, updates streak, records completion
     * @param result - The grading result
     */
    void updateProfile(const Result& result);

    /**
     * Get the user profile
     * @return Pointer to user profile
     */
    Profile* getProfile() const;

    /**
     * Set the user profile
     * @param profile - Pointer to profile (controller takes ownership)
     */
    void setProfile(Profile* profile);

    // SRS integration

    /**
     * Get the SRS scheduler
     * @return Pointer to SRS scheduler
     */
    SRSScheduler* getSRSScheduler() const;

    /**
     * Schedule next review for current exercise
     * @param difficulty - User-rated difficulty (Hard/Medium/Easy)
     */
    void scheduleReview(Difficulty difficulty);

    // Utility methods

    /**
     * Get current session progress
     * @return Pair of (completed, total) exercises
     */
    QPair<int, int> getSessionProgress() const;

    /**
     * Reset controller state
     * Clears current session, resets exercise queue
     */
    void reset();

signals:
    /**
     * Emitted when the current exercise changes
     * @param exercise - The new current exercise
     */
    void exerciseChanged(Exercise* exercise);

    /**
     * Emitted after answer is graded
     * @param result - The grading result
     */
    void answerGraded(const Result& result);

    /**
     * Emitted when the lesson session ends
     * @param totalXP - Total XP earned in session
     * @param exercisesCompleted - Number of exercises completed
     */
    void lessonCompleted(int totalXP, int exercisesCompleted);

    /**
     * Emitted when profile is updated
     * @param currentXP - Updated XP total
     * @param streak - Updated streak
     */
    void profileUpdated(int currentXP, int streak);

    /**
     * Emitted when exercise progress is updated
     * @param current - Current exercise number (1-based)
     * @param total - Total number of exercises
     */
    void progressUpdated(int current, int total);

private:
    /**
     * Select and instantiate appropriate grader for exercise type
     * Uses Strategy Pattern - creates MCQGrader, TranslateGrader, or TileOrderGrader
     * @param exercise - The exercise to grade
     * @return Pointer to appropriate grader (caller must delete)
     */
    StrategyGrader* createGraderForExercise(Exercise* exercise);

    /**
     * Calculate XP earned based on result and exercise difficulty
     * @param result - The grading result
     * @return XP amount
     */
    int calculateXP(const Result& result) const;

    /**
     * Clean up current exercise and grader
     * Deletes allocated memory
     */
    void cleanupCurrentExercise();
};

#endif // APPCONTROLLER_H
