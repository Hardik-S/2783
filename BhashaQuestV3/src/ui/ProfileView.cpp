#include "ProfileView.h"
#include <QFrame>
#include <QFont>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QProgressBar>
#include <QPushButton>
#include <QStringList>


ProfileView::ProfileView(QWidget* parent)
    : QWidget(parent)
    , currentProfile(nullptr)
{
    setupUI();
}

ProfileView::~ProfileView() { }

void ProfileView::setupUI() {
    setStyleSheet("QWidget { background-color: #5ec48c; }");

    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(12);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    QLabel* titleLabel = new QLabel("Your Learning Journey", this);
    QFont titleFont;
    titleFont.setPointSize(22);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: white;");
    mainLayout->addWidget(titleLabel);

    QFrame* focusFrame = new QFrame(this);
    focusFrame->setStyleSheet(
        "QFrame { background-color: rgba(255, 255, 255, 0.18); border-radius: 12px; }"
    );
    QVBoxLayout* focusLayout = new QVBoxLayout(focusFrame);
    focusLayout->setSpacing(4);
    focusLayout->setContentsMargins(12, 12, 12, 12);

    skillSelectionLabel = new QLabel("Select Skill", focusFrame);
    skillSelectionLabel->setStyleSheet("color: white; font-weight: bold;");
    selectedLanguageLabel = new QLabel("Selected Language: Nepali", focusFrame);
    selectedLanguageLabel->setStyleSheet("color: white;");
    topFocusLabel = new QLabel("Top Focus: Nepali Numbers", focusFrame);
    topFocusLabel->setStyleSheet("color: white;");

    focusLayout->addWidget(skillSelectionLabel);
    focusLayout->addWidget(selectedLanguageLabel);
    focusLayout->addWidget(topFocusLabel);
    mainLayout->addWidget(focusFrame);

    QFrame* profileCard = new QFrame(this);
    profileCard->setStyleSheet("QFrame { background-color: white; border-radius: 16px; padding: 18px; }");
    QVBoxLayout* profileLayout = new QVBoxLayout(profileCard);
    profileLayout->setSpacing(8);
    profileLayout->setContentsMargins(0, 0, 0, 0);

    QLabel* avatarLabel = new QLabel("👤", profileCard);
    avatarLabel->setAlignment(Qt::AlignCenter);
    avatarLabel->setStyleSheet("font-size: 32px;");
    profileLayout->addWidget(avatarLabel, 0, Qt::AlignCenter);

    usernameLabel = new QLabel("Player", profileCard);
    QFont nameFont;
    nameFont.setPointSize(18);
    nameFont.setBold(true);
    usernameLabel->setFont(nameFont);
    usernameLabel->setAlignment(Qt::AlignCenter);
    profileLayout->addWidget(usernameLabel);

    QLabel* taglineLabel = new QLabel(
        "Tracking streaks, XP, and the languages you care about.",
        profileCard
    );
    taglineLabel->setWordWrap(true);
    taglineLabel->setAlignment(Qt::AlignCenter);
    taglineLabel->setStyleSheet("color: #555555; font-size: 13px;");
    profileLayout->addWidget(taglineLabel);

    mainLayout->addWidget(profileCard);

    QHBoxLayout* statsLayout = new QHBoxLayout();
    statsLayout->setSpacing(10);

    QWidget* xpCard = createStatCard("⭐", "Total XP", "0", "#1eb85d");
    xpLabel = xpCard->findChild<QLabel*>("valueLabel");
    statsLayout->addWidget(xpCard);

    QWidget* streakCard = createStatCard("🔥", "Streak", "0 days", "#ff9800");
    streakLabel = streakCard->findChild<QLabel*>("valueLabel");
    statsLayout->addWidget(streakCard);

    QWidget* levelCard = createStatCard("🏆", "Level", "1", "#9c27b0");
    levelLabel = levelCard->findChild<QLabel*>("valueLabel");
    statsLayout->addWidget(levelCard);

    mainLayout->addLayout(statsLayout);

    QFrame* xpFrame = new QFrame(this);
    xpFrame->setStyleSheet("QFrame { background-color: rgba(255, 255, 255, 0.2); border-radius: 12px; padding: 10px; }");
    QHBoxLayout* xpLayout = new QHBoxLayout(xpFrame);
    xpLayout->setSpacing(10);
    xpLayout->setContentsMargins(8, 8, 8, 8);

    xpNextLabel = new QLabel("100 XP to Level 2", xpFrame);
    xpNextLabel->setStyleSheet("color: white;");

    xpProgress = new QProgressBar(xpFrame);
    xpProgress->setMinimum(0);
    xpProgress->setMaximum(100);
    xpProgress->setValue(0);
    xpProgress->setTextVisible(false);
    xpProgress->setStyleSheet(R"(
QProgressBar { background-color: rgba(255, 255, 255, 0.35); border: none; border-radius: 8px; }
QProgressBar::chunk { background-color: #23a455; border-radius: 8px; }
)");

    xpLayout->addWidget(xpNextLabel);
    xpLayout->addWidget(xpProgress, 1);
    mainLayout->addWidget(xpFrame);

    QFrame* snapshotFrame = new QFrame(this);
    snapshotFrame->setStyleSheet("QFrame { background-color: white; border-radius: 14px; padding: 16px; }");
    QVBoxLayout* snapshotLayout = new QVBoxLayout(snapshotFrame);
    snapshotLayout->setSpacing(8);
    snapshotLayout->setContentsMargins(0, 0, 0, 0);

    QLabel* snapshotTitle = new QLabel("Progress Snapshot", snapshotFrame);
    snapshotTitle->setStyleSheet("color: #333333; font-weight: bold;");
    snapshotLayout->addWidget(snapshotTitle);

    masteryValueLabel = new QLabel("Average Mastery: 0%", snapshotFrame);
    masteryValueLabel->setStyleSheet("color: #333333;");
    snapshotLayout->addWidget(masteryValueLabel);

    masteryBar = new QProgressBar(snapshotFrame);
    masteryBar->setMinimum(0);
    masteryBar->setMaximum(100);
    masteryBar->setValue(0);
    masteryBar->setTextVisible(false);
    masteryBar->setStyleSheet(R"(
QProgressBar { background-color: #f0f0f0; border-radius: 8px; height: 16px; }
QProgressBar::chunk { background-color: #1ab47d; border-radius: 8px; }
)");
    snapshotLayout->addWidget(masteryBar);

    accuracyValueLabel = new QLabel("Overall Accuracy: 0%", snapshotFrame);
    accuracyValueLabel->setStyleSheet("color: #4a4a4a; font-size: 12px;");
    snapshotLayout->addWidget(accuracyValueLabel);

    completionValueLabel = new QLabel("Exercises Completed: 0 / 0", snapshotFrame);
    completionValueLabel->setStyleSheet("color: #4a4a4a; font-size: 12px;");
    snapshotLayout->addWidget(completionValueLabel);

    mainLayout->addWidget(snapshotFrame);

    QLabel* skillsTitle = new QLabel("Skill Progress", this);
    QFont skillsFont;
    skillsFont.setPointSize(18);
    skillsFont.setBold(true);
    skillsTitle->setFont(skillsFont);
    skillsTitle->setStyleSheet("color: white;");
    mainLayout->addWidget(skillsTitle);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { border: none; background: transparent; }");
    QWidget* skillsContainer = new QWidget();
    skillsLayout = new QVBoxLayout(skillsContainer);
    skillsLayout->setSpacing(10);
    skillsLayout->setContentsMargins(0, 0, 0, 0);
    scrollArea->setWidget(skillsContainer);
    mainLayout->addWidget(scrollArea, 1);

    QFrame* devFrame = new QFrame(this);
    devFrame->setStyleSheet("QFrame { background-color: #ffb347; border-radius: 12px; }");
    QHBoxLayout* devLayout = new QHBoxLayout(devFrame);
    devLayout->setContentsMargins(12, 8, 12, 8);

    QLabel* devLabel = new QLabel("Developer Tools", devFrame);
    devLabel->setStyleSheet("color: #5c2f00; font-weight: bold;");
    QPushButton* timeTravelBtn = new QPushButton("Next Day (Test Streak)", devFrame);
    timeTravelBtn->setStyleSheet(R"(
QPushButton { background-color: #ff8c00; color: white; border: none; border-radius: 10px; padding: 6px 14px; }
QPushButton:hover { background-color: #ff9000; }
)");
    connect(timeTravelBtn, &QPushButton::clicked, this, &ProfileView::onTimeTravelClicked);

    devLayout->addWidget(devLabel);
    devLayout->addStretch();
    devLayout->addWidget(timeTravelBtn);
    mainLayout->addWidget(devFrame);

    footerLabel = new QLabel("🧭 Viewing your profile...", this);
    footerLabel->setAlignment(Qt::AlignCenter);
    footerLabel->setStyleSheet("color: rgba(255, 255, 255, 0.9); font-size: 11px;");
    mainLayout->addWidget(footerLabel);

    setMinimumSize(800, 640);
    setLayout(mainLayout);
}

QWidget* ProfileView::createStatCard(const QString& icon, const QString& label, const QString& value,
                                     const QString& accent) {
    QFrame* card = new QFrame(this);
    card->setStyleSheet(
        QString("QFrame { background-color: white; border: 2px solid %1; border-radius: 12px; padding: 6px 12px; }")
        .arg(accent)
    );

    QHBoxLayout* layout = new QHBoxLayout(card);
    layout->setSpacing(10);
    layout->setContentsMargins(4, 4, 4, 4);

    QLabel* iconLabel = new QLabel(icon, card);
    iconLabel->setFixedWidth(32);
    iconLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(iconLabel);

    QVBoxLayout* textLayout = new QVBoxLayout();
    textLayout->setSpacing(1);
    textLayout->setContentsMargins(0, 0, 0, 0);

    QLabel* titleLabel = new QLabel(label, card);
    titleLabel->setStyleSheet("color: #777777; font-size: 12px;");
    textLayout->addWidget(titleLabel);

    QLabel* valueLabel = new QLabel(value, card);
    valueLabel->setObjectName("valueLabel");
    QFont valueFont;
    valueFont.setPointSize(16);
    valueFont.setBold(true);
    valueLabel->setFont(valueFont);
    textLayout->addWidget(valueLabel);

    layout->addLayout(textLayout);
    layout->addStretch();

    return card;
}

void ProfileView::updateProfile(Profile* profile) {
    if (!profile) return;

    currentProfile = profile;
    usernameLabel->setText(profile->getUsername());
    selectedLanguageLabel->setText(QString("Selected Language: %1").arg(profile->getSelectedLanguage()));
    updateStats(profile->getCurrentXP(), profile->getStreak());
    refreshSkillsList();
}

void ProfileView::updateStats(int xp, int streak) {
    xpLabel->setText(QString::number(xp));
    streakLabel->setText(streak == 1 ? "1 day" : QString("%1 days").arg(streak));

    int level = (xp / 100) + 1;
    levelLabel->setText(QString::number(level));

    int xpIntoLevel = xp - (level - 1) * 100;
    xpIntoLevel = qBound(0, xpIntoLevel, 100);
    int xpToNext = level * 100 - xp;
    xpToNext = qMax(0, xpToNext);

    xpNextLabel->setText(QString("%1 XP to Level %2").arg(xpToNext).arg(level + 1));
    xpProgress->setValue(xpIntoLevel);
}

void ProfileView::reset() {
    usernameLabel->setText("Player");
    xpLabel->setText("0");
    streakLabel->setText("0 days");
    levelLabel->setText("1");
    selectedLanguageLabel->setText("Selected Language: Nepali");
    topFocusLabel->setText("Top Focus: Nepali Numbers");
    xpNextLabel->setText("100 XP to Level 2");
    xpProgress->setValue(0);
    masteryValueLabel->setText("Average Mastery: 0%");
    masteryBar->setValue(0);
    accuracyValueLabel->setText("Overall Accuracy: 0%");
    completionValueLabel->setText("Exercises Completed: 0 / 0");
    footerLabel->setText("🧭 Viewing your profile...");

    QLayoutItem* item;
    while ((item = skillsLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    currentProfile = nullptr;
}

void ProfileView::onTimeTravelClicked() {
    if (!currentProfile) {
        QMessageBox::warning(this, "Time Travel", "No profile loaded!");
        return;
    }

    currentProfile->advanceSimulatedDate(1);
    currentProfile->checkStreakValidity();
    updateProfile(currentProfile);

    QString message = QString("⏭️ Advanced to: %1\n\n"
                             "Current Streak: %2 days\n"
                             "Total XP: %3\n\n"
                             "💡 Complete a lesson to maintain your streak!")
                        .arg(currentProfile->getCurrentDate().toString("dddd, MMMM d, yyyy"))
                        .arg(currentProfile->getStreak())
                        .arg(currentProfile->getCurrentXP());

    QMessageBox::information(this, "Time Travel", message);
}

void ProfileView::refreshSkillsList() {
    if (!currentProfile) return;

    QLayoutItem* item;
    while ((item = skillsLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    QMap<QString, SkillProgress*> allProgress = currentProfile->getAllProgress();

    if (allProgress.isEmpty()) {
        QLabel* placeholder = new QLabel("No skills started yet. Complete a lesson to populate this list.", this);
        placeholder->setWordWrap(true);
        placeholder->setStyleSheet("color: white;");
        skillsLayout->addWidget(placeholder);
        topFocusLabel->setText("Top Focus: -");
        masteryValueLabel->setText("Average Mastery: 0%");
        masteryBar->setValue(0);
        accuracyValueLabel->setText("Overall Accuracy: 0%");
        completionValueLabel->setText("Exercises Completed: 0 / 0");
        return;
    }

    int totalMastery = 0;
    int totalExercises = 0;
    int completedExercises = 0;
    int totalCorrect = 0;
    int totalIncorrect = 0;
    QString topSkillId;
    int highestMastery = -1;
    int skillCount = 0;

    for (auto it = allProgress.begin(); it != allProgress.end(); ++it) {
        QString skillId = it.key();
        SkillProgress* progress = it.value();
        if (!progress) continue;

        ++skillCount;
        totalMastery += progress->getMasteryLevel();
        totalExercises += progress->getTotalExercises();
        completedExercises += progress->getExercisesCompleted();
        totalCorrect += progress->getCorrectAnswers();
        totalIncorrect += progress->getIncorrectAnswers();

        if (progress->getMasteryLevel() > highestMastery) {
            highestMastery = progress->getMasteryLevel();
            topSkillId = skillId;
        }

        QWidget* skillWidget = createSkillItemWidget(skillId, progress);
        skillsLayout->addWidget(skillWidget);
    }

    skillsLayout->addStretch();

    int avgMastery = skillCount > 0 ? totalMastery / skillCount : 0;
    masteryValueLabel->setText(QString("Average Mastery: %1%").arg(avgMastery));
    masteryBar->setValue(avgMastery);

    int totalAttempts = totalCorrect + totalIncorrect;
    double accuracy = totalAttempts > 0 ? (static_cast<double>(totalCorrect) / totalAttempts) * 100.0 : 0.0;
    accuracyValueLabel->setText(QString("Overall Accuracy: %1%").arg(QString::number(accuracy, 'f', 0)));

    completionValueLabel->setText(QString("Exercises Completed: %1 / %2")
                                  .arg(completedExercises)
                                  .arg(totalExercises));

    if (!topSkillId.isEmpty()) {
        topFocusLabel->setText(QString("Top Focus: %1").arg(formatSkillName(topSkillId)));
    } else {
        topFocusLabel->setText("Top Focus: -");
    }
}

QWidget* ProfileView::createSkillItemWidget(const QString& skillId, SkillProgress* progress) {
    QFrame* container = new QFrame(this);
    container->setStyleSheet(
        "QFrame { background-color: white; border-radius: 12px; padding: 14px; }"
    );
    QVBoxLayout* layout = new QVBoxLayout(container);
    layout->setSpacing(8);
    layout->setContentsMargins(0, 0, 0, 0);

    QLabel* titleLabel = new QLabel(formatSkillName(skillId), container);
    QFont titleFont;
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    layout->addWidget(titleLabel);

    QProgressBar* progressBar = new QProgressBar(container);
    progressBar->setMinimum(0);
    progressBar->setMaximum(100);
    progressBar->setValue(progress->getMasteryLevel());
    progressBar->setTextVisible(true);
    progressBar->setFormat("%p% Mastery");
    progressBar->setStyleSheet(R"(
QProgressBar { background-color: #f0f0f0; border-radius: 10px; height: 16px; }
QProgressBar::chunk { background-color: #23a455; border-radius: 10px; }
)");
    layout->addWidget(progressBar);

    QHBoxLayout* statsRow = new QHBoxLayout();
    statsRow->setSpacing(8);

    QLabel* completedLabel = new QLabel(
        QString("%1/%2 Completed").arg(progress->getExercisesCompleted()).arg(progress->getTotalExercises()),
        container
    );
    completedLabel->setStyleSheet("color: #555555; font-size: 12px;");
    statsRow->addWidget(completedLabel);

    statsRow->addStretch();

    double accuracy = progress->getAccuracy();
    QLabel* accuracyLabel = new QLabel(QString("%1% Accuracy").arg(QString::number(accuracy, 'f', 0)), container);
    accuracyLabel->setStyleSheet("color: #1a7f4b; font-size: 12px;");
    statsRow->addWidget(accuracyLabel);

    layout->addLayout(statsRow);

    return container;
}

QString ProfileView::formatSkillName(const QString& skillId) const {
    QString normalized = skillId;
    normalized.replace("-", " ");
    normalized.replace("_", " ");
    QStringList words = normalized.split(' ', Qt::SkipEmptyParts);
    for (int i = 0; i < words.size(); ++i) {
        if (!words[i].isEmpty()) {
            words[i] = words[i].left(1).toUpper() + words[i].mid(1).toLower();
        }
    }
    return words.join(' ');
}
