# Bhasha Quest - Deliverable 2 Video Walkthrough Script
## Duration: 4 minutes | CS3307A Final Project

---

## SEGMENT 1: INTRODUCTION (00:00-00:30)

**[Visual: Title slide on screen]**

**Narration:**
"Hello, I'm Hardik Shrestha, and this is Bhasha Quest—a desktop language learning application for Indic scripts, specifically Nepali and Kannada. 

In this video, I'll demonstrate Deliverable 2 of the Bhasha Quest project, which includes a 50%-complete functional prototype, three design patterns implemented in production code, and a clean layered architecture."

**[Visual: Hold title for 3 seconds, then transition to app window]**

---

## SEGMENT 2: FEATURE DEMO — Complete Lesson Flow (00:30-01:30)

### [Subsection A: Application Launch & Home Screen] (00:30-00:45)

**[Action: Click app icon/run executable. Show home screen loading.]**

**Narration:**
"Let me start by launching the application. The home screen displays the main menu with options to select a language and begin learning."

**[Visual: Pan across home screen showing:]**
- Language selection buttons (Nepali, Kannada)
- User profile section
- Streak counter
- Progress indicators

**[Action: Click 'Nepali' language button]**

**Narration:**
"I'm selecting Nepali as our target language. Notice the clean, modern UI with custom styling—all built using Qt Widgets with CSS-like stylesheets."

---

### [Subsection B: Lesson Selection] (00:45-00:55)

**[Visual: Lesson selection screen appears showing available lessons]**

**Narration:**
"The app displays available lessons organized by skill. Each lesson contains a mix of exercise types designed to reinforce vocabulary and grammar progressively."

**[Action: Click 'Common Greetings' lesson]**

**Narration:**
"Let's start with 'Common Greetings'—a beginner lesson."

---

### [Subsection C: Multiple Choice Exercise] (00:55-01:05)

**[Visual: First MCQ exercise appears]**

**Narration:**
"The first exercise is a multiple-choice question. The interface displays a prompt in both English and Nepali script, with four answer options. Users select the correct translation."

**[Action: Hover over correct answer, then click it]**

**Narration:**
"After selecting an answer, the application provides immediate feedback with a success sound and displays the XP earned."

**[Visual: Show success animation, XP toast notification]**

---

### [Subsection D: Character Selection Widget] (01:05-01:15)

**[Visual: Transition to Translate exercise with CharacterSelectionWidget]**

**Narration:**
"Here's an innovative feature we added: the Character Selection Widget. Rather than requiring users to have Devanagari keyboard layouts installed, we present clickable character buttons."

**[Action: Click characters in sequence to form the correct answer]**

**Narration:**
"Users tap characters to construct the answer. This approach eliminates system input method dependencies and provides a Duolingo-like tactile experience. Our CharacterSelectionGrader evaluates the character sequence."

**[Visual: Show character-by-character grading, correct sequence highlighted]**

---

### [Subsection E: Tile Order Exercise] (01:15-01:25)

**[Visual: Transition to TileOrder exercise]**

**Narration:**
"Next, we have a tile-ordering exercise—word tiles that users arrange to form a correct sentence. This reinforces grammar and word order patterns."

**[Action: Drag tiles into correct order, then submit]**

**Narration:**
"The TileOrderGrader validates the sequence, and correct answers advance the user to the next exercise."

**[Visual: Show transition to next exercise]**

---

### [Subsection F: Lesson Completion] (01:25-01:30)

**[Visual: Lesson completion screen with summary]**

**Narration:**
"After completing all exercises, the lesson summary displays total XP earned, accuracy percentage, and streak information. This data is persisted to the user's profile."

**[Visual: Show profile update reflecting new progress]**

---

## SEGMENT 3: DESIGN PATTERNS IN CODE (01:30-02:30)

### [Subsection A: Factory Pattern — ExerciseFactory] (01:30-01:50)

**[Visual: Switch to Qt Creator, open ExerciseFactory.h/cpp]**

**Narration:**
"Let's examine how design patterns solve real architectural problems. First, the Factory Pattern, implemented in ExerciseFactory."

**[Visual: Highlight relevant code sections:]**
```cpp
static Exercise* createExercise(const QString& type, 
                                const QJsonObject& spec);
```

