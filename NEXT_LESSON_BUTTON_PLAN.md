# Next Lesson Button - Implementation Plan

## Overview
Add a "Next Lesson" button to the lesson completion screen that allows users to immediately start another lesson session with the same skill, without needing to return to the skill selector.

## Visual Reference
Based on screenshot.png - The button should be positioned at the **orange line** location:
- Between the "🏆 Lesson Complete! 🏆" heading
- And the XP summary text

## Current State Analysis

### Existing Components
1. **LessonView** (`src/ui/LessonView.h/cpp`):
   - Already has `newLessonRequested()` signal defined (line 130)
   - Has `showCompletionScreen(int totalXP, int exercisesCompleted)` method (line 455-491)
   - Currently hides both submit and next buttons on completion

2. **MainWindow** (`mainwindow.h/cpp`):
   - Tracks `currentSkillId` member variable (line 62)
   - Has `onStartLessonClicked()` slot that starts lessons (line 301-342)
   - Already wired to handle various signals from LessonView

3. **ContentRepository** (`src/core/data/ContentRepository.h`):
   - Can provide exercises for any skill via `getExercisesForSkill(skillId)`
   - Returns ALL exercises for a skill each time (allows repetition)

### Current User Flow
1. User selects skill from dropdown
2. Clicks "Start Lesson"
3. Completes all exercises
4. Sees completion screen with congratulations
5. Must go back and click "Start Lesson" again to practice the same skill

### Desired User Flow
1. User selects skill from dropdown
2. Clicks "Start Lesson"
3. Completes all exercises
4. Sees completion screen with "Next Lesson" button
5. **Clicks "Next Lesson" to immediately start another session with the same skill** ⬅️ NEW

## Implementation Plan

### Phase 1: LessonView Changes

#### 1.1 Update LessonView.h
**File**: `BhashaQuest/src/ui/LessonView.h`

**Changes**:
- Add new member variable (around line 52, with other buttons):
  ```cpp
  QPushButton* nextLessonButton;  // Start next lesson button (shown on completion)
  ```

#### 1.2 Update LessonView.cpp

**File**: `BhashaQuest/src/ui/LessonView.cpp`

**Change 1 - Initialize in constructor** (line 16-24):
```cpp
LessonView::LessonView(QWidget* parent)
    : QWidget(parent)
    , mcqButtonGroup(nullptr)
    , translateInput(nullptr)
    , tileListWidget(nullptr)
    , currentExercise(nullptr)
    , nextLessonButton(nullptr)  // ADD THIS LINE
{
    setupUI();
}
```

**Change 2 - Create button in setupUI()** (around line 220, after nextButton):
```cpp
// Create Next Lesson button (shown on completion screen only)
nextLessonButton = new QPushButton("➜ Start Next Lesson", this);
nextLessonButton->setMinimumHeight(50);
nextLessonButton->setVisible(false);  // Initially hidden
nextLessonButton->setStyleSheet(
    "QPushButton { "
    "   background-color: #FF9800; "  // Orange to match the line in screenshot
    "   color: white; "
    "   border: none; "
    "   border-radius: 25px; "
    "   padding: 15px 30px; "
    "   font-size: 14px; "
    "   font-weight: bold; "
    "   text-transform: uppercase; "
    "} "
    "QPushButton:hover { "
    "   background-color: #FB8C00; "
    "   transform: scale(1.02); "
    "} "
    "QPushButton:pressed { "
    "   background-color: #F57C00; "
    "}"
);
connect(nextLessonButton, &QPushButton::clicked, [this]() {
    emit newLessonRequested();
});

// NOTE: Don't add to layout yet - will be added in showCompletionScreen()
```

**Change 3 - Modify showCompletionScreen()** (line 455-491):

Replace the entire function with:
```cpp
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

    // ADD NEXT LESSON BUTTON HERE (at orange line location)
    if (nextLessonButton) {
        nextLessonButton->setVisible(true);
        inputLayout->addWidget(nextLessonButton);

        // Add fade-in animation
        QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(nextLessonButton);
        nextLessonButton->setGraphicsEffect(effect);
        QPropertyAnimation* animation = new QPropertyAnimation(effect, "opacity");
        animation->setDuration(400);
        animation->setStartValue(0.0);
        animation->setEndValue(1.0);
        animation->setEasingCurve(QEasingCurve::OutCubic);
        animation->start(QPropertyAnimation::DeleteWhenStopped);
    }

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
```

**Change 4 - Update reset()** (line 493-508):
```cpp
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
    nextLessonButton->setVisible(false);  // ADD THIS LINE
    currentExercise = nullptr;
    typeLabel->setText("");

    // Show welcome panel again
    setWelcomePanelVisible(true);
}
```

### Phase 2: MainWindow Changes

#### 2.1 Update mainwindow.h
**File**: `BhashaQuest/mainwindow.h`

**Changes**:
- Add new private slot declaration (around line 122, after other slots):
  ```cpp
  /**
   * Handle "Next Lesson" request from completion screen
   * Starts a new lesson session with the same skill
   */
  void onNewLessonRequested();
  ```

#### 2.2 Update mainwindow.cpp
**File**: `BhashaQuest/mainwindow.cpp`

**Change 1 - Wire signal in wireSignalsAndSlots()** (around line 234):
```cpp
// LessonView → MainWindow connection for next lesson
connect(lessonView, &LessonView::newLessonRequested,
        this, &MainWindow::onNewLessonRequested);
```

