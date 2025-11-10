# BhashaQuest: Audio Integration & Bug Fixes Plan

## Executive Summary

This document outlines a comprehensive plan to:
1. **Add audio files** for number exercises (Nepali & Kannada) using text-to-speech
2. **Fix progress display bug** - exercise number not updating after submission
3. **Fix font warnings** - replace Arial/Tahoma with system-safe fonts
4. **Add time travel feature** - button to advance date for testing streak functionality

---

## Issue Analysis

### Issue 1: Exercise Number Not Updating
**Current Behavior:** Progress label shows "Exercise 1 of 5" throughout the lesson
**Root Cause:** `progressUpdated` signal emitted but not connected to UI
**Location:** `AppController::loadNextExercise()` emits signal, but MainWindow doesn't connect it

### Issue 2: Font Warnings
**Current Behavior:** Qt warnings about missing fonts (Arial, Tahoma, MS UI Gothic, etc.)
**Root Cause:** Hard-coded font families not available on all systems
**Affected Files:** LessonView.cpp, ProfileView.cpp
**Best Practice:** Use Qt font fallback chain or system defaults

### Issue 3: No Audio Files
**Current Behavior:** AudioManager logs "Playing X" but no actual audio
**Root Cause:** Audio files don't exist in assets/audio/
**Scope:** Start with number exercises (12 audio files total)

### Issue 4: Cannot Test Streak Feature
**Current Behavior:** Streak requires waiting actual days to test
**Need:** Developer tool to simulate date advancement
**Solution:** Add hidden "Time Travel" button in ProfileView

---

## Part 1: Audio File Generation Strategy

### Approach: Text-to-Speech (TTS) with gTTS
**Why TTS?**
- ✅ Free and accessible
- ✅ Supports Nepali and Kannada (via transliteration)
- ✅ Generates consistent quality
- ✅ Easy to reproduce/modify
- ✅ No copyright issues

**Technology Stack:**
- **gTTS (Google Text-to-Speech)**: Python library for TTS
- **pydub**: For audio format conversion (WAV)
- **Transliteration**: Nepali Devanagari → English for gTTS input

### Number Audio Files Needed

**Nepali Numbers (6 files):**
```
nepali-numbers/ex-num-01.mp3: एक (ek) → "ek"
nepali-numbers/ex-num-02.mp3: पाँच (panch) → "panch"
nepali-numbers/ex-num-03.mp3: एक दुई तीन (counting) → "ek dui teen"
nepali-numbers/ex-num-04.mp3: दश (das) → "das"
nepali-numbers/ex-num-05.mp3: बीस (bees) → "bees"
```

**Kannada Numbers (6 files):**
```
kannada-numbers/ex-kan-num-01.mp3: ಒಂದು (ondu) → "ondu"
kannada-numbers/ex-kan-num-02.mp3: ಐದು (aidu) → "aidu"
kannada-numbers/ex-kan-num-03.mp3: ಒಂದು ಎರಡು ಮೂರು → "ondu eradu mooru"
kannada-numbers/ex-kan-num-04.mp3: ಹತ್ತು (hattu) → "hattu"
kannada-numbers/ex-kan-num-05.mp3: ಇಪ್ಪತ್ತು (ippatthu) → "ippatthu"
```

**Total:** 12 audio files (MP3 format, 16kHz, mono)

### Audio Generation Script

**File:** `scripts/generate_audio.py`

