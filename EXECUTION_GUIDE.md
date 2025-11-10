# BhashaQuest Refactoring - Detailed Execution Guide

## How to Use This Guide

This guide provides **specific, actionable steps** for each subtask. When ClaudeCodeHaiku works on a subtask, it should:

1. Read this guide for specific instructions
2. Follow the step-by-step checklist
3. Verify all items are complete
4. Commit changes with provided commit message
5. Mark subtask complete in REFACTORING_PLAN.md

---

## TASK 1: Code Organization & Structure Cleanup

### Subtask 1.1: Standardize Directory Structure

**Objective:** Create clean, consistent directory layout

**Steps:**
1. Copy entire BhashaQuest folder from `7807/BhashaQuestv2/BhashaQuest` to `2783/BhashaQuestV3`
2. Remove temporary directories:
   - `rm -rf build/` (remove build artifacts)
   - `rm -rf .qtcreator/` (remove Qt Creator metadata)
   - `del *.user` (remove Qt Creator config files)
3. Verify directory structure matches expected:
   ```
   BhashaQuestV3/
   ├── src/
   │   ├── core/
   │   │   ├── domain/        # 27 files (Exercise types, Graders, etc.)
   │   │   ├── controllers/   # 1 file (AppController.h/cpp)
   │   │   └── data/          # 2 files (ContentRepository + content.json)
   │   └── ui/                # 4+ files (Views + main.cpp)
   ├── assets/audio/          # (will be organized in Task 6)
   ├── CMakeLists.txt
   ├── BhashaQuest.pro
   ├── main.cpp
   ├── mainwindow.h/cpp/ui
   └── README.md
   ```
4. Create missing directories if needed:
   - `mkdir -p assets/audio`
   - `mkdir -p tests`
   - `mkdir -p report`
5. Create `.gitignore` with following content:
   ```
   build/
   .qtcreator/
   *.user
   *.o
   CMakeCache.txt
   CMakeFiles/
   cmake_install.cmake
   Makefile
   moc_*.cpp
   ui_*.h
   *.dylib
   *.so
   *.exe
   .DS_Store
   ```
6. Run `git status` and verify only expected files show as untracked

**Commit Message:**
```
refactor: Task 1.1 - Standardize directory structure

- Remove build artifacts and temporary files
- Create clean directory layout for final submission
- Add comprehensive .gitignore for version control
- Ensure all source files organized in consistent structure
```

**Verification:**
- [ ] Directory structure clean and organized
- [ ] `.gitignore` properly created
- [ ] `git status` shows clean state (only tracked files)
- [ ] No build artifacts or temporary files present

---

### Subtask 1.2: Standardize File Naming & Code Formatting

**Objective:** Ensure consistent naming conventions and code style

**Steps:**
1. Verify all files follow `ClassName.h` and `ClassName.cpp` pattern:
   - List all files: Find in Explorer or terminal
   - Check class names match file names (case-sensitive)
   - Rename any mismatched files
2. Check indentation consistency (should be 4 spaces):
   - Open each major file (sample from each directory)
   - Look for mixed tabs/spaces
   - Use IDE to reformat if needed: `Ctrl+A` then auto-format
3. Verify header guards are consistent (format: `#ifndef CLASSNAME_H`)
4. Check include order in all files:
   - System headers first (`#include <iostream>`)
   - Qt headers second (`#include <QString>`)
   - Project headers last (`#include "Exercise.h"`)
5. Remove trailing whitespace:
   - Use IDE find-replace: find ` +$` (regex), replace with nothing
6. Ensure all files end with newline character

**Quick Checklist:**
- [ ] All class files match naming convention
- [ ] Include guards use correct format
- [ ] Include order consistent across files
- [ ] Indentation is 4 spaces throughout
- [ ] No trailing whitespace
- [ ] All files end with newline

**Commit Message:**
```
refactor: Task 1.2 - Standardize file naming and code formatting

- Verify all files follow ClassName.h/cpp naming convention
- Standardize indentation to 4 spaces throughout codebase
- Ensure include guards and include order are consistent
- Remove trailing whitespace and ensure proper line endings
```

**Verification:**
- [ ] All files properly named and formatted
- [ ] Code compiles without formatting issues
- [ ] IDE shows no warnings about style

---

### Subtask 1.3: Remove Temporary & Generated Files

**Objective:** Clean up all non-essential and auto-generated files

**Steps:**
1. Check for and remove generated files:
   ```
   - Find all "moc_*.cpp" files and remove
   - Find all "ui_*.h" files and remove
   - Delete "*.o" object files
   - Remove "CMakeFiles/" directory
   - Delete "CMakeCache.txt"
   ```
2. Remove Qt Creator metadata:
   ```
   - Delete ".qtcreator/" folder entirely
   - Delete all "*.user" files
   - Delete ".qmake.stash"
   ```
3. Clean any IDE-specific files:
   ```
   - Remove ".vscode/" if present
   - Remove "*.swp" or "*.swo" (vim backups)
   ```
4. Verify clean state:
   ```bash
   git status  # Should show clean
   git ls-files | grep -E "moc_|ui_|\.o$"  # Should return nothing
   ```
5. Create fresh build to verify nothing is broken:
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```

**Commit Message:**
```
refactor: Task 1.3 - Clean up temporary and generated files

- Remove all build artifacts and object files
- Delete Qt Creator metadata (.qtcreator/, *.user files)
- Remove auto-generated moc_*.cpp and ui_*.h files
- Clean up IDE-specific files for clean repository state
```

**Verification:**
- [ ] No temporary files remaining
- [ ] `.gitignore` prevents re-adding them
- [ ] `git status` shows clean state
- [ ] Fresh build succeeds

---

## TASK 2: Build System Configuration & Unification

### Subtask 2.1: Audit & Update CMakeLists.txt

**Objective:** Verify and improve CMake configuration

**Steps:**
1. Open `CMakeLists.txt` and review current content
2. Verify all source files are listed:
   - Count files in `src/core/domain/` (should be ~27)
   - Verify all are in DOMAIN_SOURCES variable
   - Check CONTROLLER_SOURCES, DATA_SOURCES, UI_SOURCES lists
3. Update CMakeLists.txt with improvements:
   ```cmake
   # Add compiler flags for warnings
   if(MSVC)
       target_compile_options(BhashaQuest PRIVATE /W4)
   else()
       target_compile_options(BhashaQuest PRIVATE -Wall -Wextra -pedantic)
   endif()

   # Add C++ standard explicitly
   target_compile_features(BhashaQuest PRIVATE cxx_std_17)

   # Add optimization for Release builds
   if(NOT CMAKE_BUILD_TYPE)
       set(CMAKE_BUILD_TYPE Release)
   endif()
   ```
4. Verify Qt6 module includes:
   - Check: `find_package(Qt6 REQUIRED COMPONENTS Core Widgets Multimedia)`
   - All three modules present
5. Test build:
   ```bash
   cd BhashaQuestV3
   rm -rf build
   mkdir build && cd build
   cmake ..
   make  # or cmake --build . on Windows
   ```
6. Check for any warnings or errors
7. Verify executable created: `build/BhashaQuest` (or `build/Release/BhashaQuest.exe` on Windows)

**Commit Message:**
```
refactor: Task 2.1 - Audit and update CMakeLists.txt

