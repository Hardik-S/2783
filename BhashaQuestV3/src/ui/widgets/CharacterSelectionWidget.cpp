#include "CharacterSelectionWidget.h"
#include "../../core/domain/CharacterUtils.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QStyle>
#include <QFont>
#include <QDebug>
#include <stdexcept>
#include <QLayoutItem>

CharacterSelectionWidget::CharacterSelectionWidget(
    const QString& correctAnswer,
    const QStringList& languageCharacterSet,
    const QString& targetLanguage,
    QWidget* parent)
    : QWidget(parent),
      selectedLabel(nullptr),
      availableLabel(nullptr),
      undoButton(nullptr),
      clearButton(nullptr),
      characterButtonsContainer(nullptr),
      charactersLayout(nullptr),
      correctAnswer(correctAnswer),
      targetLanguage(targetLanguage) {

    // Always setup UI first, even if data is invalid
    // This ensures the widget is in a safe state
    setupUI();

    // Validate inputs
    if (correctAnswer.isEmpty()) {
        qWarning() << "CharacterSelectionWidget: correctAnswer is empty";
        if (selectedLabel) selectedLabel->setText("Error: No correct answer provided");
        setStyleSheet("background-color: #ffebee;");
        return;
    }

    if (languageCharacterSet.isEmpty()) {
        qWarning() << "CharacterSelectionWidget: languageCharacterSet is empty";
        if (selectedLabel) selectedLabel->setText("Error: No character set available");
        setStyleSheet("background-color: #ffebee;");
        return;
    }

    try {
        // Generate character bank
        characterBank = CharacterUtils::generateCharacterBank(correctAnswer, languageCharacterSet);

        if (characterBank.isEmpty()) {
            qWarning() << "CharacterSelectionWidget: Generated character bank is empty";
            if (selectedLabel) selectedLabel->setText("Error: Could not generate character bank");
            setStyleSheet("background-color: #ffebee;");
            return;
        }

        availableCharacters = characterBank;

        // Create character buttons
        createCharacterBank();

        qDebug() << "CharacterSelectionWidget: Initialized successfully with"
                 << characterBank.size() << "characters";
    } catch (const std::exception& e) {
        qWarning() << "CharacterSelectionWidget: Exception in constructor:"
                  << QString::fromStdString(e.what());
        if (selectedLabel) selectedLabel->setText(QString("Error: %1").arg(QString::fromStdString(e.what())));
        setStyleSheet("background-color: #ffebee;");
    } catch (...) {
        qWarning() << "CharacterSelectionWidget: Unknown exception in constructor";
        if (selectedLabel) selectedLabel->setText("Error: Unknown initialization error");
        setStyleSheet("background-color: #ffebee;");
    }
}

CharacterSelectionWidget::~CharacterSelectionWidget() {
    // Qt handles cleanup of child widgets automatically
}

void CharacterSelectionWidget::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(12);
    mainLayout->setContentsMargins(16, 16, 16, 16);

    // Top section: Selected sequence display with buttons
    QHBoxLayout* selectedLayout = new QHBoxLayout();

    selectedLabel = new QLabel(this);
    selectedLabel->setText("Selected: (none)");
    selectedLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
    selectedLayout->addWidget(selectedLabel, 1);

    // Undo and Clear buttons
    undoButton = new QPushButton("Undo", this);
    undoButton->setMaximumWidth(80);
    undoButton->setEnabled(false);
    connect(undoButton, &QPushButton::clicked, this, &CharacterSelectionWidget::onUndoClicked);
    selectedLayout->addWidget(undoButton);

    clearButton = new QPushButton("Clear", this);
    clearButton->setMaximumWidth(80);
    clearButton->setEnabled(false);
    connect(clearButton, &QPushButton::clicked, this, &CharacterSelectionWidget::onClearClicked);
    selectedLayout->addWidget(clearButton);

    mainLayout->addLayout(selectedLayout);

    // Separator
    mainLayout->addSpacing(8);

    // Label for available characters section
    availableLabel = new QLabel("Available Characters:", this);
    availableLabel->setStyleSheet("font-weight: bold; font-size: 12px;");
    mainLayout->addWidget(availableLabel);

    // Container for character buttons with grid layout
    characterButtonsContainer = new QWidget(this);
    QGridLayout* gridLayout = new QGridLayout(characterButtonsContainer);
    gridLayout->setSpacing(8);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    // Will be populated in createCharacterBank()
    charactersLayout = new QHBoxLayout();
    charactersLayout->setSpacing(8);

    // Use a wrapper widget for the flow layout
    QWidget* charactersWrapper = new QWidget(this);
    charactersWrapper->setLayout(charactersLayout);

    mainLayout->addWidget(charactersWrapper);
    mainLayout->addStretch();

    setLayout(mainLayout);

    // Set minimum size
    setMinimumHeight(200);
}

