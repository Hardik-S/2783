#include "HomeView.h"
#include "ui_HomeView.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFont>
#include <QSpacerItem>

/**
 * HomeView Implementation
 *
 * Simple landing screen following KISS, YAGNI, DRY, SOLID principles:
 * - KISS: Minimal UI with clear purpose
 * - YAGNI: No unnecessary features or complexity
 * - DRY: Consistent button styling via reusable stylesheet patterns
 * - SOLID: Single responsibility (navigation only), Open-Closed (extensible via signals)
 */

HomeView::HomeView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HomeView)
{
    ui->setupUi(this);
    setupUI();
}

HomeView::~HomeView()
{
    delete ui;
}

void HomeView::setupUI() {
    // Enable styled background for custom widget
    setAttribute(Qt::WA_StyledBackground, true);

    // Set vibrant gradient background - Deep Blue to Rich Green
    setStyleSheet(
        "QWidget#HomeView { "
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
        "   stop:0 #0D47A1, stop:1 #1B5E20); "
        "}"
    );

    // Set object name to match stylesheet selector
    setObjectName("HomeView");

    // Create main layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(30);
    mainLayout->setContentsMargins(50, 50, 50, 50);

    // Add top spacer for vertical centering
    mainLayout->addStretch(2);

    // Title/Branding - White with shadow for visibility
    QLabel* titleLabel = new QLabel("BhashaQuest", this);
    QFont titleFont;
    titleFont.setPointSize(36);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "QLabel { "
        "   color: #FFFFFF; "
        "   background: transparent; "
        "   padding: 10px; "
        "}"
    );
    mainLayout->addWidget(titleLabel);

    // Subtitle - Light Blue Tint for contrast
    QLabel* subtitleLabel = new QLabel("Learn Languages, One Quest at a Time", this);
    QFont subtitleFont;
    subtitleFont.setPointSize(14);
    subtitleFont.setItalic(true);
    subtitleLabel->setFont(subtitleFont);
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setStyleSheet(
        "QLabel { "
        "   color: #E3F2FD; "
        "   background: transparent; "
        "}"
    );
    mainLayout->addWidget(subtitleLabel);

    mainLayout->addSpacing(40);

    // Button container for centered layout - Glass-morphism effect
    QWidget* buttonContainer = new QWidget(this);
    buttonContainer->setAttribute(Qt::WA_StyledBackground, true);
    buttonContainer->setStyleSheet(
        "QWidget { "
        "   background-color: rgba(255, 255, 255, 0.08); "
        "   border-radius: 16px; "
        "   padding: 20px; "
        "}"
    );
    QVBoxLayout* buttonLayout = new QVBoxLayout(buttonContainer);
    buttonLayout->setSpacing(20);
    buttonLayout->setContentsMargins(0, 0, 0, 0);

    // Start Learning button - Rich Green
    QPushButton* startLearningButton = new QPushButton("Start Learning", this);
    startLearningButton->setMinimumHeight(60);
    startLearningButton->setMinimumWidth(300);
    startLearningButton->setStyleSheet(
        "QPushButton { "
        "   background-color: #1B5E20; "
        "   color: #FFFFFF; "
        "   border: none; "
        "   border-radius: 10px; "
        "   padding: 18px 24px; "
        "   font-size: 18px; "
        "   font-weight: bold; "
        "} "
        "QPushButton:hover { background-color: #388E3C; } "
        "QPushButton:pressed { background-color: #1B5E20; }"
    );
    connect(startLearningButton, &QPushButton::clicked, this, &HomeView::onStartLearningClicked);
    buttonLayout->addWidget(startLearningButton, 0, Qt::AlignCenter);

    // View Profile button - Deep Orange
    QPushButton* viewProfileButton = new QPushButton("View Profile", this);
    viewProfileButton->setMinimumHeight(60);
    viewProfileButton->setMinimumWidth(300);
    viewProfileButton->setStyleSheet(
        "QPushButton { "
        "   background-color: #E65100; "
        "   color: #FFFFFF; "
        "   border: none; "
        "   border-radius: 10px; "
        "   padding: 18px 24px; "
        "   font-size: 18px; "
        "   font-weight: bold; "
        "} "
        "QPushButton:hover { background-color: #F57C00; } "
        "QPushButton:pressed { background-color: #E65100; }"
    );
    connect(viewProfileButton, &QPushButton::clicked, this, &HomeView::onViewProfileClicked);
    buttonLayout->addWidget(viewProfileButton, 0, Qt::AlignCenter);

    mainLayout->addWidget(buttonContainer);

    // Add bottom spacer for vertical centering
    mainLayout->addStretch(3);

    setLayout(mainLayout);
}

void HomeView::onStartLearningClicked() {
    emit startLearningRequested();
}

void HomeView::onViewProfileClicked() {
    emit viewProfileRequested();
}
