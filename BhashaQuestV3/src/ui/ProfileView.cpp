#include "ProfileView.h"
#include <QFrame>
#include <QFont>
#include <QPalette>
#include <QGraphicsDropShadowEffect>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

/**
 * ProfileView Implementation - ENHANCED VERSION
 * With improved UI design, animations, and better data visualization
 */

ProfileView::ProfileView(QWidget* parent)
    : QWidget(parent)
    , currentProfile(nullptr)
{
    setupUI();
}

ProfileView::~ProfileView() {
    // Qt parent-child ownership handles widget cleanup
}

void ProfileView::setupUI() {
    // Set background gradient
    setStyleSheet("QWidget { background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); }");
    
    // Create main layout
    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(25);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    // Title with modern styling
    QLabel* titleLabel = new QLabel("Your Learning Journey", this);
    QFont titleFont;
    titleFont.setPointSize(24);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("QLabel { color: white; margin-bottom: 10px; }");
    mainLayout->addWidget(titleLabel);

    // Profile card
    QWidget* profileCard = new QWidget(this);
    profileCard->setStyleSheet(
        "QWidget { "
        "   background-color: white; "
        "   border-radius: 20px; "
        "   padding: 20px; "
        "}"
    );
    
    // Add shadow effect
    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(20);
    shadowEffect->setXOffset(0);
    shadowEffect->setYOffset(5);
    shadowEffect->setColor(QColor(0, 0, 0, 40));
    profileCard->setGraphicsEffect(shadowEffect);
    
    QVBoxLayout* profileLayout = new QVBoxLayout(profileCard);

    // Avatar placeholder
    QLabel* avatarLabel = new QLabel("👤", profileCard);
    avatarLabel->setAlignment(Qt::AlignCenter);
    avatarLabel->setStyleSheet(
        "QLabel { "
        "   background-color: #E3F2FD; "
        "   border-radius: 50px; "
        "   padding: 20px; "
        "   font-size: 40px; "
        "   max-width: 100px; "
        "   max-height: 100px; "
        "}"
    );
    profileLayout->addWidget(avatarLabel, 0, Qt::AlignCenter);

    // Username
    usernameLabel = new QLabel("Language Learner", profileCard);
    QFont usernameFont;
    usernameFont.setPointSize(18);
    usernameFont.setBold(true);
    usernameLabel->setFont(usernameFont);
    usernameLabel->setAlignment(Qt::AlignCenter);
    usernameLabel->setStyleSheet("QLabel { color: #333; margin: 15px 0; }");
    profileLayout->addWidget(usernameLabel);

    mainLayout->addWidget(profileCard);

    // Stats cards container
    QHBoxLayout* statsContainer = new QHBoxLayout();
    statsContainer->setSpacing(20);

    // XP Card
    QWidget* xpCard = createStatCard("⭐", "Total XP", "0", "#4CAF50");
    xpLabel = xpCard->findChild<QLabel*>("valueLabel");
    statsContainer->addWidget(xpCard);

    // Streak Card  
    QWidget* streakCard = createStatCard("🔥", "Streak", "0 days", "#FF9800");
    streakLabel = streakCard->findChild<QLabel*>("valueLabel");
    statsContainer->addWidget(streakCard);

    // Level Card
    QWidget* levelCard = createStatCard("🏆", "Level", "1", "#9C27B0");
    levelLabel = levelCard->findChild<QLabel*>("valueLabel");
    statsContainer->addWidget(levelCard);

    mainLayout->addLayout(statsContainer);

    // Skills section
    QLabel* skillsTitle = new QLabel("Skill Progress", this);
    QFont skillsTitleFont;
    skillsTitleFont.setPointSize(18);
    skillsTitleFont.setBold(true);
    skillsTitle->setFont(skillsTitleFont);
    skillsTitle->setStyleSheet("QLabel { color: white; margin-top: 20px; }");
    mainLayout->addWidget(skillsTitle);

    // Skills container with scroll area
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet(
        "QScrollArea { "
        "   background-color: transparent; "
        "   border: none; "
        "}"
    );
    
    QWidget* skillsContainer = new QWidget();
    skillsLayout = new QVBoxLayout(skillsContainer);
    skillsLayout->setSpacing(15);
    scrollArea->setWidget(skillsContainer);

    mainLayout->addWidget(scrollArea);

    // Developer Tools section (Time Travel)
    QWidget* devToolsCard = new QWidget(this);
    devToolsCard->setStyleSheet(
        "QWidget { "
        "   background-color: #FFF3E0; "
        "   border-radius: 15px; "
        "   padding: 15px; "
        "   margin-top: 10px; "
        "}"
    );
    QVBoxLayout* devLayout = new QVBoxLayout(devToolsCard);
    devLayout->setSpacing(10);

    QLabel* devLabel = new QLabel("🔧 Developer Tools", devToolsCard);
    QFont devFont;
    devFont.setPointSize(12);
    devFont.setBold(true);
    devLabel->setFont(devFont);
    devLabel->setStyleSheet("QLabel { color: #E65100; }");
    devLayout->addWidget(devLabel);

    QPushButton* timeTravelBtn = new QPushButton("⏭️ Next Day (Test Streak)", devToolsCard);
    timeTravelBtn->setMinimumHeight(40);
    timeTravelBtn->setStyleSheet(
        "QPushButton { "
        "   background-color: #FF9800; "
        "   color: white; "
        "   border: none; "
        "   border-radius: 10px; "
        "   padding: 10px; "
        "   font-size: 13px; "
        "   font-weight: bold; "
        "} "
        "QPushButton:hover { "
        "   background-color: #FB8C00; "
        "} "
        "QPushButton:pressed { "
        "   background-color: #F57C00; "
        "}"
    );
    connect(timeTravelBtn, &QPushButton::clicked, this, &ProfileView::onTimeTravelClicked);
    devLayout->addWidget(timeTravelBtn);

    mainLayout->addWidget(devToolsCard);

    setLayout(mainLayout);
}

