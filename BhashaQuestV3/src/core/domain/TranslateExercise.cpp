#include "TranslateExercise.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>

TranslateExercise::TranslateExercise()
    : targetLanguage("Nepali") {
    type = "Translate";
}

TranslateExercise::TranslateExercise(const QString& id, const QString& englishPhrase,
                                     const QStringList& correctAnswers, const QString& targetLanguage)
    : englishPhrase(englishPhrase), correctAnswers(correctAnswers), targetLanguage(targetLanguage) {
    this->id = id;
    this->type = "Translate";
    this->prompt = QString("Translate to %1: %2").arg(targetLanguage, englishPhrase);
    this->difficulty = 2; // Default difficulty
}

QString TranslateExercise::getPrompt() const {
    if (prompt.isEmpty()) {
        return QString("Translate to %1: %2").arg(targetLanguage, englishPhrase);
    }
    return prompt;
}

QString TranslateExercise::getCorrectAnswer() const {
    // Return the first correct answer (typically native script)
    if (!correctAnswers.isEmpty()) {
        return correctAnswers[0];
    }
    return "";
}

QString TranslateExercise::getType() const {
    return type;
}

QWidget* TranslateExercise::renderUI(QWidget* parent) {
    QWidget* widget = new QWidget(parent);
    QVBoxLayout* layout = new QVBoxLayout(widget);

    // Add prompt label
    QLabel* promptLabel = new QLabel(getPrompt(), widget);
    promptLabel->setWordWrap(true);
    promptLabel->setStyleSheet("font-size: 14pt; font-weight: bold; margin-bottom: 10px;");
    layout->addWidget(promptLabel);

    // Add instruction label
    QLabel* instructionLabel = new QLabel(
        QString("Type your answer in %1:").arg(targetLanguage), widget);
    instructionLabel->setStyleSheet("font-size: 11pt; color: gray; margin-bottom: 5px;");
    layout->addWidget(instructionLabel);

    // Add input field
    QLineEdit* inputField = new QLineEdit(widget);
    inputField->setPlaceholderText("Your translation...");
    inputField->setStyleSheet("font-size: 12pt; padding: 8px;");
    inputField->setObjectName("translationInput"); // For easy access later
    layout->addWidget(inputField);

    layout->addStretch();
    widget->setLayout(layout);

    return widget;
}
