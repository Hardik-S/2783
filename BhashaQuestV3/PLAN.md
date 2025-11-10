# BhashaQuest Implementation Plan - Deliverable 2

## Overview

This is the intermediate design and partial implementation (50% of system). Success requires:
- Functional C++ code that compiles and runs in Qt Creator
- At least one creational pattern and one structural/behavioral pattern
- Three exercise types fully working
- Updated UML diagrams
- Design rationale document (2-3 pages)
- 3-5 minute video walkthrough

---

## Implementation Status

### ✅ Phase 1: Repository Setup - COMPLETED
- [x] Created folder structure (src/core/domain, controllers, data, ui)
- [x] Created assets/audio, report directories
- [x] Updated CMakeLists.txt for Qt6
- [x] Created BhashaQuest.pro for Qt Creator
- [x] Created README.md with build instructions

### ✅ Phase 2: Core Domain Implementation - COMPLETED (27 files)

#### 2.1 Base Classes & Interfaces - COMPLETED
- [x] Exercise.h - Abstract base class with pure virtual methods
- [x] StrategyGrader.h - Strategy Pattern interface
- [x] Result.h - Grading result struct
- [x] Profile.h/cpp - User profile with XP, streak tracking
- [x] SkillProgress.h/cpp - Progress tracking per skill

#### 2.2 Concrete Exercise Types - COMPLETED
- [x] MCQExercise.h/cpp - Multiple choice with 4 options
- [x] TranslateExercise.h/cpp - English → Target language
- [x] TileOrderExercise.h/cpp - Word reordering exercises

#### 2.3 Factory Pattern (Creational) - COMPLETED
- [x] ExerciseFactory.h/cpp
  - Creates exercises from JSON specifications
  - Supports MCQ, Translate, TileOrder types
  - Encapsulates object creation logic

#### 2.4 Strategy Pattern (Behavioral) - COMPLETED
- [x] MCQGrader.h/cpp - Exact index matching
- [x] TranslateGrader.h/cpp - Fuzzy matching with Levenshtein distance
- [x] TileOrderGrader.h/cpp - Sequence order matching

#### 2.5 Singleton Pattern (Creational) - COMPLETED
- [x] AudioManager.h/cpp
  - Single instance for audio resource management
  - QMediaPlayer integration for success/error sounds

#### 2.6 SRS Scheduler - COMPLETED
- [x] SRSScheduler.h/cpp
  - Simple spaced repetition: Easy→7d, Medium→3d, Hard→1d
  - Review queue management
  - Difficulty tracking

### ✅ Phase 3.1: AppController - COMPLETED

- [x] AppController.h/cpp - **JUST IMPLEMENTED**
  - Orchestrates lesson flow: load → display → grade → update profile → next
  - Qt signals/slots for UI communication
  - Manages exercise session state
  - XP calculation with difficulty multipliers
  - Mastery level updates (+5% correct, -2% incorrect)
  - Memory-safe resource management
  - Strategy Pattern integration (creates appropriate graders)
  - Profile and SRSScheduler coordination

**Key Features Implemented:**
- Session management (startLesson, endLesson, loadNextExercise)
- Answer submission and grading (submitAnswer, gradeAnswer)
- Profile updates (XP, streak, skill progress)
- SRS integration (scheduleReview, recordCompletion)
- Qt signals: exerciseChanged, answerGraded, lessonCompleted, profileUpdated

**Files Updated:**
- BhashaQuest.pro - Added AppController to SOURCES and HEADERS

---

### ✅ Phase 3.2: ContentRepository - COMPLETED

- [x] ContentRepository.h/cpp - **JUST IMPLEMENTED**
  - Loads and parses content.json using QJsonDocument
  - Creates Exercise instances via ExerciseFactory (Factory Pattern)
  - Organizes exercises by skill ID in QMap<QString, Skill> structure
  - Provides access methods: getExercisesForSkill(), getAvailableSkills()
  - Manages memory lifecycle of all Exercise objects
  - Includes metadata access: getSkillName(), getSkillLanguage()
  - Supports content reloading via reload() method