QWidget* ProfileView::createStatCard(const QString& icon, const QString& label, const QString& value, const QString& color) {
    QWidget* card = new QWidget(this);
    card->setMinimumHeight(120);
    card->setStyleSheet(
        QString("QWidget { "
        "   background-color: white; "
        "   border-radius: 15px; "
        "   padding: 20px; "
        "   border: 3px solid %1; "
        "}").arg(color)
    );

    QVBoxLayout* layout = new QVBoxLayout(card);
    layout->setSpacing(8);

    // Icon
    QLabel* iconLabel = new QLabel(icon, card);
    iconLabel->setAlignment(Qt::AlignCenter);
    iconLabel->setStyleSheet(QString("QLabel { font-size: 30px; color: %1; }").arg(color));
    layout->addWidget(iconLabel);

    // Label
    QLabel* titleLabel = new QLabel(label, card);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("QLabel { color: #666; font-size: 12px; }");
    layout->addWidget(titleLabel);

    // Value
    QLabel* valueLabel = new QLabel(value, card);
    valueLabel->setObjectName("valueLabel");
    valueLabel->setAlignment(Qt::AlignCenter);
    QFont valueFont;
    valueFont.setPointSize(20);
    valueFont.setBold(true);
    valueLabel->setFont(valueFont);
    valueLabel->setStyleSheet(QString("QLabel { color: %1; }").arg(color));
    layout->addWidget(valueLabel);

    return card;
}

void ProfileView::updateProfile(Profile* profile) {
    if (!profile) return;

    currentProfile = profile;
    usernameLabel->setText(profile->getUsername());
    updateStats(profile->getCurrentXP(), profile->getStreak());
    refreshSkillsList();
}

void ProfileView::updateStats(int xp, int streak) {
    xpLabel->setText(QString::number(xp));
    streakLabel->setText(streak == 1 ? "1 day" : QString("%1 days").arg(streak));
    
    // Calculate level based on XP (100 XP per level)
    int level = (xp / 100) + 1;
    levelLabel->setText(QString::number(level));
}

