#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QFont>

/**
 * MainWindow Implementation
 *
 * This file implements the main application window with navigation between
 * LessonView and ProfileView, and wires all components together.
 */

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , appController(nullptr)
    , contentRepository(nullptr)
    , lessonView(nullptr)
    , profileView(nullptr)
    , stackedWidget(nullptr)
    , startLessonButton(nullptr)
    , viewProfileButton(nullptr)
    , backToLessonButton(nullptr)
    , skillSelector(nullptr)
{
    ui->setupUi(this);

    // Set window properties
    setWindowTitle("BhashaQuest - Language Learning");
    resize(800, 600);

    // Initialize components
    setupUI();
    loadContent();
    wireSignalsAndSlots();

    // Set initial view to lesson view
    stackedWidget->setCurrentIndex(0);

    // Update status bar with initial profile data
    updateStatusBar(appController->getProfile()->getCurrentXP(),
                    appController->getProfile()->getStreak());
}

MainWindow::~MainWindow()
{
    delete ui;
    // All other components deleted via Qt parent-child ownership
}

// ========== UI Setup ==========

void MainWindow::setupUI() {
    // Create central widget with main layout
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    // Create header section with improved styling
    QHBoxLayout* headerLayout = new QHBoxLayout();

    // Skill selector with enhanced label
    QLabel* skillLabel = new QLabel("📚 Select Skill:", this);
    QFont labelFont = skillLabel->font();
    labelFont.setPointSize(12);
    labelFont.setBold(true);
    skillLabel->setFont(labelFont);
    skillLabel->setStyleSheet("QLabel { color: #333; }");
    headerLayout->addWidget(skillLabel);

    skillSelector = new QComboBox(this);
    skillSelector->setMinimumWidth(220);
    skillSelector->setMinimumHeight(35);
    skillSelector->setStyleSheet(
        "QComboBox { "
        "   background-color: white; "
        "   color: #333; "
        "   border: 2px solid #ddd; "
        "   border-radius: 6px; "
        "   padding: 8px 12px; "
        "   font-size: 12px; "
        "} "
        "QComboBox:hover { "
        "   border: 2px solid #4CAF50; "
        "} "
        "QComboBox:focus { "
        "   border: 2px solid #2196F3; "
        "   outline: none; "
        "} "
        "QComboBox::drop-down { "
        "   border: none; "
        "   padding-right: 10px; "
        "} "
        "QComboBox::down-arrow { "
        "   image: none; "
        "   width: 10px; "
        "   height: 10px; "
        "}"
    );
    headerLayout->addWidget(skillSelector);

    headerLayout->addSpacing(25);

    // Start Lesson button - Enhanced styling
    startLessonButton = new QPushButton("▶ Start Lesson", this);
    startLessonButton->setMinimumWidth(140);
    startLessonButton->setMinimumHeight(40);
    startLessonButton->setStyleSheet(
        "QPushButton { "
        "   background-color: #4CAF50; "
        "   color: white; "
        "   border: none; "
        "   border-radius: 8px; "
        "   padding: 12px 20px; "
        "   font-size: 13px; "
        "   font-weight: bold; "
        "   transition: all 0.3s ease; "
        "} "
        "QPushButton:hover { "
        "   background-color: #45a049; "
        "   box-shadow: 0 4px 12px rgba(76, 175, 80, 0.4); "
        "   padding: 12px 24px; "
        "} "
        "QPushButton:pressed { "
        "   background-color: #3d8b40; "
        "   padding: 13px 19px; "
        "} "
        "QPushButton:disabled { "
        "   background-color: #cccccc; "
        "   color: #666666; "
        "}"
    );
    headerLayout->addWidget(startLessonButton);

    headerLayout->addSpacing(15);

    // View Profile button - Enhanced styling
    viewProfileButton = new QPushButton("👤 View Profile", this);
    viewProfileButton->setMinimumWidth(140);
    viewProfileButton->setMinimumHeight(40);
    viewProfileButton->setStyleSheet(
        "QPushButton { "
        "   background-color: #2196F3; "
        "   color: white; "
        "   border: none; "
        "   border-radius: 8px; "
        "   padding: 12px 20px; "
        "   font-size: 13px; "
        "   font-weight: bold; "
        "   transition: all 0.3s ease; "
        "} "
        "QPushButton:hover { "
        "   background-color: #1976D2; "
        "   box-shadow: 0 4px 12px rgba(33, 150, 243, 0.4); "
        "   padding: 12px 24px; "
        "} "
        "QPushButton:pressed { "
        "   background-color: #0D47A1; "
        "   padding: 13px 19px; "
        "} "
        "QPushButton:disabled { "
        "   background-color: #cccccc; "
        "   color: #666666; "
        "}"
    );
    headerLayout->addWidget(viewProfileButton);

    headerLayout->addStretch();

    mainLayout->addLayout(headerLayout);

    // Create stacked widget to switch between views
    stackedWidget = new QStackedWidget(this);

    // Create core components
    // DRY Principle: ContentRepository resolves path automatically
    contentRepository = new ContentRepository("content.json");
    appController = new AppController(this);

    // Create views
    lessonView = new LessonView(this);
    profileView = new ProfileView(this);

    // Add views to stacked widget
    stackedWidget->addWidget(lessonView);      // Index 0
    stackedWidget->addWidget(profileView);     // Index 1

    mainLayout->addWidget(stackedWidget);

    // Create back button for profile view (initially hidden)
    backToLessonButton = new QPushButton("← Back to Lesson", this);
    backToLessonButton->setVisible(false);
    backToLessonButton->setMinimumHeight(35);
    backToLessonButton->setStyleSheet(
        "QPushButton { "
        "   background-color: #757575; "
        "   color: white; "
        "   border: none; "
        "   border-radius: 8px; "
        "   padding: 10px 15px; "
        "   font-size: 12px; "
        "   font-weight: bold; "
        "} "
        "QPushButton:hover { "
        "   background-color: #616161; "
        "   box-shadow: 0 2px 8px rgba(0, 0, 0, 0.2); "
        "} "
        "QPushButton:pressed { "
        "   background-color: #4a4a4a; "
        "}"
    );
    mainLayout->addWidget(backToLessonButton);

    setCentralWidget(centralWidget);

    // Setup status bar with initial message
    statusBar()->showMessage("🎓 Welcome to BhashaQuest! Ready to start learning?");
}

