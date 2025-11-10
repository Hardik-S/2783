#ifndef PROFILEVIEW_H
#define PROFILEVIEW_H

#include <QWidget>
#include <QLabel>
#include <QListWidget>
#include <QProgressBar>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QScrollArea>
#include "../core/domain/Profile.h"

/**
 * ProfileView - User profile and statistics display
 *
 * ARCHITECTURE: UI Layer
 * Purpose: Display user stats, XP, streak, and skill progress
 *
 * Key Responsibilities:
 * 1. Display username, XP, and streak
 * 2. Show mastery levels for each skill with progress bars
 * 3. Display accuracy statistics per skill
 * 4. Refresh display when profile is updated
 *
 * Data Flow:
 * 1. AppController emits profileUpdated(int xp, int streak) → updateStats()
 * 2. Manual refresh: call updateProfile(Profile*)
 *
 * Design Principles:
 * - Read-only view (no user input, just display)
 * - Automatically refreshes when profile data changes
 * - Clean, organized layout with visual hierarchy
 */
class ProfileView : public QWidget {
    Q_OBJECT

private:
    // UI Components - Profile Info Section
    QLabel* usernameLabel;              // Display username
    QLabel* xpLabel;                    // Display current XP
    QLabel* streakLabel;                // Display streak count
    QLabel* levelLabel;                 // Display level

    // UI Components - Skills Section
    QVBoxLayout* skillsLayout;          // Skills container layout

    // Layouts
    QVBoxLayout* mainLayout;

    // Current profile reference
    Profile* currentProfile;

public:
    explicit ProfileView(QWidget* parent = nullptr);
    ~ProfileView();

    /**
     * Update the entire profile display
     * Refreshes all stats and skill progress
     * @param profile - The user profile to display
     */
    void updateProfile(Profile* profile);

    /**
     * Update just the stats (XP, streak) without full refresh
     * Useful for responding to profileUpdated signal
     * @param xp - Current XP value
     * @param streak - Current streak value
     */
    void updateStats(int xp, int streak);

    /**
     * Reset the view to initial state
     */
    void reset();

private slots:
    /**
     * Handle time travel button click
     */
    void onTimeTravelClicked();

private:
    /**
     * Initialize UI components
     * Sets up layouts and widgets
     */
    void setupUI();

    /**
     * Refresh the skills list display
     * Shows all skills with progress bars and mastery percentages
     */
    void refreshSkillsList();

    /**
     * Create a skill item widget for the list
     * @param skillId - The skill identifier
     * @param progress - The skill progress data
     * @return Widget containing skill name, progress bar, and stats
     */
    QWidget* createSkillItemWidget(const QString& skillId, SkillProgress* progress);

    /**
     * Create a stat card for displaying metrics
     * @param icon - Icon/emoji for the stat
     * @param label - Label text
     * @param value - Value text
     * @param color - Border/accent color
     * @return Widget containing the stat card
     */
    QWidget* createStatCard(const QString& icon, const QString& label,
                           const QString& value, const QString& color);
};

#endif // PROFILEVIEW_H
