#include "LessonView.h"
#include "../core/domain/MCQExercise.h"
#include "../core/domain/TranslateExercise.h"
#include "../core/domain/TileOrderExercise.h"
#include "../core/domain/AudioManager.h"
#include <QMessageBox>
#include <QFont>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

/**
 * LessonView Implementation - ENHANCED VERSION
 * With improved UI/UX, animations, and better visual feedback
 */

LessonView::LessonView(QWidget* parent)
    : QWidget(parent)
    , mcqButtonGroup(nullptr)
    , translateInput(nullptr)
    , tileListWidget(nullptr)
    , currentExercise(nullptr)
{
    setupUI();
}

LessonView::~LessonView() {
    // Qt parent-child ownership handles widget cleanup
}

void LessonView::setupUI() {
    // Set background color for the entire widget
    setStyleSheet("QWidget { background-color: #66cc99; }");

    // Create main layout with better spacing
    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    // Create top card for progress info
    QWidget* progressCard = new QWidget(this);
    progressCard->setStyleSheet(
        "QWidget { "
        "   background-color: white; "
        "   border-radius: 10px; "
        "   padding: 15px; "
        "}"
    );
    QVBoxLayout* progressCardLayout = new QVBoxLayout(progressCard);

    // Progress label with improved styling
    progressLabel = new QLabel("", progressCard);
    QFont progressFont;
    progressFont.setPointSize(11);
    progressFont.setBold(true);
    progressLabel->setFont(progressFont);
    progressLabel->setAlignment(Qt::AlignRight);
    progressLabel->setStyleSheet("QLabel { color: #666; margin-bottom: 5px; }");
    progressCardLayout->addWidget(progressLabel);

    // Enhanced progress bar
    progressBar = new QProgressBar(progressCard);
    progressBar->setMinimum(0);
    progressBar->setMaximum(100);
    progressBar->setValue(0);
    progressBar->setTextVisible(false);
    progressBar->setMinimumHeight(8);
    progressBar->setStyleSheet(
        "QProgressBar { "
        "   border: none; "
        "   background-color: #66cc99; "
        "   border-radius: 4px; "
        "} "
        "QProgressBar::chunk { "
        "   background-color: #4CAF50; "
        "   border-radius: 4px; "
        "}"
    );
    progressCardLayout->addWidget(progressBar);

    mainLayout->addWidget(progressCard);

    // Create welcome panel (shown initially)
    createWelcomePanel();
    mainLayout->addWidget(welcomePanel);
    welcomePanel->setVisible(true);  // Explicitly show welcome panel on startup

    // Create main exercise card
    exerciseCard = new QWidget(this);
    exerciseCard->setStyleSheet(
        "QWidget { "
        "   background-color: white; "
        "   border-radius: 15px; "
        "   padding: 25px; "
        "}"
    );
    QVBoxLayout* exerciseCardLayout = new QVBoxLayout(exerciseCard);
    exerciseCardLayout->setSpacing(20);

    // Exercise type badge
    typeLabel = new QLabel("", exerciseCard);
    typeLabel->setAlignment(Qt::AlignCenter);
    typeLabel->setStyleSheet(
        "QLabel { "
        "   background-color: #2196F3; "
        "   color: white; "
        "   padding: 5px 15px; "
        "   border-radius: 15px; "
        "   font-size: 11px; "
        "   font-weight: bold; "
        "}"
    );
    typeLabel->setMaximumWidth(150);
    exerciseCardLayout->addWidget(typeLabel, 0, Qt::AlignCenter);

    // Prompt label with enhanced styling
    promptLabel = new QLabel("", exerciseCard);
    QFont promptFont;
    promptFont.setPointSize(16);
    promptFont.setBold(true);
    promptLabel->setFont(promptFont);
    promptLabel->setWordWrap(true);
    promptLabel->setMinimumHeight(80);
    promptLabel->setAlignment(Qt::AlignCenter);
    promptLabel->setStyleSheet("QLabel { color: #333; line-height: 1.5; padding: 10px; }");
    exerciseCardLayout->addWidget(promptLabel);

    // Input container
    inputContainer = new QWidget(exerciseCard);
    inputLayout = new QVBoxLayout(inputContainer);
    inputLayout->setSpacing(15);
    exerciseCardLayout->addWidget(inputContainer);

    mainLayout->addWidget(exerciseCard);
    exerciseCard->setVisible(false);  // Hidden initially until lesson starts

    // Add stretch
    mainLayout->addStretch();

    // Feedback card (initially hidden)
    feedbackCard = new QWidget(this);
    feedbackCard->setVisible(false);
    feedbackCard->setStyleSheet(
        "QWidget { "
        "   border-radius: 10px; "
        "   padding: 15px; "
        "}"
    );
    QVBoxLayout* feedbackLayout = new QVBoxLayout(feedbackCard);
    
    feedbackLabel = new QLabel("", feedbackCard);
    QFont feedbackFont;
    feedbackFont.setPointSize(13);
    feedbackFont.setBold(true);
    feedbackLabel->setFont(feedbackFont);
    feedbackLabel->setWordWrap(true);
    feedbackLabel->setAlignment(Qt::AlignCenter);
    feedbackLayout->addWidget(feedbackLabel);
    
    mainLayout->addWidget(feedbackCard);

    // Button container with improved styling
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(15);

    // Submit button with modern styling
    submitButton = new QPushButton("✓ Submit Answer", this);
    submitButton->setMinimumHeight(50);
    submitButton->setStyleSheet(
        "QPushButton { "
        "   background-color: #4CAF50; "
        "   color: white; "
        "   border: none; "
        "   border-radius: 25px; "
        "   padding: 15px 30px; "
        "   font-size: 14px; "
        "   font-weight: bold; "
        "   text-transform: uppercase; "
        "} "
        "QPushButton:hover { "
        "   background-color: #45a049; "
        "   transform: scale(1.02); "
        "} "
        "QPushButton:pressed { "
        "   background-color: #3d8b40; "
        "} "
        "QPushButton:disabled { "
        "   background-color: #cccccc; "
        "   color: #666666; "
        "}"
    );
    connect(submitButton, &QPushButton::clicked, this, &LessonView::onSubmitClicked);
    buttonLayout->addWidget(submitButton);

    // Next button with modern styling
    nextButton = new QPushButton("Next Exercise →", this);
    nextButton->setMinimumHeight(50);
    nextButton->setVisible(false);
    nextButton->setStyleSheet(
        "QPushButton { "
        "   background-color: #2196F3; "
        "   color: white; "
        "   border: none; "
        "   border-radius: 25px; "
        "   padding: 15px 30px; "
        "   font-size: 14px; "
        "   font-weight: bold; "
        "   text-transform: uppercase; "
        "} "
        "QPushButton:hover { "
        "   background-color: #1976D2; "
        "   transform: scale(1.02); "
        "} "
        "QPushButton:pressed { "
        "   background-color: #0D47A1; "
        "}"
    );
    connect(nextButton, &QPushButton::clicked, this, &LessonView::onNextClicked);
    buttonLayout->addWidget(nextButton);

    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);
}

