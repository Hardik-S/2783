# CLAUDE.md - BhashaQuest Working Directory

## Current Working Directory

**Active Project Location:** `C:\Users\hshre\OneDrive\Documents\ClaudeWorkDirectory\2783\BhashaQuestV3`

This is the current working directory for all BhashaQuest development work. All code changes, builds, and tests should be performed in this location.

## Directory Structure

```
ClaudeWorkDirectory/2783/
├── BhashaQuestV3/              # 🎯 ACTIVE PROJECT - Work here!
│   ├── src/
│   │   ├── core/
│   │   │   ├── domain/         # Exercise types, graders, business logic
│   │   │   ├── controllers/    # AppController
│   │   │   └── data/           # ContentRepository, content.json
│   │   └── ui/                 # LessonView, ProfileView, MainWindow
│   ├── assets/audio/           # Audio files
│   ├── BhashaQuest.pro         # Qt Creator project
│   ├── CMakeLists.txt          # CMake config
│   ├── CLAUDE.md               # Project-specific guidance
│   └── [other project files]
├── CLAUDE.md                   # THIS FILE (directory-level guidance)
├── UI_Fix_Plan.md              # Current UI enhancement plans
└── [other working files]
```

## Important Notes

- **ALWAYS work in the `BhashaQuestV3/` subdirectory**
- Original source is at: `C:\Users\hshre\OneDrive\Documents\ClaudeWorkDirectory\7807\BhashaQuestv2\BhashaQuest`
- Do NOT make changes to the 7807 directory
- This directory (2783) is the active development workspace

## Quick Commands

```bash
# Navigate to project
cd "C:\Users\hshre\OneDrive\Documents\ClaudeWorkDirectory\2783\BhashaQuestV3"

# Build (if needed)
cd build/Desktop_Qt_6_10_0_MinGW_64_bit-Debug
make
```

## Current Status

- Project copied from 7807/BhashaQuestv2/BhashaQuest
- **THIS IS THE NEW PRIMARY WORKING DIRECTORY FOR ALL BHASHAQUEST DEVELOPMENT**
- All changes, builds, and testing should happen in 2783/BhashaQuestV3
- Do NOT make changes to 7807 directory
- All SOLID, DRY, KISS, and YAGNI principles apply

## Recent Changes

### Next Lesson Button Feature (2025-11-10)
- Added "Next Lesson" button to completion screen
- Orange button (#FF9800) positioned at completion screen
- Allows users to immediately start another lesson with the same skill
- Changes made to:
  - `src/ui/LessonView.h/cpp` - Added button UI
  - `mainwindow.h/cpp` - Added signal/slot wiring

---

**Last Updated:** 2025-11-10
**Active Working Directory:** 2783/BhashaQuestV3
**Source Reference:** 7807/BhashaQuestv2/BhashaQuest (do not modify)
