#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <unistd.h>

#define SHM_KEY 0x1234          // Shared memory key
#define PLAYER_QUEUE_KEY 0x5678 // Message queue key
#define GAME_QUEUE_KEY 0x9ABC   // Message queue key for responses

#define MAX_BUFFER 100

typedef struct {
    long mesg_type;
    int choice;
} message_buf;

typedef struct {
    int player_type_taken[2];  // 0 for 'X', 1 for 'O'
    char board[9];             // Tic-tac-toe board
    int turn;                  // 0 for 'X's turn, 1 for 'O's turn
} SharedData;

int setup_shared_memory() {
    int shmid = shmget(SHM_KEY, sizeof(SharedData), 0644 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget failed");
        exit(1);
    }
    return shmid;
}

int setup_message_queue(key_t key) {
    int msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget failed");
        exit(1);
    }
    return msgid;
}

void initialize_board(char *board) {
    for (int i = 0; i < 9; i++) {
        board[i] = '1' + i;  // Initialize board with numbers 1-9
    }
}

int check_winner(char *board) {
    // Horizontal, vertical, and diagonal checks
    int wins[8][3] = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, {0, 4, 8}, {2, 4, 6}};
    for (int i = 0; i < 8; i++) {
        if (board[wins[i][0]] == board[wins[i][1]] && board[wins[i][1]] == board[wins[i][2]]) {
            return (board[wins[i][0]] == 'X') ? 1 : 2;
        }
    }
    return 0;  // No winner yet
}

void display_board(char *board) {
    printf("\n");
    for (int i = 0; i < 9; i += 3) {
        printf(" %c | %c | %c \n", board[i], board[i + 1], board[i + 2]);
        if (i < 6) {
            printf("---|---|---\n");
        }
    }
    printf("\n");
}

int main() {
    int shmid = setup_shared_memory();
    int player_queue_id = setup_message_queue(PLAYER_QUEUE_KEY);
    int game_queue_id = setup_message_queue(GAME_QUEUE_KEY);

    SharedData *shared_data = (SharedData *)shmat(shmid, NULL, 0);
    if (shared_data == (void *)-1) {
        perror("Shared memory attach failed");
        exit(1);
    }

    // Initialize game state
    initialize_board(shared_data->board);
    shared_data->turn = 0;  // 'X' starts first

    message_buf rbuf;

    while (1) {
        if (msgrcv(player_queue_id, &rbuf, sizeof(rbuf), 1, 0) > 0) {  // Receive messages of type '1'
            printf("New Move by Player %c: %d\n", (shared_data->turn == 0 ? 'X' : 'O'), rbuf.choice);

            int index = rbuf.choice - 1;

            if (index >= 0 && index < 9 && shared_data->board[index] == '1' + index) {
                shared_data->board[index] = (shared_data->turn == 0 ? 'X' : 'O');
                shared_data->turn = 1 - shared_data->turn;  // Switch turns
                display_board(shared_data->board);

                // Check for winner
                int winner = check_winner(shared_data->board);
                if (winner != 0) {
                    printf("Player %c wins!\n", (winner == 1 ? 'X' : 'O'));
                    break;  // Game over
                } else if (shared_data->board[0] != '1' || shared_data->board[8] != '9') {
                    printf("Game Draw!\n");
                    break;  // Draw
                }
            } else {
                // Invalid move or cell taken
                printf("Invalid move. Try again.\n");
            }
        } else {
            perror("msgrcv failed");
            break;  // Break loop on message queue failure
        }
    }

    // Clean up shared memory and message queue
    shmdt(shared_data);  
    shmctl(shmid, IPC_RMID, NULL);  
    msgctl(player_queue_id, IPC_RMID, NULL); 
    msgctl(game_queue_id, IPC_RMID, NULL);

    return 0;
}