void LessonView::createWelcomePanel() {
    welcomePanel = new QWidget(this);
    welcomePanel->setStyleSheet(
        "QWidget { "
        "   background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); "
        "   border-radius: 15px; "
        "   padding: 40px; "
        "}"
    );

    QVBoxLayout* welcomeLayout = new QVBoxLayout(welcomePanel);
    welcomeLayout->setSpacing(20);
    welcomeLayout->setAlignment(Qt::AlignCenter);

    // App title
    QLabel* titleLabel = new QLabel("🌍 BhashaQuest 🌍", welcomePanel);
    QFont titleFont;
    titleFont.setPointSize(28);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("QLabel { color: white; }");
    welcomeLayout->addWidget(titleLabel);

    // Tagline
    QLabel* taglineLabel = new QLabel("Learn Languages Through Interactive Lessons", welcomePanel);
    QFont taglineFont;
    taglineFont.setPointSize(14);
    taglineFont.setItalic(true);
    taglineLabel->setFont(taglineFont);
    taglineLabel->setAlignment(Qt::AlignCenter);
    taglineLabel->setStyleSheet("QLabel { color: white; opacity: 0.9; }");
    welcomeLayout->addWidget(taglineLabel);

    // Add spacing
    welcomeLayout->addSpacing(20);

    // Welcome message
    QLabel* welcomeTextLabel = new QLabel(
        "Welcome to your language learning journey!\n"
        "👆 Select a skill and click Start Lesson!",
        welcomePanel
    );
    QFont welcomeFont;
    welcomeFont.setPointSize(13);
    welcomeTextLabel->setFont(welcomeFont);
    welcomeTextLabel->setAlignment(Qt::AlignCenter);
    welcomeTextLabel->setWordWrap(true);
    welcomeTextLabel->setStyleSheet("QLabel { color: white; line-height: 1.6; }");
    welcomeLayout->addWidget(welcomeTextLabel);

    // Add spacing
    welcomeLayout->addSpacing(20);

    // Features section
    QLabel* featuresLabel = new QLabel(
        "✨ Built for fun learning:\n\n"
        "🎯 Interactive Exercises  •  📈 Track Your Progress",
        welcomePanel
    );
    QFont featuresFont;
    featuresFont.setPointSize(11);
    featuresLabel->setFont(featuresFont);
    featuresLabel->setAlignment(Qt::AlignCenter);
    featuresLabel->setWordWrap(true);
    featuresLabel->setStyleSheet("QLabel { color: white; opacity: 0.95; }");
    welcomeLayout->addWidget(featuresLabel);

    welcomeLayout->addStretch();
    welcomePanel->setLayout(welcomeLayout);
}

