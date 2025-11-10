#include "TileOrderExercise.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QRandomGenerator>
#include <algorithm>

TileOrderExercise::TileOrderExercise() {
    type = "TileOrder";
}

TileOrderExercise::TileOrderExercise(const QString& id, const QString& prompt,
                                     const QStringList& tiles, const QStringList& correctOrder)
    : tiles(tiles), correctOrder(correctOrder) {
    this->id = id;
    this->prompt = prompt;
    this->type = "TileOrder";
    this->difficulty = 3; // Default difficulty
}

QString TileOrderExercise::getPrompt() const {
    return prompt;
}

QString TileOrderExercise::getCorrectAnswer() const {
    // Return correct order as a space-separated string
    return correctOrder.join(" ");
}

QString TileOrderExercise::getType() const {
    return type;
}

void TileOrderExercise::shuffle() {
    // Shuffle the tiles randomly
    QStringList shuffled = tiles;
    std::shuffle(shuffled.begin(), shuffled.end(),
                 std::default_random_engine(QRandomGenerator::global()->generate()));
    tiles = shuffled;
}

QWidget* TileOrderExercise::renderUI(QWidget* parent) {
    QWidget* widget = new QWidget(parent);
    QVBoxLayout* layout = new QVBoxLayout(widget);

    // Add prompt label
    QLabel* promptLabel = new QLabel(prompt, widget);
    promptLabel->setWordWrap(true);
    promptLabel->setStyleSheet("font-size: 14pt; font-weight: bold; margin-bottom: 10px;");
    layout->addWidget(promptLabel);

    // Add instruction label
    QLabel* instructionLabel = new QLabel("Tap the tiles in the correct order:", widget);
    instructionLabel->setStyleSheet("font-size: 11pt; color: gray; margin-bottom: 10px;");
    layout->addWidget(instructionLabel);

    // Add tiles container
    QWidget* tilesContainer = new QWidget(widget);
    QHBoxLayout* tilesLayout = new QHBoxLayout(tilesContainer);
    tilesLayout->setSpacing(10);

    for (const QString& tile : tiles) {
        QPushButton* tileButton = new QPushButton(tile, tilesContainer);
        tileButton->setStyleSheet(
            "QPushButton {"
            "   font-size: 12pt;"
            "   padding: 10px 15px;"
            "   background-color: #e0e0e0;"
            "   border: 2px solid #999;"
            "   border-radius: 5px;"
            "}"
            "QPushButton:hover {"
            "   background-color: #d0d0d0;"
            "}"
            "QPushButton:pressed {"
            "   background-color: #4CAF50;"
            "   color: white;"
            "}"
        );
        tilesLayout->addWidget(tileButton);
    }

    tilesContainer->setLayout(tilesLayout);
    layout->addWidget(tilesContainer);

    // Add selected order display
    QLabel* selectedLabel = new QLabel("Selected order: ", widget);
    selectedLabel->setObjectName("selectedOrderLabel");
    selectedLabel->setStyleSheet("font-size: 11pt; margin-top: 10px;");
    layout->addWidget(selectedLabel);

    layout->addStretch();
    widget->setLayout(layout);

    return widget;
}