void MainWindow::wireSignalsAndSlots() {
    // Button connections
    connect(startLessonButton, &QPushButton::clicked, this, &MainWindow::onStartLessonClicked);
    connect(viewProfileButton, &QPushButton::clicked, this, &MainWindow::onViewProfileClicked);
    connect(backToLessonButton, &QPushButton::clicked, this, &MainWindow::onBackToLessonClicked);

    // LessonView → AppController connections
    connect(lessonView, &LessonView::answerSubmitted,
            appController, &AppController::submitAnswer);
    connect(lessonView, &LessonView::nextExerciseRequested,
            appController, &AppController::loadNextExercise);

    // AppController → LessonView connections
    connect(appController, &AppController::exerciseChanged,
            lessonView, &LessonView::updateExercise);
    connect(appController, &AppController::answerGraded,
            lessonView, &LessonView::displayFeedback);
    connect(appController, &AppController::progressUpdated,
            lessonView, &LessonView::updateProgress);
    connect(appController, &AppController::lessonCompleted,
            this, &MainWindow::onLessonCompleted);
    connect(appController, &AppController::lessonCompleted,
            lessonView, &LessonView::showCompletionScreen);

    // AppController → ProfileView connections
    connect(appController, &AppController::profileUpdated,
            profileView, &ProfileView::updateStats);
    connect(appController, &AppController::profileUpdated,
            this, &MainWindow::updateStatusBar);
}

