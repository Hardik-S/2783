# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Principles

Always apply SOLID, DRY, KISS, and YAGNI principles. Always respect course requirements.

## Repository Overview

This is a workspace for CS 3307 (Software Engineering) coursework at Western University. The primary project is **Bhasha Quest**, a language learning application built with C++ and Qt6.

**📍 CURRENT STATUS:**
- **Phase Completed:** 4.3 (Main Application Wiring)
- **Last Update:** MainWindow and main.cpp wired with all components
- **Next Phase:** 4.4 Assets (placeholder audio files) - Optional
- **Overall Progress:** ~90% complete (Domain ✅ + Controller ✅ + Data ✅ + UI ✅ + Wiring ✅)

### Workspace Structure

```
ClaudeWorkDirectory/7807/
├── BhashaQuest/                   # 🎯 MAIN PROJECT (ACTIVE)
│   ├── src/
│   │   ├── core/
│   │   │   ├── domain/            # ✅ COMPLETED (27 files)
│   │   │   │   ├── Exercise.h/cpp, MCQExercise.h/cpp, etc.
│   │   │   │   ├── ExerciseFactory.h/cpp (Factory Pattern)
│   │   │   │   ├── StrategyGrader.h/cpp + 3 graders (Strategy Pattern)
│   │   │   │   ├── AudioManager.h/cpp (Singleton Pattern)
│   │   │   │   ├── Profile.h/cpp, SkillProgress.h/cpp
│   │   │   │   └── SRSScheduler.h/cpp
│   │   │   ├── controllers/       # ✅ COMPLETED
│   │   │   │   └── AppController.h/cpp
│   │   │   └── data/              # ✅ COMPLETED
│   │   │       ├── ContentRepository.h/cpp
│   │   │       └── content.json (10 exercises, 3 skills)
│   │   └── ui/                    # ✅ COMPLETED
│   │       ├── LessonView.h/cpp/ui (✅ COMPLETED)
│   │       └── ProfileView.h/cpp/ui (✅ COMPLETED)
│   ├── main.cpp (✅ COMPLETED - application entry point)
│   ├── mainwindow.h/cpp/ui (✅ COMPLETED - navigation and wiring)
│   ├── assets/audio/               # 🔜 PENDING (placeholder audio files)
│   ├── report/                     # 🔜 PENDING (diagrams, rationale)
│   ├── BhashaQuest.pro             # ✅ Qt Creator project (updated with AppController)
│   ├── CMakeLists.txt              # ✅ CMake build config
│   ├── README.md                   # ✅ Build instructions
│   ├── CLAUDE.md                   # 📄 This file
│   ├── PLAN.md                     # 📋 Detailed implementation plan (updated)
│   ├── main.cpp                    # Basic Qt main (will be replaced)
│   ├── mainwindow.h/cpp/ui         # Basic Qt window (scaffold)
│   └── .qtcreator/                 # Qt Creator metadata
├── PreviousWork/                   # Archive of Phase 1-2 completion
│   ├── code/src/core/domain/       # Original 27 domain files
│   ├── README.md, CLAUDE.md
│   └── [PDFs]
└── 3307/                           # Course materials
    ├── DeliverableTwo/Plan.md      # Original plan (now superseded by BhashaQuest/PLAN.md)
    └── [PDFs]                      # Specs, tutorials, samples
```

## Implementation Status by Layer

### ✅ Domain Layer (src/core/domain/) - COMPLETED
All 27 files implemented with design patterns:

**Exercise Hierarchy:**
- Exercise.h/cpp - Abstract base class
- MCQExercise.h/cpp - Multiple choice (4 options, correct index)
- TranslateExercise.h/cpp - Translation (multiple valid answers)
- TileOrderExercise.h/cpp - Word reordering

**Factory Pattern (Creational):**
- ExerciseFactory.h/cpp - Creates exercises from JSON specs

**Strategy Pattern (Behavioral):**
- StrategyGrader.h/cpp - Abstract grading interface
- MCQGrader.h/cpp - Exact index matching
- TranslateGrader.h/cpp - Fuzzy matching with Levenshtein distance ≤2
- TileOrderGrader.h/cpp - Sequence matching

**Singleton Pattern (Creational):**
- AudioManager.h/cpp - Centralized QMediaPlayer management

**Business Logic:**
- SRSScheduler.h/cpp - Spaced repetition (Easy→7d, Medium→3d, Hard→1d)
- Profile.h/cpp - User data (XP, streak, last activity)
- SkillProgress.h/cpp - Per-skill mastery tracking
- Result.h - Grading result struct

