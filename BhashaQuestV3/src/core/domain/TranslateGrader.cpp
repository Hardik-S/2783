#include "TranslateGrader.h"
#include <algorithm>

Result TranslateGrader::grade(const QString& userAnswer, const Exercise* exercise) {
    // Cast to TranslateExercise to access Translate-specific methods
    const TranslateExercise* translateEx = dynamic_cast<const TranslateExercise*>(exercise);

    if (!translateEx) {
        return Result(false, 0, "Error: Invalid exercise type for TranslateGrader");
    }

    QString trimmedAnswer = userAnswer.trimmed();

    if (trimmedAnswer.isEmpty()) {
        return Result(false, 0, "Please enter a translation");
    }

    // Get all correct answers
    QStringList correctAnswers = translateEx->getCorrectAnswers();

    // Check for fuzzy match against any correct answer
    bool correct = fuzzyMatch(trimmedAnswer, correctAnswers);

    if (correct) {
        return Result(true, 10, QString("Correct! Well done!"));
    } else {
        QString correctAnswer = translateEx->getCorrectAnswer();
        return Result(false, 0, QString("Incorrect. Correct answer: %1").arg(correctAnswer));
    }
}

bool TranslateGrader::fuzzyMatch(const QString& userAnswer, const QStringList& correctAnswers) const {
    for (const QString& correctAnswer : correctAnswers) {
        // Exact match (case-sensitive for native scripts)
        if (userAnswer == correctAnswer) {
            return true;
        }

        // Case-insensitive match (for romanization)
        if (userAnswer.toLower() == correctAnswer.toLower()) {
            return true;
        }

        // Fuzzy match with Levenshtein distance <= 2 (allow minor typos)
        int distance = levenshteinDistance(userAnswer.toLower(), correctAnswer.toLower());
        if (distance <= 2) {
            return true;
        }
    }

    return false;
}

int TranslateGrader::levenshteinDistance(const QString& s1, const QString& s2) const {
    int len1 = s1.length();
    int len2 = s2.length();

    // Create distance matrix
    std::vector<std::vector<int>> matrix(len1 + 1, std::vector<int>(len2 + 1));

    // Initialize first row and column
    for (int i = 0; i <= len1; ++i) {
        matrix[i][0] = i;
    }
    for (int j = 0; j <= len2; ++j) {
        matrix[0][j] = j;
    }

    // Calculate distances
    for (int i = 1; i <= len1; ++i) {
        for (int j = 1; j <= len2; ++j) {
            int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;

            matrix[i][j] = std::min({
                matrix[i - 1][j] + 1,      // Deletion
                matrix[i][j - 1] + 1,      // Insertion
                matrix[i - 1][j - 1] + cost // Substitution
            });
        }
    }

    return matrix[len1][len2];
}
