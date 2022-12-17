#include <stdio.h>    // for printf(), scanf()
#include <stdlib.h>   // for exit( -1)
#include <string.h>   // for strcpy
#include <assert.h>   // for assert() sanity checks
#include <ctype.h>    // for toupper()
#include <time.h>     // for time()

// Declare globals
#define WORD_LENGTH 5     // All words have 5 letters, + 1 NULL at the end when stored
//#define WORDS_FILE_NAME "wordsLarge.txt"
#define WORDS_FILE_NAME  "wordsLarge.txt"
#define MAX_NUMBER_OF_WORDS 12947   // Number of words in the full set of words file
#define true 1   // Make boolean logic easier to understand
#define false 0  // Make boolean logic easier to understand


typedef struct wordCount wordCountStruct;
struct wordCount{
    char word[ WORD_LENGTH + 1];   // The word length plus NULL
    int score;                     // Score for the word
};


/* Comparator for use in built-in qsort(..) function.  Parameters are declared to be a
// generic type, so they will match with anything.
// This is a two-part comparison.  First the scores are compared.  If they are the same,
// then the words themselves are also compared, so that the results are in descending
// order by score, and within score they are in alphabetic order*/
int compareFunction( const void * a, const void * b) {
    // Before using parameters we have cast them into the actual type they are in our program
    // and then extract the numerical value used in comparison
    int firstScore = ((wordCountStruct *) a)->score;
    int secondScore = ((wordCountStruct *) b)->score;

    // If scores are different, then that's all we need for our comparison.
    if (firstScore != secondScore) {
        // We reverse the values, so the result is in descending vs. the otherwise ascending order
        // return firstScore - secondScore;   // ascending order
        return secondScore - firstScore;      // descending order
    }
    else {
        // Scores are equal, so check words themselves, to put them in alphabetical order
        return strcmp( ((wordCountStruct *)a)->word,  ((wordCountStruct *)b)->word );
    }
}


//Helper function to check if the word is in the input file => DONE
int checkWord(char word[], wordCountStruct *words, int wordCount){

    size_t i;
    for(i = 0; i < wordCount; i++){
        if(strcmp(word, words[i].word) == 0){
            return 1;
        }
    }

    //printf("The word is not in the file. Please try again.\n");
    return 0;
}


//Helper function to generate the score of each word => DONE
void generateWordScore(wordCountStruct* allWords, const int* wordCount) {

    //26 letters in the alphabet
    int letter[26];
   //Popular english letters
    char popularLetters[5] = {'e', 't', 'a', 'o', 'i'};
    int i = 0;
    int j = 0;

    if(checkWord(allWords[i].word, allWords, *wordCount) == 1){
        for(j = 0; j < 26; j++){
            letter[j] = 0;
        }
        for(j = 0; j < 5; j++){
            letter[allWords[i].word[j] - 'a']++;
        }
        for(j = 0; j < 26; j++){
            allWords[i].score += letter[j] * letter[j];
        }
    }
    else{
        allWords[i].score = 0;
        printf("The word is not in the file.\n");
    }
}


//Helper function to print the words and their scores => DONE
void wordleOutput(char* computerGuess, const int result[]){

    int j = 0;
    while(j < 5){
        if (result[j] == 2){
            printf("%c ", computerGuess[j] - ('A' - true)/2);
        }
        else{
            printf("%c ", computerGuess[j]);
        }

        j++;
    }

    printf("\n");
    printf("       ");

    //Separate loop for displaying the underlying asterisks
    int n = 0;
    while(n < 5){
        if(result[n] == true){
            printf("* ");
        }
        else{
            printf("  ");
        }
        n++;
    }
    printf("\n");
}


//Helper function to check if a word contains the letter and remove this word from the search if the condition is false => FIXME
void removeLetters(
        char target,
        char choice,
        wordCountStruct* allWords,
        int* wordCount,
        wordCountStruct* finalWord,
        int pos
) {

    //Step 1: create a copy of the original array
    wordCountStruct* words = allWords;
    int count = 0;
    finalWord[count++] = words[0];

    //Step 2: check if the word contains the letter
    int i = 1;
    do{
        int signal = 0;
        for(int j = 0; j < WORD_LENGTH; j++){
            //Check if the match occurred
            if(target == words[i].word[j]){
                signal = 1;
                break;
            }
            if(target == words[i].word[j]){
                signal = 0;
                break;
            }
        }

        //Step 3: remove the word from the search
        switch(choice){
            case 'r':
                if(signal == 1){
                    finalWord[count++] = words[i];
                }
                break;
            case 'i':
                if(signal == 0){
                    finalWord[count++] = words[i];
                }
                break;
            case 'p':
                if (target == words[i].word[pos]) {
                    finalWord[count++] = words[i];
                }
                break;
            default:
                break;
        }
        i++;
    }while(i < *wordCount);

    *wordCount = count;
}


