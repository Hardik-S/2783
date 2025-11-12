#ifndef LESSONFACADE_H
#define LESSONFACADE_H

#include <QObject>
#include <QPair>
#include <QDebug>
#include "../controllers/AppController.h"
#include "../data/ContentRepository.h"
#include "../domain/AudioManager.h"

/**
 * LessonFacade - Facade for coordinating UI interactions with domain/controller logic.
 *
 * Simplifies the UI API by exposing a single entrypoint for starting lessons, grading
 * answers, and emitting sanitized signals derived from AppController.
 */
class LessonFacade : public QObject {
    Q_OBJECT

public:
    explicit LessonFacade(AppController* controller, ContentRepository* repository, QObject* parent = nullptr);

    /**
     * Initialize a lesson session for the specified skill.
     * Returns true if a valid sequence was started.
     */
    bool initializeSkillSession(const QString& skillId);

    /**
     * Query the current session progress (completed, total)
     */
    QPair<int, int> getCurrentProgress() const;

public slots:
    void submitCurrentAnswer(const QString& answer);
    void advanceExercise();
    void requestReview(Difficulty difficulty);

signals:
    void exerciseChanged(Exercise* exercise);
    void answerGraded(const Result& result);
    void progressUpdated(int current, int total);
    void lessonCompleted(int totalXP, int exercisesCompleted);
    void profileUpdated(int currentXP, int streak);
    void reviewScheduled(Difficulty difficulty);

private slots:
    void handleAnswerGraded(const Result& result);

private:
    AppController* controller;
    ContentRepository* repository;
    SRSScheduler* scheduler;
    ExerciseSequencePtr currentSequence;
    QString activeSkillId;
};

#endif // LESSONFACADE_H
