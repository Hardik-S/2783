#ifndef MCQEXERCISE_H
#define MCQEXERCISE_H

#include "Exercise.h"
#include <QStringList>

/**
 * MCQExercise - Multiple Choice Question exercise type
 *
 * Presents 4 options and expects user to select the correct one
 * Example: "What is 'hello' in Nepali?" with options including "नमस्ते"
 */
class MCQExercise : public Exercise {
private:
    QStringList options;        // 4 answer options
    int correctIndex;           // Index of the correct option (0-3)

public:
    MCQExercise();
    MCQExercise(const QString& id, const QString& prompt,
                const QStringList& options, int correctIndex);

    // Override pure virtual methods from Exercise
    QString getPrompt() const override;
    QString getCorrectAnswer() const override;
    QString getType() const override;
    QWidget* renderUI(QWidget* parent = nullptr) override;

    // MCQ-specific methods
    QStringList getOptions() const { return options; }
    int getCorrectIndex() const { return correctIndex; }

    void setOptions(const QStringList& opts) { options = opts; }
    void setCorrectIndex(int index) { correctIndex = index; }
    void setPrompt(const QString& newPrompt) { prompt = newPrompt; }
};

#endif // MCQEXERCISE_H