- Verify all source files are properly listed
- Add compiler warning flags (-Wall -Wextra)
- Explicitly set C++17 standard requirement
- Configure Release build optimization
- Ensure all Qt6 modules are correctly found
```

**Verification:**
- [ ] CMakeLists.txt compiles without errors
- [ ] Executable builds successfully
- [ ] No compiler warnings
- [ ] Assets copy correctly to build directory

---

### Subtask 2.2: Update Qt Creator Project File (BhashaQuest.pro)

**Objective:** Ensure Qt .pro file is up-to-date and functional

**Steps:**
1. Open `BhashaQuest.pro` file
2. Update Qt modules section:
   ```pro
   QT += core gui multimedia
   ```
3. Verify CONFIG settings:
   ```pro
   CONFIG += c++17
   CONFIG += warn_on
   ```
4. Check SOURCE section includes all .cpp files
5. Check HEADERS section includes all .h files
6. Update include paths if needed:
   ```pro
   INCLUDEPATH += src
   INCLUDEPATH += src/core/domain
   INCLUDEPATH += src/core/controllers
   INCLUDEPATH += src/core/data
   INCLUDEPATH += src/ui
   ```
7. Verify RESOURCES section if using Qt resources
8. Test in Qt Creator:
   - Open file → project
   - Configure with Qt6 kit
   - Run build (Ctrl+B)
   - Verify no errors
9. Run application (Ctrl+R) and test basic functionality:
   - Window appears
   - Can click buttons
   - No crashes

**Commit Message:**
```
refactor: Task 2.2 - Update Qt Creator project file

- Verify all source and header files listed in BhashaQuest.pro
- Ensure Qt6 modules properly configured (Core, Widgets, Multimedia)
- Add C++17 standard configuration
- Update include paths for clean build
- Test build in Qt Creator environment
```

**Verification:**
- [ ] BhashaQuest.pro builds without errors in Qt Creator
- [ ] Application runs successfully
- [ ] No missing file warnings
- [ ] Qt6 modules properly detected

---

### Subtask 2.3: Create Build Instructions & Troubleshooting

**Objective:** Document clear, step-by-step build instructions

**Steps:**
1. Update README.md with comprehensive build section:
   ```markdown
   ## Build Instructions

   ### Prerequisites
   - Qt6 (Core, Widgets, Multimedia modules)
   - CMake 3.16 or higher
   - C++17 compatible compiler
     - GCC 7+, Clang 5+, MSVC 2017+

   ### Building with CMake

   ```bash
   cd BhashaQuestV3
   mkdir build && cd build
   cmake ..
   make  # or cmake --build . on Windows
   ./BhashaQuest  # or BhashaQuest.exe on Windows
   ```

   ### Building with Qt Creator

   1. Open Qt Creator
   2. File → Open File or Project
   3. Select BhashaQuest.pro
   4. Configure with Qt6 kit
   5. Build (Ctrl+B) and Run (Ctrl+R)
   ```
2. Add troubleshooting section to README:
   ```markdown
   ## Troubleshooting

   **Build fails: "Qt6 not found"**
   - Install Qt6 from qt.io
   - Set CMAKE_PREFIX_PATH to Qt6 installation
   - Example: export CMAKE_PREFIX_PATH=/path/to/qt6

   **CMake Error: "Could not find Qt6Core"**
   - Verify Qt6 installation includes Core module
   - Check CMakeLists.txt lists required modules

   **Asset files not found at runtime**
   - Verify content.json exists in build directory
   - Check audio files in assets/audio/ directory
   - CMakeLists.txt should copy assets automatically
   ```
3. Add cleaning instructions:
   ```markdown
   ## Cleaning Build

   To remove build artifacts:

   ```bash
   rm -rf build/      # Remove entire build directory
   ```
   ```
4. Add testing instructions (placeholder for Task 5):
   ```markdown
   ## Running Tests

   Tests will be added in future updates.
   ```
5. Create ARCHITECTURE.md with system overview
6. Update README links to point to new docs

**Commit Message:**
```
refactor: Task 2.3 - Create build instructions and troubleshooting

- Add comprehensive build instructions for CMake and Qt Creator
- Document system requirements and dependencies
- Add troubleshooting section for common build issues
- Include cleaning and testing instructions
- Update README with links to detailed documentation
```

**Verification:**
- [ ] README.md is clear and complete
- [ ] Build instructions are tested and work
- [ ] Troubleshooting covers common issues
- [ ] New developer can build app from README alone

---

## TASK 3: Code Documentation & Comments

### Subtask 3.1: Document Design Patterns & Architecture

**Objective:** Add clear documentation explaining design patterns and architecture

**Steps:**
1. Create `ARCHITECTURE.md` with overview:
   ```markdown
   # BhashaQuest Architecture

   ## Layered Architecture

   ```
   UI Layer (Qt Widgets)
       ↓ signals/slots
   Controller Layer (AppController)
       ↓ orchestrates
   Domain Layer (Exercise, Grader, etc.)
       ↓ uses
   Data Layer (ContentRepository, JSON)
   ```

   ## Key Principle
   UI components communicate ONLY through AppController.
   Domain objects are never directly accessed from UI.
   ```
2. Add Factory Pattern documentation:
   - Open `src/core/domain/ExerciseFactory.h`
   - Add detailed comment block:
   ```cpp
   /**
    * Factory Pattern Implementation
    *
    * ExerciseFactory creates Exercise instances based on type.
    * This allows new exercise types to be added without modifying
    * the factory client code (ContentRepository).
    *
    * @see ExerciseFactory::createExercise()
    * @see MCQExercise, TranslateExercise, TileOrderExercise
    */
   ```
3. Add Strategy Pattern documentation:
   - Open `src/core/domain/StrategyGrader.h`
   - Add detailed comment explaining pattern:
   ```cpp
   /**
    * Strategy Pattern Implementation
    *
    * StrategyGrader is the strategy interface.
    * Concrete implementations: MCQGrader, TranslateGrader, TileOrderGrader
    *
    * AppController uses appropriate grader for each exercise type.
    * This allows grading algorithms to be swapped at runtime.
    *
    * @see StrategyGrader, MCQGrader, TranslateGrader, TileOrderGrader
    * @see AppController::gradeAnswer()
    */
   ```
4. Add Singleton Pattern documentation:
   - Open `src/core/domain/AudioManager.h`
   - Add detailed comment:
   ```cpp
   /**
    * Singleton Pattern Implementation
    *
    * AudioManager provides centralized audio playback management.
    * getInstance() ensures only one instance exists throughout app lifetime.
    * Prevents multiple QMediaPlayer instances from conflicting.
    *
    * @see AudioManager::getInstance()
    */
   ```
5. Add data flow documentation in AppController:
   ```cpp
   /**
    * Data Flow for Answer Submission:
    *
    * 1. LessonView::answerSubmitted(answer) signal
    * 2. AppController::submitAnswer(answer) slot
    * 3. Create appropriate grader via createGraderForExercise()
    * 4. Call grader->grade(answer, exercise) - Strategy Pattern
    * 5. Update profile with XP and mastery
    * 6. Emit answerGraded(result) → LessonView displays feedback
    * 7. Load next exercise via loadNextExercise()
    * 8. Emit exerciseChanged(nextExercise) → LessonView updates UI
    *
    * @see submitAnswer(), gradeAnswer(), updateProfile()
    */
   ```
6. Create visual documentation showing layer separation

**Commit Message:**
```
refactor: Task 3.1 - Document design patterns and architecture