- [x] content.json - **10 exercises created across 3 skills**
  - **nepali-greetings**: 4 exercises (1 MCQ, 1 Translate, 1 TileOrder, 1 MCQ)
  - **nepali-food**: 3 exercises (1 Translate, 1 MCQ, 1 TileOrder)
  - **nepali-numbers**: 3 exercises (1 MCQ, 1 Translate, 1 TileOrder)
  - All exercises include: id, type, prompt, skillId, difficulty (1-3), audioFile
  - Exercise-specific fields properly structured per type

**Files Updated:**
- BhashaQuest.pro - Added ContentRepository to SOURCES and HEADERS
- CMakeLists.txt - Already configured to copy content.json to build directory

**Design Integration:**
- ContentRepository uses ExerciseFactory::createExercise() for all exercise instantiation
- Maintains separation: Repository owns Exercise objects, returns pointers for read-only access
- Error handling with QDebug logging for parse failures

---

## 🔄 CURRENT PHASE: Phase 4 - UI Implementation (3-4 hours)

**Next Steps:** Implement LessonView, ProfileView, and main application window.

**content.json** - Implemented structure:
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
          "correctIndex": 0,
          "difficulty": 2
        },
        {
          "id": "ex2",
          "type": "Translate",
          "prompt": "Translate: water",
          "correctAnswers": ["पानी", "pani"],
          "difficulty": 3
        },
        {
          "id": "ex3",
          "type": "TileOrder",
          "prompt": "Order the words to say 'Thank you'",
          "tiles": ["धन्यवाद", "छ"],
          "correctOrder": ["धन्यवाद", "छ"],
          "difficulty": 2
        }
      ]
    }
  ]
}
```

**Tasks:** ✅ ALL COMPLETED
- [x] Implement ContentRepository.h/cpp
- [x] Create sample content.json with 10 exercises (4 MCQ, 3 Translate, 3 TileOrder)
- [x] Implement JSON parsing and Exercise creation via Factory
- [x] Update BhashaQuest.pro to include ContentRepository files
- [x] CMakeLists.txt already configured to copy content.json to build directory

---

### 4.1 LessonView (Qt Widgets) ✅ COMPLETED

Main lesson screen with:
- Exercise prompt display (QLabel)
- Dynamic input widgets based on exercise type:
  - MCQ: QRadioButton group with QButtonGroup
  - Translate: QLineEdit with placeholder text
  - TileOrder: Draggable QListWidget with InternalMove mode
- Submit answer button (QPushButton)
- Feedback message display (QLabel with color coding - green/red)
- Progress bar (QProgressBar showing N/M)
- Next exercise button (shown after grading)

**Signal/Slot Connections:**
- `answerSubmitted(QString)` signal → connects to AppController::submitAnswer()
- `nextExerciseRequested()` signal → connects to AppController::loadNextExercise()
- `newLessonRequested()` signal → connects to start new lesson flow
- Receives: AppController::exerciseChanged → updateExercise()
- Receives: AppController::answerGraded → displayFeedback()
- Receives: AppController::lessonCompleted → showCompletionScreen()

**Implementation Details:**
- LessonView.h: Complete class interface with public methods, signals, slots
- LessonView.cpp: Dynamic widget creation with type checking (dynamic_cast)
- Answer collection: MCQ returns index, Translate returns text, TileOrder returns semicolon-separated list
- Feedback styling: CSS applied with green background for correct, red for incorrect
- Progress tracking: Updates both progress bar (percentage) and text label (N of M)

**Tasks:** ✅ ALL COMPLETED
- [x] Create LessonView.h/cpp
- [x] Design UI layout (programmatic QVBoxLayout with proper spacing)
- [x] Implement dynamic widget creation based on Exercise type
- [x] Define signals for AppController communication
- [x] Add feedback display (green for correct, red for incorrect)
- [x] Implement progress tracking display
- [x] Create LessonView.ui with stylesheet
- [x] Update BhashaQuest.pro

### 4.2 ProfileView (Stats Display) ✅ COMPLETED + LAYOUT FIXED

Simple screen showing:
- Username (QLabel with centered blue text)
- Current XP and Streak (QGridLayout with icons ⭐ and 🔥)
- Skills list with progress bars (QListWidget + custom QWidget items)
- Mastery percentages per skill with color-coded accuracy

**Signal/Slot Connections:**
- Receives: AppController::profileUpdated → updateStats()
- Manual refresh: updateProfile(Profile*)

**Implementation Details:**
- ProfileView.h: Complete class interface with public methods (updateProfile, updateStats, reset)
- ProfileView.cpp: Custom skill item widgets with progress bars and stats
- Skills display: Formatted skill names, mastery progress bars (0-100%), completion stats, color-coded accuracy
- Accuracy color coding: Green (≥80%), Yellow (≥60%), Red (<60%)
- Stats frame: White background with rounded corners, grid layout for XP and streak
- Empty state: Shows placeholder message when no skills started

**Visual Design (Optimized Layout - Fixed 2025-11-09):**
- Title: "Your Learning Journey" (24pt, bold, centered) - *Reduced from 28pt*
- Username: Blue color (#2196F3), 22pt bold
- Profile card: Optimized padding (15px), smaller avatar (80x80)
- Stat cards: Compact height (100px), efficient spacing
- Skills section: **Takes majority of vertical space with stretch factor**
- Skills title: 18pt bold - *Reduced from 22pt*
- Progress bars: Green (#4CAF50) with percentage display
- Background: Purple gradient (#667eea to #764ba2)
- Developer tools: **Hidden by default** with toggle button (🔧)

**Layout Optimizations Applied:**
- Main layout spacing: 15px (reduced from 25px)
- Main margins: 20px (reduced from 30px)
- Scroll area: **Stretch factor = 1** (fills available vertical space)
- Skills spacing: 10px (reduced from 15px)
- Minimum window size: 800x600 (prevents squishing)
- Developer tools: Collapsible via toggle button (saves ~120px vertical space)

**Key Fix:** Added stretch factor to scroll area (`mainLayout->addWidget(scrollArea, 1)`) to ensure skills section expands to fill available space instead of being compressed.

**Tasks:** ✅ ALL COMPLETED
- [x] Create ProfileView.h/cpp
- [x] Display Profile data (XP, streak, username)
- [x] Show SkillProgress with mastery levels
- [x] Add visual polish (icons, colors)
- [x] Create ProfileView.ui with stylesheet
- [x] Update BhashaQuest.pro
- [x] **FIX LAYOUT ISSUES** (2025-11-09) - See ProfileViewFix_PLAN.md for details

### 4.3 Main Window & Application Entry ✅ COMPLETED

**main.cpp:**
- Creates QApplication with metadata (app name, version, organization)
- Creates MainWindow and shows it
- Starts Qt event loop with app.exec()
- All component initialization delegated to MainWindow constructor

**MainWindow:**
- Navigation: QStackedWidget switches between LessonView (index 0) and ProfileView (index 1)
- Header controls:
  - Skill selector (QComboBox populated from ContentRepository)
  - "Start Lesson" button (green, triggers lesson with selected skill)
  - "View Profile" button (blue, switches to profile view)
  - "Back to Lesson" button (gray, shown only in profile view)
- Status bar: Displays current XP and streak count
- Owns all components: AppController, ContentRepository, LessonView, ProfileView

**Signal/Slot Wiring:**
All connections established in wireSignalsAndSlots():

1. **LessonView → AppController:**
   - answerSubmitted → submitAnswer
   - nextExerciseRequested → loadNextExercise

2. **AppController → LessonView:**
   - exerciseChanged → updateExercise
   - answerGraded → displayFeedback
   - lessonCompleted → showCompletionScreen

3. **AppController → ProfileView:**
   - profileUpdated → updateStats

4. **AppController → MainWindow:**
   - profileUpdated → updateStatusBar
   - lessonCompleted → onLessonCompleted (shows message box)

**Implementation Details:**
- MainWindow.h: Complete interface with all component pointers and slots
- MainWindow.cpp: setupUI(), wireSignalsAndSlots(), loadContent()
- Skill selection: Populates combo box with "Name (Language)" format
- Lesson flow: Start Lesson → Reset view → Load exercises → Start controller → Update progress
- Completion handling: Shows message box, updates profile view, updates status bar
- Navigation: Stacked widget with smooth view switching
- Back button: Conditionally visible only in profile view

**UI Layout:**
- Header row: Skill selector + Start Lesson + View Profile buttons
- Main area: QStackedWidget with LessonView and ProfileView
- Footer: Back button (hidden in lesson view) + status bar

**Tasks:** ✅ ALL COMPLETED
- [x] Update main.cpp with QApplication initialization
- [x] Update MainWindow.h with component references
- [x] Update MainWindow.cpp with complete UI setup and wiring
- [x] Wire all signal/slot connections between components
- [x] Implement navigation between views
- [x] Add status bar updates
- [x] Update MainWindow.ui

### 4.4 Assets ✅ COMPLETED

**Audio File Generation Strategy:**

For **Deliverable 2 (D2)**, placeholder audio files are acceptable per CLAUDE.md guidance ("actual audio files optional"). The codebase is designed with future upgrades in mind:

**Current Implementation (D2 - Phase 1):**
- ✅ Silent MP3 files generated using FFmpeg
- ✅ 30 audio files for all exercises in content.json
- ✅ 1-second duration, minimal file size
- ✅ Meets functional requirement: eliminates "No such file or directory" errors
- ✅ AudioManager path resolution updated for runtime compatibility

**Future Upgrade Path (D3/Final - Phase 2):**
When realistic audio is desired, files can be replaced with Text-to-Speech (TTS) generated audio:

**Recommended TTS Services:**
1. **Google Cloud Text-to-Speech** - Best language support for Hindi/Kannada
   - Supports Hindi (close to Nepali), Kannada
   - Neural voices with natural pronunciation
   - Pricing: ~$4 per 1 million characters (free tier available)

2. **Amazon Polly** - Good alternative with AWS integration
   - Supports multiple Indian languages
   - Free tier: 5 million characters/month for 12 months
   - Neural TTS available

3. **Open-Source Alternative: eSpeak-NG**
   - Free, offline, cross-platform
   - Lower quality but no cost
   - Suitable for budget-conscious implementation

**Upgrade Process (Zero Code Changes Required):**
1. Generate TTS audio files using chosen service
2. Replace existing MP3 files in `assets/audio/` directory
3. Ensure filenames match content.json references
4. Rebuild project (copies new files to build directory)

**Design Principle:**
The AudioManager uses a generic file path approach, making it **audio-content agnostic**. Swapping placeholder files for realistic audio requires no code modifications.

**Tasks:** ✅ ALL COMPLETED
- [x] Create assets/audio directory structure
- [x] Generate 30 silent MP3 files using FFmpeg (1 second each)
- [x] Update AudioManager for application-relative path resolution
- [x] Update BhashaQuest.pro to copy assets/ to build directory
- [x] Update CMakeLists.txt to copy assets/ to build directory
- [x] Test AudioManager playback (verify FFmpeg errors resolved)
- [x] Document upgrade path in PLAN.md for future TTS integration

---

## Phase 5: Updated UML Diagrams (1.5 hours)

### 5.1 Class Diagram
**Must include:**
- All 27+ classes implemented
- Design patterns marked:
  - **Factory Pattern**: ExerciseFactory with creation methods
  - **Strategy Pattern**: StrategyGrader hierarchy (MCQGrader, TranslateGrader, TileOrderGrader)
  - **Singleton Pattern**: AudioManager with getInstance()
- Relationships:
  - AppController → Profile, SRSScheduler, Exercise, StrategyGrader
  - ExerciseFactory → Exercise subclasses
  - ContentRepository → ExerciseFactory
  - UI Views → AppController (signals/slots)

**Tasks:**
- [ ] Update class diagram in draw.io or PlantUML
- [ ] Highlight pattern implementations with stereotypes or colors
- [ ] Export as PDF to report/class_diagram.pdf

### 5.2 Sequence Diagram
**"Complete Exercise" Use Case:**
1. User clicks Submit in LessonView
2. LessonView emits signal → AppController::submitAnswer(answer)
3. AppController → createGraderForExercise(currentExercise)
4. AppController → grader->grade(answer, exercise)
5. Grader returns Result
6. AppController → Profile::addXP(xp)
7. AppController → SkillProgress::recordResult(correct)
8. AppController → SRSScheduler::recordCompletion(exerciseId)
9. AppController emits answerGraded(result) → LessonView displays feedback
10. AppController → loadNextExercise()
11. AppController emits exerciseChanged(exercise) → LessonView updates UI

**Tasks:**
- [ ] Create/update sequence diagram
- [ ] Export as PDF to report/sequence_diagram.pdf

---

## Phase 6: Design Rationale Document (1 hour)

**File:** `report/design_rationale.pdf` (2-3 pages)

**Content Outline:**

### 1. Pattern Justifications (~½ page)

**Factory Pattern (ExerciseFactory)**
- **Why:** Encapsulates exercise creation logic, enabling extensibility
- **Benefit:** Adding new exercise types (e.g., ListenExercise) requires only:
  1. Creating new Exercise subclass
  2. Adding factory method
  3. No changes to AppController or ContentRepository
- **Implementation:** ExerciseFactory::createExercise() reads type from JSON and instantiates appropriate class

**Strategy Pattern (StrategyGrader)**
- **Why:** Different exercise types require different grading algorithms
- **Benefit:** Pluggable grading logic keeps Exercise classes focused on data
- **Implementation:** AppController creates appropriate grader (MCQ/Translate/TileOrder) and calls unified grade() interface
- **Example:** TranslateGrader uses Levenshtein distance for fuzzy matching, MCQGrader uses exact index comparison

**Singleton Pattern (AudioManager)**
- **Why:** Only one instance of QMediaPlayer needed for audio feedback
- **Benefit:** Centralized resource management prevents multiple players from conflicting
- **Implementation:** Private constructor, static getInstance() method

### 2. Architecture Overview (~½ page)

**Layered Architecture:**
```
UI Layer (LessonView, ProfileView)
    ↓ Qt signals/slots