void LessonView::setWelcomePanelVisible(bool visible) {
    if (welcomePanel) {
        welcomePanel->setVisible(visible);
    }
}

void LessonView::updateExercise(Exercise* exercise) {
    if (!exercise) {
        promptLabel->setText("No exercise loaded");
        return;
    }

    currentExercise = exercise;

    // Hide welcome panel when exercise loads
    setWelcomePanelVisible(false);

    // Show exercise card now that we have an exercise
    exerciseCard->setVisible(true);

    // Clear previous widgets and feedback
    clearInputWidgets();
    feedbackCard->setVisible(false);
    feedbackLabel->setText("");
    setNextButtonVisible(false);
    submitButton->setEnabled(true);

    // Display prompt
    promptLabel->setText(exercise->getPrompt());

    // Add audio button if audio file exists
    if (!exercise->getAudioFile().isEmpty()) {
        QPushButton* audioBtn = new QPushButton("🔊 Listen", inputContainer);
        audioBtn->setMinimumHeight(40);
        audioBtn->setStyleSheet(
            "QPushButton { "
            "   background-color: #00BCD4; "
            "   color: white; "
            "   border: none; "
            "   border-radius: 10px; "
            "   padding: 10px 20px; "
            "   font-size: 13px; "
            "   font-weight: bold; "
            "} "
            "QPushButton:hover { "
            "   background-color: #00ACC1; "
            "} "
            "QPushButton:pressed { "
            "   background-color: #0097A7; "
            "}"
        );
        connect(audioBtn, &QPushButton::clicked, [=]() {
            QString audioPath = "assets/audio/" + exercise->getAudioFile();
            AudioManager::getInstance().playAudio(audioPath);
        });
        inputLayout->addWidget(audioBtn);
    }

    // Update exercise type badge
    QString type = exercise->getType();
    if (type == "MCQ") {
        typeLabel->setText("🎯 Multiple Choice");
        typeLabel->setStyleSheet(
            "QLabel { background-color: #FF9800; color: white; "
            "padding: 5px 15px; border-radius: 15px; "
            "font-size: 11px; font-weight: bold; }"
        );
        createMCQWidgets(exercise);
    } else if (type == "Translate") {
        typeLabel->setText("✍️ Translation");
        typeLabel->setStyleSheet(
            "QLabel { background-color: #9C27B0; color: white; "
            "padding: 5px 15px; border-radius: 15px; "
            "font-size: 11px; font-weight: bold; }"
        );
        createTranslateWidgets(exercise);
    } else if (type == "TileOrder") {
        typeLabel->setText("🔤 Word Order");
        typeLabel->setStyleSheet(
            "QLabel { background-color: #00BCD4; color: white; "
            "padding: 5px 15px; border-radius: 15px; "
            "font-size: 11px; font-weight: bold; }"
        );
        createTileOrderWidgets(exercise);
    } else {
        promptLabel->setText("Unknown exercise type: " + type);
    }

    // Add fade-in animation
    QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(this);
    promptLabel->setGraphicsEffect(effect);
    QPropertyAnimation* animation = new QPropertyAnimation(effect, "opacity");
    animation->setDuration(300);
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);
    animation->start(QPropertyAnimation::DeleteWhenStopped);
}