### ✅ Controller Layer (src/core/controllers/) - COMPLETED

**AppController.h/cpp** - **JUST IMPLEMENTED**

Orchestrates the lesson flow: Load → Display → Grade → Update Profile → Next

**Key Responsibilities:**
- Session management (startLesson, endLesson, loadNextExercise)
- Answer submission and grading (submitAnswer, gradeAnswer)
- Profile updates (XP calculation with difficulty multipliers)
- Mastery updates (+5% correct, -2% incorrect)
- SRS integration (scheduleReview, recordCompletion)
- Qt signals for UI communication

**Qt Signals Emitted:**
- `exerciseChanged(Exercise*)` - New exercise loaded
- `answerGraded(const Result&)` - After grading
- `lessonCompleted(int totalXP, int exercisesCompleted)` - Session end
- `profileUpdated(int currentXP, int streak)` - Profile changes

**Design Integration:**
- Uses Strategy Pattern (creates appropriate grader per exercise type)
- Uses Factory Pattern (via exercise queue from ContentRepository)
- Maintains strict layer separation (UI never touches domain directly)

### ✅ Data Layer (src/core/data/) - COMPLETED

**ContentRepository.h/cpp** - **JUST IMPLEMENTED**

Loads and manages exercise content from JSON file.

**Key Responsibilities:**
- Parse content.json using QJsonDocument
- Create Exercise instances via ExerciseFactory (Factory Pattern integration)
- Organize exercises by skill ID
- Cache loaded content for performance
- Provide access methods for skills and exercises

**Public Interface:**
- `loadContent()` - Parses JSON and creates exercises
- `getExercisesForSkill(skillId)` - Returns exercise list for a skill
- `getAvailableSkills()` - Returns list of skill IDs
- `getSkillName(skillId)`, `getSkillLanguage(skillId)` - Metadata access
- `reload()` - Reloads content from file

