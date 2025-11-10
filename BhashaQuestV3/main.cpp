#include "mainwindow.h"
#include <QApplication>

/**
 * BhashaQuest - Language Learning Application
 *
 * Main entry point for the application.
 * Creates QApplication and MainWindow, then starts the event loop.
 *
 * All component initialization (AppController, ContentRepository, Views)
 * happens inside MainWindow constructor.
 */
int main(int argc, char *argv[])
{
    // Create Qt application
    QApplication app(argc, argv);

    // Set application metadata
    app.setApplicationName("BhashaQuest");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("CS3307 Project");

    // Create and show main window
    // MainWindow handles all component initialization and wiring
    MainWindow window;
    window.show();

    // Start event loop
    return app.exec();
}