- Create comprehensive ARCHITECTURE.md explaining layered design
- Add Factory Pattern documentation in ExerciseFactory.h
- Add Strategy Pattern documentation in StrategyGrader.h
- Add Singleton Pattern documentation in AudioManager.h
- Document complete data flow for exercise submission
- Add visual diagrams explaining layer separation
```

**Verification:**
- [ ] ARCHITECTURE.md explains all layers
- [ ] Each pattern has clear documentation
- [ ] Comments explain WHY patterns are used
- [ ] Links between files are clear

---

### Subtask 3.2: Add Method-Level Documentation

**Objective:** Add Doxygen-style documentation to all public methods

**Steps:**
1. Add documentation to all public classes. For each major class:
   - Exercise.h - Abstract base class
   - MCQExercise.h, TranslateExercise.h, TileOrderExercise.h
   - ExerciseFactory.h
   - StrategyGrader.h and concrete graders
   - AudioManager.h
   - AppController.h
   - ContentRepository.h
   - Profile.h, SkillProgress.h, SRSScheduler.h

2. Use this Doxygen format for all public methods:
   ```cpp
   /**
    * Brief one-liner description.
    *
    * Longer description explaining what method does,
    * how it works, and when to use it.
    *
    * @param paramName Description of parameter
    * @return Description of return value
    * @throws ExceptionType If something goes wrong
    * @see RelatedMethod, RelatedClass
    *
    * @example
    * ```cpp
    * Exercise* ex = factory.createExercise("MCQ", spec);
    * ```
    */
   ```

3. Document key algorithms:
   - TranslateGrader::grade() - Explain Levenshtein distance fuzzy matching
   - SRSScheduler - Explain interval calculation (Easy→7d, etc.)
   - Profile::updateStreak() - Explain streak logic

4. Document error cases:
   - What happens if content.json is missing?
   - What if audio file not found?
   - Invalid exercise type?

5. Add @file comments to each header:
   ```cpp
   /**
    * @file ExerciseFactory.h
    * @brief Factory for creating Exercise instances
    *
    * Implements the Factory Pattern for creating different exercise types.
    * Supports MCQ, Translate, and TileOrder exercises.
    *
    * @see Exercise, MCQExercise, TranslateExercise, TileOrderExercise
    */
   ```

**Commit Message:**
```
refactor: Task 3.2 - Add comprehensive method-level documentation

- Add Doxygen-style comments to all public methods
- Document parameters, return values, and exceptions
- Explain complex algorithms (Levenshtein distance, SRS intervals)
- Add @file comments to all headers
- Include usage examples in method documentation
- Document error handling and edge cases
```

**Verification:**
- [ ] All public methods have documentation
- [ ] Complex algorithms are explained
- [ ] Error cases are documented
- [ ] Format is consistent (Doxygen style)

---

### Subtask 3.3: Create DEVELOPER_GUIDE.md

**Objective:** Provide clear guidance for extending the application

**Steps:**
1. Create `DEVELOPER_GUIDE.md` with step-by-step extension guides:

   ```markdown
   # Developer Guide - BhashaQuest

   ## Adding a New Exercise Type

   This guide explains how to add a new exercise type (e.g., ListenExercise).

   ### Step 1: Create Exercise Class

   1. Create `ListenExercise.h` inheriting from Exercise
   2. Implement pure virtual methods
   3. Add type-specific data members (e.g., audioFile, options)

   ### Step 2: Create Grader Class

   1. Create `ListenGrader.h` inheriting from StrategyGrader
   2. Implement grade() method with grading logic

   ### Step 3: Update Factory

   1. Add case in ExerciseFactory::createExercise():
      ```cpp
      if (type == "Listen") {
          return new ListenExercise(spec);
      }
      ```

   ### Step 4: Update UI

   1. Add widget creation in LessonView::updateExercise()
   2. Add answer collection logic

   ### Step 5: Update JSON

   1. Add new exercise type to content.json:
      ```json
      {
          "id": "ex-listen-1",
          "type": "Listen",
          "audioFile": "path/to/audio.mp3"
      }
      ```

   ### Step 6: Test

   1. Write unit test for ListenGrader
   2. Test complete lesson flow with new type
   ```

2. Add JSON content schema documentation:
   ```markdown
   ## Content JSON Schema

   ### Skill Object
   ```json
   {
       "id": "skill-id",
       "name": "Skill Name",
       "language": "Target Language",
       "exercises": [...]
   }
   ```

   ### MCQ Exercise
   ```json
   {
       "id": "ex1",
       "type": "MCQ",
       "prompt": "Question text",
       "options": ["option1", "option2", "option3", "option4"],
       "correctIndex": 0,
       "skillId": "skill-id",
       "difficulty": 1
   }
   ```

   ### Translate Exercise
   ```json
   {
       "id": "ex2",
       "type": "Translate",
       "prompt": "English text",
       "correctAnswers": ["correct", "accepted"],
       "skillId": "skill-id",
       "difficulty": 2
   }
   ```

   ### TileOrder Exercise
   ```json
   {
       "id": "ex3",
       "type": "TileOrder",
       "prompt": "Reorder words",
       "tiles": ["word1", "word2", "word3"],
       "correctOrder": [0, 1, 2],
       "skillId": "skill-id",
       "difficulty": 3
   }
   ```
   ```

3. Add debugging tips:
   ```markdown
   ## Debugging Tips

   ### Enable Qt Debug Output
   ```cpp
   qDebug() << "Debug message";
   qWarning() << "Warning message";
   ```

   ### Check Content Loading
   - Add debug statements in ContentRepository::loadContent()
   - Verify JSON parsing succeeds

   ### Test Audio Playback
   - Check audio file paths are correct
   - Verify file exists in build directory
   - Check AudioManager::getInstance()->play()
   ```

4. Add architecture decision records (ADRs):
   ```markdown
   ## Architecture Decisions

   ### ADR 1: Why Layered Architecture?
   - Maintains separation of concerns
   - UI never directly touches domain logic
   - Easier to test and maintain

   ### ADR 2: Why Qt Signals/Slots?
   - Decouples UI from controller
   - Thread-safe communication
   - Qt-native approach
   ```

**Commit Message:**
```
refactor: Task 3.3 - Create comprehensive DEVELOPER_GUIDE.md

- Add step-by-step guide for adding new exercise types
- Document JSON schema with examples for each exercise type
- Include debugging tips and common issues
- Document architecture decisions and rationale
- Provide troubleshooting section for common problems
- Include code examples for clear understanding
```

**Verification:**
- [ ] DEVELOPER_GUIDE.md is complete
- [ ] Step-by-step guides are clear
- [ ] JSON schema is documented with examples
- [ ] New developer could follow guide to add feature

---

## TASK 4: User Interface Polish & Consistency

### Subtask 4.1: Fix Layout & Responsiveness Issues

**Objective:** Ensure UI works well at different window sizes

**Steps:**
1. Test application at various window sizes:
   - 800×600 (minimum)
   - 1024×768
   - 1280×720
   - 1920×1080
   - Test resizing dynamically

2. Check LessonView for issues:
   - Exercise prompt should be readable
   - Answer widgets (buttons, text fields) should be accessible
   - Buttons should not overlap or become cut off
   - Progress bar should be visible and properly sized
   - Feedback message area should expand/contract appropriately

3. Check ProfileView for issues:
   - Skill list should scroll if many skills
   - Progress bars should not overflow container
   - Stats display should center properly
   - Layout should work with 1 skill and 20 skills

4. Check MainWindow for issues:
   - Skill selector dropdown should be readable
   - Buttons should not overlap
   - Status bar should always be visible
   - Stacked widget should properly switch views

5. Fix identified issues:
   - Use QVBoxLayout/QHBoxLayout properly
   - Set minimum/maximum sizes for widgets
   - Use stretch factors for responsive sizing
   - Add scrollbars where needed (QScrollArea)

6. Test with high DPI display (if possible) - ensure text is readable

**Commit Message:**
```
refactor: Task 4.1 - Fix layout and responsiveness issues

