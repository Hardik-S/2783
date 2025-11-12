#ifndef EXERCISESEQUENCE_H
#define EXERCISESEQUENCE_H

#include <QList>

class Exercise;

/**
 * ExerciseSequence - Iterator-aware wrapper around an ordered set of exercises.
 * Centralizes traversal logic so clients can iterate without touching QList.
 */
class ExerciseSequence {
public:
    class Iterator {
    public:
        Iterator(const ExerciseSequence* sequence = nullptr, int position = 0);
        Exercise* operator*() const;
        Iterator& operator++();
        bool operator!=(const Iterator& other) const;
        bool operator==(const Iterator& other) const;
        int position() const;

    private:
        const ExerciseSequence* sequence;
        int index;
    };

    ExerciseSequence();
    explicit ExerciseSequence(const QList<Exercise*>& exercises);

    bool isEmpty() const;
    int size() const;
    QList<Exercise*> toList() const;

    Iterator begin() const;
    Iterator end() const;

private:
    QList<Exercise*> exerciseList;
};

#endif // EXERCISESEQUENCE_H
