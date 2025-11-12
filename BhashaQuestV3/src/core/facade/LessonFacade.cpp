#include "LessonFacade.h"

LessonFacade::LessonFacade(AppController* controller, ContentRepository* repository, QObject* parent)
    : QObject(parent)
    , controller(controller)
    , repository(repository)
    , scheduler(controller ? controller->getSRSScheduler() : nullptr)
{
    if (!controller) {
        qWarning() << "LessonFacade: AppController dependency is missing";
        return;
    }

    connect(controller, &AppController::exerciseChanged,
            this, &LessonFacade::exerciseChanged);
    connect(controller, &AppController::answerGraded,
            this, &LessonFacade::handleAnswerGraded);
    connect(controller, &AppController::progressUpdated,
            this, &LessonFacade::progressUpdated);
    connect(controller, &AppController::lessonCompleted,
            this, &LessonFacade::lessonCompleted);
    connect(controller, &AppController::profileUpdated,
            this, &LessonFacade::profileUpdated);
}

bool LessonFacade::initializeSkillSession(const QString& skillId) {
    if (!controller || !repository) {
        qWarning() << "LessonFacade: Cannot start session without controller and repository";
        return false;
    }

    if (skillId.isEmpty()) {
        qWarning() << "LessonFacade: Empty skill ID passed to initializeSkillSession";
        return false;
    }

    ExerciseSequencePtr sequence = repository->createSequenceForSkill(skillId);

    if (!sequence || sequence->isEmpty()) {
        qWarning() << "LessonFacade: No exercises found for skill" << skillId;
        return false;
    }

    activeSkillId = skillId;
    currentSequence = sequence;
    controller->startLesson(skillId, currentSequence);
    return true;
}

QPair<int, int> LessonFacade::getCurrentProgress() const {
    if (!controller) {
        return QPair<int, int>(0, 0);
    }
    return controller->getSessionProgress();
}

void LessonFacade::submitCurrentAnswer(const QString& answer) {
    if (!controller) {
        return;
    }
    controller->submitAnswer(answer);
}

void LessonFacade::advanceExercise() {
    if (!controller) {
        return;
    }
    controller->loadNextExercise();
}

void LessonFacade::requestReview(Difficulty difficulty) {
    if (!controller) {
        return;
    }

    controller->scheduleReview(difficulty);
    if (scheduler) {
        qDebug() << "LessonFacade: review queue size" << scheduler->getReviewQueue().size();
    }
    emit reviewScheduled(difficulty);
}

void LessonFacade::handleAnswerGraded(const Result& result) {
    if (result.correct) {
        AudioManager::getInstance().playSuccess();
    } else {
        AudioManager::getInstance().playError();
    }

    emit answerGraded(result);
}