**Narration:**
"The Factory encapsulates object creation logic. When ContentRepository loads exercise data from JSON, it passes the exercise type and specification to createExercise(). The factory instantiates the correct Exercise subclass—MCQExercise, TranslateExercise, or TileOrderExercise."

**[Action: Scroll to show private helper methods]**

**Narration:**
"Notice the private helper methods for each type. This design follows the Open/Closed Principle: adding a new exercise type requires only modifying the Factory, not ContentRepository or AppController. The pattern isolates creation complexity in one place."

**Benefits highlighted:**
- Encapsulation
- Extensibility (easy to add ListenExercise in D3)
- Maintainability

---

### [Subsection B: Strategy Pattern — StrategyGrader] (01:50-02:10)

**[Visual: Open StrategyGrader.h and concrete implementations]**

**Narration:**
"The Strategy Pattern allows runtime selection of algorithms. In Bhasha Quest, different exercise types require different grading approaches."

**[Visual: Highlight abstract base class:]**
```cpp
class StrategyGrader {
  virtual Result grade(const QString& answer, 
                       const Exercise* exercise) = 0;
};
```

**Narration:**
"MCQExercise uses simple index matching. TranslateExercise uses fuzzy string matching. TileOrderExercise validates sequence order. Rather than embedding this logic in Exercise classes, each grading algorithm is a separate Strategy implementation."

**[Action: Show MCQGrader, TranslateGrader, TileOrderGrader implementations]**

**Narration:**
"Notice CharacterSelectionGrader—a specialized strategy for character-by-character matching. This demonstrates extensibility: the AppController dynamically selects the appropriate grader at runtime based on exercise type."

**Benefits highlighted:**
- Flexibility (swap graders without modifying Exercise classes)
- Testability (each algorithm tested independently)
- Runtime Selection (dynamic dispatch based on type)

---

### [Subsection C: Singleton Pattern — AudioManager] (02:10-02:30)

**[Visual: Open AudioManager.h]**

**Narration:**
"Finally, the Singleton Pattern ensures a single, reusable audio management instance throughout the application lifecycle."

**[Visual: Highlight key code:]**
```cpp
static AudioManager& getInstance();
AudioManager(const AudioManager&) = delete;  // No copy
```

**Narration:**
"AudioManager manages expensive Qt resources: QMediaPlayer and QAudioOutput. A Singleton prevents resource conflicts—imagine multiple audio players competing for the device. We delete copy and move constructors to enforce the single instance constraint."

**[Action: Show usage in AppController]**

**Narration:**
"When a user submits an answer, AppController calls AudioManager::getInstance().playSuccess() or playError(). Any component can access audio functionality globally through getInstance()."

**Benefits highlighted:**
- Resource Management
- Global Access Point
- No Initialization Overhead (lazy instantiation)

---

## SEGMENT 4: ARCHITECTURE EXPLANATION (02:30-03:30)

### [Subsection A: Layered MVC Architecture Diagram] (02:30-02:50)

**[Visual: Display ASCII architecture diagram or architecture document]**

**Narration:**
"Bhasha Quest employs a strict layered MVC architecture with four distinct layers: Presentation, Controller, Domain, and Data."

**[Visual: Point to each layer while describing:]**

**Layer breakdown:**
- **Presentation (UI Layer):** Qt Widgets—HomeView, LessonView, ProfileView, CharacterSelectionWidget
- **Controller Layer:** AppController orchestrates workflows
- **Domain Layer:** Exercise hierarchy, StrategyGrader family, ExerciseFactory, SRSScheduler, AudioManager
- **Data Layer:** ContentRepository (JSON loading), Profile persistence

---

### [Subsection B: Unidirectional Dependency Flow] (02:50-03:10)

**[Visual: Show dependency flow diagram]**

**Narration:**
"The key principle: unidirectional dependency flow—UI depends on Controller, Controller on Domain, Domain on Data. The domain layer has zero knowledge of UI implementation."

**[Action: Navigate through source tree in Qt Creator]**

**Narration:**
"Notice the folder structure enforces this separation. The UI layer in src/ui/ never directly accesses domain objects. All communication flows through AppController using Qt's signal/slot mechanism."

