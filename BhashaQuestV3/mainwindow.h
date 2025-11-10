#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QComboBox>
#include <QStatusBar>
#include "src/core/controllers/AppController.h"
#include "src/core/data/ContentRepository.h"
#include "src/ui/LessonView.h"
#include "src/ui/ProfileView.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/**
 * MainWindow - Main application window
 *
 * ARCHITECTURE: UI Layer (Top-level window)
 * Purpose: Manage navigation between views and coordinate all components
 *
 * Key Responsibilities:
 * 1. Create and manage AppController, ContentRepository, and Views
 * 2. Provide navigation between LessonView and ProfileView
 * 3. Display status bar with quick stats
 * 4. Wire all signal/slot connections
 * 5. Handle lesson selection and starting
 *
 * Component Ownership:
 * - Owns AppController, ContentRepository, LessonView, ProfileView
 * - All components deleted automatically via Qt parent-child ownership
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;

    // Core components
    AppController* appController;
    ContentRepository* contentRepository;

    // Views
    LessonView* lessonView;
    ProfileView* profileView;

    // Navigation
    QStackedWidget* stackedWidget;      // Switch between lesson and profile views

    // Controls
    QPushButton* startLessonButton;
    QPushButton* viewProfileButton;
    QPushButton* backToLessonButton;
    QComboBox* skillSelector;

    // Current state
    QString currentSkillId;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /**
     * Initialize all UI components
     * Sets up layouts, buttons, and status bar
     */
    void setupUI();

    /**
     * Wire all signal/slot connections
     * Connects AppController signals to view slots and vice versa
     */
    void wireSignalsAndSlots();

    /**
     * Load content from JSON file
     * Populates skill selector and prepares exercises
     */
    void loadContent();

    /**
     * Populate skill selector dropdown
     * Fills combo box with available skills from ContentRepository
     */
    void populateSkillSelector();

private slots:
    /**
     * Handle "Start Lesson" button click
     * Starts a lesson for the selected skill
     */
    void onStartLessonClicked();

    /**
     * Handle "View Profile" button click
     * Switches to profile view
     */
    void onViewProfileClicked();

    /**
     * Handle "Back to Lesson" button click
     * Switches back to lesson view
     */
    void onBackToLessonClicked();

    /**
     * Handle lesson completion
     * Shows completion message and updates status bar
     */
    void onLessonCompleted(int totalXP, int exercisesCompleted);

    /**
     * Update status bar with current stats
     * Shows XP and streak in status bar
     */
    void updateStatusBar(int xp, int streak);
};
#endif // MAINWINDOW_H
