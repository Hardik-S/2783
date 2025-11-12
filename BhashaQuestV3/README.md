# Bhasha Quest

A language learning application built with C++ and Qt6 for CS 3307 Software Engineering at Western University.

## Project Overview

Bhasha Quest is a Duolingo-inspired language learning application featuring multiple exercise types, spaced repetition scheduling, and gamification elements. This is the Deliverable 2 (50% implementation) submission.

## Features Implemented (D2)

- **3 Exercise Types:**
  - Multiple Choice Questions (MCQ)
  - Translation Exercises (English → Target Language)
  - Tile Ordering Exercises (Word Reordering)

- **Design Patterns:**
  - **Factory Pattern**: ExerciseFactory for creating exercise instances
  - **Strategy Pattern**: StrategyGrader hierarchy for pluggable grading algorithms
  - **Singleton Pattern**: AudioManager for centralized audio playback

- **Core Functionality:**
  - Lesson flow: Load → Display → Grade → Update Profile → Next
  - Basic SRS (Spaced Repetition System) scheduling
  - User profile with XP and streak tracking
  - JSON-based content management

## Build Instructions

### Prerequisites

- Qt6 (Core, Widgets, Multimedia modules)
- qmake (bundled with Qt)
- C++17 compatible compiler (MinGW, Clang, or MSVC)

### Building with qmake (recommended)

```bash
qmake BhashaQuestV3.pro
make          # or mingw32-make on Windows if you are not in Qt Creator
./BhashaQuest
```

### Building with Qt Creator

1. Open `BhashaQuestV3.pro` in Qt Creator
2. Configure a Qt6 kit that matches your toolchain
3. Build & run (Ctrl+R / ⌘R)

> **Note:** Qt Creator can still load `CMakeLists.txt`, but the qmake project is the maintained workflow—other build files may be stale.

## Architecture

The application follows a layered architecture:

```
UI Layer (Qt Widgets)
    ↓ signals/slots
Controller Layer (AppController)
    ↓ orchestrates
Domain Layer (Exercise, Grader, SRSScheduler, Profile)
    ↓ uses
Data Layer (ContentRepository, JSON)
```

### Design Patterns Usage

**Factory Pattern** (`src/core/domain/ExerciseFactory.h`)
- Creates exercise instances based on type specification from JSON
- Enables extensibility for new exercise types without modifying client code

**Strategy Pattern** (`src/core/domain/StrategyGrader.h`)
- Pluggable grading algorithms (MCQGrader, TranslateGrader, TileOrderGrader)
- Each exercise type uses appropriate grading strategy

**Singleton Pattern** (`src/core/domain/AudioManager.h`)
- Single instance of audio playback manager
- Centralized resource management for QMediaPlayer

## Project Structure

```
code/
├── CMakeLists.txt              # Build configuration
├── src/
│   ├── core/
│   │   ├── domain/             # Business logic & patterns
│   │   │   ├── Exercise.h/cpp
│   │   │   ├── MCQExercise.h/cpp
│   │   │   ├── TranslateExercise.h/cpp
│   │   │   ├── TileOrderExercise.h/cpp
│   │   │   ├── ExerciseFactory.h/cpp
│   │   │   ├── StrategyGrader.h/cpp
│   │   │   ├── MCQGrader.h/cpp
│   │   │   ├── TranslateGrader.h/cpp
│   │   │   ├── TileOrderGrader.h/cpp
│   │   │   ├── SRSScheduler.h/cpp
│   │   │   ├── Profile.h/cpp
│   │   │   ├── SkillProgress.h/cpp
│   │   │   ├── AudioManager.h/cpp
│   │   │   └── Result.h
│   │   ├── controllers/
│   │   │   └── AppController.h/cpp
│   │   └── data/
│   │       ├── ContentRepository.h/cpp
│   │       └── content.json
│   └── ui/
│       ├── LessonView.h/cpp
│       ├── ProfileView.h/cpp
│       └── main.cpp
└── assets/
    └── audio/                  # Audio feedback files

report/
├── design_rationale.pdf        # Design decisions and pattern justifications
├── class_diagram.pdf           # Updated UML class diagram
└── sequence_diagram.pdf        # "Complete Exercise" flow
```

## Video Walkthrough

[Video link will be added here]

## Future Work (D3)

- Additional exercise types: Listen and Speak exercises
- Observer pattern for real-time UI updates
- SQLite persistence for user data
- Advanced SM-2 spaced repetition algorithm
- Comprehensive unit testing

## Team

Deliverable 2 submission for CS 3307 Software Engineering

## License

Academic project for Western University