void LessonView::displayFeedback(const Result& result) {
    feedbackCard->setVisible(true);
    
    if (result.correct) {
        feedbackLabel->setText("✨ " + result.feedback);
        feedbackCard->setStyleSheet(
            "QWidget { "
            "   background: linear-gradient(135deg, #66BB6A 0%, #4CAF50 100%); "
            "   border-radius: 10px; "
            "   padding: 15px; "
            "}"
        );
        feedbackLabel->setStyleSheet("QLabel { color: white; }");
        
        // Add success animation
        QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(feedbackCard);
        feedbackCard->setGraphicsEffect(effect);
        QPropertyAnimation* animation = new QPropertyAnimation(effect, "opacity");
        animation->setDuration(500);
        animation->setStartValue(0.0);
        animation->setEndValue(1.0);
        animation->setEasingCurve(QEasingCurve::OutBounce);
        animation->start(QPropertyAnimation::DeleteWhenStopped);
    } else {
        feedbackLabel->setText("❌ " + result.feedback);
        feedbackCard->setStyleSheet(
            "QWidget { "
            "   background: linear-gradient(135deg, #EF5350 0%, #F44336 100%); "
            "   border-radius: 10px; "
            "   padding: 15px; "
            "}"
        );
        feedbackLabel->setStyleSheet("QLabel { color: white; }");
    }

    // Disable submit button, show next button
    submitButton->setEnabled(false);
    setNextButtonVisible(true);
}

void LessonView::updateProgress(int completed, int total) {
    // Update progress label (show as 1-based index)
    int displayIndex = (completed == 0) ? 1 : completed;
    progressLabel->setText(QString("Exercise %1 of %2").arg(displayIndex).arg(total));

    // Update progress bar with animation
    if (total > 0) {
        int percentage = (completed * 100) / total;
        
        // Animate progress bar
        QPropertyAnimation* animation = new QPropertyAnimation(progressBar, "value");
        animation->setDuration(500);
        animation->setStartValue(progressBar->value());
        animation->setEndValue(percentage);
        animation->setEasingCurve(QEasingCurve::InOutQuad);
        animation->start(QPropertyAnimation::DeleteWhenStopped);
    } else {
        progressBar->setValue(0);
    }
}

void LessonView::showCompletionScreen(int totalXP, int exercisesCompleted) {
    clearInputWidgets();
    typeLabel->setText("🎉 Congratulations!");
    typeLabel->setStyleSheet(
        "QLabel { background-color: #FFD700; color: #333; "
        "padding: 8px 20px; border-radius: 15px; "
        "font-size: 14px; font-weight: bold; }"
    );
    
    promptLabel->setText("🏆 Lesson Complete! 🏆");
    promptLabel->setStyleSheet(
        "QLabel { color: #333; font-size: 24px; font-weight: bold; "
        "padding: 20px; }"
    );
    
    feedbackCard->setVisible(true);
    feedbackLabel->setText(
        QString("Amazing work! You completed %1 exercises\n"
                "and earned %2 XP! Keep up the great progress!")
        .arg(exercisesCompleted)
        .arg(totalXP)
    );
    feedbackCard->setStyleSheet(
        "QWidget { "
        "   background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); "
        "   border-radius: 15px; "
        "   padding: 20px; "
        "}"
    );
    feedbackLabel->setStyleSheet(
        "QLabel { color: white; font-size: 16px; line-height: 1.5; }"
    );

    submitButton->setVisible(false);
    nextButton->setVisible(false);
    progressBar->setValue(100);
}

