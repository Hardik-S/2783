#include "MCQExercise.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QRadioButton>
#include <QButtonGroup>

MCQExercise::MCQExercise()
    : correctIndex(0) {
    type = "MCQ";
}

MCQExercise::MCQExercise(const QString& id, const QString& prompt,
                         const QStringList& options, int correctIndex)
    : correctIndex(correctIndex) {
    this->id = id;
    this->prompt = prompt;
    this->options = options;
    this->type = "MCQ";
    this->difficulty = 1; // Default difficulty
}

QString MCQExercise::getPrompt() const {
    return prompt;
}

QString MCQExercise::getCorrectAnswer() const {
    if (correctIndex >= 0 && correctIndex < options.size()) {
        return options[correctIndex];
    }
    return "";
}

QString MCQExercise::getType() const {
    return type;
}

QWidget* MCQExercise::renderUI(QWidget* parent) {
    QWidget* widget = new QWidget(parent);
    QVBoxLayout* layout = new QVBoxLayout(widget);

    // Add prompt label
    QLabel* promptLabel = new QLabel(prompt, widget);
    promptLabel->setWordWrap(true);
    promptLabel->setStyleSheet("font-size: 14pt; font-weight: bold; margin-bottom: 10px;");
    layout->addWidget(promptLabel);

    // Add radio buttons for options
    QButtonGroup* buttonGroup = new QButtonGroup(widget);
    for (int i = 0; i < options.size(); ++i) {
        QRadioButton* radioButton = new QRadioButton(options[i], widget);
        radioButton->setStyleSheet("font-size: 12pt; padding: 5px;");
        buttonGroup->addButton(radioButton, i);
        layout->addWidget(radioButton);
    }

    layout->addStretch();
    widget->setLayout(layout);

    return widget;
}
