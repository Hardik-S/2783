#ifndef CHARACTERUTILS_H
#define CHARACTERUTILS_H

#include <QString>
#include <QStringList>

/**
 * CharacterUtils - Utility functions for character selection exercises
 *
 * Provides functions for:
 * - Generating character banks (n+3 characters) for selection exercises
 * - Extracting unique characters from strings
 * - Managing language-specific character sets
 */
class CharacterUtils {
public:
    /**
     * Generate a character bank for character selection exercise
     *
     * Takes the correct answer and generates a shuffled list of n+3 characters
     * where n is the number of unique characters in the correct answer.
     *
     * @param correctAnswer The correct translation/answer string
     * @param languageCharacterSet Available characters for the target language
     * @return Shuffled QStringList containing n+3 characters (each char as separate string)
     */
    static QStringList generateCharacterBank(
        const QString& correctAnswer,
        const QStringList& languageCharacterSet
    );

    /**
     * Extract unique characters from a string
     *
     * Breaks down a string into individual Unicode characters and removes duplicates.
     *
     * @param text Input string to extract characters from
     * @return QStringList of unique characters in order of appearance
     */
    static QStringList extractUniqueCharacters(const QString& text);

    /**
     * Extract all characters from a string (including duplicates)
     *
     * Breaks down a string into individual Unicode characters.
     *
     * @param text Input string to extract characters from
     * @return QStringList of all characters
     */
    static QStringList extractAllCharacters(const QString& text);

    /**
     * Get random characters not in the source set
     *
     * Generates n random distractor characters from the language character set
     * that are NOT in the source string.
     *
     * @param sourceText Text to avoid characters from
     * @param languageCharacterSet Available characters for selection
     * @param count Number of random characters to generate
     * @return QStringList of random distractor characters
     */
    static QStringList getRandomDistractors(
        const QString& sourceText,
        const QStringList& languageCharacterSet,
        int count
    );
};

#endif // CHARACTERUTILS_H
