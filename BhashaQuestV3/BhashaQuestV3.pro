QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    src/core/controllers/AppController.cpp \
    src/core/data/ContentRepository.cpp \
    src/core/domain/AudioManager.cpp \
    src/core/domain/Exercise.cpp \
    src/core/domain/ExerciseFactory.cpp \
    src/core/domain/MCQExercise.cpp \
    src/core/domain/MCQGrader.cpp \
    src/core/domain/Profile.cpp \
    src/core/domain/SkillProgress.cpp \
    src/core/domain/SRSScheduler.cpp \
    src/core/domain/ExerciseSequence.cpp \
    src/core/domain/StrategyGrader.cpp \
    src/core/domain/TileOrderExercise.cpp \
    src/core/domain/TileOrderGrader.cpp \
    src/core/domain/TranslateExercise.cpp \
    src/core/domain/TranslateGrader.cpp \
    src/core/facade/LessonFacade.cpp \
    src/ui/HomeView.cpp \
    src/ui/LessonView.cpp \
    src/ui/ProfileView.cpp

HEADERS += \
    mainwindow.h \
    src/core/controllers/AppController.h \
    src/core/data/ContentRepository.h \
    src/core/domain/AudioManager.h \
    src/core/domain/Exercise.h \
    src/core/domain/ExerciseFactory.h \
    src/core/domain/MCQExercise.h \
    src/core/domain/MCQGrader.h \
    src/core/domain/Profile.h \
    src/core/domain/Result.h \
    src/core/domain/SkillProgress.h \
    src/core/domain/SRSScheduler.h \
    src/core/domain/ExerciseSequence.h \
    src/core/domain/StrategyGrader.h \
    src/core/domain/TileOrderExercise.h \
    src/core/domain/TileOrderGrader.h \
    src/core/domain/TranslateExercise.h \
    src/core/domain/TranslateGrader.h \
    src/core/facade/LessonFacade.h \
    src/ui/HomeView.h \
    src/ui/LessonView.h \
    src/ui/ProfileView.h

FORMS += \
    mainwindow.ui \
    src/ui/HomeView.ui \
    src/ui/LessonView.ui \
    src/ui/ProfileView.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# YAGNI Principle: Simple file copy, no complex resource embedding needed for development
# Copy content.json and assets directory to build directory
CONFIG(debug, debug|release) {
    win32 {
        QMAKE_POST_LINK += copy /Y $$shell_quote($$shell_path($$PWD/src/core/data/content.json)) $$shell_quote($$shell_path($$OUT_PWD/debug/content.json)) $$escape_expand(\n\t)
        QMAKE_POST_LINK += xcopy /Y /E /I $$shell_quote($$shell_path($$PWD/assets)) $$shell_quote($$shell_path($$OUT_PWD/debug/assets)) $$escape_expand(\n\t)
    }
    unix {
        QMAKE_POST_LINK += cp $$shell_quote($$PWD/src/core/data/content.json) $$shell_quote($$OUT_PWD/content.json) $$escape_expand(\n\t)
        QMAKE_POST_LINK += cp -r $$shell_quote($$PWD/assets) $$shell_quote($$OUT_PWD/assets) $$escape_expand(\n\t)
    }
}
CONFIG(release, debug|release) {
    win32 {
        QMAKE_POST_LINK += copy /Y $$shell_quote($$shell_path($$PWD/src/core/data/content.json)) $$shell_quote($$shell_path($$OUT_PWD/release/content.json)) $$escape_expand(\n\t)
        QMAKE_POST_LINK += xcopy /Y /E /I $$shell_quote($$shell_path($$PWD/assets)) $$shell_quote($$shell_path($$OUT_PWD/release/assets)) $$escape_expand(\n\t)
    }
    unix {
        QMAKE_POST_LINK += cp $$shell_quote($$PWD/src/core/data/content.json) $$shell_quote($$OUT_PWD/content.json) $$escape_expand(\n\t)
        QMAKE_POST_LINK += cp -r $$shell_quote($$PWD/assets) $$shell_quote($$OUT_PWD/assets) $$escape_expand(\n\t)
    }
}
