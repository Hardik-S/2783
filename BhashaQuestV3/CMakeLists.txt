cmake_minimum_required(VERSION 3.16)
project(BhashaQuest VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt6 packages
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Multimedia)

# Source files
set(DOMAIN_SOURCES
    src/core/domain/Exercise.cpp
    src/core/domain/MCQExercise.cpp
    src/core/domain/TranslateExercise.cpp
    src/core/domain/TileOrderExercise.cpp
    src/core/domain/ExerciseFactory.cpp
    src/core/domain/StrategyGrader.cpp
    src/core/domain/MCQGrader.cpp
    src/core/domain/TranslateGrader.cpp
    src/core/domain/TileOrderGrader.cpp
    src/core/domain/SRSScheduler.cpp
    src/core/domain/Profile.cpp
    src/core/domain/SkillProgress.cpp
    src/core/domain/AudioManager.cpp
)

set(CONTROLLER_SOURCES
    src/core/controllers/AppController.cpp
)

set(DATA_SOURCES
    src/core/data/ContentRepository.cpp
)

set(UI_SOURCES
    src/ui/HomeView.cpp
    src/ui/LessonView.cpp
    src/ui/ProfileView.cpp
    main.cpp
    mainwindow.cpp
)

# Header files
set(HEADERS
    src/core/domain/Exercise.h
    src/core/domain/MCQExercise.h
    src/core/domain/TranslateExercise.h
    src/core/domain/TileOrderExercise.h
    src/core/domain/ExerciseFactory.h
    src/core/domain/StrategyGrader.h
    src/core/domain/MCQGrader.h
    src/core/domain/TranslateGrader.h
    src/core/domain/TileOrderGrader.h
    src/core/domain/SRSScheduler.h
    src/core/domain/Profile.h
    src/core/domain/SkillProgress.h
    src/core/domain/AudioManager.h
    src/core/domain/Result.h
    src/core/controllers/AppController.h
    src/core/data/ContentRepository.h
    src/ui/HomeView.h
    src/ui/LessonView.h
    src/ui/ProfileView.h
    mainwindow.h
)

# UI files
set(UI_FILES
    src/ui/HomeView.ui
    src/ui/LessonView.ui
    src/ui/ProfileView.ui
    mainwindow.ui
)

# Create executable
add_executable(BhashaQuest
    ${DOMAIN_SOURCES}
    ${CONTROLLER_SOURCES}
    ${DATA_SOURCES}
    ${UI_SOURCES}
    ${HEADERS}
    ${UI_FILES}
)

# Link Qt6 libraries
target_link_libraries(BhashaQuest
    Qt6::Core
    Qt6::Widgets
    Qt6::Multimedia
)

# Include directories
target_include_directories(BhashaQuest PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/core/domain
    ${CMAKE_SOURCE_DIR}/src/core/controllers
    ${CMAKE_SOURCE_DIR}/src/core/data
    ${CMAKE_SOURCE_DIR}/src/ui
)

# Copy content.json and assets to build directory
file(COPY ${CMAKE_SOURCE_DIR}/src/core/data/content.json
     DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/assets
     DESTINATION ${CMAKE_BINARY_DIR})