void MainWindow::loadContent() {
    // Load exercises from JSON
    if (!contentRepository->loadContent()) {
        QMessageBox::critical(this, "Error", "Failed to load content from content.json");
        return;
    }

    // Populate skill selector
    populateSkillSelector();

    // Initialize profile view with current profile
    profileView->updateProfile(appController->getProfile());
}

void MainWindow::populateSkillSelector() {
    // Clear existing items
    skillSelector->clear();

    // Add placeholder item first
    skillSelector->addItem("-- Select a Skill to Begin --", "");  // Empty skillId for placeholder

    // Get available skills
    QList<QString> skills = contentRepository->getAvailableSkills();

    if (skills.isEmpty()) {
        skillSelector->addItem("No skills available");
        startLessonButton->setEnabled(false);
        return;
    }

    // Add skills to combo box
    for (const QString& skillId : skills) {
        QString skillName = contentRepository->getSkillName(skillId);
        QString language = contentRepository->getSkillLanguage(skillId);
        QString displayText = QString("%1 (%2)").arg(skillName).arg(language);

        skillSelector->addItem(displayText, skillId);  // Store skillId as user data
    }

    // Default to placeholder (index 0), user must select a skill
    skillSelector->setCurrentIndex(0);
    currentSkillId = "";  // No skill selected initially
}

// ========== Private Slots ==========

void MainWindow::onStartLessonClicked() {
    // Get selected skill
    int currentIndex = skillSelector->currentIndex();
    if (currentIndex < 0) {
        QMessageBox::warning(this, "No Skill Selected", "Please select a skill first.");
        return;
    }

    currentSkillId = skillSelector->currentData().toString();

    // Check if placeholder is selected (empty skillId)
    if (currentSkillId.isEmpty()) {
        QMessageBox::information(this, "Select a Skill",
            "Please select a skill from the dropdown to begin your lesson.");
        return;
    }

    // Get exercises for selected skill
    QList<Exercise*> exercises = contentRepository->getExercisesForSkill(currentSkillId);

    if (exercises.isEmpty()) {
        QMessageBox::warning(this, "No Exercises", "No exercises available for this skill.");
        return;
    }

    // Reset lesson view
    lessonView->reset();

    // Start lesson
    // 3am: appController->startLesson(exercises);
    appController->startLesson(currentSkillId, exercises);

    // Update progress display
    lessonView->updateProgress(0, exercises.size());

    // Switch to lesson view
    stackedWidget->setCurrentIndex(0);
    backToLessonButton->setVisible(false);

    // Update status
    statusBar()->showMessage("▶️ Lesson started! Good luck! 💪");
}

void MainWindow::onViewProfileClicked() {
    // Update profile view with latest data
    profileView->updateProfile(appController->getProfile());

    // Switch to profile view
    stackedWidget->setCurrentIndex(1);
    backToLessonButton->setVisible(true);

    // Update status
    statusBar()->showMessage("👤 Viewing your profile...");
}

void MainWindow::onBackToLessonClicked() {
    // Switch back to lesson view
    stackedWidget->setCurrentIndex(0);
    backToLessonButton->setVisible(false);

    // Update status
    statusBar()->showMessage("📚 Back to lessons");
}

void MainWindow::onLessonCompleted(int totalXP, int exercisesCompleted) {
    // Update status bar
    statusBar()->showMessage(QString("🎉 Lesson complete! Earned ⭐ %1 XP").arg(totalXP));

    // Show completion message
    QMessageBox::information(this, "Lesson Complete!",
                             QString("Congratulations! You completed %1 exercises and earned %2 XP.")
                             .arg(exercisesCompleted)
                             .arg(totalXP));

    // Refresh profile view
    profileView->updateProfile(appController->getProfile());
}

void MainWindow::updateStatusBar(int xp, int streak) {
    // Update status bar with current stats and emojis
    statusBar()->showMessage(
        QString("⭐ XP: %1  |  🔥 Streak: %2 days").arg(xp).arg(streak)
    );
}