**[Visual: Show signal/slot example in LessonView]**

**Narration:**
"When LessonView emits answerSubmitted(), AppController receives it through a slot, processes the answer using StrategyGrader, and emits resultReady() back to the UI. This loose coupling allows UI changes without touching business logic."

**Key benefit:**
"This architecture is testable. We can unit-test domain classes like SRSScheduler or ExerciseFactory without Qt dependencies."

---

### [Subsection C: Repository Pattern & Data Abstraction] (03:10-03:30)

**[Visual: Open ContentRepository and Profile persistence code]**

**Narration:**
"The Repository Pattern abstracts data storage. ContentRepository loads structured content from JSON files, shielding the domain layer from storage details."

**[Visual: Show JSON content file structure]**

**Narration:**
"Content is organized as JSON packs—language definitions, lessons, exercises. The repository interface remains stable even if we migrate to SQLite in Deliverable 3."

**[Action: Show Profile serialization code]**

**Narration:**
"User profiles are persisted as JSON—XP, streak, skill progress. The same Repository abstraction enables switching backends via configuration, demonstrating true separation of concerns."

**Architecture summary:**
"This clean architecture makes extending the system straightforward. In Deliverable 3, we'll add ListenExercise and SpeakExercise by creating new Exercise subclasses, corresponding graders, and updating the Factory—no architectural changes needed."

---

## SEGMENT 5: CONCLUSION (03:30-04:00)

**[Visual: Return to home screen with lesson completion]**

**Narration:**
"Bhasha Quest demonstrates how design patterns and clean architecture solve real-world problems. The Factory Pattern eliminates complex conditional logic. The Strategy Pattern enables flexible grading algorithms. The Singleton Pattern manages expensive resources efficiently."

**[Visual: Show file structure and design rationale document briefly]**

**Narration:**
"All implementation details—UML diagrams, design rationale, testing strategy—are documented in the GitHub repository. The codebase compiles and runs in Qt Creator with full functionality for MCQ, Translate, and TileOrder exercises."

**[Visual: Show project structure]**

**Narration:**
"For Deliverable 3, we're adding ListenExercise and SpeakExercise, implementing the Observer pattern for real-time UI updates, and adding comprehensive GoogleTest unit tests. The foundation is solid—adding new features is an extension, not a refactoring."

**[Visual: Final title slide]**

**Closing:**
"Thank you for watching. Bhasha Quest demonstrates how thoughtful object-oriented design creates maintainable, extensible software. Questions are welcome."

**[Visual: Fade to black, hold 2 seconds]**

---

## TECHNICAL NOTES FOR RECORDING

- **Total Duration:** 4 minutes
- **Pacing:** Deliberate, not rushed—allow viewers to see code and understand
- **Screen Capture:** 1920x1080 at 30fps recommended
- **Voice:** Clear, steady tone; pause briefly after key points
- **Code Visibility:** Font size 16-18pt minimum for readability
- **Transitions:** Smooth; use screen fade between segments if possible

---

## KEY METRICS TO HIGHLIGHT

- **Architecture:** 4 layers with unidirectional dependencies
- **Patterns:** 3 (Factory, Strategy, Singleton)
- **Exercise Types:** 3 (MCQ, Translate, TileOrder)
- **Classes:** 8+ non-trivial classes
- **Code Organization:** Clear separation of UI, Controller, Domain, Data
- **Implementation:** ~50% of full feature set for Deliverable 2

---

## BACKUP TALKING POINTS (If Q&A format)

1. **Why three design patterns?** 
   - Factory solves object creation complexity
   - Strategy handles algorithmic variation
   - Singleton manages shared resources

2. **Why CharacterSelectionWidget?**
   - Eliminates system input method dependency
   - Better UX than text typing for Indic scripts
   - Guarantees Indic script rendering

3. **Why this architecture?**
   - Testability: Domain logic isolated from Qt
   - Extensibility: New exercise types plug in easily
   - Maintainability: Clear layer responsibilities

4. **What's in Deliverable 3?**
   - ListenExercise (audio playback + comprehension)
   - SpeakExercise (recording + stub ASR)
   - Observer pattern for real-time updates
   - GoogleTest/GMock unit tests
   - SQLite persistence option

---

**END OF SCRIPT**
