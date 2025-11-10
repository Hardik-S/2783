#include "ExerciseFactory.h"
#include "MCQExercise.h"
#include "TranslateExercise.h"
#include "TileOrderExercise.h"
#include <QJsonArray>

Exercise* ExerciseFactory::createExercise(const QString& type, const QJsonObject& spec) {
    // Factory Pattern: Route to appropriate creation method based on type
    if (type == "MCQ") {
        return createMCQ(spec);
    } else if (type == "Translate") {
        return createTranslate(spec);
    } else if (type == "TileOrder") {
        return createTileOrder(spec);
    }

    // Unknown type - return nullptr
    return nullptr;
}

Exercise* ExerciseFactory::createMCQ(const QJsonObject& spec) {
    QString id = spec["id"].toString();
    QString prompt = spec["prompt"].toString();

    // Extract options array
    QStringList options;
    QJsonArray optionsArray = spec["options"].toArray();
    for (const QJsonValue& value : optionsArray) {
        options.append(value.toString());
    }

    int correctIndex = spec["correctIndex"].toInt(0);

    // Create and configure MCQ exercise
    MCQExercise* exercise = new MCQExercise(id, prompt, options, correctIndex);

    // Set optional fields
    if (spec.contains("skillId")) {
        exercise->setSkillId(spec["skillId"].toString());
    }
    if (spec.contains("difficulty")) {
        exercise->setDifficulty(spec["difficulty"].toInt(1));
    }
    if (spec.contains("audioFile")) {
        exercise->setAudioFile(spec["audioFile"].toString());
    }

    return exercise;
}

Exercise* ExerciseFactory::createTranslate(const QJsonObject& spec) {
    QString id = spec["id"].toString();
    QString englishPhrase = spec["englishPhrase"].toString();

    // Extract correct answers array (supports multiple valid answers)
    QStringList correctAnswers;
    QJsonArray answersArray = spec["correctAnswers"].toArray();
    for (const QJsonValue& value : answersArray) {
        correctAnswers.append(value.toString());
    }

    QString targetLanguage = spec["targetLanguage"].toString("Nepali");

    // Create and configure Translate exercise
    TranslateExercise* exercise = new TranslateExercise(id, englishPhrase, correctAnswers, targetLanguage);

    // Set optional fields
    if (spec.contains("skillId")) {
        exercise->setSkillId(spec["skillId"].toString());
    }
    if (spec.contains("difficulty")) {
        exercise->setDifficulty(spec["difficulty"].toInt(2));
    }
    if (spec.contains("prompt")) {
        exercise->setPrompt(spec["prompt"].toString());
    }
    if (spec.contains("audioFile")) {
        exercise->setAudioFile(spec["audioFile"].toString());
    }

    return exercise;
}

Exercise* ExerciseFactory::createTileOrder(const QJsonObject& spec) {
    QString id = spec["id"].toString();
    QString prompt = spec["prompt"].toString();

    // Extract tiles array
    QStringList tiles;
    QJsonArray tilesArray = spec["tiles"].toArray();
    for (const QJsonValue& value : tilesArray) {
        tiles.append(value.toString());
    }

    // Extract correct order array
    QStringList correctOrder;
    QJsonArray orderArray = spec["correctOrder"].toArray();
    for (const QJsonValue& value : orderArray) {
        correctOrder.append(value.toString());
    }

    // Create and configure TileOrder exercise
    TileOrderExercise* exercise = new TileOrderExercise(id, prompt, tiles, correctOrder);

    // Set optional fields
    if (spec.contains("skillId")) {
        exercise->setSkillId(spec["skillId"].toString());
    }
    if (spec.contains("difficulty")) {
        exercise->setDifficulty(spec["difficulty"].toInt(3));
    }

    if (spec.contains("audioFile")) {
        exercise->setAudioFile(spec["audioFile"].toString());
    }

    // Optionally shuffle tiles if requested
    if (spec.contains("shuffled") && spec["shuffled"].toBool()) {
        exercise->shuffle();
    }

    return exercise;
}