```python
#!/usr/bin/env python3
"""
Generate TTS audio files for BhashaQuest number exercises
Uses gTTS for text-to-speech generation
"""

import os
from gtts import gTTS
from pathlib import Path

# Audio mapping: filename -> (text, language_code)
AUDIO_FILES = {
    # Nepali numbers (using Hindi TTS as proxy)
    "ek.mp3": ("ek", "hi"),
    "panch.mp3": ("paanch", "hi"),
    "counting.mp3": ("ek dui teen", "hi"),
    "das.mp3": ("das", "hi"),
    "bees.mp3": ("bees", "hi"),

    # Kannada numbers (using Kannada TTS)
    "ondu.mp3": ("ಒಂದು", "kn"),
    "aidu.mp3": ("ಐದು", "kn"),
    "kan-counting.mp3": ("ಒಂದು ಎರಡು ಮೂರು", "kn"),
    "hattu.mp3": ("ಹತ್ತು", "kn"),
    "ippatthu.mp3": ("ಇಪ್ಪತ್ತು", "kn"),
}

def generate_audio_files(output_dir: str = "../assets/audio"):
    """Generate all audio files using gTTS"""

    output_path = Path(output_dir)
    output_path.mkdir(parents=True, exist_ok=True)

    print(f"Generating {len(AUDIO_FILES)} audio files...")

    for filename, (text, lang) in AUDIO_FILES.items():
        filepath = output_path / filename

        try:
            print(f"  Creating {filename} ('{text}', {lang})...")
            tts = gTTS(text=text, lang=lang, slow=False)
            tts.save(str(filepath))
            print(f"    ✓ Saved to {filepath}")

        except Exception as e:
            print(f"    ✗ Error generating {filename}: {e}")

    print("\nDone! Generated audio files in:", output_path.absolute())

if __name__ == "__main__":
    generate_audio_files()
```

**Dependencies:** `pip install gtts`

**Usage:**
```bash
cd BhashaQuest/scripts
python3 generate_audio.py
```

### Audio File Naming Convention
Match content.json audioFile references:
- `ek.mp3` (not `ex-num-01.mp3`) for simplicity
- Store in `assets/audio/` (flat structure for now)

### Fallback Strategy
If audio file missing:
1. AudioManager logs warning but doesn't crash
2. Exercise continues without audio
3. User experience degraded but functional

---

## Part 2: Bug Fix - Exercise Number Not Updating

### Root Cause
`AppController::progressUpdated(int, int)` signal is emitted but never connected to `LessonView::updateProgress(int, int)`.

### Solution: Wire Signal in MainWindow

**File:** `mainwindow.cpp`

**Location:** `MainWindow::wireSignalsAndSlots()`

**Add Connection:**
```cpp
// Connect progress updates
connect(appController, &AppController::progressUpdated,
        lessonView, &LessonView::updateProgress);
```

**Why This Works:**
- AppController emits `progressUpdated(currentExerciseIndex, exerciseQueue.size())` after loading each exercise
- Signal carries (current, total) to LessonView
- LessonView updates label: "Exercise N of M"

### Verification
After fix, progress should update:
- Exercise 1 of 5 → Submit → Exercise 2 of 5 → Submit → Exercise 3 of 5...

---

## Part 3: Font Fix - Eliminate Warnings

### Issue
Hard-coded fonts like "Arial", "Tahoma" cause warnings on Linux/systems without these fonts:
```
qt.text.font.db: OpenType support missing for "Arial", script 11
```

### Solution: Use Qt Font Families

**Strategy:**
1. Use `QFont()` default constructor (uses system default)
2. Set only size/weight, let Qt choose family
3. For special needs, use fallback chains

**Changes Required:**

#### LessonView.cpp
Replace all:
```cpp
QFont progressFont("Arial", 11);
```

With:
```cpp
QFont progressFont;
progressFont.setPointSize(11);
progressFont.setBold(true);
```

Or use fallback:
```cpp
QFont progressFont;
progressFont.setFamily("Sans Serif");  // Generic family
progressFont.setPointSize(11);
```

#### ProfileView.cpp
Same approach:
```cpp
// Instead of: QFont titleFont("Arial", 24);
QFont titleFont;
titleFont.setPointSize(24);
titleFont.setBold(true);
```

### Font Fallback Chain (Best Practice)
```cpp
QFont font;
font.setFamily("Noto Sans, Segoe UI, DejaVu Sans, Sans Serif");
```

This provides:
- **Noto Sans**: Excellent Unicode support (Devanagari, Kannada)
- **Segoe UI**: Windows default
- **DejaVu Sans**: Linux default
- **Sans Serif**: Generic fallback

### Implementation Files
- `src/ui/LessonView.cpp` (8 font declarations)
- `src/ui/ProfileView.cpp` (6 font declarations)

---

## Part 4: Time Travel Feature for Streak Testing

### Requirements
- Hidden button in ProfileView (developer feature)
- Advances `Profile::lastActivityDate` by 1 day
- Updates UI to reflect new streak
- Simulates "next day" scenario

### Implementation