**Change 2 - Implement onNewLessonRequested()** (add after onLessonCompleted, around line 378):
```cpp
void MainWindow::onNewLessonRequested() {
    // Check if we have a current skill selected
    if (currentSkillId.isEmpty()) {
        QMessageBox::information(this, "No Skill Selected",
            "Please select a skill from the dropdown to start a lesson.");

        // Switch back to lesson view so user can select a skill
        stackedWidget->setCurrentIndex(0);
        backToLessonButton->setVisible(false);
        return;
    }

    // Get exercises for the same skill
    QList<Exercise*> exercises = contentRepository->getExercisesForSkill(currentSkillId);

    if (exercises.isEmpty()) {
        QMessageBox::warning(this, "No Exercises",
            "No exercises available for this skill.");
        return;
    }

    // Reset lesson view
    lessonView->reset();

    // Start new lesson with same skill
    appController->startLesson(currentSkillId, exercises);

    // Update progress display
    lessonView->updateProgress(0, exercises.size());

    // Ensure we're on lesson view
    stackedWidget->setCurrentIndex(0);
    backToLessonButton->setVisible(false);

    // Update status
    QString skillName = contentRepository->getSkillName(currentSkillId);
    statusBar()->showMessage(
        QString("▶️ Starting next lesson for %1! Keep up the momentum! 🚀").arg(skillName)
    );
}
```

## Testing Checklist

### Test Case 1: Basic Flow
1. ✅ Launch application
2. ✅ Select a skill (e.g., "Greetings (Nepali)")
3. ✅ Click "Start Lesson"
4. ✅ Complete all exercises
5. ✅ Verify completion screen shows with:
   - Congratulations message
   - **"➜ Start Next Lesson" button** (orange, positioned at orange line location)
   - XP summary below the button
6. ✅ Click "Start Next Lesson"
7. ✅ Verify new lesson session starts with same skill
8. ✅ Verify status bar updates
9. ✅ Complete lesson again and repeat

### Test Case 2: Visual Verification
1. ✅ Completion screen layout matches screenshot expectation:
   - Progress bar (100%)
   - "🎉 Congratulations!" badge
   - "🏆 Lesson Complete! 🏆" heading
   - **Orange "Next Lesson" button** ⬅️ AT ORANGE LINE LOCATION
   - XP summary text below
2. ✅ Button has fade-in animation
3. ✅ Button has hover effect (darkens on hover)
4. ✅ Button styling is consistent with app theme

### Test Case 3: Edge Cases
1. ✅ Complete lesson, navigate to profile view, come back - button should not appear
2. ✅ Click "Next Lesson" → verify exercises reload correctly
3. ✅ Check that XP continues to accumulate across multiple lesson sessions
4. ✅ Verify profile view shows updated mastery after multiple sessions

### Test Case 4: Multiple Skills
1. ✅ Complete lesson for Skill A
2. ✅ Click "Next Lesson" → verify starts lesson for Skill A again (not Skill B)
3. ✅ Select Skill B from dropdown
4. ✅ Start lesson for Skill B
5. ✅ Complete and verify "Next Lesson" starts Skill B session

## File Modifications Summary

### Files to Modify
1. `BhashaQuestV3/src/ui/LessonView.h` - Add member variable
2. `BhashaQuestV3/src/ui/LessonView.cpp` - Add button creation, completion screen update, reset update
3. `BhashaQuestV3/mainwindow.h` - Add slot declaration
4. `BhashaQuestV3/mainwindow.cpp` - Add signal connection and slot implementation

### No Changes Needed
- `AppController` - Already handles lesson restart correctly
- `ContentRepository` - Already returns all exercises for a skill
- Other UI components

## Design Decisions

### Button Color: Orange (#FF9800)
**Rationale**: Matches the orange line in the screenshot, creates visual continuity, and stands out as a call-to-action without clashing with existing green (submit) and blue (profile) buttons.

### Button Position: Between heading and XP summary
**Rationale**: Exactly matches the orange line location in the screenshot, creates logical flow (complete → celebrate → continue).

### Button Text: "➜ Start Next Lesson"
**Rationale**:
- Clear action verb "Start"
- Indicates it's a "Next" lesson (implies continuation)
- Arrow emoji suggests forward movement
- Matches existing button text patterns in the app

### Reuse Same Skill Logic
**Rationale**:
- Allows practice/repetition (beneficial for language learning)
- Maintains user context (they just completed this skill)
- Simpler UX (no need to select skill again)
- If user wants different skill, they can use the dropdown

### Signal Reuse: `newLessonRequested()`
**Rationale**: Signal already exists in LessonView, just needed wiring - follows existing architecture and reduces code changes.

## Implementation Estimate
- **Complexity**: Low-Medium
- **Lines Changed**: ~80 lines total
- **Time Estimate**: 30-45 minutes
- **Testing Time**: 15-20 minutes
- **Total**: ~1 hour

## Dependencies
- Qt6 (already in use)
- No new dependencies required
- All required signals/slots already exist in framework

## Future Enhancements (Out of Scope)
1. Add "Choose Different Skill" button alongside "Next Lesson"
2. Track completion count per skill and show progress
3. Suggest next skill when current skill is mastered
4. Add difficulty progression (easy → hard lessons)

## Notes
- The existing `newLessonRequested()` signal was already defined but never wired - this plan completes the implementation
- Button animation matches existing patterns in LessonView
- All styling uses existing app color scheme patterns
- Implementation maintains clean separation: UI layer (LessonView) emits signals, Controller layer (MainWindow) handles logic