void ProfileView::reset() {
    usernameLabel->setText("Language Learner");
    xpLabel->setText("0");
    streakLabel->setText("0 days");
    levelLabel->setText("1");

    // Clear skills
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

    // Advance date by 1 day
    currentProfile->advanceSimulatedDate(1);

    // Check streak validity (will reset if more than 1 day passed without activity)
    currentProfile->checkStreakValidity();

    // Refresh UI to show updated stats
    updateProfile(currentProfile);

    // Show confirmation with current simulated date
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

    // Clear existing items
    QLayoutItem* item;
    while ((item = skillsLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    QMap<QString, SkillProgress*> allProgress = currentProfile->getAllProgress();

    if (allProgress.isEmpty()) {
        QLabel* placeholderLabel = new QLabel("No skills started yet.\nComplete a lesson to begin your journey!", this);
        placeholderLabel->setWordWrap(true);
        placeholderLabel->setAlignment(Qt::AlignCenter);
        placeholderLabel->setStyleSheet(
            "QLabel { "
            "   color: white; "
            "   font-size: 14px; "
            "   font-style: italic; "
            "   padding: 30px; "
            "   background-color: rgba(255, 255, 255, 0.1); "
            "   border-radius: 10px; "
            "}"
        );
        skillsLayout->addWidget(placeholderLabel);
        return;
    }

    for (auto it = allProgress.begin(); it != allProgress.end(); ++it) {
        QString skillId = it.key();
        SkillProgress* progress = it.value();
        
        if (progress) {
            QWidget* skillWidget = createSkillItemWidget(skillId, progress);
            skillsLayout->addWidget(skillWidget);
        }
    }
}

QWidget* ProfileView::createSkillItemWidget(const QString& skillId, SkillProgress* progress) {
    QWidget* container = new QWidget(this);
    container->setStyleSheet(
        "QWidget { "
        "   background-color: white; "
        "   border-radius: 15px; "
        "   padding: 20px; "
        "}"
    );

    QVBoxLayout* layout = new QVBoxLayout(container);
    layout->setSpacing(12);

    // Format skill name
    QString displayName = skillId;
    displayName.replace("-", " ");
    QStringList words = displayName.split(" ");
    for (int i = 0; i < words.size(); ++i) {
        if (!words[i].isEmpty()) {
            words[i] = words[i].at(0).toUpper() + words[i].mid(1);
        }
    }
    displayName = words.join(" ");

    // Skill name with language badge
    QHBoxLayout* headerLayout = new QHBoxLayout();
    
    QLabel* nameLabel = new QLabel(displayName, container);
    QFont nameFont;
    nameFont.setPointSize(14);
    nameFont.setBold(true);
    nameLabel->setFont(nameFont);
    nameLabel->setStyleSheet("QLabel { color: #333; }");
    headerLayout->addWidget(nameLabel);
    
    headerLayout->addStretch();
    
    // Language badge
    QString language = skillId.contains("kannada") ? "Kannada" : "Nepali";
    QLabel* langBadge = new QLabel(language, container);
    langBadge->setStyleSheet(
        "QLabel { "
        "   background-color: #E3F2FD; "
        "   color: #2196F3; "
        "   padding: 3px 10px; "
        "   border-radius: 10px; "
        "   font-size: 11px; "
        "   font-weight: bold; "
        "}"
    );
    headerLayout->addWidget(langBadge);
    
    layout->addLayout(headerLayout);

    // Mastery progress bar
    QProgressBar* progressBar = new QProgressBar(container);
    progressBar->setMinimum(0);
    progressBar->setMaximum(100);
    progressBar->setValue(progress->getMasteryLevel());
    progressBar->setTextVisible(true);
    progressBar->setFormat(QString("%1% Mastery").arg(progress->getMasteryLevel()));
    progressBar->setMinimumHeight(30);
    
    // Color based on mastery level
    QString progressColor = "#4CAF50";  // Green for > 60%
    if (progress->getMasteryLevel() < 30) {
        progressColor = "#F44336";  // Red for < 30%
    } else if (progress->getMasteryLevel() < 60) {
        progressColor = "#FF9800";  // Orange for 30-60%
    }
    
    progressBar->setStyleSheet(
        QString("QProgressBar { "
        "   border: 2px solid #e0e0e0; "
        "   border-radius: 15px; "
        "   text-align: center; "
        "   background-color: #f5f5f5; "
        "   font-weight: bold; "
        "   color: #333; "
        "} "
        "QProgressBar::chunk { "
        "   background-color: %1; "
        "   border-radius: 13px; "
        "}").arg(progressColor)
    );
    layout->addWidget(progressBar);

    // Stats row
    QHBoxLayout* statsRow = new QHBoxLayout();
    statsRow->setSpacing(20);

    // Completed exercises
    QLabel* completedLabel = new QLabel(
        QString("📝 %1/%2 Completed").arg(progress->getExercisesCompleted()).arg(progress->getTotalExercises()),
        container
    );
    completedLabel->setStyleSheet("QLabel { color: #666; font-size: 12px; }");
    statsRow->addWidget(completedLabel);

    statsRow->addStretch();

    // Accuracy with color coding
    double accuracy = progress->getAccuracy();
    QString accuracyText = QString("🎯 %1% Accuracy").arg(QString::number(accuracy, 'f', 0));
    QLabel* accuracyLabel = new QLabel(accuracyText, container);
    
    QString accuracyColor = "#4CAF50";  // Green
    if (accuracy < 60.0) {
        accuracyColor = "#F44336";  // Red
    } else if (accuracy < 80.0) {
        accuracyColor = "#FF9800";  // Orange
    }
    
    accuracyLabel->setStyleSheet(
        QString("QLabel { color: %1; font-weight: bold; font-size: 12px; }").arg(accuracyColor)
    );
    statsRow->addWidget(accuracyLabel);

    layout->addLayout(statsRow);
    container->setLayout(layout);
    
    return container;
}
