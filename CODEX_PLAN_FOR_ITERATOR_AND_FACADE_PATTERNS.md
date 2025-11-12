# CODEX Plan for Iterator and Facade Patterns

## 1. Context & Existing Architecture
- `AppController` currently owns the lesson flow (`startLesson`, `loadNextExercise`, `submitAnswer`, `endLesson`) and keeps a `QList<Exercise*> exerciseQueue` along with a numeric index to step through the queue (see `src/core/controllers/AppController.cpp`).
- `ContentRepository` loads skills from `content.json`, keeps them in a `QMap<QString, Skill>` where each `Skill` holds a `QList<Exercise*>`, and exposes `getExercisesForSkill()` so the controller can copy the raw list (see `src/core/data/ContentRepository.cpp/.h`).
- UI components (`LessonView`, `ProfileView`) talk to `AppController` via signals/slots but sometimes need to coordinate with the scheduler (`SRSScheduler`) and audio (`AudioManager`) directly through the controller.

These constraints mean new patterns must weave into the controller/repository interaction without breaking ownership semantics or reintroducing tight coupling.

## 2. Iterator Pattern Brainstorm
### Objective
Give the controller (and eventually the facade) a richer, encapsulated way to traverse exercise sequences (per skill, review queue, or mixed difficulties) without exposing internal containers or relying on `QList` indexing logic.

### Proposed Iterator Design
1. Introduce a new `ExerciseSequence` abstraction in `src/core/domain/` (or a sibling namespace) that wraps one or more `ContentRepository::Skill` exercise lists.
2. Inside `ExerciseSequence`, add a nested `Iterator` with the Qt-style `operator++`, `operator*`, `operator!=`, and/or explicit `next()`/`hasNext()` methods so both Qt and STL idioms feel natural. Back it with indices referencing the owning `Skill` list(s) so we can iterate in skill order or shuffle for spaced repetition review.
3. Add factory helpers in `ContentRepository` that produce sequences (e.g., `createSequenceForSkill(skillId)`, `createSequenceForReview(const QList<QString>& exerciseIds)`) instead of returning raw `QList`s. The existing `Skill` struct stays private and the iterator exposes only `Exercise*` handles.
4. Update `AppController::startLesson` to accept `ExerciseSequence` (or `QSharedPointer<ExerciseSequence>`) and consume it through the iterator rather than copying the `QList`. This removes `currentExerciseIndex` / `exerciseQueue` and replaces them with either the iterator itself or a thin `ExerciseCursor` that keeps a pointer to the iterator plus completion metadata.
5. Keep backwards compatibility by providing thin wrappers around `ExerciseSequence` that can produce `QList<Exercise*>` if needed by legacy callers (e.g., UI helpers that still expect a list). Document that `ExerciseSequence` is now the primary interface for lesson navigation.

### Benefits
- Centralizes traversal logic, so future features (filtered exercises, review loops) reuse the iterator instead of re-implementing indexing across `QList`s.
- Provides a place to implement adaptive ordering (e.g., shuffle recommended exercises) by injecting custom iterator implementations.
- Makes `AppController` easier to unit test because the sequence can be mocked without touching `QList` internals.

## 3. Facade Pattern Brainstorm
### Objective
Hide the orchestration complexity between `LessonView`, `AppController`, `ContentRepository`, `SRSScheduler`, and `AudioManager` behind a single `LessonFacade` so that UI layers only need to interact with a consistent protocol (`start`, `submit`, `next`, `refresh stats`).

### Proposed Facade Design
1. Create `LessonFacade` (or `PracticeSessionFacade`) in a new module (e.g., `src/core/facade`). Its constructor accepts references/pointers to `AppController`, `ContentRepository`, `SRSScheduler`, and `AudioManager` (possibly the same ones already owned by `AppController` or delegated from a higher-level `App` class).
2. Expose high-level methods like `initializeSkillSession(skillId)`, `submitCurrentAnswer(QString)`, `advanceExercise()`, `requestReview(Difficulty)`, and `getCurrentProgress()`. Internally it uses the iterator from Section 2 to feed exercises and delegates grading/profile updates to `AppController` while also triggering scheduler/audio updates as needed.
3. Provide signal/slot passthroughs so `LessonView` can connect to `LessonFacade::exerciseChanged`, `answerGraded`, and `profileUpdated` without talking to multiple subsystems. The facade can also aggregate `profileUpdated` + `progressUpdated` to reduce the number of UI connections.
4. Move any UI-facing ballistics (e.g., XP calculation, streak update notifications) into the facade so the view remains passive. The facade becomes the new owner of session state (with `AppController` as a worker) and offers simpler testing points.
5. Refactor `MainWindow`/`LessonView` to talk to the facade instead of `AppController` directly. For features such as audio feedback or SRS review scheduling, the facade simply forwards to the controller or scheduler, keeping the UI shielded from implementation changes.

### Benefits
- Reduces the number of signal/slot connections each view must manage; the facade becomes the single translator between UI and domain.
- Easier to extend for future UX flows (e.g., mobile-late session) because the facade can offer new methods while reusing the internal domain logic.
- Provides a natural location for instrumentation (logging, analytics) that spans multiple components.

## 4. Execution Strategy
1. Draft the `ExerciseSequence`/`ExerciseIterator` interface first and add unit tests that traverse sample skills from `ContentRepository`. Verify `AppController` can start a session using the iterator without modifying its public API yet (use adapter if needed).
2. Rename `AppController` internals (`exerciseQueue`, `currentExerciseIndex`) to `activeSequence` and `sequenceCursor`, consume the iterator, and retire direct `QList` indexing. Update `startLesson`, `loadNextExercise`, and `hasNextExercise` accordingly.
3. Introduce `LessonFacade`, wiring it up in `MainWindow` so the view interacts only with the facade. Incrementally migrate UI signal connections from `AppController` to the facade to avoid regressions.
4. Update documentation (README/markdown) summarizing the new pattern usage and describe how iterator/facade simplify future extensions like review loops or additional views.

## 5. Validation Notes
- Keep the existing content of `ContentRepository` unchanged until the iterator proves stable; adapter layers can temporarily convert `QList`→`ExerciseSequence`.
- Manual testing: run the Qt UI and confirm lessons still progress, profile updates still arrive, and review scheduling behaves the same.
- Optional: add unit tests for the iterator and facade components so future contributors understand the expected behavior.