void LessonView::reset() {
    clearInputWidgets();
    feedbackCard->setVisible(false);
    feedbackLabel->setText("");
    promptLabel->setText("");
    progressBar->setValue(0);
    progressLabel->setText("Exercise 1 of 0");
    submitButton->setVisible(true);
    submitButton->setEnabled(true);
    nextButton->setVisible(false);
    currentExercise = nullptr;
    typeLabel->setText("");

    // Show welcome panel again
    setWelcomePanelVisible(true);
}

void LessonView::onSubmitClicked() {
    if (!currentExercise) {
        return;
    }

    QString answer = collectAnswer();

    if (answer.isEmpty()) {
        // Show inline warning instead of popup
        feedbackCard->setVisible(true);
        feedbackLabel->setText("⚠️ Please provide an answer before submitting");
        feedbackCard->setStyleSheet(
            "QWidget { "
            "   background-color: #FFC107; "
            "   border-radius: 10px; "
            "   padding: 15px; "
            "}"
        );
        feedbackLabel->setStyleSheet("QLabel { color: #333; }");
        return;
    }

    emit answerSubmitted(answer);
}

void LessonView::onNextClicked() {
    // Add fade-out effect before loading next
    feedbackCard->setVisible(false);
    emit nextExerciseRequested();
}

