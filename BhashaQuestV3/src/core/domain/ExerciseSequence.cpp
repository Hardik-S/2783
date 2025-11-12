#include "ExerciseSequence.h"
#include "Exercise.h"

ExerciseSequence::Iterator::Iterator(const ExerciseSequence* sequence, int position)
    : sequence(sequence)
    , index(position)
{
}

Exercise* ExerciseSequence::Iterator::operator*() const {
    if (!sequence || index >= sequence->exerciseList.size()) {
        return nullptr;
    }
    return sequence->exerciseList.at(index);
}

ExerciseSequence::Iterator& ExerciseSequence::Iterator::operator++() {
    if (sequence && index < sequence->exerciseList.size()) {
        ++index;
    }
    return *this;
}

bool ExerciseSequence::Iterator::operator!=(const Iterator& other) const {
    return sequence != other.sequence || index != other.index;
}

int ExerciseSequence::Iterator::position() const {
    return index;
}

ExerciseSequence::ExerciseSequence()
    : exerciseList()
{
}

ExerciseSequence::ExerciseSequence(const QList<Exercise*>& exercises)
    : exerciseList(exercises)
{
}

bool ExerciseSequence::isEmpty() const {
    return exerciseList.isEmpty();
}

int ExerciseSequence::size() const {
    return exerciseList.size();
}

QList<Exercise*> ExerciseSequence::toList() const {
    return exerciseList;
}

ExerciseSequence::Iterator ExerciseSequence::begin() const {
    return Iterator(this, 0);
}

ExerciseSequence::Iterator ExerciseSequence::end() const {
    return Iterator(this, exerciseList.size());
}