- Test UI at multiple window sizes (800×600 to 1920×1080)
- Fix stretched or misaligned widgets in LessonView
- Ensure responsive behavior when resizing window
- Add scrollbars where needed for long content
- Verify all buttons and inputs are accessible
- Test with high DPI displays for readability
```

**Verification:**
- [ ] UI works at 800×600 minimum size
- [ ] UI works at 1920×1080 maximum size
- [ ] Resizing window doesn't break layout
- [ ] Scrollbars appear when needed
- [ ] No overlapping widgets

---

### Subtask 4.2: Standardize Styling & Color Scheme

**Objective:** Create consistent visual style across application

**Steps:**
1. Define color palette (add to stylesheet):
   ```cpp
   // Primary Colors
   #define COLOR_PRIMARY_GREEN "#4CAF50"
   #define COLOR_PRIMARY_BLUE "#2196F3"

   // Semantic Colors
   #define COLOR_SUCCESS_GREEN "#4CAF50"
   #define COLOR_ERROR_RED "#F44336"
   #define COLOR_WARNING_YELLOW "#FF9800"
   #define COLOR_NEUTRAL_GRAY "#757575"

   // Background Colors
   #define COLOR_BG_LIGHT "#F5F5F5"
   #define COLOR_BG_WHITE "#FFFFFF"
   #define COLOR_BG_DARK "#212121"
   ```

2. Create stylesheet.css with consistent definitions:
   ```css
   QPushButton {
       background-color: #4CAF50;
       color: white;
       padding: 8px 16px;
       border-radius: 4px;
       font-weight: bold;
   }

   QPushButton:hover {
       background-color: #45a049;
   }

   QPushButton:pressed {
       background-color: #3d8b40;
   }

   QLabel#title {
       font-size: 18px;
       font-weight: bold;
       color: #212121;
   }

   QProgressBar {
       border: none;
       border-radius: 4px;
       background-color: #e0e0e0;
       height: 10px;
   }

   QProgressBar::chunk {
       background-color: #4CAF50;
       border-radius: 4px;
   }
   ```

3. Apply stylesheet consistently:
   - Define fonts: title (18pt), body (12pt), label (11pt)
   - Apply to all buttons: consistent size, padding, hover states
   - Apply to all text labels: consistent font sizes
   - Apply to all progress bars: consistent appearance

4. Implement focus indicators:
   - Add visible outline when widget has focus
   - Use COLOR_PRIMARY_BLUE for focus outline
   - Ensure accessibility for keyboard navigation

5. Test contrast ratios for accessibility:
   - Text on background should have >= 4.5:1 ratio (WCAG AA)
   - Check with contrast checker tool
   - Adjust colors if needed for accessibility

6. Apply dark mode considerations (optional for future):
   - Structure code to support theme switching
   - Use QStyleFactory for consistent appearance

**Commit Message:**
```
refactor: Task 4.2 - Standardize styling and color scheme

- Define consistent color palette across application
- Create stylesheet with unified button, label, progress bar styles
- Standardize font sizes (18pt title, 12pt body, 11pt label)
- Ensure focus indicators visible for keyboard navigation
- Verify WCAG AA contrast ratios for accessibility
- Test styling at different window sizes
```

**Verification:**
- [ ] Color scheme consistent across all views
- [ ] Button styling uniform everywhere
- [ ] Font sizes consistent (18/12/11pt)
- [ ] Focus indicators clearly visible
- [ ] Contrast ratios meet WCAG AA standard

---

### Subtask 4.3: Improve User Feedback & Error Handling

**Objective:** Add clear feedback and graceful error handling

**Steps:**
1. Improve visual feedback for user actions:
   - Button press should show visual feedback (press animation)
   - Progress bar should update smoothly
   - Answer submission should disable button until processed
   - Feedback message should be clearly visible

2. Implement error handling for common issues:
   ```cpp
   // Missing content.json
   if (!QFile::exists("content.json")) {
       QMessageBox::critical(this, "Error",
           "content.json not found. Please verify assets are properly deployed.");
       return;
   }

   // JSON parsing error
   QJsonParseError error;
   QJsonDocument doc = QJsonDocument::fromJson(data, &error);
   if (error.error != QJsonParseError::NoError) {
       qWarning() << "JSON Parse Error:" << error.errorString();
       QMessageBox::warning(this, "Error",
           "Failed to parse content. Please check content.json format.");
       return;
   }

   // Missing audio file
   QString audioFile = exercise->getAudioFile();
   if (!QFile::exists(audioFile)) {
       qWarning() << "Audio file not found:" << audioFile;
       // Gracefully continue without audio
       // Or: QMessageBox::warning(this, "Warning", "Audio file not found.");
   }
   ```

3. Add loading indicators:
   - Show "Loading..." label while loading content
   - Disable buttons during processing
   - Show progress during lesson loading

4. Improve feedback messages:
   - Correct answer: "Great! That's correct!"
   - Incorrect answer: "Not quite. The correct answer is [X]"
   - Feedback should include learning value

5. Add input validation:
   - Validate translate answer is not empty
   - Validate MCQ selection before submit
   - Show clear error if input invalid

6. Graceful degradation:
   - If audio not available, continue without sound (with warning)
   - If skill has no exercises, show helpful message
   - If exercise fails to load, show clear error

7. Status messages:
   - Update status bar with current action
   - Show XP earned after each exercise
   - Display streak count prominently

**Commit Message:**
```
refactor: Task 4.3 - Improve user feedback and error handling

- Add visual feedback for all user actions (button presses, loading)
- Implement comprehensive error handling for missing files
- Add helpful error messages for common issues
- Implement graceful degradation (e.g., continue without audio)
- Add input validation with clear error messages
- Improve status messages and feedback text
- Show clear learning feedback after each exercise
```

**Verification:**
- [ ] Button presses show visual feedback
- [ ] Error messages are clear and helpful
- [ ] Application handles missing files gracefully
- [ ] Loading states are clearly indicated
- [ ] User always knows what's happening

---

## TASK 5: Testing Infrastructure & Quality Assurance

### Subtask 5.1: Set Up Unit Testing Framework

**Objective:** Integrate testing framework and create test infrastructure

**Steps:**
1. Choose testing framework: Google Test (gtest)
   - Download/include Google Test library
   - Or: Use Qt Test framework (part of Qt)

2. Set up test directory structure:
   ```
   tests/
   ├── unit/
   │   ├── CMakeLists.txt
   │   ├── grader_tests.cpp
   │   ├── factory_tests.cpp
   │   └── profile_tests.cpp
   └── integration/
       └── lesson_flow_tests.cpp
   ```

3. Update main CMakeLists.txt to include tests:
   ```cmake
   enable_testing()
   add_subdirectory(tests)

   # In tests/CMakeLists.txt:
   find_package(GTest REQUIRED)
   add_executable(unit_tests ${TEST_SOURCES})
   target_link_libraries(unit_tests PRIVATE GTest::gtest_main)
   gtest_discover_tests(unit_tests)
   ```

4. Create sample test file (tests/unit/grader_tests.cpp):
   ```cpp
   #include <gtest/gtest.h>
   #include "MCQGrader.h"
   #include "MCQExercise.h"

   class MCQGraderTest : public ::testing::Test {
   protected:
       MCQGrader grader;
       // Setup test fixtures
   };

   TEST_F(MCQGraderTest, CorrectAnswerReturnsTrue) {
       // Test implementation
   }
   ```

5. Configure build system:
   - Ensure tests compile with main application code
   - Set up test execution via CMake
   - Create test running script

6. Document how to run tests:
   ```bash
   cd build
   ctest  # or make test
   ```

**Commit Message:**
```
refactor: Task 5.1 - Set up unit testing infrastructure