void LessonView::clearInputWidgets() {
    if (mcqButtonGroup) {
        delete mcqButtonGroup;
        mcqButtonGroup = nullptr;
    }
    qDeleteAll(mcqRadioButtons);
    mcqRadioButtons.clear();

    if (translateInput) {
        delete translateInput;
        translateInput = nullptr;
    }

    if (tileListWidget) {
        delete tileListWidget;
        tileListWidget = nullptr;
    }

    QLayoutItem* item;
    while ((item = inputLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
}

void LessonView::createMCQWidgets(Exercise* exercise) {
    MCQExercise* mcqExercise = dynamic_cast<MCQExercise*>(exercise);
    if (!mcqExercise) {
        promptLabel->setText("Error: Invalid MCQ exercise");
        return;
    }

    mcqButtonGroup = new QButtonGroup(this);
    QStringList options = mcqExercise->getOptions();

    for (int i = 0; i < options.size(); ++i) {
        QRadioButton* radioButton = new QRadioButton(options[i], inputContainer);
        QFont optionFont;
        optionFont.setPointSize(13);
        radioButton->setFont(optionFont);
        radioButton->setStyleSheet(
            "QRadioButton { "
            "   padding: 10px; "
            "   margin: 5px 0; "
            "   background-color: #f9f9f9; "
            "   border: 2px solid #e0e0e0; "
            "   border-radius: 8px; "
            "} "
            "QRadioButton:hover { "
            "   background-color: #e8f5e9; "
            "   border-color: #4CAF50; "
            "} "
            "QRadioButton::indicator { "
            "   width: 20px; "
            "   height: 20px; "
            "} "
            "QRadioButton::indicator:checked { "
            "   background-color: #4CAF50; "
            "   border: 2px solid #4CAF50; "
            "   border-radius: 10px; "
            "}"
        );

        mcqButtonGroup->addButton(radioButton, i);
        mcqRadioButtons.append(radioButton);
        inputLayout->addWidget(radioButton);
    }
}

void LessonView::createTranslateWidgets(Exercise* exercise) {
    (void)exercise;  // Parameter reserved for future use
    translateInput = new QLineEdit(inputContainer);
    QFont inputFont;
    inputFont.setPointSize(14);
    translateInput->setFont(inputFont);
    translateInput->setPlaceholderText("Type your translation here...");
    translateInput->setMinimumHeight(50);
    translateInput->setStyleSheet(
        "QLineEdit { "
        "   padding: 15px; "
        "   border: 2px solid #e0e0e0; "
        "   border-radius: 10px; "
        "   background-color: white; "
        "   font-size: 14px; "
        "} "
        "QLineEdit:focus { "
        "   border-color: #9C27B0; "
        "   outline: none; "
        "}"
    );

    connect(translateInput, &QLineEdit::returnPressed, this, &LessonView::onSubmitClicked);
    inputLayout->addWidget(translateInput);
    translateInput->setFocus();
}

void LessonView::createTileOrderWidgets(Exercise* exercise) {
    TileOrderExercise* tileExercise = dynamic_cast<TileOrderExercise*>(exercise);
    if (!tileExercise) {
        promptLabel->setText("Error: Invalid TileOrder exercise");
        return;
    }

    QLabel* instructionLabel = new QLabel("📝 Drag tiles to arrange them in the correct order:", inputContainer);
    instructionLabel->setStyleSheet(
        "QLabel { "
        "   color: #666; "
        "   font-size: 12px; "
        "   font-style: italic; "
        "   margin-bottom: 10px; "
        "}"
    );
    inputLayout->addWidget(instructionLabel);

    tileListWidget = new QListWidget(inputContainer);
    tileListWidget->setDragDropMode(QAbstractItemView::InternalMove);
    tileListWidget->setMinimumHeight(180);
    QFont tileFont;
    tileFont.setPointSize(13);
    tileListWidget->setFont(tileFont);
    tileListWidget->setStyleSheet(
        "QListWidget { "
        "   background-color: white; "
        "   border: 2px solid #e0e0e0; "
        "   border-radius: 10px; "
        "   padding: 10px; "
        "} "
        "QListWidget::item { "
        "   background-color: #00BCD4; "
        "   color: white; "
        "   border: none; "
        "   border-radius: 8px; "
        "   padding: 12px; "
        "   margin: 5px; "
        "   font-weight: bold; "
        "} "
        "QListWidget::item:hover { "
        "   background-color: #00ACC1; "
        "   cursor: move; "
        "} "
        "QListWidget::item:selected { "
        "   background-color: #0097A7; "
        "}"
    );

    QStringList tiles = tileExercise->getTiles();
    for (const QString& tile : tiles) {
        tileListWidget->addItem(tile);
    }

    inputLayout->addWidget(tileListWidget);
}

QString LessonView::collectAnswer() const {
    if (!currentExercise) {
        return QString();
    }

    QString type = currentExercise->getType();

    if (type == "MCQ") {
        if (mcqButtonGroup) {
            int selectedId = mcqButtonGroup->checkedId();
            if (selectedId >= 0) {
                return QString::number(selectedId);
            }
        }
    }
    else if (type == "Translate") {
        if (translateInput) {
            return translateInput->text().trimmed();
        }
    }
    else if (type == "TileOrder") {
        if (tileListWidget) {
            QStringList orderedTiles;
            for (int i = 0; i < tileListWidget->count(); ++i) {
                orderedTiles.append(tileListWidget->item(i)->text());
            }
            return orderedTiles.join(";");
        }
    }

    return QString();
}

void LessonView::setNextButtonVisible(bool visible) {
    nextButton->setVisible(visible);
    if (visible) {
        // Add slide-in animation for next button
        QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(nextButton);
        nextButton->setGraphicsEffect(effect);
        QPropertyAnimation* animation = new QPropertyAnimation(effect, "opacity");
        animation->setDuration(300);
        animation->setStartValue(0.0);
        animation->setEndValue(1.0);
        animation->start(QPropertyAnimation::DeleteWhenStopped);
    }
}
