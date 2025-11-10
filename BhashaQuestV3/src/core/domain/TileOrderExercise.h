#ifndef TILEORDEREXERCISE_H
#define TILEORDEREXERCISE_H

#include "Exercise.h"
#include <QStringList>

/**
 * TileOrderExercise - Word tile ordering exercise type
 *
 * Presents scrambled word tiles that user must reorder correctly
 * Example: Reorder ["म", "नमस्ते", "भन्छु"] to form "नमस्ते म भन्छु" (Hello, I say)
 */
class TileOrderExercise : public Exercise {
private:
    QStringList tiles;              // Word tiles in scrambled order
    QStringList correctOrder;       // Correct order of tiles

public:
    TileOrderExercise();
    TileOrderExercise(const QString& id, const QString& prompt,
                      const QStringList& tiles, const QStringList& correctOrder);

    // Override pure virtual methods from Exercise
    QString getPrompt() const override;
    QString getCorrectAnswer() const override;
    QString getType() const override;
    QWidget* renderUI(QWidget* parent = nullptr) override;

    // TileOrder-specific methods
    QStringList getTiles() const { return tiles; }
    QStringList getCorrectOrder() const { return correctOrder; }

    void setTiles(const QStringList& newTiles) { tiles = newTiles; }
    void setCorrectOrder(const QStringList& order) { correctOrder = order; }
    void setPrompt(const QString& newPrompt) { prompt = newPrompt; }
    void shuffle(); // Shuffle tiles randomly
};

#endif // TILEORDEREXERCISE_H
