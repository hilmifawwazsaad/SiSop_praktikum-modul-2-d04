#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <dirent.h>

#define MAX_PATH_LENGTH 100
#define FILENAME_LENGTH 100
#define LINE_BUFFER_SIZE 256
#define MAX_GUESS 4000
#define NUM_HORSES 3
#define MAX_RACES 10
#define HORSE_DATA_PATH "/usr/operating-system/praktikum-modul-2-d04/task-1/horses"

// Define Horse structure
typedef struct {
    char horseName[100];
    char ownerName[100];
    int primeGuess;  // Random number for prime calculation
    int position;    // Position in the race
} Horse;

// Function to check if a number is prime
int is_prime(int num) {
    if (num <= 1) return 0;
    for (int i = 2; i <= num / 2; i++) {
        if (num % i == 0) return 0;
    }
    return 1;
}

// Function to simulate a horse race
void* race(void* arg) {
    Horse* horse = (Horse*)arg;
    horse->primeGuess = rand() % MAX_GUESS + 1;  // Random number for prime calculation
    while (!is_prime(horse->primeGuess)) {  // Keep finding prime numbers
        horse->primeGuess++;
    }
    return NULL;  // End of the race
}

// Function to fetch horse data and save it into files
void HorseFetcher() {
    const char* horseData[NUM_HORSES] = {
        "Morioh:DJumanto",
        "Silver:Smith",
        "Gold:Jones"
    };

    for (int i = 0; i < NUM_HORSES; i++) {
        char filename[FILENAME_LENGTH];
        sprintf(filename, "%s/horse_%d.txt", HORSE_DATA_PATH, i + 1);

        FILE* file = fopen(filename, "w");
        if (file == NULL) {
            fprintf(stderr, "Error opening file: %s\n", filename);
            exit(EXIT_FAILURE);
        }

        fprintf(file, "%s\n", horseData[i]);
        fclose(file);
    }

    printf("Horse data fetched and saved successfully.\n");
}

// Function to list files in a directory
void list_directory(const char* dirname) {
    DIR* dir = opendir(dirname);
    if (dir == NULL) {
        perror("Unable to open directory");
        return;
    }

    struct dirent* entry;
    printf("Files in directory: %s\n", dirname);
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    closedir(dir);
}

// Function to read all lines from horse data files
void readHorseData(const char* directory, int numFiles) {
    for (int i = 0; i < numFiles; i++) {
        char filename[FILENAME_LENGTH];
        sprintf(filename, "%s/horse_%d.txt", directory, i + 1);

        FILE* file = fopen(filename, "r");
        if (file == NULL) {
            fprintf(stderr, "Error opening file: %s\n", filename);
            exit(EXIT_FAILURE);
        }

        printf("Reading from file: %s\n", filename);

        char line[LINE_BUFFER_SIZE];
        while (fgets(line, LINE_BUFFER_SIZE, file) != NULL) {
            printf("%s", line);  // Print each line read from the file
        }

        fclose(file);
    }
}

// Function to run a horse race and save results
void HorseRaceHooray(const char* directory, int raceNumber) {
    Horse horses[NUM_HORSES];
    pthread_t tid[NUM_HORSES];

    // Create threads to simulate the race
    for (int i = 0; i < NUM_HORSES; i++) {
        pthread_create(&tid[i], NULL, race, &horses[i]);
    }

    // Join threads
    for (int i = 0; i < NUM_HORSES; i++) {
        pthread_join(tid[i], NULL);
    }

    // Sort horses by prime guess
    for (int i = 0; i < NUM_HORSES - 1; i++) {
        for (int j = i + 1; j < NUM_HORSES; j++) {
            if (horses[i].primeGuess > horses[j].primeGuess) {
                Horse temp = horses[i];
                horses[i] = horses[j];
                horses[j] = temp;
            }
        }
    }

    // Save race results to a file
    char raceFileName[FILENAME_LENGTH];
    sprintf(raceFileName, "%s/HorseRace_%d.txt", directory, raceNumber);

    FILE* resultFile = fopen(raceFileName, "w");
    if (resultFile == NULL) {
        fprintf(stderr, "Error opening result file: %s\n", raceFileName);
        exit(EXIT_FAILURE);
    }

    time_t now = time(NULL);
    struct tm* t = localtime(&now);

    fprintf(resultFile, "----Horse Race (%d)----\n", raceNumber);
    fprintf(resultFile, "Date: %02d/%02d/%04d\n", t->tm_mday, t->tm_mon + 1, t->tm_year + 1900);
    fprintf(resultFile, "Time: %02d:%02d:%02d\n", t->tm_hour, t->tm_min, t->tm_sec);

    for (int i = 0; i < NUM_HORSES; i++) {
        fprintf(resultFile, "%d. %s %s %d\n", i + 1, horses[i].horseName, horses[i].ownerName, horses[i].primeGuess);
    }

    fclose(resultFile);

    printf("Horse race completed. Check '%s' for results.\n", raceFileName);
}

// Function to display past horse race results
void HorseChampionLeaderboard() {
    printf("Choose your Horse Race history:\n");

    char fileName[FILENAME_LENGTH];
    int foundRaces = 0;

    for (int i = 1; i <= MAX_RACES; i++) {
        sprintf(fileName, "%s/HorseRace_%d.txt", HORSE_DATA_PATH, i);
        FILE* file = fopen(fileName, "r");
        if (file != NULL) {
            printf("%d. Horse Race %d\n", i, i);
            fclose(file);
            foundRaces++;
        }
    }

    if (foundRaces == 0) {
        printf("No Horse Race history found.\n");
        return;
    }

    int raceNumber;
    printf("Enter the number of the Horse Race you want to view: ");
    scanf("%d", &raceNumber);

    sprintf(fileName, "%s/HorseRace_%d.txt", HORSE_DATA_PATH, raceNumber);
    FILE* file = fopen(fileName, "r");

    if (file == NULL) {
        printf("Error opening race file: %s\n", fileName);
        return;
    }

    printf("----HORSE CHAMPIONSHIP (Race %d)----\n", raceNumber);

    char line[256];
    while (fgets(line, sizeof(line), file) != NULL) {
        printf("%s", line);  // Print each line from the race file
    }

    fclose(file);
}

// Main function with a simple menu to interact with the program
int main() {
    int choice;
    srand(time(NULL));  // Seed the random number generator

    do {
        printf("\n-- Welcome to HorseChamp --\n");
        printf("1. Fetch Horse Data\n");
        printf("2. Start Horse Race\n");
        printf("3. Show Winners\n");
        printf("4. Exit\n");  // Add an option to exit
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                HorseFetcher();  // Fetch horse data
                break;
            case 2:
                HorseRaceHooray(HORSE_DATA_PATH, 1);  // Start horse race
                break;
            case 3:
                HorseChampionLeaderboard();  // Show past winners
                break;
            case 4:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 4);  // Exit loop on choice 4

    return 0;
}
