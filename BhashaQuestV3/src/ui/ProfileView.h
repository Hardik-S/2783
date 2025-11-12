#ifndef PROFILEVIEW_H
#define PROFILEVIEW_H

#include <QWidget>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include "../core/domain/Profile.h"

class ProfileView : public QWidget {
    Q_OBJECT

public:
    explicit ProfileView(QWidget* parent = nullptr);
    ~ProfileView();

    void updateProfile(Profile* profile);
    void updateStats(int xp, int streak);
    void reset();

private slots:
    void onTimeTravelClicked();

private:
    void setupUI();
    void refreshSkillsList();
    QWidget* createStatCard(const QString& icon, const QString& label, const QString& value,
                            const QString& accent);
    QWidget* createSkillItemWidget(const QString& skillId, SkillProgress* progress);
    QString formatSkillName(const QString& skillId) const;

    QLabel* skillSelectionLabel;
    QLabel* selectedLanguageLabel;
    QLabel* topFocusLabel;

    QLabel* usernameLabel;
    QLabel* xpLabel;
    QLabel* streakLabel;
    QLabel* levelLabel;

    QLabel* xpNextLabel;
    QProgressBar* xpProgress;

    QLabel* masteryValueLabel;
    QProgressBar* masteryBar;
    QLabel* accuracyValueLabel;
    QLabel* completionValueLabel;

    QLabel* footerLabel;

    QVBoxLayout* skillsLayout;
    QVBoxLayout* mainLayout;

    Profile* currentProfile;
};

#endif // PROFILEVIEW_H