/* Read in words from file into array.  We've previously read the data file once to
   find out how many words are in the file.*/
void readWordsFromFile(
        char fileName[],        // Filename we'll read from
        wordCountStruct *words, // Array of words where we'll store words we read from file
        int *wordCount)          // How many words.  Gets updated here and returned
{
    FILE *inFilePtr  = fopen(fileName, "r");  // Connect logical name to filename
    assert( inFilePtr != NULL);               // Ensure file open worked correctly

    // Read each word from file and store into array, initializing the score for that word to 0.
    char inputString[ 6];
    *wordCount = 0;
    while( fscanf( inFilePtr, "%s", inputString) != EOF) {
        strcpy( words[ *wordCount].word, inputString);
        words[ *wordCount].score = 0;
        (*wordCount)++;
    }

    // Close the file
    fclose( inFilePtr);
}


//Helper function to remove letters from the copy of the final guess => DONE
void eraser(int choice, wordCountStruct* clean, wordCountStruct* finalGuess, int wordCount, char computerGuess[], int result[], int index){

    //Step 1: create a copy of the original array
    wordCountStruct* words = clean;
    int count = 0;
    finalGuess[count++] = words[0];

    //Step 2: remove the word from the search
    int i = 1;
    do{
        switch(choice){
            case 1:
                if(words[i].word[index] == computerGuess[index]){
                    finalGuess[count++] = words[i];
                }
                break;
            case 2:
                if(words[i].word[index] != computerGuess[index]){
                    finalGuess[count++] = words[i];
                }
                break;
            case 3:
                if(words[i].word[index] == computerGuess[index]){
                    finalGuess[count++] = words[i];
                }
                break;
            default:
                break;
        }
        i++;
    }while(i < wordCount);

    if(choice == 0){
        removeLetters(computerGuess[index], 'i', clean, &wordCount, finalGuess, index);
    }
    else if(choice == 1) {
        removeLetters(computerGuess[index], 'r', clean, &wordCount, finalGuess, index);
    }
    else{
        removeLetters(computerGuess[index], 'p', clean, &wordCount, finalGuess, index);
    }

}