- Integrate Google Test or Qt Test framework
- Create tests/ directory structure for unit and integration tests
- Update CMakeLists.txt to build and run tests
- Create sample unit test for MCQGrader
- Document test execution procedure
- Set up test discovery and reporting
```

**Verification:**
- [ ] Test framework integrated
- [ ] Test directory created
- [ ] Sample tests compile and run
- [ ] `ctest` or `make test` executes tests
- [ ] Test results are reported

---

### Subtask 5.2: Write Core Unit Tests

**Objective:** Add unit tests for critical domain logic

**Steps:**
1. Test MCQGrader:
   ```cpp
   TEST(MCQGrader, CorrectIndexReturnsTrue) {
       MCQGrader grader;
       MCQExercise ex("Choose color", {"red", "blue", "green"}, 0);
       Result result = grader.grade("0", &ex);
       EXPECT_TRUE(result.correct);
   }

   TEST(MCQGrader, WrongIndexReturnsFalse) {
       MCQGrader grader;
       MCQExercise ex("Choose color", {"red", "blue", "green"}, 0);
       Result result = grader.grade("1", &ex);
       EXPECT_FALSE(result.correct);
   }
   ```

2. Test TranslateGrader:
   ```cpp
   TEST(TranslateGrader, ExactMatchReturnsTrue) {
       TranslateGrader grader;
       TranslateExercise ex("Translate water", {"पानी", "pani"});
       Result result = grader.grade("पानी", &ex);
       EXPECT_TRUE(result.correct);
   }

   TEST(TranslateGrader, FuzzyMatchWithinDistance) {
       TranslateGrader grader;
       TranslateExercise ex("Translate water", {"water"});
       Result result = grader.grade("watr", &ex);  // Levenshtein distance = 1
       EXPECT_TRUE(result.correct);
   }

   TEST(TranslateGrader, FuzzyMatchBeyondDistance) {
       TranslateGrader grader;
       TranslateExercise ex("Translate water", {"water"});
       Result result = grader.grade("xyz", &ex);  // Distance > 2
       EXPECT_FALSE(result.correct);
   }
   ```

3. Test TileOrderGrader:
   ```cpp
   TEST(TileOrderGrader, CorrectOrderReturnsTrue) {
       TileOrderGrader grader;
       TileOrderExercise ex("Reorder", {"hello", "world"}, {0, 1});
       Result result = grader.grade("0;1", &ex);
       EXPECT_TRUE(result.correct);
   }

   TEST(TileOrderGrader, WrongOrderReturnsFalse) {
       TileOrderGrader grader;
       TileOrderExercise ex("Reorder", {"hello", "world"}, {0, 1});
       Result result = grader.grade("1;0", &ex);
       EXPECT_FALSE(result.correct);
   }
   ```

4. Test ExerciseFactory:
   ```cpp
   TEST(ExerciseFactory, CreatesMCQExercise) {
       QJsonObject spec;
       spec["type"] = "MCQ";
       spec["prompt"] = "Choose";
       spec["options"] = QJsonArray{"a", "b", "c", "d"};
       spec["correctIndex"] = 0;

       Exercise* ex = ExerciseFactory::createExercise(spec);
       EXPECT_NE(ex, nullptr);
       EXPECT_EQ(ex->getType(), "MCQ");
       delete ex;
   }
   ```

5. Test Profile logic:
   ```cpp
   TEST(Profile, AddXPIncreaseTotal) {
       Profile profile("TestUser");
       EXPECT_EQ(profile.getTotalXP(), 0);
       profile.addXP(10);
       EXPECT_EQ(profile.getTotalXP(), 10);
   }

   TEST(Profile, StreakIncrementOnConsecutiveDays) {
       Profile profile("TestUser");
       profile.updateStreak();
       EXPECT_EQ(profile.getStreak(), 1);
   }
   ```

6. Aim for >70% code coverage on domain logic
7. Run coverage report: `lcov` or IDE coverage tools

**Commit Message:**
```
refactor: Task 5.2 - Write comprehensive unit tests

- Test MCQGrader with correct and incorrect answers
- Test TranslateGrader fuzzy matching with Levenshtein distance
- Test TileOrderGrader with correct and incorrect orderings
- Test ExerciseFactory creates correct exercise types
- Test Profile XP calculation and streak logic
- Achieve >70% code coverage on domain layer
- Add test documentation and usage examples
```

**Verification:**
- [ ] All grader tests pass
- [ ] Factory tests pass
- [ ] Profile tests pass
- [ ] Code coverage >70% on domain layer
- [ ] Tests run via `ctest`

---

### Subtask 5.3: Add Integration Tests & Manual Test Cases

**Objective:** Test complete flows and create manual testing guide

**Steps:**
1. Create integration tests for complete lesson flow:
   ```cpp
   TEST(LessonFlow, CompleteExerciseSequence) {
       // Setup
       ContentRepository repo;
       repo.loadContent();

       AppController controller(&repo);
       QVector<Exercise*> exercises = repo.getExercisesForSkill("nepali-greetings");

       // Start lesson
       controller.startLesson(exercises);

       // Answer first exercise
       Result result = controller.submitAnswer("0");  // MCQ answer
       EXPECT_TRUE(result.correct || !result.correct);  // Just verify processing

       // Check profile updated
       Profile* profile = controller.getProfile();
       EXPECT_GT(profile->getTotalXP(), 0);
   }
   ```

2. Test error scenarios:
   ```cpp
   TEST(ErrorHandling, MissingContentJSON) {
       ContentRepository repo;
       // Temporarily move content.json
       bool loaded = repo.loadContent();
       EXPECT_FALSE(loaded);  // Should fail gracefully
   }

   TEST(ErrorHandling, InvalidJSONFormat) {
       // Test with corrupted JSON file
   }
   ```

3. Create MANUAL_TEST_CASES.md:
   ```markdown
   # Manual Test Cases

   ## Test 1: Launch Application
   - [ ] Application starts without crashes
   - [ ] Window displays with all controls visible
   - [ ] Skill selector populated with all skills

   ## Test 2: Start Lesson
   - [ ] Click "Start Lesson" button
   - [ ] First exercise displays correctly
   - [ ] Appropriate widgets created (MCQ buttons, etc.)

   ## Test 3: Answer MCQ Exercise
   - [ ] Select radio button option
   - [ ] Click Submit button
   - [ ] Feedback displays (green for correct, red for incorrect)
   - [ ] Progress bar updates (1/N)
   - [ ] Click "Next" button

   ## Test 4: Answer Translate Exercise
   - [ ] Type answer in text field
   - [ ] Press Enter or click Submit
   - [ ] Feedback displays with correct answer
   - [ ] Fuzzy matching works (typos accepted)

   ## Test 5: Answer TileOrder Exercise
   - [ ] Drag tiles to reorder
   - [ ] Submit reordered sequence
   - [ ] Feedback displays correctness

   ## Test 6: Complete Lesson
   - [ ] Complete all exercises in skill
   - [ ] Completion screen displays
   - [ ] Total XP shown correctly
   - [ ] Message box shows congratulations

   ## Test 7: View Profile
   - [ ] Click "View Profile" button
   - [ ] All stats display correctly
   - [ ] Progress bars show mastery levels
   - [ ] Accuracy percentages display

   ## Test 8: Return to Lesson
   - [ ] Click "Back to Lesson" from profile
   - [ ] Return to lesson view
   - [ ] Skill selector visible
   ```

4. Test audio functionality:
   - Verify audio files exist in build directory
   - Test audio playback (if audio files present)
   - Test graceful handling if audio missing

5. Performance testing:
   - Time application startup
   - Time content loading
   - Verify smooth 60fps UI updates
   - Monitor memory usage

6. Create TEST_REPORT.md:
   ```markdown
   # Test Report

   ## Unit Tests
   - Grader Tests: ✅ 12 passing
   - Factory Tests: ✅ 8 passing
   - Profile Tests: ✅ 10 passing

   ## Integration Tests
   - Lesson Flow: ✅ 5 passing
   - Error Handling: ✅ 6 passing

   ## Manual Tests
   - [List all manual test results]

   ## Code Coverage
   - Domain Layer: 85%
   - Controller Layer: 70%
   - Total: 75%
   ```

**Commit Message:**
```
refactor: Task 5.3 - Add integration tests and manual test cases

