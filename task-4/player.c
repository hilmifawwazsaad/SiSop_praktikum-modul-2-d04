#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <unistd.h>

#define SHM_KEY 0x1234          // Shared memory key
#define PLAYER_QUEUE_KEY 0x5678  // Message queue key (player to game)

#define MAX_BUFFER 100

typedef struct {
    long mesg_type;
    int choice;
} message_buf;

typedef struct {
    int player_type_taken[2]; // 0 for 'X', 1 for 'O'
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
    int msgid = msgget(PLAYER_QUEUE_KEY, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget failed");
        exit(1);
    }
    return msgid;
}

int main() {
    int shmid = setup_shared_memory();
    SharedData *shared_data = (SharedData *)shmat(shmid, NULL, 0);

    int msgid = setup_message_queue();

    // Choose player type
    int player_type = 0;
    printf("Available player types:\n1. X\n2. O\nEnter your choice (1 or 2): ");
    scanf("%d", &player_type);

    if (player_type < 1 || player_type > 2 || shared_data->player_type_taken[player_type - 1] != 0) {
        printf("'%c' has already been taken, choose other player types\n", (player_type == 1 ? 'X' : 'O'));
        return 1;
    }

    shared_data->player_type_taken[player_type - 1] = 1;
    printf("You play as '%c'\n", (player_type == 1 ? 'X' : 'O'));

    // Sending moves to game.c
    message_buf sbuf;
    sbuf.mesg_type = player_type;  // Message type '1' for Player 'X', '2' for Player 'O'

    while (1) {  
        printf("Your turn. Enter a cell number (1-9): ");
        int cell_number;
        scanf("%d", &cell_number);

        sbuf.choice = cell_number;
        msgsnd(msgid, &sbuf, sizeof(sbuf), 0);  // Send move
        
        // You might need additional code here to handle responses from game.c
    }

    // Cleanup
    shmdt(shared_data);  

    return 0;
}
