#ifndef LESSONVIEW_H
#define LESSONVIEW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QButtonGroup>
#include <QLineEdit>
#include <QListWidget>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include "../core/domain/Exercise.h"
#include "../core/domain/Result.h"
#include "../core/domain/TranslateExercise.h"

/**
 * LessonView - Main UI component for displaying and interacting with exercises
 *
 * ARCHITECTURE: UI Layer
 * Purpose: Display exercises, collect user input, show feedback
 *
 * Key Responsibilities:
 * 1. Display exercise prompt and dynamic input widgets
 * 2. Handle user interactions (submit button, input widgets)
 * 3. Show grading feedback (correct/incorrect with color coding)
 * 4. Display session progress (N/M exercises completed)
 * 5. Communicate with AppController via Qt signals/slots
 *
 * Data Flow:
 * 1. AppController emits exerciseChanged(Exercise*) → updateExercise()
 * 2. User clicks Submit → emit answerSubmitted(QString)
 * 3. AppController emits answerGraded(Result) → displayFeedback()
 * 4. AppController emits lessonCompleted() → showCompletionScreen()
 *
 * Design Principles:
 * - UI only communicates with AppController, never touches domain objects directly
 * - Dynamic widget creation based on Exercise type (polymorphism)
 * - Clean separation: rendering logic in view, business logic in controller
 */
class CharacterSelectionWidget;

class LessonView : public QWidget {
    Q_OBJECT

private:
    // UI Components - Static (always visible)
    QLabel* promptLabel;                // Displays exercise prompt
    QLabel* feedbackLabel;              // Shows "Correct!" or "Incorrect" with feedback
    QProgressBar* progressBar;          // Shows N/M progress
    QPushButton* submitButton;          // Submit answer button
    QPushButton* nextButton;            // Next exercise button (visible after grading)
    QPushButton* nextLessonButton;      // Start next lesson button (shown on completion)
    QLabel* progressLabel;              // Text label "Exercise N of M"
    QLabel* typeLabel;                  // Exercise type badge
    QWidget* feedbackCard;              // Feedback container
    QWidget* welcomePanel;              // Welcome screen shown initially
    QWidget* exerciseCard;              // Main exercise container

    // UI Components - Dynamic (change per exercise type)
    QWidget* inputContainer;            // Container for dynamic input widgets
    QVBoxLayout* mainLayout;            // Main vertical layout
    QVBoxLayout* inputLayout;           // Layout for input widgets

    // MCQ-specific widgets
    QButtonGroup* mcqButtonGroup;       // Radio button group for MCQ
    QList<QRadioButton*> mcqRadioButtons;

    // Translate-specific widgets
    QLineEdit* translateInput;          // Text input for translation

    CharacterSelectionWidget* characterSelectionWidget; // Character picker for script entry

    // TileOrder-specific widgets
    QListWidget* tileListWidget;        // Draggable list for tile ordering

    // Current exercise state
    Exercise* currentExercise;          // Current exercise being displayed

public:
    explicit LessonView(QWidget* parent = nullptr);
    ~LessonView();

    /**
     * Update the view to display a new exercise
     * Dynamically creates appropriate input widgets based on exercise type
     * @param exercise - The exercise to display
     */
    void updateExercise(Exercise* exercise);

    /**
     * Display grading feedback after answer submission
     * Shows correct/incorrect message with color coding
     * @param result - The grading result from AppController
     */
    void displayFeedback(const Result& result);

    /**
     * Update the progress bar and label
     * @param completed - Number of exercises completed
     * @param total - Total number of exercises in session
     */
    void updateProgress(int completed, int total);

    /**
     * Show lesson completion screen with stats
     * @param totalXP - Total XP earned in session
     * @param exercisesCompleted - Number of exercises completed
     */
    void showCompletionScreen(int totalXP, int exercisesCompleted);

    /**
     * Reset the view to initial state
     * Clears all dynamic widgets and feedback
     */
    void reset();

signals:
    /**
     * Emitted when user clicks Submit button
     * @param answer - The user's answer
     */
    void answerSubmitted(const QString& answer);

    /**
     * Emitted when user clicks Next button
     */
    void nextExerciseRequested();

    /**
     * Emitted when user clicks "Start New Lesson" on completion screen
     */
    void newLessonRequested();

private slots:
    /**
     * Handle submit button click
     * Collects user input and emits answerSubmitted signal
     */
    void onSubmitClicked();

    /**
     * Handle next button click
     * Emits nextExerciseRequested signal
     */
    void onNextClicked();

private:
    /**
     * Initialize UI components
     * Sets up static layout and widgets
     */
    void setupUI();

    /**
     * Create and setup the welcome panel
     * Displayed initially before any lesson starts
     */
    void createWelcomePanel();

    /**
     * Show or hide the welcome panel
     * @param visible - true to show, false to hide
     */
    void setWelcomePanelVisible(bool visible);

    /**
     * Clear dynamic input widgets
     * Removes MCQ/Translate/TileOrder widgets from previous exercise
     */
    void clearInputWidgets();

    /**
     * Create MCQ input widgets
     * Creates radio buttons for multiple choice options
     * @param exercise - MCQ exercise (must be cast to MCQExercise*)
     */
    void createMCQWidgets(Exercise* exercise);

    /**
     * Create Translate input widgets
     * Creates text input field for translation
     * @param exercise - Translate exercise
     */
    void createTranslateWidgets(Exercise* exercise);

    /**
     * Create TileOrder input widgets
     * Creates draggable list widget for tile reordering
     * @param exercise - TileOrder exercise (must be cast to TileOrderExercise*)
     */
    void createTileOrderWidgets(Exercise* exercise);

    /**
     * Collect user answer from current input widgets
     * Handles different widget types (MCQ, Translate, TileOrder)
     * @return User's answer as QString
     */
    QString collectAnswer() const;

    /**
     * Show or hide the Next button
     * @param visible - true to show, false to hide
     */
    void setNextButtonVisible(bool visible);
};

#endif // LESSONVIEW_H