- Create integration tests for complete lesson flow
- Test error scenarios (missing files, invalid JSON)
- Create comprehensive manual test cases in MANUAL_TEST_CASES.md
- Test audio functionality and graceful degradation
- Perform performance testing and memory monitoring
- Create TEST_REPORT.md with results and coverage
- Document all test results and findings
```

**Verification:**
- [ ] Integration tests pass
- [ ] Error handling tested
- [ ] Manual test cases documented
- [ ] TEST_REPORT.md complete
- [ ] All test results documented

---

## TASK 6: Asset Organization & Resource Management

### Subtask 6.1: Organize Audio Assets

**Objective:** Structure and organize all audio files

**Steps:**
1. Review current audio files:
   - Check `assets/audio/` directory
   - List all current audio files
   - Identify what each audio file is for

2. Create organized directory structure:
   ```
   assets/
   └── audio/
       ├── feedback/
       │   ├── correct.mp3        # Success sound
       │   ├── incorrect.mp3      # Error sound
       │   └── complete.mp3       # Lesson completion sound
       ├── exercises/
       │   ├── nepali-greetings/
       │   │   ├── hello.mp3
       │   │   ├── goodbye.mp3
       │   │   └── thank-you.mp3
       │   ├── nepali-food/
       │   │   ├── rice.mp3
       │   │   └── water.mp3
       │   └── nepali-numbers/
       │       ├── one.mp3
       │       ├── two.mp3
       │       └── three.mp3
       └── README.md               # Audio documentation
   ```

3. Create README.md in assets/audio/:
   ```markdown
   # Audio Assets

   ## Feedback Sounds
   - correct.mp3: Success feedback (played on correct answer)
   - incorrect.mp3: Error feedback (played on incorrect answer)
   - complete.mp3: Lesson completion sound

   ## Exercise Audio
   Organized by skill, then exercise name.

   ### Naming Convention
   - Format: [exercise-name].mp3
   - Example: hello.mp3, goodbye.mp3, thank-you.mp3

   ## Audio Specifications
   - Format: MP3 or WAV
   - Sample Rate: 44.1 kHz or higher
   - Bitrate: 128 kbps or higher
   - Duration: 1-3 seconds each

   ## Adding New Audio
   1. Record or obtain audio file
   2. Convert to MP3 (128 kbps recommended)
   3. Place in appropriate folder
   4. Update content.json with correct path
   ```

4. Verify all referenced files in content.json exist:
   - Read content.json
   - For each exercise, verify audioFile path exists
   - Create missing files or update references

5. Update content.json paths if needed:
   - Paths should be relative: `assets/audio/exercises/skill-id/file.mp3`
   - Verify paths work from build directory

**Commit Message:**
```
refactor: Task 6.1 - Organize audio assets with structure

- Create organized directory structure for audio files
- Separate feedback sounds from exercise audio
- Organize exercise audio by skill
- Verify all content.json references point to existing files
- Create README.md documenting audio organization
- Ensure consistent naming convention for all files
```

**Verification:**
- [ ] Audio directory structure clean and organized
- [ ] All referenced files exist
- [ ] content.json paths are correct
- [ ] README.md documents organization
- [ ] No broken audio references

---

### Subtask 6.2: Create Placeholder Assets

**Objective:** Generate or obtain placeholder audio files

**Steps:**
1. Create placeholder audio files:
   - Option A: Use text-to-speech to generate placeholder audio
   - Option B: Use silence tracks as placeholders
   - Option C: Record simple placeholder sounds

2. Generate basic audio files (or document where to obtain):
   ```
   For each exercise in content.json:
   - Create or link audio file
   - Place in correct directory
   - Ensure file format is MP3
   - Test playback in application
   ```

3. Create feedback sounds:
   ```
   assets/audio/feedback/
   ├── correct.mp3      # Short positive sound (chime/ding)
   ├── incorrect.mp3    # Short negative sound (buzz)
   └── complete.mp3     # Positive completion sound
   ```

4. Test audio playback:
   - Run application
   - Go to lesson
   - Complete exercise
   - Verify audio plays (or gracefully skipped if missing)

5. Document audio sources:
   - Create AUDIO_SOURCES.md
   - List where each audio file came from
   - Note any licenses or attribution

6. Consider audio optimization:
   - Files should be < 500KB each
   - All same format (MP3 recommended)
   - Consistent volume levels

**Commit Message:**
```
refactor: Task 6.2 - Create or obtain placeholder audio files

- Generate or obtain placeholder audio for all exercises
- Create feedback sounds (correct, incorrect, complete)
- Place all files in correct directories per naming convention
- Test audio playback in running application
- Create AUDIO_SOURCES.md documenting origins
- Ensure files are optimized for distribution
```

**Verification:**
- [ ] All content.json exercises have audio files
- [ ] Audio files exist in correct directories
- [ ] Audio playback works in application
- [ ] File sizes reasonable (< 500KB each)
- [ ] AUDIO_SOURCES.md documents all sources

---

### Subtask 6.3: Set Up Asset Deployment

**Objective:** Ensure assets copy correctly to build output

**Steps:**
1. Verify CMakeLists.txt asset copying:
   ```cmake
   # In CMakeLists.txt, should have:
   file(COPY ${CMAKE_SOURCE_DIR}/assets
        DESTINATION ${CMAKE_BINARY_DIR})
   file(COPY ${CMAKE_SOURCE_DIR}/src/core/data/content.json
        DESTINATION ${CMAKE_BINARY_DIR})
   ```

2. Test build process:
   ```bash
   cd BhashaQuestV3
   rm -rf build
   mkdir build && cd build
   cmake ..
   make
   # Check build directory
   ls assets/
   ls content.json
   ```

3. Verify executable can locate assets:
   - Application should find content.json in current directory
   - Audio files should be found relative to executable
   - Test by running executable from build directory

4. Create deployment checklist:
   ```markdown
   # Asset Deployment Checklist

   - [ ] CMakeLists.txt includes asset copy commands
   - [ ] `cmake` command creates assets/ in build directory
   - [ ] content.json copies to build directory
   - [ ] All audio files copy to build/assets/audio/
   - [ ] Application can locate all files at runtime
   - [ ] No missing file errors on startup
   - [ ] Audio plays correctly in lesson
   ```

5. Create deployment guide:
   ```markdown
   # Deployment Guide

   ## For Distribution
   1. Build application: `mkdir build && cd build && cmake .. && make`
   2. Executable location: `build/BhashaQuest` (or .exe on Windows)
   3. Required files alongside executable:
      - assets/ directory (with all subdirectories)
      - content.json file
   4. Create distribution package:
      ```bash
      mkdir BhashaQuest-release
      cp build/BhashaQuest BhashaQuest-release/
      cp -r assets/ BhashaQuest-release/
      cp src/core/data/content.json BhashaQuest-release/
      # Users download and run BhashaQuest executable
      ```
   ```

6. Test deployment:
   - Copy executable and required files to temp directory
   - Run executable from that location
   - Verify all assets load correctly

**Commit Message:**
```
refactor: Task 6.3 - Set up proper asset deployment