**Design Integration:**
- Uses ExerciseFactory::createExercise() for all exercise creation
- Manages memory lifecycle of all Exercise objects
- Returns pointers but retains ownership (caller doesn't delete)

**content.json Structure:**
- 3 skills: "nepali-greetings", "nepali-food", "nepali-numbers"
- 10 total exercises: 4 MCQ, 3 Translate, 3 TileOrder
- Each exercise has: id, type, prompt, skillId, difficulty, audioFile
- MCQ: options array + correctIndex
- Translate: correctAnswers array (supports multiple valid answers)
- TileOrder: tiles array + correctOrder array

### ✅ UI Layer (src/ui/) - COMPLETED

**✅ LessonView.h/cpp/ui - COMPLETED**

Main lesson interface that displays exercises and collects user input.

**Key Responsibilities:**
- Display exercise prompts with dynamic widget creation
- Collect user answers via type-specific input widgets
- Show grading feedback with color-coded messaging
- Track and display session progress (N/M exercises)
- Communicate with AppController via Qt signals/slots

**Dynamic Widget Creation:**
- **MCQ**: QButtonGroup + QRadioButton[] for 4 options
- **Translate**: QLineEdit with placeholder text and Enter key support
- **TileOrder**: QListWidget with drag-drop enabled (InternalMove mode)

**Qt Signals Emitted:**
- `answerSubmitted(QString)` - User clicks Submit with collected answer
- `nextExerciseRequested()` - User clicks Next after viewing feedback
- `newLessonRequested()` - User requests new lesson from completion screen

**Public Methods:**
- `updateExercise(Exercise*)` - Displays new exercise with appropriate widgets
- `displayFeedback(const Result&)` - Shows correct/incorrect with styled message
- `updateProgress(int completed, int total)` - Updates progress bar and label
- `showCompletionScreen(int totalXP, int exercisesCompleted)` - End-of-lesson summary
- `reset()` - Clears all state and returns to initial view

**Answer Collection:**
- MCQ: Returns selected index as string (e.g., "0", "1", "2", "3")
- Translate: Returns trimmed text input
- TileOrder: Returns semicolon-separated list (e.g., "tile1;tile2;tile3")

**Styling:**
- Correct feedback: Green background (#e8f5e9), green text
- Incorrect feedback: Red background (#ffebee), red text
- Buttons: Green (#4CAF50) with hover/pressed states
- Progress bar: Green chunk with rounded corners

---

**✅ ProfileView.h/cpp/ui - COMPLETED**

User profile and statistics display showing progress across all skills.

**Key Responsibilities:**
- Display username, XP, and streak count
- Show mastery levels for each skill with progress bars
- Display accuracy statistics with color coding
- Refresh display when profile is updated

**Public Methods:**
- `updateProfile(Profile*)` - Refresh entire profile display with all stats
- `updateStats(int xp, int streak)` - Quick update for XP and streak only
- `reset()` - Clear all display and return to initial state

**Signal/Slot Integration:**
- Receives: AppController::profileUpdated(int xp, int streak) → updateStats()
- Manual refresh via updateProfile() with Profile pointer

**UI Components:**
- Title: "Your Profile" (18pt, bold, centered)
- Username label: Blue (#2196F3), 16pt bold, centered
- Stats frame: White background with rounded corners, grid layout
  - XP label with ⭐ icon (green color)
  - Streak label with 🔥 icon (orange color)
- Skills list: QListWidget with custom item widgets per skill

**Custom Skill Item Widgets:**
Each skill displayed with:
- Skill name (formatted and capitalized from skill ID)
- Mastery progress bar: 0-100% with green (#4CAF50) fill
- Completion stats: "Completed: N/M exercises"
- Accuracy percentage with color coding:
  - Green (#4CAF50) if ≥80%
  - Yellow (#FF9800) if ≥60%
  - Red (#F44336) if <60%

**Empty State:**
- Shows placeholder message: "No skills started yet. Complete a lesson to get started!"
- Gray italic text when no skill progress available

**Styling:**
- Background: Light gray (#f5f5f5)
- Stats frame: White with rounded corners
- Skills list: White background, rounded borders
- Progress bars: Green chunk (#4CAF50) with percentage labels

---

**✅ MainWindow.h/cpp/ui + main.cpp - COMPLETED**

Main application window that coordinates all components and provides navigation.

**Key Responsibilities:**
- Own and manage AppController, ContentRepository, LessonView, ProfileView
- Provide navigation between lesson and profile views via QStackedWidget
- Display skill selector and start lesson controls
- Show status bar with XP and streak
- Wire all signal/slot connections between components

**UI Components:**
- Header section:
  - Skill selector (QComboBox) with "Name (Language)" format
  - "Start Lesson" button (green) - starts lesson for selected skill
  - "View Profile" button (blue) - switches to profile view
- Main area: QStackedWidget switching between:
  - Index 0: LessonView
  - Index 1: ProfileView
- Footer:
  - "Back to Lesson" button (gray, visible only in profile view)
  - Status bar showing "XP: N | Streak: N days"

**Signal/Slot Wiring:**
All connections established in wireSignalsAndSlots():
- LessonView signals → AppController slots
- AppController signals → LessonView, ProfileView, MainWindow slots
- Button clicks → MainWindow navigation slots

**Component Initialization:**
1. setupUI(): Creates all widgets, layouts, buttons
2. loadContent(): Loads content.json via ContentRepository, populates skill selector
3. wireSignalsAndSlots(): Connects all signals and slots
4. Sets initial view to LessonView (index 0)
5. Updates status bar with initial profile data

**Lesson Flow:**
1. User selects skill from dropdown
2. Clicks "Start Lesson"
3. MainWindow gets exercises from ContentRepository
4. Calls appController->startLesson(exercises)
5. Resets LessonView and updates progress (0/N)
6. Switches to LessonView
7. AppController emits exerciseChanged → LessonView displays first exercise

**Navigation Flow:**
- Start Lesson: Switches to LessonView (index 0), hides back button
- View Profile: Switches to ProfileView (index 1), shows back button, refreshes profile data
- Back to Lesson: Switches to LessonView (index 0), hides back button

**Completion Handling:**
- AppController emits lessonCompleted(totalXP, exercisesCompleted)
- MainWindow shows QMessageBox with congratulations
- Updates status bar with total XP earned
- Refreshes ProfileView with updated stats

**main.cpp:**
- Creates QApplication with metadata
- Creates and shows MainWindow
- Starts event loop with app.exec()
- All initialization delegated to MainWindow constructor

### Project Structure

```
BhashaQuest/
├── src/
│   ├── core/
│   │   ├── domain/                # ✅ 27 files
│   │   ├── controllers/           # ✅ AppController
│   │   └── data/                  # ✅ ContentRepository + content.json
│   └── ui/                        # ✅ LessonView + ProfileView
├── assets/audio/                  # 🔜 Audio files
├── report/                        # 🔜 Diagrams, rationale
├── BhashaQuest.pro                # Qt Creator project file
├── CMakeLists.txt                 # CMake build configuration
├── README.md                      # Build instructions
├── CLAUDE.md                      # This file
└── PLAN.md                        # Detailed implementation plan
```

## Build & Development Commands

### Building with Qt Creator (Recommended)

1. Open Qt Creator
2. File → Open File or Project
3. Navigate to `BhashaQuest/BhashaQuest.pro`
4. Configure with Qt6 kit
5. Build (Ctrl+B) and Run (Ctrl+R)

### Building with CMake (Alternative)

```bash
cd BhashaQuest
mkdir build && cd build
cmake ..
make
./BhashaQuest
```

**Note:** Both build systems are maintained. BhashaQuest.pro includes all components as of Phase 4.3.

### Testing Manual Walkthrough ✅ READY FOR TESTING

**Application is now fully functional!** Test these flows:

1. **Launch Application:**
   - Open Qt Creator, load BhashaQuest.pro
   - Build (Ctrl+B) and Run (Ctrl+R)
   - Window should appear with skill selector, buttons, and status bar

2. **Start Lesson Flow:**
   - Select a skill from dropdown (e.g., "Greetings (Nepali)")
   - Click "Start Lesson"
   - First exercise should display with appropriate widgets

3. **Exercise Interaction:**
   - **MCQ**: Select radio button, click Submit → See green/red feedback
   - **Translate**: Type answer, press Enter or Submit → See feedback with fuzzy matching
   - **TileOrder**: Drag tiles to reorder, Submit → See feedback
   - Click "Next Exercise" after each

4. **Lesson Completion:**
   - Complete all exercises in skill
   - Verify completion screen shows total XP and exercises completed
   - Verify message box appears with congratulations
   - Status bar should update with new XP total

5. **Profile View:**
   - Click "View Profile" button
   - Verify username, XP, and streak display
   - Check skill progress bars show mastery percentage
   - Verify accuracy color coding (green/yellow/red)
   - Click "← Back to Lesson"

6. **Progress Tracking:**
   - Start another lesson
   - Complete exercises and verify XP accumulates
   - Check profile view shows updated mastery levels

**Current State:** ✅ **FULLY IMPLEMENTED AND READY TO RUN** - All core functionality complete (Domain + Controller + Data + UI + Wiring). Application can be built and tested end-to-end.

## Architecture & Design Patterns

### Core Design Patterns (Required for D2)

**Factory Pattern (Creational)**
- `ExerciseFactory::createExercise(type, spec)` instantiates exercise types
- Enables extensibility for new exercise types without modifying client code
- Used by: ContentRepository to create exercises from JSON

**Strategy Pattern (Behavioral)**
- `StrategyGrader` interface with concrete implementations (MCQGrader, TranslateGrader, TileOrderGrader)
- Allows pluggable grading algorithms per exercise type
- Used by: AppController to evaluate user answers

**Singleton Pattern (Creational)**
- `AudioManager::getInstance()` provides centralized audio resource management
- Ensures single instance of QMediaPlayer
- Used by: UI and domain layers for success/error audio feedback

### Architectural Layers

```
UI Layer (Qt Widgets)
    ↓ signals/slots
Controller Layer (AppController)
    ↓ orchestrates
Domain Layer (Exercise, Grader, SRSScheduler, Profile)
    ↓ uses
Data Layer (ContentRepository, JSON)
```

**Key principle:** UI components only interact with AppController, never directly with domain objects.

### Data Flow for Exercise Completion (IMPLEMENTED)

**Complete flow as implemented in AppController:**

1. User submits answer in `LessonView` → emits signal
2. Signal connects to `AppController::submitAnswer(userAnswer)`
3. AppController calls `createGraderForExercise(currentExercise)`:
   - Returns `MCQGrader*`, `TranslateGrader*`, or `TileOrderGrader*` based on exercise type
4. AppController calls `grader->grade(userAnswer, currentExercise)` (Strategy Pattern)
5. Grader returns `Result{correct, score, feedback}`
6. AppController calls `updateProfile(result)`:
   - Calculates XP: `baseScore * difficulty` (e.g., 10 * 3 = 30 XP)
   - Adds XP: `profile->addXP(xp)`
   - Updates streak: `profile->updateStreak()`
   - Updates skill progress: `progress->recordResult(correct)`
   - Adjusts mastery: `+5%` if correct, `-2%` if incorrect
7. AppController calls `srsScheduler->recordCompletion(exerciseId)`
8. AppController emits `answerGraded(result)` → LessonView displays feedback
9. AppController calls `loadNextExercise()`:
   - Gets next exercise from queue
   - Creates appropriate grader
   - Emits `exerciseChanged(exercise)` → LessonView updates UI
10. When queue exhausted: emits `lessonCompleted(totalXP, exercisesCompleted)`

## Content Management

### content.json Structure

Exercises are defined in `code/src/core/data/content.json`:

```json
{
  "skills": [
    {
      "id": "nepali-greetings",
      "name": "Greetings",
      "language": "Nepali",
      "exercises": [
        {
          "id": "ex1",
          "type": "MCQ",
          "prompt": "Select the correct translation for 'hello'",
          "options": ["नमस्ते", "धन्यवाद", "गुडबाई", "कृपया"],
          "correctIndex": 0
        }
      ]
    }
  ]
}
```

Exercise types:
- **MCQ**: Multiple choice with 4 options
- **Translate**: English → target language (accepts romanization or native script)
- **TileOrder**: Reorder word tiles to form correct phrase

## Implementation Guidelines

### Deliverable 2 (50% Implementation) Requirements

Must implement:
- 3 exercise types working end-to-end (MCQ, Translate, TileOrder)
- Factory Pattern clearly visible
- Strategy Pattern clearly visible
- Singleton Pattern (AudioManager)
- Basic SRS scheduling (simple interval: Easy→7d, Medium→3d, Hard→1d)
- Lesson flow: load → display → grade → update profile → next

Not required for D2:
- ListenExercise, SpeakExercise
- Observer pattern
- SQLite persistence
- Advanced SM-2 algorithm

### Code Quality Standards

- Use meaningful variable names
- Add comments explaining pattern usage locations
- Keep functions small and focused
- Maintain strict layer separation (UI → Controller → Domain → Data)
- Commit frequently with clear messages

### Qt-Specific Considerations

- Use Qt6 (not Qt5) - affects QMediaPlayer API
- Signal/slot mechanism for UI-controller communication
- QJsonDocument/QJsonObject for content parsing
- QString for text handling (supports Unicode for non-Latin scripts)

## SRS (Spaced Repetition System) Logic

Simple implementation for D2:
- Track: last_review_date, next_review_date, interval, difficulty
- Review intervals: Easy → +7 days, Medium → +3 days, Hard → +1 day
- `SRSScheduler::getReviewQueue()` returns exercises where next_review_date ≤ today
- Future (D3): Implement full SM-2 algorithm with ease factor

## Grading Logic Details

**MCQGrader**: Exact index match (strict)

**TranslateGrader**: Fuzzy matching
- Accept multiple correct answers (e.g., "पानी" OR "pani")
- Allow minor typos (Levenshtein distance ≤ 2)
- Case-insensitive for romanization

**TileOrderGrader**: Sequence matching
- Compare user tile order to correct sequence
- All tiles must be in exact order

## Common Development Workflows

### Adding a New Exercise Type

1. Create `NewExercise.h/cpp` inheriting from `Exercise`
2. Implement pure virtual methods: `getPrompt()`, `getCorrectAnswer()`, `renderUI()`
3. Create `NewExerciseGrader.h/cpp` inheriting from `StrategyGrader`
4. Add creation logic to `ExerciseFactory::createExercise()`
5. Update JSON schema in `content.json`
6. Update class diagram

### Adding New Content

1. Edit `code/src/core/data/content.json`
2. Add exercise object with required fields (id, type, prompt, etc.)
3. Restart app (ContentRepository loads on startup)

### Debugging Build Issues

- Ensure Qt6 is in CMAKE_PREFIX_PATH
- Check CMakeLists.txt includes all source files
- Verify Qt modules: Core, Widgets, Multimedia (for audio)
- Use Qt Creator's "Clean All" and rebuild if cache issues occur

## File Naming Conventions

- Header files: `ClassName.h`
- Implementation: `ClassName.cpp`
- One class per file pair
- Match class name to file name exactly (case-sensitive)

## Submission Deliverables (D2)

Required artifacts:
1. **code/**: Compiling Qt6 application
2. **report/design_rationale.pdf**: 2-3 pages explaining pattern choices and architecture
3. **report/class_diagram.pdf**: Updated UML class diagram showing all classes and patterns
4. **report/sequence_diagram.pdf**: "Complete Exercise" use case flow
5. **README.md**: Build instructions and video link
6. **Video**: 3-5 minute walkthrough demonstrating features and code patterns

## Key Success Criteria

✅ Code must compile and run in Qt Creator without errors
✅ All 3 exercise types functional in running application
✅ Factory, Strategy, Singleton patterns clearly identifiable in code
✅ Complete lesson cycle works: start → answer → grade → next
✅ UML diagrams match actual code implementation
