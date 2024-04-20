#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

#define SHM_KEY 1234
#define MSG_KEY 5678

#define MAX_BUFFER 100

typedef struct {
    long mesg_type;
    int choice;
} message_buf;

typedef struct {
    int player_type_taken[2]; // 0 for X, 1 for O
    char board[9];            // Tic-tac-toe board
    int turn;                 // 0 for X's turn, 1 for O's turn
} SharedData;

int setup_shared_memory() {
    int shmid = shmget(SHM_KEY, sizeof(SharedData), 0644 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget failed");
        exit(1);
    }
    return shmid;
}

int setup_message_queue() {
    int msgid = msgget(MSG_KEY, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget failed");
        exit(1);
    }
    return msgid;
}

void initialize_board(SharedData *data) {
    for (int i = 0; i < 9; i++) {
        data->board[i] = '1' + i;  // Initialize board with numbers 1-9
    }
    data->turn = 0;  // X starts first
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
        printf(" %c | %c | %c \n", board[i], board[i+1], board[i+2]);
        if (i < 6) {
            printf("---|---|---\n");
        }
    }
    printf("\n");
}

int main() {
    int shmid = setup_shared_memory();
    int msgid = setup_message_queue();
    SharedData *data = (SharedData *)shmat(shmid, NULL, 0);
    if (data == (void *)-1) {
        perror("Shared memory attach");
        shmctl(shmid, IPC_RMID, NULL); // Clean up shared memory on failure
        msgctl(msgid, IPC_RMID, NULL); // Clean up message queue on failure
        exit(1);
    }

    memset(data->player_type_taken, 0, sizeof(data->player_type_taken));
    initialize_board(data);

    message_buf rbuf;
    int winner = 0;
    int moves = 0;

    printf("Game started!\n");
    display_board(data->board);

    while (winner == 0 && moves < 9) {
        if (msgrcv(msgid, &rbuf, sizeof(rbuf), 0, 0) > 0) {
            printf("New Move by Player %c: %d\n", (rbuf.mesg_type == 1 ? 'X' : 'O'), rbuf.choice);

            int index = rbuf.choice - 1;
            if (index >= 0 && index < 9 && data->board[index] == '1' + index) {
                data->board[index] = (rbuf.mesg_type == 1 ? 'X' : 'O');
                moves++;
                display_board(data->board);
                winner = check_winner(data->board);
                if (winner != 0) {
                    printf("Player %c wins!\n", (winner == 1 ? 'X' : 'O'));
                } else if (moves == 9) {
                    printf("Game Draw!\n");
                }
                data->turn = 1 - data->turn;
            } else {
                printf("Invalid move. Try again.\n");
            }
        }
    }

    // Clean up
    shmdt(data);                        // Detach from shared memory
    shmctl(shmid, IPC_RMID, NULL);      // Remove shared memory
    msgctl(msgid, IPC_RMID, NULL);      // Remove message queue
    return 0;
}

#define SOCKET_PATH "/tmp/tictactoe_socket"

void setup_server_socket(int *server_fd) {
    struct sockaddr_un addr;

    if ((*server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket error");
        exit(-1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);
    unlink(SOCKET_PATH);

    if (bind(*server_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind error");
        exit(-1);
    }

    if (listen(*server_fd, 5) == -1) {
        perror("listen error");
        exit(-1);
    }
}

void accept_client(int server_fd, int *client_fd) {
    if ((*client_fd = accept(server_fd, NULL, NULL)) == -1) {
        perror("accept error");
        exit(-1);
    }
}