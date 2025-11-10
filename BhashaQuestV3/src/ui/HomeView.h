#ifndef HOMEVIEW_H
#define HOMEVIEW_H

#include <QWidget>

namespace Ui {
class HomeView;
}

/**
 * HomeView - Landing screen for BhashaQuest
 *
 * ARCHITECTURE: UI Layer (Home/Landing Screen)
 * Purpose: Provide simple navigation to main app features
 *
 * DESIGN PRINCIPLES:
 * - KISS: Simple layout with two clear navigation options
 * - YAGNI: Only includes essential navigation, no extra features
 * - DRY: Reuses consistent styling patterns
 * - SOLID: Single responsibility - navigation only
 *
 * Key Responsibilities:
 * 1. Display welcome message and app branding
 * 2. Provide "Start Learning" button to navigate to lessons
 * 3. Provide "View Profile" button to navigate to profile
 * 4. Emit signals for navigation requests
 */
class HomeView : public QWidget
{
    Q_OBJECT

public:
    explicit HomeView(QWidget *parent = nullptr);
    ~HomeView();

signals:
    /**
     * Emitted when user wants to start a lesson
     */
    void startLearningRequested();

    /**
     * Emitted when user wants to view their profile
     */
    void viewProfileRequested();

private slots:
    /**
     * Handle "Start Learning" button click
     */
    void onStartLearningClicked();

    /**
     * Handle "View Profile" button click
     */
    void onViewProfileClicked();

private:
    Ui::HomeView *ui;

    /**
     * Setup UI components and styling
     */
    void setupUI();
};

#endif // HOMEVIEW_H
