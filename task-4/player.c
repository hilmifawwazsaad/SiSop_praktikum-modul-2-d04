#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/types.h>

#define SHM_KEY 1234
#define MSG_KEY 5678


typedef struct {
    long mesg_type;
    int choice;
} message_buf;

typedef struct {
    int player_type_taken[2]; // 0 for X, 1 for O
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

int main() {
    int shmid = setup_shared_memory();
    int msgid = setup_message_queue();
    SharedData *data = (SharedData *)shmat(shmid, NULL, 0);

    if (data == (void *)-1) {
        perror("Shared memory attach");
        exit(1);
    }

    int player_type = 0;
    printf("Available player types:\n1. X\n2. O\nEnter your choice (1 or 2): ");
    scanf("%d", &player_type);

    if (player_type < 1 || player_type > 2 || data->player_type_taken[player_type-1] != 0) {
        printf("'X' or 'O' has already been taken, choose other player types\n");
        exit(0);
    } else {
        data->player_type_taken[player_type-1] = 1;
        printf("You play as '%c'\n", (player_type == 1 ? 'X' : 'O'));
    }

    int cell_number;
    printf("Enter a cell number (1-9): ");
    scanf("%d", &cell_number);

    message_buf sbuf;
    sbuf.mesg_type = player_type;  // Player X or O
    sbuf.choice = cell_number;

    msgsnd(msgid, &sbuf, sizeof(sbuf), 0);

    shmdt(data);
    return 0;
}
