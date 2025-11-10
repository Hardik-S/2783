#ifndef CHARACTERSELECTIONWIDGET_H
#define CHARACTERSELECTIONWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStringList>
#include <QList>

/**
 * CharacterSelectionWidget - UI component for character selection exercises
 *
 * ARCHITECTURE: UI Layer
 * Purpose: Display character bank and manage character selection for translation exercises
 *
 * Key Responsibilities:
 * 1. Display character bank as selectable buttons
 * 2. Show selected characters in order
 * 3. Allow undo/clear operations
 * 4. Return selected sequence for grading
 * 5. Provide visual feedback for selection state
 *
 * UI Layout:
 * ┌─────────────────────────────────┐
 * │ Selected: [न] [म] [स] | Undo Clear│
 * ├─────────────────────────────────┤
 * │ Available Characters:            │
 * │ ┌──┐ ┌──┐ ┌──┐ ┌──┐ ┌──┐ ┌──┐  │
 * │ │न│ │स│ │त│ │क│ │य│ │ध│      │
 * │ └──┘ └──┘ └──┘ └──┘ └──┘ └──┘  │
 * └─────────────────────────────────┘
 *
 * Data Flow:
 * 1. Created by TranslateExercise::renderUI()
 * 2. Constructor generates character bank from correct answer
 * 3. User clicks characters → selected sequence updated
 * 4. LessonView calls getSelectedSequence() → returns semicolon-separated chars
 * 5. AppController submits to CharacterSelectionGrader
 */
class CharacterSelectionWidget : public QWidget {
    Q_OBJECT

private:
    // UI Components
    QLabel* selectedLabel;              // Shows selected characters
    QLabel* availableLabel;             // Label for available characters section
    QPushButton* undoButton;            // Undo last selection
    QPushButton* clearButton;           // Clear all selections
    QWidget* characterButtonsContainer; // Container for character buttons
    QHBoxLayout* charactersLayout;      // Layout for character buttons

    // Data state
    QString correctAnswer;              // The correct answer (for reference)
    QString targetLanguage;             // Target language (Nepali, Kannada, etc.)
    QStringList characterBank;          // All available characters (shuffled)
    QStringList selectedSequence;       // Characters selected by user in order
    QStringList availableCharacters;    // Characters still available to select

    // Character buttons
    QList<QPushButton*> characterButtons;

public:
    /**
     * Constructor - Initialize character selection widget
     *
     * @param correctAnswer The correct answer string (used to generate bank)
     * @param languageCharacterSet Available characters for the target language
     * @param targetLanguage Language name (e.g., "Nepali", "Kannada")
     * @param parent Parent widget
     */
    explicit CharacterSelectionWidget(
        const QString& correctAnswer,
        const QStringList& languageCharacterSet,
        const QString& targetLanguage = "Nepali",
        QWidget* parent = nullptr
    );

    ~CharacterSelectionWidget();

    /**
     * Get the selected character sequence
     * @return Semicolon-separated string of selected characters (e.g., "न;म;स")
     */
    QString getSelectedSequence() const;

    /**
     * Reset all selections
     * Clears selected sequence and makes all characters available again
     */
    void reset();

    /**
     * Enable or disable the widget
     * Disables all buttons during feedback display
     * @param enabled true to enable, false to disable
     */
    void setEnabled(bool enabled);

    /**
     * Get the number of selected characters
     * @return Count of selected characters
     */
    int getSelectionCount() const { return selectedSequence.length(); }

signals:
    /**
     * Emitted when a character is selected
     */
    void characterSelected();

private slots:
    /**
     * Handle character button click
     * Adds selected character to sequence
     */
    void onCharacterClicked();

    /**
     * Handle undo button click
     * Removes last selected character
     */
    void onUndoClicked();

    /**
     * Handle clear button click
     * Clears all selections
     */
    void onClearClicked();

private:
    /**
     * Initialize UI components
     * Sets up layout with selected sequence display and character buttons
     */
    void setupUI();

    /**
     * Create character bank and display buttons
     * Generates shuffled character bank and creates clickable buttons
     */
    void createCharacterBank();

    /**
     * Update UI state after selection change
     * Refreshes selected label and button states
     */
    void updateUIState();

    /**
     * Update the selected sequence display label
     */
    void updateSelectedLabel();

    /**
     * Update button availability states
     * Disables buttons for already-selected characters
     */
    void updateButtonStates();
};

#endif // CHARACTERSELECTIONWIDGET_H