// Find a secret word
void findSecretWord(
        wordCountStruct allWords[],    // Array of all the words
        int wordCount,                  // How many words there are in allWords
        char secretWord[])              // The word to be guessed
{
    char computerGuess[ 6];  // Allocate space for the computer guess
    char computerGuessCopy[ 6];
    char secretWordCopy[ 6];
    strcpy( computerGuessCopy, computerGuess);
    strcpy( secretWordCopy, secretWord);
    wordCountStruct* guessedWord = allWords;
    char alphabet[26];
    for(int i = 0; i < 26; i++){
        alphabet[i] = 'a' + i;
    }
    char popularLetters[ 26] = {'e', 't', 'a', 'o', 'i', 'n', 's'};
    char addGuess[ 6];

    //Pick a random letter from the alphabet and assign it to the computer guess. Then check if the letter is in the secret word.
    int randomLetter;

    printf("Trying to find secret word: \n");
    // Display secret word with a space between letters, to match the guess words below.
    printf("       ");
    for( int i = 0; i < WORD_LENGTH; i++) {
        printf("%c ", secretWord[ i]);
    }
    printf("\n");
    printf("\n");

    // Loop until the word is found
    int guessNumber = 1;
    while( true) {
        if (checkWord(secretWordCopy, guessedWord, wordCount) != 0) {

            strcpy(computerGuess, guessedWord[0].word);
            int result[WORD_LENGTH];

            int i = 0;
            while(i < WORD_LENGTH){
                
                int choice;
                char target = computerGuess[i];
                
                // Display the computer guess
                for(int i = 0; i < WORD_LENGTH; i++){
                    //printf("%c ", computerGuessCopy[i]);
                    if(computerGuessCopy[i] != secretWordCopy[i]){
                        //printf("%c ", computerGuessCopy[i]);
                        continue;
                    }
                    else{
                        //printf("%c ", toupper(computerGuessCopy[i]));
                        addGuess[i] = computerGuessCopy[i]; //FIXME
                        //printf("FINAL GUESS IS: %s", finalGuess);
                        computerGuessCopy[ i] = computerGuessCopy[ i];
                        computerGuessCopy[i] = ' ';
                        secretWordCopy[i] = ' ';
                        if((secretWord)){
                            if(addGuess[i] == secretWord[i]) {
                                //printf("FINAL GUESS IS: %s", finalGuess);
                                break;
                            }
                        }
                    }
                }

                if(target == secretWord[i]){
                    choice = 2;
                }
                else{
                    choice = 0;
                    for(int j = 0; j < 5; j++){
                        if(target == secretWord[j]){
                            choice = 1;
                            break;
                        }
                    }
                }

                //Allocate space for the final guess
                wordCountStruct *finalGuess = (wordCountStruct *) calloc(wordCount, sizeof(wordCountStruct));
                result[i] = choice;

                //Call the helper function to remove letters from the copy of the final guess
                eraser(choice, guessedWord, finalGuess, wordCount, computerGuess, result, i);
                guessedWord = finalGuess;
                i++;
            }

            printf("%5d. ", guessNumber);

            //Check if duplicate letters are in the secret word
            int duplicate;
            for(int i = 0; i < WORD_LENGTH; i++){
                for(int j = 0; j < WORD_LENGTH; j++){
                    if(computerGuessCopy[i] == secretWordCopy[j]){
                        duplicate = 1;
                        //If there are duplicate letters, check if both of them are in the secret word at any position
                        if(duplicate == 1){
                            for(int i = 0; i < WORD_LENGTH; i++){
                                for(int j = 0; j < WORD_LENGTH; j++){
                                    if(computerGuessCopy[i] == secretWordCopy[j]){
                                        computerGuessCopy[ i] = computerGuessCopy[ i];
                                        //computerGuessCopy[i] = ' ';
                                        secretWordCopy[j] = ' ';
                                    }
                                }
                            }
                        }
                    }
                }
            }

            //Display the final result after cleaning a word
            wordleOutput(computerGuess, result);

            //Shift the guessed letter to the matching position in the secret word
            for(int i = 0; i < WORD_LENGTH; i++){
                if(computerGuessCopy[i] == secretWordCopy[i]){
                    computerGuessCopy[i] = secretWordCopy[i];
                }
                else{
                    computerGuessCopy[ i] = computerGuessCopy[ i];
                }
            }
            if (strcmp(computerGuess, secretWord) != 0) {
                wordCountStruct *cleanedWord = (wordCountStruct *) calloc(wordCount - 1, sizeof(wordCountStruct));
                if (cleanedWord == NULL) {
                    exit(-1);
                }
                else{
                    for (int i = 1; i < wordCount; i++) {
                        cleanedWord[i - 1] = guessedWord[i];
                    }
                    wordCount--;
                }

                guessedWord = cleanedWord;
            }
            else{

                printf("Got it!");
                printf("\n");
                break;
            }

            // Update guess number
            guessNumber++;
        }
        else{
            printf("The chosen word is not in the dictionary. Please, try again!\n");
            exit(-1);
        }
    }
}



int main() {
    char wordsFileName[81];                   // Stores the answers file name
    strcpy(wordsFileName, WORDS_FILE_NAME);   // Set the filename, defined at top of program.
    srand( (unsigned) time( NULL));           // Seed the random number generator to be current time
    // Declare space for all the words, of a maximum known size.
    wordCountStruct* allWords = (wordCountStruct*)calloc(MAX_NUMBER_OF_WORDS,sizeof(wordCountStruct));
    // Start out the wordCount to be the full number of words.  This will decrease as
    //    play progresses each time through the game.
    int wordCount = 0;
    // The secret word that the computer will try to find, plus the return character from fgets.
    char secretWord[ WORD_LENGTH + 1];

    // Read in words from file, update wordCount and display information
    readWordsFromFile( wordsFileName, allWords, &wordCount);
    printf("Using file %s with %d words. \n", wordsFileName, wordCount);
    generateWordScore(allWords, &wordCount);

    // Run the word-guessing game three times
    for( int i=0; i<3; i++) {
        //Reset secret Word
        strcpy( secretWord, "");
        // Display prompt
        printf("-----------------------------------------------------------\n");
        printf("\n");
        char userInput[81] = { 0 };                // Used for menu input of secret word
        printf("Enter a secret word or just r to choose one at random: ");
        scanf(" %s", userInput);
        // Eliminate the return character at end or userInput if it is there
        int length = (int) strlen( userInput);
        if( userInput[ length] == '\n') {
            userInput[ length] = '\0';
        }
        strcpy( secretWord, userInput);   // Store the secret word from user input

        // If input was 'r' then choose a word at random.
        if( strlen( secretWord) <= 1) {
            // Randomly select a secret word to be guessed.
            int randomIndex = rand() % wordCount;
            strcpy( secretWord, allWords[ randomIndex].word);
        }
        // Run the game once with the current secret word
        findSecretWord( allWords, wordCount, secretWord);
    }

    printf("Done\n");
    printf("\n");
    return 0;
}