- Verify CMakeLists.txt correctly copies assets to build directory
- Test that executable can locate all assets at runtime
- Create deployment checklist for distribution
- Create deployment guide for packaging application
- Test deployment by running from different directory
- Document asset location requirements
```

**Verification:**
- [ ] Assets copy correctly during build
- [ ] Application finds all assets at runtime
- [ ] Executable runs from any directory with assets
- [ ] Deployment guide is clear and tested
- [ ] Distribution checklist complete

---

## TASK 7: Final Documentation & Submission Preparation

### Subtask 7.1: Update README.md & Create Submission Package

**Objective:** Create comprehensive README and organize submission

**Steps:**
1. Rewrite README.md comprehensively:
   ```markdown
   # BhashaQuest - Language Learning Application

   A Duolingo-inspired language learning application built with C++ and Qt6.
   Designed for the CS 3307 Software Engineering course at Western University.

   ## Features

   - **3 Exercise Types:** Multiple Choice, Translation, Word Reordering
   - **Spaced Repetition:** Smart scheduling for effective learning
   - **User Profiles:** Track progress and XP across skills
   - **Gamification:** Streaks and XP rewards for engagement

   ## Quick Start

   ### Prerequisites
   - Qt6 (Core, Widgets, Multimedia)
   - CMake 3.16+
   - C++17 compiler

   ### Building

   ```bash
   cd BhashaQuestV3
   mkdir build && cd build
   cmake ..
   make
   ./BhashaQuest  # or BhashaQuest.exe on Windows
   ```

   ### Running Tests

   ```bash
   cd build
   ctest
   ```

   ## Architecture

   See [ARCHITECTURE.md](ARCHITECTURE.md) for detailed architecture documentation.

   The application follows a layered architecture:
   - **UI Layer:** Qt Widgets (LessonView, ProfileView)
   - **Controller Layer:** AppController (orchestration)
   - **Domain Layer:** Exercise types, Graders, SRS scheduler
   - **Data Layer:** ContentRepository, JSON content

   ## Design Patterns

   - **Factory Pattern:** ExerciseFactory creates exercise instances
   - **Strategy Pattern:** StrategyGrader for pluggable grading algorithms
   - **Singleton Pattern:** AudioManager for centralized audio management

   ## Project Structure

   ```
   src/
   ├── core/
   │   ├── domain/        # 27 files (exercises, graders, business logic)
   │   ├── controllers/   # AppController (orchestration)
   │   └── data/          # ContentRepository, content.json
   └── ui/                # LessonView, ProfileView, MainWindow

   assets/audio/         # Organized audio files
   tests/               # Unit and integration tests
   report/              # Documentation and diagrams
   ```

   ## Documentation

   - [ARCHITECTURE.md](ARCHITECTURE.md) - Architecture and design
   - [DEVELOPER_GUIDE.md](DEVELOPER_GUIDE.md) - Extension guidelines
   - [TEST_REPORT.md](TEST_REPORT.md) - Test results and coverage

   ## Author

   Deliverable submission for CS 3307 Software Engineering

   ## License

   Academic project for Western University
   ```

2. Create SUBMISSION_CHECKLIST.md:
   ```markdown
   # Submission Checklist

   ## Code Quality
   - [ ] All source files follow naming conventions
   - [ ] Code formatted consistently (4-space indentation)
   - [ ] No compiler warnings (`-Wall -Wextra`)
   - [ ] No deprecated Qt4 references
   - [ ] Memory properly managed (no leaks)

   ## Testing
   - [ ] All unit tests pass
   - [ ] All integration tests pass
   - [ ] Manual test cases completed
   - [ ] Code coverage >70% on domain layer
   - [ ] Error scenarios tested

   ## Build & Deployment
   - [ ] CMakeLists.txt builds cleanly
   - [ ] BhashaQuest.pro builds in Qt Creator
   - [ ] Executable runs without errors
   - [ ] All assets deploy correctly
   - [ ] Works on Windows, macOS, Linux

   ## Documentation
   - [ ] README.md complete and accurate
   - [ ] ARCHITECTURE.md explains design
   - [ ] DEVELOPER_GUIDE.md for extensions
   - [ ] TEST_REPORT.md includes results
   - [ ] All design patterns documented
   - [ ] Code comments explain complex logic

   ## User Experience
   - [ ] UI responsive at different sizes
   - [ ] Consistent styling and colors
   - [ ] Clear error messages
   - [ ] User feedback for all actions
   - [ ] No broken UI layouts

   ## Submission Package
   - [ ] Source code organized and clean
   - [ ] Build instructions clear
   - [ ] Architecture diagrams included
   - [ ] All deliverables present
   - [ ] Ready for final submission
   ```

3. Create FINAL_CHECKLIST.md:
   ```markdown
   # Final Submission Checklist

   ## Must Have (100% Complete)
   - [ ] Code compiles without errors
   - [ ] All 3 exercise types work
   - [ ] Factory Pattern implemented
   - [ ] Strategy Pattern implemented
   - [ ] Singleton Pattern implemented
   - [ ] Complete lesson flow works
   - [ ] User profile tracking works
   - [ ] All documentation complete

   ## Should Have (>90% Complete)
   - [ ] Unit tests > 70% coverage
   - [ ] UI polished and responsive
   - [ ] Error handling robust
   - [ ] Code well-commented
   - [ ] Build instructions clear

   ## Nice to Have (<90%)
   - [ ] Integration tests comprehensive
   - [ ] Performance optimized
   - [ ] Audio assets included
   - [ ] Video walkthrough created
   ```

**Commit Message:**
```
refactor: Task 7.1 - Update README and create submission package

- Rewrite README.md with comprehensive project overview
- Add quick start guide with build instructions
- Include architecture summary and links to detailed docs
- Create SUBMISSION_CHECKLIST.md for quality assurance
- Create FINAL_CHECKLIST.md for completion verification
- Organize all deliverables for final submission
- Verify all documentation links work correctly
```

**Verification:**
- [ ] README.md is clear and complete
- [ ] All code sections are accessible
- [ ] Build instructions are tested and work
- [ ] Links to other documentation work
- [ ] Submission checklists are comprehensive

---

### Subtask 7.2: Create Architecture & Design Documentation

**Objective:** Generate visual diagrams and comprehensive design docs

**Steps:**
1. Create/update UML class diagram:
   - Show all classes (27 domain + controller + data + UI)
   - Show inheritance relationships
   - Show composition/aggregation relationships
   - Highlight patterns: Factory, Strategy, Singleton
   - Tool: Draw.io, Lucidchart, or similar
   - Save as: `report/class_diagram.pdf`

   Example classes to include:
   ```
   Exercise (abstract)
   ├── MCQExercise
   ├── TranslateExercise
   └── TileOrderExercise

   StrategyGrader (abstract)
   ├── MCQGrader
   ├── TranslateGrader
   └── TileOrderGrader

   ExerciseFactory
   ContentRepository
   AppController
   Profile
   SkillProgress
   SRSScheduler
   AudioManager (Singleton)
   ```

2. Create sequence diagram for "Complete Exercise" flow:
   - Actor: User
   - Objects: LessonView, AppController, Grader, Profile, SRSScheduler
   - Sequence:
     1. User submits answer
     2. AppController gradeAnswer()
     3. Create appropriate Grader (Strategy Pattern)
     4. Grader->grade(answer, exercise)
     5. AppController->updateProfile(result)
     6. Profile->recordResult()
     7. SRSScheduler->recordCompletion()
     8. AppController emit answerGraded(result)
     9. LessonView display feedback
   - Save as: `report/sequence_diagram.pdf`

3. Create architecture diagram:
   - Show 4 layers: UI, Controller, Domain, Data
   - Show components in each layer
   - Show dependencies between layers
   - Show design patterns locations
   - Save as: `report/architecture_diagram.pdf`

4. Create design rationale document:
   ```markdown
   # Design Rationale

   ## Architectural Decision: Layered Architecture

   **Decision:** Use layered architecture with UI → Controller → Domain → Data

   **Rationale:**
   - Separation of concerns: Each layer has clear responsibility
   - Testability: Domain logic can be tested without UI
   - Maintainability: Changes to UI don't affect domain logic
   - Reusability: Domain logic can be used by different UIs

   ## Design Pattern: Factory Pattern

   **Location:** ExerciseFactory
   **Problem:** Creating Exercise instances based on type from JSON
   **Solution:** Factory encapsulates creation logic
   **Benefits:**
   - Easy to add new exercise types
   - Consistent object creation
   - Decouples client from concrete classes

   ## Design Pattern: Strategy Pattern

   **Location:** StrategyGrader and concrete graders
   **Problem:** Different grading algorithms for different exercise types
   **Solution:** Strategy interface with pluggable implementations
   **Benefits:**
   - Easy to add new grading strategies
   - Runtime selection of algorithm
   - Follows Open/Closed Principle

   ## Design Pattern: Singleton Pattern

   **Location:** AudioManager
   **Problem:** Only one audio playback manager needed
   **Solution:** Private constructor + getInstance()
   **Benefits:**
   - Ensures single instance
   - Global access point
   - Resource management (one QMediaPlayer)
   ```

5. Create comparison documents (optional):
   - Why Qt signals/slots over observer pattern?
   - Why JSON over SQLite for D2?
   - Why C++ over other languages?

6. Review all diagrams for accuracy:
   - Verify all classes shown
   - Verify relationships correct
   - Verify patterns clearly indicated

**Commit Message:**
```
refactor: Task 7.2 - Create comprehensive architecture documentation

