\*\*AGENTS.md – BhashaQuest Project Automation \& Role Definitions\*\*

\*(Optimized for GPT-5 multi-agent collaboration to ace the CS3307 Deliverable 2 report + demo)\*



---



\## 🎯 \*\*Objective\*\*



Coordinate intelligent, domain-specialized agents to:



1\. Produce a \*\*publish-ready Deliverable 2 report\*\* (UMLs + rationale + architecture).

2\. Prepare an \*\*impeccable demo and recording\*\* aligned with course rubrics.

3\. Maintain consistency across \*\*design diagrams, documentation, and code\*\*.



---



\## 🧩 \*\*Core Agents \& Responsibilities\*\*



| \*\*Agent Name\*\*                     | \*\*Role \& Expertise\*\*                                                                   | \*\*Inputs / Dependencies\*\*                                                                                            | \*\*Primary Outputs\*\*                                                                              |

| ---------------------------------- | -------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------ |

| \*\*📐 UML-Architect Agent\*\*         | Expert in PlantUML \& Qt architecture documentation. Ensures UMLs match implementation. | `class\_diagram\_updated.png`, `sequence\_diagram\_updated.png`, `TASK\_1.1\_COMPLETION\_SUMMARY.md`, `design\_rationale.md` | Validated UML set (class + sequence + architecture diagram snippets) for final report.           |

| \*\*🧠 Design-Rationale Agent\*\*      | Synthesizes design justification, pattern rationale, and Deliverable 2 reflection.     | `design\_rationale.md`, HTML export, A1-2 proposal, project spec.                                                     | Concise, rubric-aligned design rationale summary for report and video narration.                 |

| \*\*🎞️ Video-Director Agent\*\*       | Converts technical content into engaging 4-minute walkthrough flow.                    | `video\_script.md`, `pre\_recording\_checklist.md`, demo timing spec.                                                   | Optimized narration + visual cue script (with timestamps and transitions).                       |

| \*\*🧑‍💻 Code-Verifier Agent\*\*      | Cross-checks that UML and rationale reflect actual Qt/C++ implementation.              | C++ source structure (through summary files), AppController, ExerciseFactory logic.                                  | “Implementation Consistency Report” — confirms pattern integration, naming, and layer alignment. |

| \*\*🗂️ Report-Compiler Agent\*\*      | Packages outputs for submission per CS3307 rubric.                                     | `design\_rationale.md`, UML exports, `TASK\_1.1\_COMPLETION\_SUMMARY.md`, video link.                                    | `/report/BhashaQuest\_D2\_Report.pdf` + README.md summary + Brightspace submission bundle.         |

| \*\*🧭 Rubric-Compliance Agent\*\*     | Evaluates every deliverable element against the official CS3307A rubric.               | `CS3307A Final Project Specification.pdf`, `Assignment Instructions.pdf`.                                            | Checklist annotating Exemplary criteria met/missed (+ next steps for D3).                        |

| \*\*🧑‍🏫 Presentation-Coach Agent\*\* | Refines tone, timing, and clarity for on-camera delivery.                              | `video\_script.md`, `pre\_recording\_checklist.md`.                                                                     | Annotated script w/ intonation markers + segment duration tracking.                              |

| \*\*🔍 Quality-Assurance Agent\*\*     | Ensures technical accuracy, grammar, and consistency across docs.                      | All textual files.                                                                                                   | Proofed final submission set ready for PDF conversion.                                           |



---



\## 🧠 \*\*Inter-Agent Workflow\*\*



1\. \*\*UML-Architect Agent\*\* validates diagrams → exports SVG/PNG → passes to Design-Rationale Agent.

2\. \*\*Design-Rationale Agent\*\* summarizes patterns + architecture choices for report and video sections.

3\. \*\*Code-Verifier Agent\*\* checks that all patterns exist in code and are properly referenced.

4\. \*\*Rubric-Compliance Agent\*\* cross-checks output against Deliverable 2 rubric; flags gaps (e.g., missing pattern evidence, timing overruns).

5\. \*\*Video-Director \& Presentation-Coach Agents\*\* finalize the demo script and ensure recording matches the pre-recording checklist.

6\. \*\*Report-Compiler Agent\*\* integrates all verified outputs into final PDF and README.

7\. \*\*Quality-Assurance Agent\*\* performs final pass before submission.



---



\## 🧰 \*\*Data \& Reference Map\*\*



| File                                            | Purpose                                                      |

| ----------------------------------------------- | ------------------------------------------------------------ |

| `TASK\_1.1\_COMPLETION\_SUMMARY.md`                | Class diagram update summary for Deliverable 2.              |

| `design\_rationale.md` / `design\_rationale.html` | Core report document — architecture + pattern justification. |

| `pre\_recording\_checklist.md`                    | Recording prep workflow for demo.                            |

| `video\_script.md`                               | 4-minute narration + segment timings.                        |

| `CS3307A Final Project Specification.pdf`       | Rubric and submission criteria.                              |

| `3307 – 251163014 – A1-2.pdf`                   | Deliverable 1 proposal baseline (used to track D2 progress). |



---



\## 🧩 \*\*Performance Optimization Rules\*\*



1\. \*\*Truth Alignment:\*\* All agents cross-reference class and sequence diagrams with `AppController`, `ExerciseFactory`, and `StrategyGrader` implementations.

2\. \*\*Rubric Traceability:\*\* Each section tagged (E1 – E5) to the “Exemplary” criteria in CS3307A rubric.

3\. \*\*Demo Synchronization:\*\* Every code segment explained in `video\_script.md` must appear visibly on screen in the recording.

4\. \*\*Time Discipline:\*\* Video Director Agent enforces segment limits ± 3 seconds from plan.

5\. \*\*Version Control:\*\* Agents commit final artifacts to `/report/` with timestamped suffix (e.g., `D2\_Final\_2025-11-10`).

6\. \*\*Validation:\*\* Before submission, run `rubric\_checklist --deliverable=2` to confirm completeness.



---



\## ✅ \*\*Output Artifacts Expected\*\*



```

/report/

├── BhashaQuest\_D2\_Report.pdf          ← compiled rationale + diagrams

├── class\_diagram\_updated.png

├── sequence\_diagram\_updated.png

├── architecture\_diagram.svg

├── README.md (w/ video link)

└── BhashaQuest\_D2\_Walkthrough.mp4     ← 4-min demo

```



---



\## 🧩 \*\*Next Milestone (Deliverable 3 Prep)\*\*



\* Extend agents for \*\*testing integration\*\* (GoogleTest Agent) and \*\*SQLite migration Agent\*\*.

\* Add \*\*Observer Pattern demonstration\*\* and ensure new ListenExercise / SpeakExercise types register in ExerciseFactory.

\* Record unit test evidence for SRSScheduler and StrategyGrader.



---



\*\*This AGENTS.md defines the autonomous collaboration schema for Deliverable 2.\*\*

Running these agents in sequence ensures 100 % rubric compliance and a polished submission.