Controller Layer (AppController)
    ↓ orchestrates
Domain Layer (Exercise, Grader, Profile, SRSScheduler)
    ↓ uses
Data Layer (ContentRepository, content.json)
```

**Key Principle:** Separation of concerns
- UI knows only about AppController, not domain classes
- Domain layer has no Qt dependencies (except QString)
- Controller coordinates without business logic

### 3. Implementation Status (~½ page)

**Completed (Phase 1-3.1):**
- ✅ All domain classes (27 files)
- ✅ 3 design patterns implemented and documented
- ✅ AppController with Qt signals/slots
- ✅ SRS scheduling logic
- ✅ Profile and progress tracking
- ✅ XP and mastery calculations

**In Progress (Phase 3.2-4):**
- 🔄 ContentRepository and JSON content
- 🔄 UI implementation (LessonView, ProfileView)

**Deliverable 2 Scope (50%):**
- 3 exercise types (MCQ, Translate, TileOrder) ✅
- End-to-end lesson flow (pending UI completion)
- Basic SRS algorithm ✅

### 4. Next Steps for D3 (~½ page)

**Additional Exercise Types:**
- ListenExercise (audio playback + transcription)
- SpeakExercise (microphone input + speech recognition)

**Advanced Patterns:**
- Observer pattern for real-time UI updates
- Decorator pattern for exercise modifiers (timed, hard mode)

**Persistence:**
- SQLite integration for user profiles
- Progress saving across sessions

**Testing:**
- Unit tests for graders
- Integration tests for lesson flow
- UI automation tests

**Tasks:**
- [ ] Write design rationale document
- [ ] Export as PDF to report/design_rationale.pdf

---

## Phase 7: Build & Test (1 hour)

### 7.1 Compilation

**Qt Creator:**
1. Open BhashaQuest.pro
2. Configure with Qt6 kit
3. Build (Ctrl+B)
4. Run (Ctrl+R)

**CMake (alternative):**
```bash
cd BhashaQuest
mkdir build && cd build
cmake ..
make
./BhashaQuest
```

**Tasks:**
- [ ] Ensure clean build with no errors/warnings
- [ ] Test in Qt Creator
- [ ] Verify both build systems work

### 7.2 Functional Testing

**Test Cases:**
1. [ ] Start lesson → exercises load correctly
2. [ ] MCQ: Select correct answer → +XP, green feedback
3. [ ] MCQ: Select wrong answer → red feedback, show correct answer
4. [ ] Translate: Type correct answer (native script) → +XP
5. [ ] Translate: Type correct answer (romanization) → +XP
6. [ ] Translate: Minor typo (1-2 chars) → still accepted
7. [ ] TileOrder: Arrange correctly → +XP
8. [ ] TileOrder: Wrong order → feedback
9. [ ] Complete all exercises → lesson completion screen
10. [ ] Profile view → shows updated XP and streak
11. [ ] Skills → mastery levels update correctly
12. [ ] Audio feedback plays (success/error sounds)

**Tasks:**
- [ ] Manual walkthrough of all test cases
- [ ] Document any bugs found
- [ ] Fix critical issues

---

## Phase 8: Video Walkthrough (1 hour)

**Length:** 3-5 minutes
**Tools:** OBS Studio, QuickTime, or Windows Game Bar

**Script:**

### 1. Introduction (30 sec)
- "This is Bhasha Quest, a language learning application for CS 3307 Deliverable 2"
- "Built with C++ and Qt6"
- "Implements 50% of the system with 3 design patterns"

### 2. Live Demo (1.5-2 min)
- Show main menu
- Click "Start Lesson"
- **MCQ Exercise:**
  - Read prompt: "What is 'hello' in Nepali?"
  - Select correct answer "नमस्ते"
  - Show feedback: "Correct! +20 XP"
- **Translate Exercise:**
  - Prompt: "Translate: water"
  - Type "pani" (romanization)
  - Show acceptance and XP gain
- **TileOrder Exercise:**
  - Prompt: "Order the words to say 'Thank you'"
  - Drag tiles to correct order
  - Show completion
- End lesson → show summary screen
- Navigate to Profile view
- Show XP, streak, skill progress bars

### 3. Code Walkthrough (1-1.5 min)

**Show in IDE:**

**Factory Pattern:**
- Open ExerciseFactory.cpp
- Highlight createExercise() method
- Explain: "Based on JSON type, factory creates appropriate Exercise subclass"

**Strategy Pattern:**
- Open StrategyGrader.h (interface)
- Open MCQGrader.cpp and TranslateGrader.cpp
- Highlight grade() methods
- Explain: "Different grading algorithms plugged in via Strategy Pattern"

**Singleton Pattern:**
- Open AudioManager.h
- Highlight getInstance() and private constructor
- Explain: "Singleton ensures only one audio player instance"

**AppController:**
- Open AppController.cpp
- Highlight submitAnswer() method showing:
  1. gradeAnswer() call
  2. updateProfile() call
  3. Signal emission
- Explain: "Controller orchestrates domain layer without business logic"

### 4. Architecture Overview (30 sec)
- Show architecture diagram (layers)
- Explain separation: UI → Controller → Domain → Data
- Mention strict layer boundaries

**Tasks:**
- [ ] Record video with screen capture and narration
- [ ] Edit for clarity and timing
- [ ] Export as MP4
- [ ] Upload to YouTube (unlisted) or OneDrive
- [ ] Add link to README.md

---

## Phase 9: GitHub & Submission (30 mins)

### 9.1 Organize Repository

**Expected Structure:**
```
BhashaQuest/
├── src/
│   ├── core/
│   │   ├── domain/ (14 .h + 13 .cpp = 27 files)
│   │   ├── controllers/ (AppController.h/cpp)
│   │   └── data/ (ContentRepository.h/cpp, content.json)
│   └── ui/ (LessonView, ProfileView, main.cpp)
├── assets/
│   └── audio/ (success.wav, error.wav)
├── report/
│   ├── design_rationale.pdf
│   ├── class_diagram.pdf
│   └── sequence_diagram.pdf
├── BhashaQuest.pro
├── CMakeLists.txt
├── README.md
├── CLAUDE.md
└── PLAN.md (this file)
```

### 9.2 README.md Requirements

- [x] Project overview ✅
- [x] Build instructions (Qt Creator and CMake) ✅
- [x] Features implemented ✅
- [x] Design patterns used ✅
- [ ] Video link (add when available)
- [ ] How to run and test

### 9.3 Git Workflow

**Final commits:**
```bash
git add .
git commit -m "Phase 3.2: Add ContentRepository and JSON content"
# (after UI is done)
git commit -m "Phase 4: Complete UI implementation (LessonView, ProfileView)"
git commit -m "D2 Submission: 50% implementation - all patterns working"
git push origin main
```

**Tasks:**
- [ ] Commit all Phase 3.2 changes
- [ ] Commit all Phase 4 changes
- [ ] Final commit with submission tag
- [ ] Push to GitHub
- [ ] Verify all files visible on GitHub

### 9.4 Brightspace Submission

**Upload to Brightspace:**
1. design_rationale.pdf
2. class_diagram.pdf
3. sequence_diagram.pdf
4. Link to video
5. Link to GitHub repository

---

## Timeline Summary (Remaining Work)

| Phase | Task | Status | Estimated Time |
|-------|------|--------|----------------|
| 1 | Repository Setup | ✅ DONE | - |
| 2 | Core Domain (27 files) | ✅ DONE | - |
| 3.1 | AppController | ✅ DONE | - |
| **3.2** | **ContentRepository** | **🔄 NEXT** | **1-2 hr** |
| **4** | **UI Implementation** | **🔜 PENDING** | **3-4 hr** |
| 5 | UML Diagrams | 🔜 PENDING | 1.5 hr |
| 6 | Design Rationale | 🔜 PENDING | 1 hr |
| 7 | Build & Test | 🔜 PENDING | 1 hr |
| 8 | Video Walkthrough | 🔜 PENDING | 1 hr |
| 9 | GitHub & Submission | 🔜 PENDING | 0.5 hr |

**Total Remaining:** ~9-11 hours of focused work

---

## Critical Success Factors

### ✅ Must Have (Already Completed):
- [x] Code structure with Qt Creator project
- [x] All 3 exercise types implemented (MCQ, Translate, TileOrder)
- [x] Factory Pattern (ExerciseFactory)
- [x] Strategy Pattern (StrategyGrader + 3 concrete implementations)
- [x] Singleton Pattern (AudioManager)
- [x] AppController orchestration layer
- [x] SRS scheduling logic
- [x] Profile and progress tracking

### 🔄 Must Have (In Progress):
- [ ] ContentRepository with JSON loading
- [ ] UI implementation (LessonView, ProfileView)
- [ ] Complete lesson flow: load → display → grade → next
- [ ] Updated UML diagrams matching code
- [ ] Design rationale document
- [ ] Video demonstration

### ⚠️ Nice to Have:
- Polish UI animations
- Comprehensive error handling
- Rich JSON content set (20+ exercises)
- Real audio files for AudioManager

---

## Implementation Notes

### What's Working:
- ✅ Domain layer completely implemented
- ✅ All design patterns functional
- ✅ AppController with Qt signals/slots
- ✅ XP calculations and mastery tracking
- ✅ Both build systems (Qt Creator .pro and CMake)

### Current Focus:
- 🎯 ContentRepository to load exercises from JSON
- 🎯 UI to make the application interactive and testable

### Code Quality Checklist:
- [x] Meaningful variable names
- [x] Comments explaining pattern usage
- [x] Functions small and focused
- [x] Clear layer separation (UI → Controller → Domain → Data)
- [ ] Frequent commits with clear messages

---

## Final Pre-Submission Checklist

- [ ] Code builds cleanly in Qt Creator
- [ ] All 3 exercise types testable end-to-end in UI
- [ ] Factory Pattern visible and documented
- [ ] Strategy Pattern visible and documented
- [ ] Singleton Pattern visible and documented
- [ ] Updated class diagram matches implementation
- [ ] Updated sequence diagram matches flow
- [ ] Design rationale PDF explains all decisions
- [ ] Video demonstrates features and code
- [ ] README.md complete with video link
- [ ] All files organized in correct folders
- [ ] GitHub repository clean and up-to-date
- [ ] Brightspace submission ready

---

## Questions Answered

**Q: How does exercise type determine which grader to use?**
A: AppController::createGraderForExercise() checks exercise->getType() and instantiates appropriate grader (MCQGrader, TranslateGrader, or TileOrderGrader).

**Q: How does SRSScheduler determine review queue?**
A: getReviewQueue() returns exercises where next_review_date <= QDate::currentDate().

**Q: How is UI separated from domain?**
A: UI components (LessonView, ProfileView) only interact with AppController via Qt signals/slots. They never access Exercise, Grader, or Profile directly.

**Q: Where does JSON content come from?**
A: ContentRepository::loadContent() reads src/core/data/content.json at startup, parses with QJsonDocument, and creates Exercise objects via ExerciseFactory.

---

**Last Updated:** After Phase 3.1 (AppController) completion
**Next Action:** Implement ContentRepository (Phase 3.2)