- Generate UML class diagram showing all classes and relationships
- Highlight design patterns in diagram (Factory, Strategy, Singleton)
- Create sequence diagram for "Complete Exercise" flow
- Create layered architecture diagram showing all components
- Write design rationale explaining each decision
- Document why each pattern was chosen
- Save all diagrams as PDF for submission
- Verify all diagrams are accurate and complete
```

**Verification:**
- [ ] Class diagram shows all classes
- [ ] Sequence diagram shows complete flow
- [ ] Architecture diagram shows layers clearly
- [ ] Design rationale explains all decisions
- [ ] All diagrams saved as PDF
- [ ] Diagrams match actual code implementation

---

### Subtask 7.3: Final Code Review & Optimization

**Objective:** Review entire codebase for quality and prepare for final submission

**Steps:**
1. Code quality review:
   - Check all files for consistent formatting
   - Verify all comments are clear and accurate
   - Remove any commented-out code
   - Remove any debug statements (except logging)
   - Check for any hardcoded values that should be configurable

2. Memory safety check:
   - Look for potential memory leaks (new without delete)
   - Check for dangling pointers
   - Verify all exception-safe code (if using exceptions)
   - Use tools if available: valgrind on Linux, Clang static analyzer

3. Performance check:
   - Identify hot paths (frequently called functions)
   - Profile if possible using Qt Creator
   - Look for unnecessary object copies
   - Check for inefficient string operations
   - Consider optimizations (but only if needed)

4. Compiler warnings:
   - Build with `-Wall -Wextra -pedantic`
   - Fix any warnings
   - Ensure clean build output

5. Code review checklist:
   ```markdown
   # Code Review Checklist

   ## Formatting & Style
   - [ ] Consistent indentation (4 spaces)
   - [ ] Consistent brace placement
   - [ ] Consistent naming convention
   - [ ] No trailing whitespace
   - [ ] Proper line lengths (<100 chars)

   ## Comments & Documentation
   - [ ] All public methods documented
   - [ ] Complex algorithms explained
   - [ ] Design patterns clearly marked
   - [ ] No commented-out code
   - [ ] No debug output left

   ## Code Quality
   - [ ] DRY principle (no duplication)
   - [ ] SOLID principles followed
   - [ ] No hardcoded values (magic numbers)
   - [ ] Error handling complete
   - [ ] Edge cases considered

   ## Testing
   - [ ] All unit tests pass
   - [ ] All integration tests pass
   - [ ] Manual test cases pass
   - [ ] Code coverage adequate
   - [ ] Error scenarios tested

   ## Performance
   - [ ] No unnecessary object copies
   - [ ] Efficient algorithms used
   - [ ] No memory leaks
   - [ ] No dangling pointers
   - [ ] Startup time reasonable

   ## Build & Deployment
   - [ ] Compiles without warnings
   - [ ] Builds with CMake
   - [ ] Builds with Qt Creator
   - [ ] Executable runs correctly
   - [ ] Assets deploy properly
   ```

6. Final verification:
   - Run complete build from scratch
   - Run all tests
   - Run manual tests
   - Check git status (clean working directory)
   - Verify all files committed

7. Create FINAL_REVIEW_REPORT.md:
   ```markdown
   # Final Code Review Report

   ## Summary
   - Total files reviewed: X
   - Issues found: Y
   - Issues resolved: Y
   - Code quality: Excellent/Good/Acceptable

   ## Code Metrics
   - Lines of code (domain layer): XXXX
   - Lines of code (total): XXXX
   - Number of classes: 27+3
   - Number of patterns: 3 (Factory, Strategy, Singleton)

   ## Issues Found & Resolution
   1. [Issue]: [Resolution]
   2. [Issue]: [Resolution]

   ## Testing Results
   - Unit tests: XX passing, 0 failing
   - Integration tests: XX passing, 0 failing
   - Manual tests: XX passing, 0 failing
   - Code coverage: XX%

   ## Performance
   - Startup time: X seconds
   - Lesson load time: X seconds
   - Memory usage: X MB
   - CPU usage: Minimal

   ## Compliance
   - [ ] All requirements met
   - [ ] All tests passing
   - [ ] All documentation complete
   - [ ] Ready for submission
   ```

**Commit Message:**
```
refactor: Task 7.3 - Final code review and optimization

- Comprehensive code review of entire codebase
- Fix formatting and style inconsistencies
- Remove commented-out code and debug output
- Verify memory safety (no leaks or dangling pointers)
- Performance optimization where needed
- Build with warnings enabled, fix all warnings
- Create FINAL_REVIEW_REPORT.md with metrics
- Final verification: all tests pass, code compiles cleanly
- Prepare ready-for-submission codebase
```

**Verification:**
- [ ] Code compiles with `-Wall -Wextra` (0 warnings)
- [ ] All tests pass (unit, integration, manual)
- [ ] Code reviewed for quality
- [ ] Memory safety verified
- [ ] Final report complete
- [ ] Ready for submission

---

## Execution Summary

### Quick Reference - What to Do

```
For Each Subtask:
1. Read the detailed instructions above
2. Execute all steps listed
3. Run all verification checks
4. Commit with provided message
5. Update progress tracking
6. Move to next subtask

When Task Complete:
1. Verify all 3 subtasks done
2. Test full application
3. Ensure no regressions
4. Mark task complete
```

### Success Definition

**Entire Refactoring Complete When:**
- ✅ All 21 subtasks finished
- ✅ Code compiles without warnings
- ✅ All tests pass (>70% coverage)
- ✅ Application runs without errors
- ✅ UI is polished and responsive
- ✅ All documentation complete
- ✅ Ready for final submission

---

**Document Version:** 1.0
**Last Updated:** 2025-11-10
**Status:** Ready for Implementation
