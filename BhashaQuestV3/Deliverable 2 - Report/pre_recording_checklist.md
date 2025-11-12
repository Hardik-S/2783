# Pre-Recording Setup Checklist - Bhasha Quest D2 Video

## ✅ SYSTEM & ENVIRONMENT PREPARATION

### Display & Resolution
- [ ] Set desktop resolution to **1920x1080** (or 1280x720 minimum)
- [ ] Disable screen scaling/zoom for clarity
- [ ] Close all unnecessary windows and applications
- [ ] Clear desktop of clutter (icons, temporary files)
- [ ] Disable screen saver and auto-lock
- [ ] Disable notifications (Windows: Focus Assist ON, macOS: Do Not Disturb ON)

### Audio Setup
- [ ] Test microphone volume levels (aim for -12dB to -6dB on input)
- [ ] Use external USB microphone if available (better quality than built-in)
- [ ] Test audio recording in OBS or screen capture tool
- [ ] Close browser tabs/apps that might produce background audio
- [ ] Silence phone and other devices nearby
- [ ] Record 10-second test clip to verify audio quality
- [ ] Adjust microphone gain if audio is too quiet or distorted

### Recording Software
- [ ] Download and install **OBS Studio** (https://obsproject.com/) if not present
- [ ] Set OBS output format: MP4 (H.264 codec)
- [ ] Configure bitrate: 5-8 Mbps for 1920x1080, 30fps
- [ ] Set frame rate: 30fps
- [ ] Test recording with 30-second clip
- [ ] Verify output file size is reasonable (~2.5MB for 30 sec at these settings)

---

## 📋 APPLICATION PREPARATION

### Qt Creator & Project Setup
- [ ] Open BhashaQuestV3 project in Qt Creator
- [ ] Build project to ensure no compilation errors
  ```bash
  qmake BhashaQuestV3.pro
  make
  ```
- [ ] Launch executable to verify app runs without crashes
- [ ] Test all three exercise types (MCQ, Translate, TileOrder)
- [ ] Verify CharacterSelectionWidget displays correctly
- [ ] Check that audio feedback plays (success/error sounds)

### Code Files for Demo
- [ ] Open **ExerciseFactory.h/cpp** in Qt Creator (keep tab open for Segment 3A)
- [ ] Open **StrategyGrader.h** (keep tab open for Segment 3B)
- [ ] Open **MCQGrader.cpp** (show implementation example)
- [ ] Open **CharacterSelectionGrader.cpp** (show innovation)
- [ ] Open **AudioManager.h/cpp** (keep tab open for Segment 3C)
- [ ] Open project file structure view to show architecture layers
- [ ] Prepare folder tree showing: src/ui/, src/core/controllers/, src/core/domain/, src/core/data/

### Documentation Files
- [ ] Have **design_rationale.pdf** open in PDF viewer (reference during explanation)
- [ ] Have **architecture diagram** visible (from D1 or create simple ASCII version)
- [ ] Bookmark GitHub repository link for reference

---

## 🎬 RECORDING WORKSPACE SETUP

### Monitor Layout (if multi-monitor available)
- **Primary Monitor:** Screen recording (app + code windows)
- **Secondary Monitor:** Script and timing reference (DO NOT record this monitor)

### Script & Reference Materials
- [ ] Print video script on paper OR display on second monitor
- [ ] Highlight key transitions: 
  - 00:30 "Switch to feature demo"
  - 01:30 "Switch to ExerciseFactory code"
  - 02:30 "Switch to architecture diagram"
  - 03:30 "Conclusion segment"
- [ ] Set timer or clock visible to track segment timing
- [ ] Mark script with pronunciation guides for technical terms (Devanagari, etc.)

### File Organization
- [ ] Create temp folder: `/tmp/recording_session/`
- [ ] Store test recordings there to verify quality before final take
- [ ] Prepare final output location for MP4 file

---

## 🔧 PRE-RECORDING TECHNICAL TESTS

### OBS Configuration (5-10 minutes before recording)
```
Scene Setup:
├─ Source 1: Screen Capture (primary monitor)
├─ Source 2: Audio Input (microphone)
└─ Recording Settings:
    ├─ Format: MP4
    ├─ Encoder: H.264
    ├─ Bitrate: 6 Mbps
    └─ Resolution: 1920x1080, 30fps
```

### Test Recording Procedure
1. [ ] Start OBS recording
2. [ ] Speak test phrase: "Testing audio levels. This is a screen recording test."
3. [ ] Switch between app windows (App → Qt Creator → Browser)
4. [ ] Type a few characters to verify keyboard audio is minimal
5. [ ] Stop recording after 30 seconds
6. [ ] Review test file:
   - [ ] Audio clear and audible (not too quiet/loud)
   - [ ] Screen capture shows crisp text
   - [ ] No stuttering or frame drops
   - [ ] Window transitions smooth

### Troubleshooting Test Issues
- **Audio too quiet?** Increase microphone gain +6dB, re-test
- **Audio distorted?** Reduce microphone gain -3dB, re-test
- **Screen blurry?** Verify 1920x1080 resolution, check OBS scaling
- **Frame drops?** Close background apps, reduce OBS bitrate to 5 Mbps

---

## 👀 FINAL VISUAL CHECKS (2 minutes before recording)

### Application State
- [ ] Home screen displays correctly with all UI elements visible
- [ ] Lesson loads without lag (<2 seconds)
- [ ] Exercise content displays clearly (text, buttons, layouts)
- [ ] Indic script (Devanagari/Kannada) renders properly on screen
- [ ] Buttons are responsive to clicks
- [ ] No visual artifacts, pixelation, or rendering issues

### Code Editor (Qt Creator)
- [ ] Font size is at least 16pt for readability
- [ ] Code highlighting is visible (syntax colors are distinct)
- [ ] No compilation warnings/errors shown
- [ ] Class/method names are clearly visible when scrolling
- [ ] Folder structure is expanded and labeled for architecture demo

### Recording Software (OBS)
- [ ] Recording indicator is visible and clearly shows RECORDING status
- [ ] CPU/GPU usage is reasonable (<50% CPU)
- [ ] Dropped frames counter is at 0
- [ ] Output file is being written to correct location

---

## 🎙️ VOCAL PREPARATION

### Pre-Recording Voice Check (1 minute)
- [ ] Read script aloud at normal speaking pace
- [ ] Identify difficult words/phrases to practice:
  - "Devanagari" (dev-uh-NAH-guh-ree)
  - "Kannada" (KAHN-uh-duh)
  - "StrategyGrader" (STRAT-uh-jee GRAY-der)
- [ ] Practice smooth transitions between segments
- [ ] Practice pausing for emphasis (after design pattern introductions)
- [ ] Drink water to keep voice clear

### Recording Tips
- [ ] Speak slowly and deliberately (easier to follow than rushed)
- [ ] Pause ~1-2 seconds after major points to let ideas sink in
- [ ] Make eye contact with camera (webcam) if recording face cam overlay
- [ ] Avoid "um" and "uh" fillers (silence is better)
- [ ] Breathe naturally between sentences

---

## 🎯 SEGMENT-BY-SEGMENT PREPARATION

### Segment 1: Introduction (00:00-00:30)
- [ ] App is running, home screen visible
- [ ] Title slide or intro graphic ready
- [ ] Narration script is clear and practiced
- [ ] Transition to Feature Demo is smooth

### Segment 2: Feature Demo (00:30-01:30)
- [ ] App is at home screen, ready to select language
- [ ] Nepali lesson is loaded and ready to launch
- [ ] Exercise content is prepared (no loading delays during demo)
- [ ] Audio feedback is tested and working
- [ ] CharacterSelectionWidget displays all characters correctly
- [ ] Transitions between exercises are smooth

### Segment 3: Design Patterns (01:30-02:30)
- [ ] ExerciseFactory.h/cpp is open in Qt Creator
- [ ] All pattern files are tabbed and ready to switch between
- [ ] Code is formatted and readable on screen
- [ ] You can navigate quickly to relevant sections
- [ ] Keep OBS recording during code explanation

### Segment 4: Architecture (02:30-03:30)
- [ ] Architecture diagram is visible (in document or PDF)
- [ ] Folder structure is navigable in file explorer
- [ ] design_rationale.pdf can be displayed briefly
- [ ] Explanation flows logically from UI → Controller → Domain → Data

### Segment 5: Conclusion (03:30-04:00)
- [ ] Return to app home screen for visual closure
- [ ] Script is concise and memorable
- [ ] Final slide or title card is ready

---

## ⏱️ TIMING VERIFICATION

### Segment Timing Reference
```
Segment 1: Introduction        00:00 → 00:30 (30 sec)
Segment 2: Feature Demo        00:30 → 01:30 (60 sec)
  - Launch & Home              00:30 → 00:45
  - Lesson Selection           00:45 → 00:55
  - MCQ Exercise               00:55 → 01:05
  - Character Selection        01:05 → 01:15
  - TileOrder Exercise         01:15 → 01:25
  - Completion                 01:25 → 01:30

Segment 3: Design Patterns     01:30 → 02:30 (60 sec)
  - Factory Pattern            01:30 → 01:50 (20 sec)
  - Strategy Pattern           01:50 → 02:10 (20 sec)
  - Singleton Pattern          02:10 → 02:30 (20 sec)

Segment 4: Architecture        02:30 → 03:30 (60 sec)
  - Layered MVC                02:30 → 02:50 (20 sec)
  - Dependency Flow            02:50 → 03:10 (20 sec)
  - Repository Pattern         03:10 → 03:30 (20 sec)

Segment 5: Conclusion          03:30 → 04:00 (30 sec)

TOTAL: 4 minutes 0 seconds
```

- [ ] Practice transitioning between segments to hit timing targets
- [ ] If running over, identify what can be shortened (details, not substance)
- [ ] If running under, identify expansion points (elaboration, not filler)

---

## 📹 RECORDING DAY FINAL CHECKLIST (5 minutes before start)

- [ ] Microphone is plugged in and recognized by OBS
- [ ] Volume levels are set and tested
- [ ] Screen resolution is confirmed at 1920x1080
- [ ] All unnecessary apps are closed
- [ ] Notifications are disabled
- [ ] Script is visible (printed or on second monitor)
- [ ] Timer is visible for segment tracking
- [ ] OBS is set to record to correct output directory
- [ ] Test MP4 file from earlier test is saved as reference
- [ ] You are comfortable with the narration (practiced at least once)
- [ ] All application windows are positioned for clear recording
- [ ] Code font size is verified to be readable

---

## 🚀 RECORDING PROCEDURE

### Take 1: Full Recording
1. [ ] Start OBS recording
2. [ ] Begin narration and follow script
3. [ ] Keep steady pace and follow timing targets
4. [ ] Transition smoothly between segments
5. [ ] Stop OBS when conclusion segment ends
6. [ ] **Do NOT stop recording for mistakes**—edit in post if needed

### Review Take 1
1. [ ] Playback MP4 file completely
2. [ ] Check audio levels are consistent and clear
3. [ ] Verify screen capture shows all text/code clearly
4. [ ] Note any sections needing re-recording

### Take 2 (if needed)
- [ ] Re-record only segments with major issues
- [ ] OR record entire take again if flow was significantly disrupted

---

## 🎬 POST-RECORDING (After video is recorded)

- [ ] Export final MP4 from OBS (Settings verify: H.264, 1920x1080, 30fps)
- [ ] Verify file size is reasonable (~10-15MB for 4-minute video)
- [ ] Save to: `/mnt/user-data/outputs/BhashaQuest_D2_Walkthrough.mp4`
- [ ] Test playback in VLC or similar to ensure it's readable
- [ ] Update README.md with video link
- [ ] Prepare for upload to YouTube (Unlisted) or Google Drive

---

**✅ Ready to record!**  
Review this checklist 30 minutes before recording to ensure nothing is missed. Good luck!