#### Profile.h - Add Method
```cpp
public:
    /**
     * Advance simulated date by N days (for testing)
     * @param days - Number of days to advance (default: 1)
     */
    void advanceSimulatedDate(int days = 1);

private:
    QDate simulatedCurrentDate;  // For testing (if null, use QDate::currentDate())
```

#### Profile.cpp - Implementation
```cpp
void Profile::advanceSimulatedDate(int days) {
    if (!simulatedCurrentDate.isValid()) {
        simulatedCurrentDate = QDate::currentDate();
    }
    simulatedCurrentDate = simulatedCurrentDate.addDays(days);
    qDebug() << "Time Travel: Advanced to" << simulatedCurrentDate.toString();
}

// Update other methods to use simulated date:
QDate Profile::getCurrentDate() const {
    return simulatedCurrentDate.isValid() ? simulatedCurrentDate : QDate::currentDate();
}

void Profile::updateStreak() {
    QDate today = getCurrentDate();  // Use simulated date
    // ... rest of logic
}
```

#### ProfileView - Add Time Travel Button

**ProfileView.cpp - setupUI():**
```cpp
// Add time travel section at bottom
QWidget* devToolsCard = new QWidget(this);
devToolsCard->setStyleSheet(
    "QWidget { background-color: #FFF3E0; border-radius: 10px; padding: 15px; }"
);
QVBoxLayout* devLayout = new QVBoxLayout(devToolsCard);

QLabel* devLabel = new QLabel("🔧 Developer Tools", devToolsCard);
devLabel->setStyleSheet("QLabel { color: #E65100; font-weight: bold; }");
devLayout->addWidget(devLabel);

QPushButton* timeTravelBtn = new QPushButton("⏭️ Next Day (Test Streak)", devToolsCard);
timeTravelBtn->setStyleSheet(
    "QPushButton { "
    "   background-color: #FF9800; color: white; "
    "   border: none; border-radius: 8px; padding: 10px; "
    "}"
);
connect(timeTravelBtn, &QPushButton::clicked, this, &ProfileView::onTimeTravelClicked);
devLayout->addWidget(timeTravelBtn);

mainLayout->addWidget(devToolsCard);
```

**ProfileView.cpp - Add Slot:**
```cpp
void ProfileView::onTimeTravelClicked() {
    if (!currentProfile) return;

    // Advance date by 1 day
    currentProfile->advanceSimulatedDate(1);

    // Update streak based on new date
    // Streak++ if exercise completed today, else reset to 0
    currentProfile->checkStreakValidity();

    // Refresh UI
    updateProfile(currentProfile);

    // Show confirmation
    QMessageBox::information(this, "Time Travel",
        QString("Advanced to %1\nStreak: %2 days")
            .arg(currentProfile->getCurrentDate().toString("yyyy-MM-dd"))
            .arg(currentProfile->getStreak()));
}
```

**ProfileView.h - Add Slot:**
```cpp
private slots:
    void onTimeTravelClicked();
```

### User Experience
1. Complete a lesson → Streak becomes 1
2. Click "Next Day" → Date advances, streak becomes 2 (if exercise completed)
3. Click "Next Day" again (without completing exercise) → Streak resets to 0
4. Visual feedback via message box

---

## Part 5: Audio Playback Integration

### Current State
- AudioManager exists with Singleton pattern
- `playAudio(QString filename)` implemented
- Exercises have `audioFile` field in JSON

### Integration Points

#### 1. LessonView - Add Audio Button

**LessonView.cpp - updateExercise():**
```cpp
void LessonView::updateExercise(Exercise* exercise) {
    // ... existing code ...

    // Add audio playback button
    if (!exercise->getAudioFile().isEmpty()) {
        QPushButton* audioBtn = new QPushButton("🔊 Listen", inputContainer);
        audioBtn->setStyleSheet(
            "QPushButton { "
            "   background-color: #00BCD4; color: white; "
            "   border-radius: 8px; padding: 10px; "
            "   font-size: 12px; "
            "}"
        );
        connect(audioBtn, &QPushButton::clicked, [=]() {
            QString audioPath = "assets/audio/" + exercise->getAudioFile();
            AudioManager::getInstance().playAudio(audioPath);
        });
        inputLayout->insertWidget(0, audioBtn);  // Add at top
    }
}
```