void CharacterSelectionWidget::createCharacterBank() {
    // Clear existing buttons (and stretch) safely
    while (charactersLayout->count() > 0) {
        QLayoutItem* item = charactersLayout->takeAt(0);
        if (!item) {
            continue;
        }
        QWidget* widget = item->widget();
        if (widget) {
            disconnect(widget, nullptr, this, nullptr);
            delete widget;
        }
        delete item;
    }
    characterButtons.clear();

    // Create button for each character in the bank
    for (const QString& character : characterBank) {
        QPushButton* button = new QPushButton(character, this);

        // Style the button
        button->setMinimumWidth(50);
        button->setMinimumHeight(50);
        button->setMaximumWidth(60);
        button->setMaximumHeight(60);

        // Set font for better character display
        QFont font = button->font();
        font.setPointSize(12);
        font.setBold(true);
        button->setFont(font);

        // Style sheet
        button->setStyleSheet(
            "QPushButton {"
            "    background-color: #4CAF50;"
            "    color: white;"
            "    border: none;"
            "    border-radius: 4px;"
            "    padding: 8px;"
            "}"
            "QPushButton:hover {"
            "    background-color: #45a049;"
            "}"
            "QPushButton:pressed {"
            "    background-color: #3d8b40;"
            "}"
            "QPushButton:disabled {"
            "    background-color: #cccccc;"
            "    color: #999999;"
            "}"
        );

        // Connect button click
        connect(button, &QPushButton::clicked, this, &CharacterSelectionWidget::onCharacterClicked);

        charactersLayout->addWidget(button);
        characterButtons.append(button);
    }

    // Add stretch at the end to keep buttons left-aligned
    charactersLayout->addStretch();
}

QString CharacterSelectionWidget::getSelectedSequence() const {
    return selectedSequence.join(";");
}

void CharacterSelectionWidget::reset() {
    selectedSequence.clear();
    availableCharacters = characterBank;
    updateUIState();
}

void CharacterSelectionWidget::setEnabled(bool enabled) {
    QWidget::setEnabled(enabled);
    for (QPushButton* button : characterButtons) {
        button->setEnabled(enabled && availableCharacters.contains(button->text()));
    }
    undoButton->setEnabled(enabled && !selectedSequence.isEmpty());
    clearButton->setEnabled(enabled && !selectedSequence.isEmpty());
}

void CharacterSelectionWidget::onCharacterClicked() {
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    QString character = button->text();

    // Add to selected sequence
    selectedSequence.append(character);

    // Remove from available
    availableCharacters.removeOne(character);

    // Update UI
    updateUIState();

    // Emit signal
    emit characterSelected();
}

void CharacterSelectionWidget::onUndoClicked() {
    if (selectedSequence.isEmpty()) return;

    // Remove last selected character
    QString lastCharacter = selectedSequence.takeLast();

    // Add back to available
    availableCharacters.append(lastCharacter);

    // Update UI
    updateUIState();
}

void CharacterSelectionWidget::onClearClicked() {
    selectedSequence.clear();
    availableCharacters = characterBank;
    updateUIState();
}

void CharacterSelectionWidget::updateUIState() {
    updateSelectedLabel();
    updateButtonStates();
}

void CharacterSelectionWidget::updateSelectedLabel() {
    if (selectedSequence.isEmpty()) {
        selectedLabel->setText("Selected: (none)");
    } else {
        QString display = "Selected: ";
        for (const QString& ch : selectedSequence) {
            display += "[" + ch + "] ";
        }
        selectedLabel->setText(display.trimmed());
    }

    // Update button states
    undoButton->setEnabled(!selectedSequence.isEmpty());
    clearButton->setEnabled(!selectedSequence.isEmpty());
}

void CharacterSelectionWidget::updateButtonStates() {
    for (QPushButton* button : characterButtons) {
        QString character = button->text();
        // Disable button if character already selected
        button->setEnabled(availableCharacters.contains(character));
    }
}

#if __has_include("moc_CharacterSelectionWidget.cpp")
#include "moc_CharacterSelectionWidget.cpp"
#endif