#### 2. Auto-play on Exercise Load (Optional)
```cpp
void LessonView::updateExercise(Exercise* exercise) {
    // ... existing code ...

    // Auto-play audio when exercise loads
    if (!exercise->getAudioFile().isEmpty()) {
        QString audioPath = "assets/audio/" + exercise->getAudioFile();
        AudioManager::getInstance().playAudio(audioPath);
    }
}
```

#### 3. Feedback Audio (Success/Error)
Already wired in AudioManager, just need files:
- `assets/audio/success.wav` - played on correct answer
- `assets/audio/error.wav` - played on incorrect answer

**Generate with script:**
```python
# Add to generate_audio.py
FEEDBACK_SOUNDS = {
    "success.mp3": ("Correct!", "en"),
    "error.mp3": ("Try again", "en"),
}
```

---

## Implementation Checklist

### Phase 1: Audio Files (30 mins)
- [ ] Create `scripts/` directory
- [ ] Write `generate_audio.py` script
- [ ] Install dependencies: `pip install gtts`
- [ ] Run script to generate 12 number audio files
- [ ] Verify files created in `assets/audio/`
- [ ] Test playback manually

### Phase 2: Bug Fixes (15 mins)
- [ ] Fix progress update signal connection in `mainwindow.cpp`
- [ ] Replace hard-coded fonts in `LessonView.cpp`
- [ ] Replace hard-coded fonts in `ProfileView.cpp`
- [ ] Test exercise number updates correctly

### Phase 3: Time Travel Feature (30 mins)
- [ ] Add `simulatedCurrentDate` to Profile.h
- [ ] Add `advanceSimulatedDate()` method to Profile.cpp
- [ ] Update `getCurrentDate()` helper method
- [ ] Modify `updateStreak()` to use simulated date
- [ ] Add Time Travel button to ProfileView.cpp
- [ ] Add `onTimeTravelClicked()` slot
- [ ] Test streak increment/reset logic

### Phase 4: Audio Integration (20 mins)
- [ ] Add audio button to LessonView exercise display
- [ ] Wire AudioManager playback
- [ ] Test with number exercises
- [ ] (Optional) Add auto-play on exercise load
- [ ] (Optional) Generate success/error sounds

### Phase 5: Testing (30 mins)
- [ ] Build project without errors
- [ ] Test exercise progression (number updates)
- [ ] Test font rendering (no warnings)
- [ ] Test time travel (streak logic)
- [ ] Test audio playback (number exercises)
- [ ] Test on different exercise types (MCQ, Translate, TileOrder)

**Total Estimated Time:** 2 hours

---

## File Structure After Implementation

```
BhashaQuest/
├── assets/
│   └── audio/
│       ├── ek.mp3              # Nepali: 1
│       ├── panch.mp3           # Nepali: 5
│       ├── counting.mp3        # Nepali: 1,2,3
│       ├── das.mp3             # Nepali: 10
│       ├── bees.mp3            # Nepali: 20
│       ├── ondu.mp3            # Kannada: 1
│       ├── aidu.mp3            # Kannada: 5
│       ├── kan-counting.mp3    # Kannada: 1,2,3
│       ├── hattu.mp3           # Kannada: 10
│       ├── ippatthu.mp3        # Kannada: 20
│       ├── success.mp3         # Feedback: correct
│       └── error.mp3           # Feedback: wrong
├── scripts/
│   ├── generate_audio.py       # TTS generation script
│   └── requirements.txt        # Python dependencies
└── src/
    ├── core/
    │   ├── controllers/
    │   │   ├── AppController.cpp  # (no changes)
    │   │   └── AppController.h    # (no changes)
    │   └── domain/
    │       ├── Profile.h          # + advanceSimulatedDate(), getCurrentDate()
    │       ├── Profile.cpp        # + time travel logic
    │       ├── AudioManager.h     # (no changes)
    │       └── AudioManager.cpp   # (no changes)
    └── ui/
        ├── LessonView.h           # (no changes)
        ├── LessonView.cpp         # + audio button, fixed fonts
        ├── ProfileView.h          # + onTimeTravelClicked slot
        ├── ProfileView.cpp        # + time travel button, fixed fonts
        ├── mainwindow.h           # (no changes)
        └── mainwindow.cpp         # + progress signal connection
```

---

## Testing Strategy

### Test 1: Audio Playback
1. Start lesson for "Numbers & Counting" (Nepali or Kannada)
2. Exercise loads → Audio plays automatically (or click 🔊 button)
3. Verify audio is audible and matches exercise
4. Check console for "AudioManager: Playing assets/audio/X.mp3"

### Test 2: Progress Display
1. Start lesson with 5 exercises
2. Verify label shows "Exercise 1 of 5"
3. Submit answer → Verify label updates to "Exercise 2 of 5"
4. Continue until "Exercise 5 of 5"
5. Complete lesson → Verify completion screen

### Test 3: Font Rendering
1. Build and run application
2. Check console for font warnings
3. Verify NO warnings about "OpenType support missing"
4. Verify UI still renders correctly on Windows/Linux/Mac

### Test 4: Time Travel & Streak
1. Complete 1 exercise (any skill)
2. Check profile → Streak = 1
3. Click "Next Day" button
4. Complete 1 exercise → Streak = 2
5. Click "Next Day" button (without completing exercise)
6. Check profile → Streak = 0 (reset)
7. Verify message box shows current simulated date

---

## Alternative Approaches Considered

### Audio Generation

**Approach 1: Manual Recording** ❌
- Pros: Highest quality, native speaker
- Cons: Time-consuming, requires equipment, hard to reproduce

**Approach 2: Online TTS Services (ElevenLabs, Azure)** ❌
- Pros: Professional quality
- Cons: Requires API keys, costs money, internet dependency

**Approach 3: gTTS (Chosen)** ✅
- Pros: Free, supports Nepali/Kannada, reproducible, simple
- Cons: Quality lower than native speaker, requires internet for generation

**Approach 4: Pre-downloaded Audio Files** ❌
- Pros: No generation needed
- Cons: Copyright issues, hard to find quality Nepali/Kannada audio

### Font Handling

**Approach 1: Bundle Custom Fonts** ❌
- Pros: Guaranteed consistency
- Cons: Increases app size, licensing issues

**Approach 2: System Font Fallback (Chosen)** ✅
- Pros: No warnings, works on all systems, respects user preferences
- Cons: Slight visual variation across platforms

**Approach 3: Ignore Warnings** ❌
- Pros: No code changes needed
- Cons: Cluttered console, may fail on some systems

### Time Travel Implementation

**Approach 1: Mock QDate::currentDate() Globally** ❌
- Pros: Affects entire app
- Cons: Too invasive, affects Qt internals

**Approach 2: Profile-level Simulated Date (Chosen)** ✅
- Pros: Isolated, testable, doesn't affect other components
- Cons: Requires updating methods to use helper

**Approach 3: System Date Change** ❌
- Pros: Tests real behavior
- Cons: Requires admin privileges, affects entire system

---

## Success Criteria

✅ **Audio Integration:**
- 12 audio files generated and playable
- Audio button appears on number exercises
- Audio plays when clicked (no crashes)

✅ **Progress Fix:**
- Exercise number updates after each submission
- Shows "Exercise N of M" correctly throughout lesson

✅ **Font Fix:**
- No font warnings in console
- UI renders correctly on multiple platforms

✅ **Time Travel:**
- Button visible in ProfileView
- Clicking advances date and updates streak
- Streak logic correct (increment if active, reset if not)

---

## Future Enhancements (Out of Scope)

- [ ] Generate audio for all 30 exercises (not just numbers)
- [ ] Add volume control slider
- [ ] Add audio playback on correct/incorrect answer
- [ ] Support offline TTS (pyttsx3)
- [ ] Add audio waveform visualization
- [ ] Allow user to record own audio
- [ ] Persist simulated date across app restarts

---

## Conclusion

This plan provides a comprehensive, practical approach to:
1. **Add audio** using free TTS (gTTS)
2. **Fix critical bugs** (progress display, fonts)
3. **Enable testing** (time travel for streaks)

All solutions follow best practices:
- ✅ SOLID principles maintained
- ✅ DRY - reusable audio generation script
- ✅ KISS - simple implementations
- ✅ YAGNI - only necessary features

**Next Step:** Proceed with implementation using the checklist above.
