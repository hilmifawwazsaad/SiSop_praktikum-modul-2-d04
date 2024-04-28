[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/o3jj1gXA)
<div align=center>

|    NRP     |      Name      |
| :--------: | :------------: |
| 5025221000 | Student 1 Name |
| 5025221047 | Muhammad Rayyaan Fatikhahur Rakhim |
| 5025221103 | Hilmi Fawwaz Sa'ad |

# Praktikum Modul 2 _(Module 2 Lab Work)_

</div>

### Daftar Soal _(Task List)_

- [Task 1 - HORSECHAMP](/task-1/)

- [Task 2 - Tugas Akhir Yuan dan Bubu _(Yuan's and Bubu's Final Project)_](/task-2/)

- [Task 3 - Ramalan Cuaca Rama _(Rama's Weather Forecast)_](/task-3/)

- [Task 4 - Tic Tac Toe](/task-4/)

### Laporan Resmi Praktikum Modul 2 _(Module 2 Lab Work Report)_

Tulis laporan resmi di sini!

_Write your lab work report here!_
## 1️⃣ Soal 1
DJumanto adalah seorang pengembang hebat yang telah menciptakan banyak kode keren. Saat ini, ia ingin membuat sebuah proyek baru yang disebut "horsechamp", sebuah program untuk lomba balapan kuda bernama horsechamp.c. Program ini memiliki 4 fungsi utama, yaitu HorseFetcher, HorseRaceHooray, HorseChampionLeaderboard, dan Main.
```bash
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

#define MAX_HORSES 120
#define MAX_PRIME 4000
#define RACE_NUMBER 1

void HorseFetcher() {
    int result = system("wget 'https://docs.google.com/uc?export=download&id=1gJkoTbiHZcJ1M36vqZPTrj3qTQkRMElQ' -O horses.zip");
    if (result != 0) {
        fprintf(stderr, "Download failed.\n");
        return;
    }

    result = system("unzip -o horses.zip -d horse_files");
    if (result != 0) {
        fprintf(stderr, "Unzip failed.\n");
        return;
    }
}

typedef struct {
    char horse_name[20];
    char owner[20];
    int prime_number;
} HorseInfo;

bool is_prime(int n) {
    if (n <= 1)
        return false;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0)
            return false;
    }
    return true;
}

void randomize_array(HorseInfo *arr, int count) {
    srand(time(NULL));
    for (int i = count - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        HorseInfo temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

void *race_thread(void *arg) {
    HorseInfo *horse = (HorseInfo *)arg;
    int prime_value = 0;
    while (prime_value == 0) {
        int rand_num = rand() % MAX_PRIME + 1;
        if (is_prime(rand_num)) {
            prime_value = rand_num;
        }
    }
    usleep(1000); // Simulate delay
    horse->prime_number = prime_value;
    return NULL;
}

void DisplayHorseFiles() {
    printf("-----------------------------------------------------------------------\n");
    printf("Available Horse Data Files:\n");

    DIR *dir = opendir("horse_files");
    if (dir == NULL) {
        perror("Failed to open 'horse_files'");
        return;
    }

    struct dirent *entry;
    int count = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, "horse_") != NULL) {
            count++;
            printf("%d. %s\n", count, entry->d_name);
        }
    }

    closedir(dir);
}

void HorseRaceHooray() {
    DisplayHorseFiles();
    int choice;
    printf("Choose a number: ");
    if (scanf("%d", &choice) != 1 || choice < 1) {
        printf("Invalid choice. Try again.\n");
        return;
    }

    char selected_horse[50];
    snprintf(selected_horse, sizeof(selected_horse), "horse_%d", choice);

    char horse_path[100];
    snprintf(horse_path, sizeof(horse_path), "horse_files/%s.txt", selected_horse);

    FILE *file = fopen(horse_path, "r");
    if (file == NULL) {
        perror("File open error");
        return;
    }

    HorseInfo horses[MAX_HORSES];
    int horse_count = 0;
    while (horse_count < MAX_HORSES && fscanf(file, "%[^:]:%s\n", horses[horse_count].horse_name, horses[horse_count].owner) == 2) {
        horse_count++;
    }
    fclose(file);

    randomize_array(horses, horse_count);

    pthread_t threads[horse_count];
    for (int i = 0; i < horse_count; i++) {
        if (pthread_create(&threads[i], NULL, race_thread, (void *)&horses[i]) != 0) {
            perror("Thread creation error");
            return;
        }
    }

    for (int i = 0; i < horse_count; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Thread join error");
            return;
        }
    }

    char result_filename[200];
    int race_no = RACE_NUMBER;

    while (true) {
        snprintf(result_filename, sizeof(result_filename), "HorseRace_%d.txt", race_no);
        FILE *check_file = fopen(result_filename, "r");
        if (check_file == NULL) {
            break;
        }
        fclose(check_file);
        race_no++;
    }

    FILE *output_file = fopen(result_filename, "w");
    if (output_file == NULL) {
        perror("Output file error");
        return;
    }

    time_t current_time = time(NULL);
    struct tm *local_time = localtime(&current_time);

    fprintf(output_file, "----Horse Race %d------\n", race_no);
    fprintf(output_file, "Date: %02d/%02d/%04d\n", local_time->tm_mday, local_time->tm_mon + 1, local_time->tm_year + 1900);
    fprintf(output_file, "Time: %02d:%02d:%02d\n\n", local_time->tm_hour, local_time->tm_min, local_time->tm_sec);

    for (int i = 0; i < horse_count; i++) {
        fprintf(output_file, "%d. %s %s %d\n", i + 1, horses[i].horse_name, horses[i].owner, horses[i].prime_number);
    }

    fclose(output_file);
    printf("Horse race completed, results stored in: %s\n", result_filename);
}

void HorseChampionLeaderboard(const char *directory) {
    int choice;
    printf("Select Horse Race History:\n");

    DIR *dir = opendir(directory);
    if (dir == NULL) {
        perror("Failed to open directory.\n");
        return;
    }

    struct dirent *entry;
    int history_count = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, "HorseRace_") != NULL) {
            history_count++;
            printf("%d. %s\n", history_count, entry->d_name);
        }
    }

    if (history_count == 0) {
        printf("No history found.\n");
        closedir(dir);
        return;
    }

    printf("Enter number: ");
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > history_count) {
        printf("Invalid choice.\n");
        closedir(dir);
        return;
    }

    closedir(dir);

    char file_path[100];
    snprintf(file_path, sizeof(file_path), "%s/HorseRace_%d.txt", directory, choice);

    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        printf("Failed to open file.\n");
        return;
    }

    printf("-----------------------------------------------------------------------\n");
    printf("\n----HORSE CHAMPIONSHIP (%d)----\n", choice);

    bool skip_first = true;
    char line[500];
    while (fgets(line, sizeof(line), file)) {
        if (skip_first) {
            skip_first = false;
            continue;
        }
        printf("%s", line);
    }

    fclose(file);
}

int main() {
    int choice;
    do {
        printf("-----------------------------------------------------------------------\n");
        printf("Main Menu:\n");
        printf("1. Download Horse Data\n");
        printf("2. Begin Horse Race\n");
        printf("3. View Winners\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                HorseFetcher();
                if (access("horses.zip", F_OK) != -1) {
                    printf("Horse data downloaded successfully.\n");
                } else {
                    printf("Failed to download horse data.\n");
                }
                break;
            case 2:
                HorseRaceHooray();
                break;
            case 3:
                HorseChampionLeaderboard(".");
                break;
            case 4:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 4);

    return 0;
}
```



### Problem 1a
Buatlah fungsi HorseFetcher, yang bertugas mengambil data kuda dari internet dan menyimpannya dalam file dengan nama horse_1.txt, horse_2.txt, horse_3.txt, dan seterusnya. Kuda dan pemilik kuda dipisahkan dengan “:”, sebagai contoh “Morioh:DJumanto”, DJumanto adalah pemilik dari kuda Morioh.

**Jawab**
```bash
void HorseFetcher() {
    int result = system("wget 'https://docs.google.com/uc?export=download&id=1gJkoTbiHZcJ1M36vqZPTrj3qTQkRMElQ' -O horses.zip");
    if (result != 0) {
        fprintf(stderr, "Download failed.\n");
        return;
    }

    result = system("unzip -o horses.zip -d horse_files");
    if (result != 0) {
        fprintf(stderr, "Unzip failed.\n");
        return;
    }
}
```
HorseFetcher adalah fungsi yang mengunduh dan mengekstrak data kuda dari Google Drive menggunakan wget dan unzip. Jika ada kesalahan saat mengunduh atau mengekstrak, pesan kesalahan akan ditampilkan.

- Perintah Pengunduhan
```bash
int result = system("wget 'https://docs.google.com/uc?export=download&id=1gJkoTbiHZcJ1M36vqZPTrj3qTQkRMElQ' -O horses.zip");
```
system menjalankan perintah shell di dalam kode C. Di sini, fungsi wget digunakan untuk mengunduh file dari Google Drive. Opsi -O horses.zip mengarahkan output ke file bernama horses.zip.
Hasil pengunduhan disimpan dalam variabel result. Jika result bernilai selain 0, itu menandakan bahwa perintah gagal.

- Pemeriksaan Kesalahan Pengunduhan
```bash
if (result != 0) {
    fprintf(stderr, "Download failed.\n");
    return;
}
```
Pemeriksaan apakah perintah wget berhasil. Jika result bukan 0, berarti terjadi kesalahan saat mengunduh file. Dalam hal ini, fungsi menulis pesan kesalahan ke stderr dan keluar dari fungsi (return).

- Bagian Perintah Ekstraksi
```bash
result = system("unzip -o horses.zip -d horse_files");
```
Setelah unduhan berhasil, perintah unzip digunakan untuk mengekstrak file horses.zip ke direktori horse_files. Opsi -o digunakan untuk mengekstrak dan menimpa file yang ada jika diperlukan.

- Bagian Perintah Ekstraksi
```bash
if (result != 0) {
    fprintf(stderr, "Unzip failed.\n");
    return;
}
```
Pemeriksaan apakah perintah unzip berhasil. Jika result bukan 0, berarti ada kesalahan saat mengekstrak file. Dalam hal ini, fungsi menulis pesan kesalahan ke stderr dan mengakhiri eksekusi dengan return.


> [Format : Penjelasan (sesuaikan dengan style modul 1), Dokumentasi]

### Problem 1b
Buatlah fungsi HorseRaceHooray, yang bertugas melakschildan balapan kuda. Parameter yang diguakan adalah salah satu file kuda yang telah kalian fetch sebelumnya. Gunakan thread untuk menjalankan perlombaan tersebut. Setiap kuda akan melakukan perhitungan bilangan prima dengan angka acak antara 1 sampai 4000. Kuda yang menyelesaikan perhitungan lebih awal akan menempati posisi lebih tinggi dari kuda lainnya. Setelah perlombaan selesai, simpan hasil lomba dalam format HorseRace_(nomor lomba).txt.

- Fungsi HorseRaceHooray
```bash
void HorseRaceHooray() {
    DisplayHorseFiles();
    int choice;
    printf("Choose a number: ");
    if (scanf("%d", &choice) != 1 || choice < 1) {
        printf("Invalid choice. Try again.\n");
        return;
    }

    char selected_horse[50];
    snprintf(selected_horse, sizeof(selected_horse), "horse_%d", choice);

    char horse_path[100];
    snprintf(horse_path, sizeof(horse_path), "horse_files/%s.txt", selected_horse);

    FILE *file = fopen(horse_path, "r");
    if (file == NULL) {
        perror("File open error");
        return;
    }

    HorseInfo horses[MAX_HORSES];
    int horse_count = 0;
    while (horse_count < MAX_HORSES && fscanf(file, "%[^:]:%s\n", horses[horse_count].horse_name, horses[horse_count].owner) == 2) {
        horse_count++;
    }
    fclose(file);

    randomize_array(horses, horse_count);

    pthread_t threads[horse_count];
    for (int i = 0; i < horse_count; i++) {
        if (pthread_create(&threads[i], NULL, race_thread, (void *)&horses[i]) != 0) {
            perror("Thread creation error");
            return;
        }
    }

    for (int i = 0; i < horse_count; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Thread join error");
            return;
        }
    }

    char result_filename[200];
    int race_no = RACE_NUMBER;

    while (true) {
        snprintf(result_filename, sizeof(result_filename), "HorseRace_%d.txt", race_no);
        FILE *check_file = fopen(result_filename, "r");
        if (check_file == NULL) {
            break;
        }
        fclose(check_file);
        race_no++;
    }

    FILE *output_file = fopen(result_filename, "w");
    if (output_file == NULL) {
        perror("Output file error");
        return;
    }

    time_t current_time = time(NULL);
    struct tm *local_time = localtime(&current_time);

    fprintf(output_file, "----Horse Race %d------\n", race_no);
    fprintf(output_file, "Date: %02d/%02d/%04d\n", local_time->tm_mday, local_time->tm_mon + 1, local_time->tm_year + 1900);
    fprintf(output_file, "Time: %02d:%02d:%02d\n\n", local_time->tm_hour, local_time's minute, local_time's second);

    for (int i = 0; i < horse_count; i++) {
        fprintf(output_file, "%d. %s %s %d\n", i + 1, horses[i].horse_name, horses[i].owner, horses[i].prime_number);
    }

    fclose(output_file);
    printf("Horse race completed, results stored in: %s\n", result_filename);
}
```
HorseRaceHooray adalah fungsi untuk memulai perlombaan kuda. Pertama, ia menampilkan file data kuda yang tersedia dan meminta pengguna untuk memilih salah satunya. Jika tidak valid, ia menampilkan pesan kesalahan. Setelah valid, ia membuka file dan membaca informasi kuda ke dalam array HorseInfo. Setelah itu, ia mengacak array dan membuat thread untuk setiap kuda, menjalankan fungsi race_thread. Setelah semua thread selesai, ia menyimpan hasil perlombaan ke dalam file dan menampilkan pesan bahwa perlombaan selesai.

- Bagian Meminta user memilih kuda
```bash
    DisplayHorseFiles();  // Menampilkan daftar file kuda yang tersedia
    int choice;
    printf("Choose a number: ");  // Meminta pengguna memilih file data kuda
    if (scanf("%d", &choice) != 1 || choice < 1) {
        printf("Invalid choice. Try again.\n");  // Validasi input pengguna
        return;  // Keluar dari fungsi jika input tidak valid
    }
```
Kode ini menampilkan file kuda yang tersedia menggunakan DisplayHorseFiles. Kemudian, meminta pengguna memilih nomor file kuda dengan scanf. Jika input tidak valid (mis. bukan angka atau kurang dari 1), program memberikan pesan kesalahan dan keluar dari fungsi.

- Membuka File Data Kuda dan Membaca Isinya
```bash
    char selected_horse[50];
    snprintf(selected_horse, sizeof(selected_horse), "horse_%d", choice);  // Menentukan nama file berdasarkan pilihan pengguna

    char horse_path[100];
    snprintf(horse_path, sizeof(horse_path), "horse_files/%s.txt", selected_horse);  // Membuat path file data kuda

    FILE *file = fopen(horse_path, "r");  // Membuka file data kuda
    if (file == NULL) {
        perror("File open error");  // Jika file tidak ditemukan atau tidak dapat dibuka
        return;
    }

    HorseInfo horses[MAX_HORSES];
    int horse_count = 0;  // Menghitung jumlah kuda yang ada dalam file
    while (horse_count < MAX_HORSES && fscanf(file, "%[^:]:%s\n", horses[horse_count].horse_name, horses[horse_count].owner) == 2) {
        horse_count++;  // Membaca informasi kuda dari file
    }
    fclose(file);  // Menutup file setelah selesai membaca

```
Bagian ini menentukan nama file yang dipilih pengguna dan membuka file tersebut. Jika file tidak dapat dibuka, fungsi akan keluar. Jika file terbuka, program membaca informasi kuda ke dalam array HorseInfo hingga jumlah maksimal kuda atau akhir file.

- Mengacak Data Kuda dan Memulai Thread Perlombaan
```bash
    randomize_array(horses, horse_count);  // Mengacak data kuda agar urutan tidak selalu sama

    pthread_t threads[horse_count];  // Membuat array untuk menyimpan thread kuda
    for (int i = 0; i < horse_count; i++) {
        if (pthread_create(&threads[i], NULL, race_thread, (void *)&horses[i]) != 0) {  // Membuat thread untuk setiap kuda
            perror("Thread creation error");
            return;  // Jika terjadi kesalahan saat membuat thread
        }
    }

    for (int i = 0; i < horse_count; i++) {
        if (pthread_join(threads[i], NULL) != 0) {  // Menunggu semua thread selesai
            perror("Thread join error");
            return;  // Jika terjadi kesalahan saat menggabungkan thread
        }
    }

```
Bagian ini mengacak data kuda menggunakan randomize_array. Kemudian, membuat thread untuk setiap kuda yang menjalankan fungsi race_thread. Jika terjadi kesalahan saat membuat atau menggabungkan thread, pesan kesalahan ditampilkan dan fungsi keluar.

- Menyimpan Hasil Perlombaan
```bash
    char result_filename[200];
    int race_no = RACE_NUMBER;

    while (true) {  // Menentukan nama file hasil perlombaan
        snprintf(result_filename, sizeof(result_filename), "HorseRace_%d.txt", race_no);  // Nama file berdasarkan nomor perlombaan
        FILE *check_file = fopen(result_filename, "r");  // Memeriksa apakah file sudah ada
        if (check_file == NULL) {  // Jika file belum ada, keluar dari loop
            break;
        }
        fclose(check_file);  // Jika file sudah ada, tutup file dan tingkatkan nomor perlombaan
        race_no++;
    }

    FILE *output_file = fopen(result_filename, "w");  // Membuka file hasil perlombaan untuk ditulis
    if (output_file == NULL) {  // Jika file tidak dapat dibuka
        perror("Output file error");
        return;
    }

    time_t current_time = time(NULL);  // Mengambil waktu saat ini
    struct tm *local_time = localtime(&current_time);  // Mengonversi waktu ke format lokal

    fprintf(output_file, "----Horse Race %d------\n", race_no);  // Menulis informasi dasar perlombaan
    fprintf(output_file, "Date: %02d/%02d/%04d\n", local_time->tm_mday, local_time->tm_mon + 1, local_time->tm_year + 1900);  // Menulis tanggal
    fprintf(output_file, "Time: %02d:%02d:%02d\n\n", local_time->tm_hour, local_time->tm_minute, local_time->tm_second);  // Menulis waktu

    for (int i = 0; i < horse_count; i++) {
        fprintf(output_file, "%d. %s %s %d\n", i + 1, horses[i].horse_name, horses[i].owner, horses[i].prime_number);  // Menulis hasil kuda
    }

    fclose(output_file);  // Menutup file setelah selesai menulis
    printf("Horse race completed, results stored in: %s\n", result_filename);  // Menampilkan pesan bahwa perlombaan selesai

```
Bagian ini menyimpan hasil perlombaan ke dalam file. Program membuat nama file hasil perlombaan berdasarkan nomor perlombaan. Jika nama sudah ada, nomor ditingkatkan dan memeriksa lagi. Setelah menentukan nama file, hasil ditulis ke file, termasuk informasi dasar, tanggal, waktu, dan hasil setiap kuda. Jika terjadi kesalahan saat membuka file, program keluar dengan pesan kesalahan. Setelah selesai, program menutup file dan menampilkan pesan bahwa perlombaan telah selesai.

**Jawab**

> [Format : Penjelasan (sesuaikan dengan style modul 1), Dokumentasi]

### Problem 1c
Buatlah fungsi HorseChampionLeaderboard, yang bertugas menampilkan pemenang dari pertandingan-pertandingan sebelumnya. Berikan opsi kepada pengguna untuk memilih lomba mana yang ingin dilihat hasilnya.

**Jawab**
- Fungsi HorseChampionLeaderboard
```bash
void HorseChampionLeaderboard(const char *directory) {
    int choice;
    printf("Select Horse Race History:\n");

    DIR *dir = opendir(directory);
    if (dir == NULL) {
        perror("Failed to open directory.\n");
        return;
    }

    struct dirent *entry;
    int history_count = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, "HorseRace_") != NULL) {
            history_count++;
            printf("%d. %s\n", history_count, entry's name);
        }
    }

    if (history_count == 0) {
        printf("No history found.\n");
        closedir(dir);
        return;
    }

    printf("Enter number: ");
    if (scanf("%d", &choice) != 1 atau choice < 1 atau choice > history_count) {
        printf("Invalid choice.\n");
        closedir(dir);
        return;
    }

    char file_path[100];
    snprintf(file_path, sizeof(file_path), "%s/HorseRace_%d.txt", directory, choice);

    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        printf("Failed to open file.\n");
        return;
    }

    printf("-----------------------------------------------------------------------\n");
    printf("\n----HORSE CHAMPIONSHIP (%d)----\n", choice);

    bool skip_first = true;
    char line[500];
    while (fgets(line, sizeof(line), file)) {
        if (skip_first) {
            skip_first = false;
            continue;
        }
        printf("%s", line);
    }

    fclose(file);
}
```
 
 - Pendahuluan
```bash
void HorseChampionLeaderboard(const char *directory) {
    int choice;
    printf("Select Horse Race History:\n");
```
Fungsi HorseChampionLeaderboard bertugas untuk menampilkan riwayat perlombaan kuda dari direktori yang diberikan. Parameter directory adalah nama direktori di mana file riwayat disimpan.
int choice; mendefinisikan variabel untuk menampung pilihan pengguna.
printf("Select Horse Race History:\n"); menampilkan prompt untuk meminta pengguna memilih riwayat perlombaan.

 - Membuka direktori dan menghitung file
```bash
    DIR *dir = opendir(directory);
    if (dir == NULL) {
        perror("Failed to open directory.\n");
        return;
    }

    struct dirent *entry;
    int history_count = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, "HorseRace_") != NULL) {
            history_count++;
            printf("%d. %s\n", history_count, entry->d_name);
        }
    }
```

Penjelasan mendetail dari kode fungsi HorseChampionLeaderboard ini akan dilakukan per bagian untuk memberikan konteks dan pemahaman yang lebih jelas.

Pendahuluan
c
Copy code
void HorseChampionLeaderboard(const char *directory) {
    int choice;
    printf("Select Horse Race History:\n");
Fungsi HorseChampionLeaderboard bertugas untuk menampilkan riwayat perlombaan kuda dari direktori yang diberikan. Parameter directory adalah nama direktori di mana file riwayat disimpan.
int choice; mendefinisikan variabel untuk menampung pilihan pengguna.
printf("Select Horse Race History:\n"); menampilkan prompt untuk meminta pengguna memilih riwayat perlombaan.
Membuka Direktori dan Menghitung File
c
Copy code
    DIR *dir = opendir(directory);
    if (dir == NULL) {
        perror("Failed to open directory.\n");
        return;
    }

    struct dirent *entry;
    int history_count = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, "HorseRace_") != NULL) {
            history_count++;
            printf("%d. %s\n", history_count, entry->d_name);
        }
    }
DIR *dir = opendir(directory); mencoba membuka direktori yang diberikan.
if (dir == NULL) { ... } memeriksa apakah direktori berhasil dibuka; jika gagal, fungsi menampilkan pesan kesalahan dan keluar.
struct dirent *entry; mendefinisikan variabel untuk membaca entri direktori.
int history_count = 0; menginisialisasi penghitung file riwayat.
while ((entry = readdir(dir)) != NULL) { ... } membaca setiap entri dalam direktori.
if (strstr(entry->d_name, "HorseRace_") != NULL) { ... } memeriksa apakah nama file berisi "HorseRace_"; jika iya, menambah history_count dan mencetak nama file beserta indeksnya.

 - Bagian Validasi Pilihan Pengguna
```bash
    if (history_count == 0) {
        printf("No history found.\n");
        closedir(dir);
        return;
    }

    printf("Enter number: ");
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > history_count) {
        printf("Invalid choice.\n");
        closedir(dir);
        return;
    }

    closedir(dir);

```
if (history_count == 0) { ... } memeriksa apakah ada file riwayat yang ditemukan. Jika tidak ada, fungsi menampilkan pesan bahwa tidak ada riwayat dan menutup direktori.
printf("Enter number: "); meminta pengguna memilih nomor file riwayat.
if (scanf("%d", &choice) != 1 || choice < 1 || choice > history_count) { ... } memeriksa apakah input pengguna valid. Jika tidak, menampilkan pesan kesalahan dan keluar.
closedir(dir); menutup direktori setelah selesai.

 - Bagian Membuka File Riwayat dan Menampilkan Konten
```bash
    char file_path[100];
    snprintf(file_path, sizeof(file_path), "%s/HorseRace_%d.txt", directory, choice);

    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        printf("Failed to open file.\n");
        return;
    }

    printf("-----------------------------------------------------------------------\n");
    printf("\n----HORSE CHAMPIONSHIP (%d)----\n", choice);

    bool skip_first = true;
    char line[500];
    while (fgets(line, sizeof(line), file)) {
        if (skip_first) {
            skip_first = false;
            continue;
        }
        printf("%s", line);
    }

    fclose(file);
}
```
char file_path[100]; mendefinisikan variabel untuk menyimpan jalur file.
snprintf(file_path, sizeof(file_path), "%s/HorseRace_%d.txt", directory, choice); membuat jalur file berdasarkan direktori dan pilihan pengguna.
FILE *file = fopen(file_path, "r"); membuka file riwayat yang dipilih.
if (file == NULL) { ... } memeriksa apakah file berhasil dibuka. Jika tidak, menampilkan pesan kesalahan dan keluar.
printf("-----------------------------------------------------------------------\n"); mencetak pembatas visual untuk memisahkan output.
printf("\n----HORSE CHAMPIONSHIP (%d)----\n", choice); mencetak header untuk bagian perlombaan yang dipilih.
bool skip_first = true; mendefinisikan variabel untuk melewatkan baris pertama (yang mungkin berisi header).
char line[500]; mendefinisikan buffer untuk membaca baris dari file.
while (fgets(line, sizeof(line), file)) { ... } membaca setiap baris dalam file.
if (skip_first) { ... } melewatkan baris pertama dan mengatur skip_first = false.
printf("%s", line); mencetak isi baris yang dibaca dari file.
fclose(file); menutup file setelah selesai membaca isinya.
> [Format : Penjelasan (sesuaikan dengan style modul 1), Dokumentasi]

### Kendala

> [Format : Penjelasan Kendala + Solusi (jika ada)]

## 2️⃣ Soal 2
Yuan dan Bubu ditugaskan untuk mengunduh gambar oleh dosennya, namun dengan beberapa tantangan. Mereka diberikan satu folder yang berisikan file random. Dimana pada folder tersebut, terdapat beberapa file yang berisi petunjuk terkait gambar yang harus diunduh oleh Yuan dan Bubu.
### Problem 2a
Pertama-tama, mereka harus mengunduh sebuah file zip dari google drive, kemudian mengekstraknya dan menghapus file zip tersebut. Setelah itu, mereka harus membuat folder baru dengan nama “task” dan memindahkan file yang berkaitan dengan petunjuk ke dalam folder “task”. File yang berkaitan memiliki penamaan dengan awalan "task_{id}" dan berekstensi txt. Lalu hapus folder hasil zip sebelumya.

**Jawab**

_1. Dalam Main Function, kita buat dahulu variabel dan pemanggilan fungsi yang dibutuhkan untuk menyelesaikan problem 2a_
```C
int main()
{
    // SOAL POIN A
    char *url = "https://drive.google.com/uc?export=download&id=1CdWKLXVAc6P26sF-5oSrlVAsSExA1PQh";
    char *output_filename = "task_sisop.zip";
    char *path = "task";
    pthread_t yuan_thread, bubu_thread; // thread identifier

    // fungsi download file
    char *downloaded_file = download_file(url, output_filename);

    // fungsi create directory
    create_directory(path);

    // fungs unzip file
    unzip_file(downloaded_file, path);

    // hapus file tidak berguna yang sudah di download
    if (remove(downloaded_file) != 0)
    {
        perror("Gagal menghapus file yang sudah di download");
    }
    else
    {
        printf("Menghapus file: %s\n", downloaded_file);
    }

    // membebaskan memory
    free(downloaded_file);

    // mencari file .txt dan memindahkannya ke folder task
    search_task(path);
 
    ...
```
- `pthread_t yuan_thread, bubu_thread;` untuk menampung identitas thread yang akan dibuat. Digunakan pada problem 2b dan 2c
- Setelah proses ekstrak/unzip selesai, file .zip akan dihapus
- `free(downloaded_file)` untuk membebaskan memori yang dialokasikan untuk `downloaded_file` setelah file tidak digunakan

_2. Membuat fungsi untuk mendowload file .zip dari google drive_
```C
char *download_file(char *url, char *output_name)
{
    // membuat array argumen untuk wget
    char *argz[] = {"wget", "--no-check-certificate", "-O", output_name, url, NULL};

    pid_t pid;
    pid = fork();

    if (pid == 0)
    {
        // child process: mengksekusi wget command
        execv("/usr/bin/wget", argz);
        perror("wget failed");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        // parent process: menunggu child process selesai
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
        {
            printf("Download zip dari link berhasil\n");
        }
        else
        {
            printf("Download zip dari link gagal\n");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        // gagal membuat fork
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    // mengembalikan nama file yang sudah di download
    return strdup(output_name);
}
```
- Menggunakan `fork()` untuk mendownload file .zip tersebut
- Array `*argz[]` digunakan untuk menyimpan argumen dalam perintah `wget`
- `--no-check-certificate` untuk mengabaikan sertifikat SSL
- `-O` untuk menentukan nama file output dari parameter `output_name`
- Jika `pid == 0` maka akan dijalankan perintah `execv` untuk menjalankan perintah `wget` dengan argumen yang telah disiapkan sebelumnya
- Apabila child berjalan dengan normal, maka `pid > 0` menampilkan output bahwa zip telah berhasil didownload.  Hasil ini merupakan proses parent yang menunggu proses child selesai dengan `waitpid`
- `strdup(output_name)` untuk mengembalikan nama file yang sudah diunduh untuk mengalokasikan memori baru dan menyalin string `output_name` ke memori tersebut

_3. Menyiapkan fungsi untuk membuat directory sebagai tempat file hasil ekstrak .zip_
```C
void create_directory(const char *path)
{
    // cek apakah directory sudah ada
    struct stat st;
    if (stat(path, &st) == -1)
    {
        // jika directory belum ada, maka buat directory
        if (mkdir(path, 0777) == -1)
        {
            perror("Gagal membuat directory");
            exit(EXIT_FAILURE);
        }
        printf("Membuat direktori: %s\n", path);
    }
}
```
- Fungsi `stat` digunakan untuk mengecek apakah direktori sudah ada. Hasilnya akan disimpan dalam struktur `st`
- Jika `stat` mengembalikan nilai -1, maka fungsi `mkdir` dipanggil untuk membuat direktori sesuai path yang diberikan dengan permission `0777` 

_4. Membuat fungsi untuk ekstrak/unzip file .zip yang sudah didownload sebelumnya. Hasil zip langsung dipindahkan dalam direktori "task"_
```C
void unzip_file(char *filename, char *path)
{
    // membuat array argumen untuk unzip
    char *arguments[] = {"unzip", "-o", "-q", filename, "-d", path, NULL};

    pid_t pid;
    pid = fork();

    if (pid == 0)
    {
        // child process: mengksekusi unzip command
        execv("/usr/bin/unzip", arguments);
        perror("unzip failed");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        // parent process: menunggu child process selesai
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
        {
            printf("Unzip berhasil\n");
        }
        else
        {
            printf("Unzip gagal\n");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        // gagal membuat fork
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
}
```
- Menggunakan `fork()` untuk ekstrak file .zip tersebut
- Array `*arguments[]` digunakan untuk menyimpan argumen dalam perintah `unzip`
- `-o` untuk melewati konfirmasi overwrite
- `-q` untuk mode quiet agar tidak menampilkan output yang berlebihan
- `-d` untuk menentukan direktori output
- Jika `pid == 0` maka akan dijalankan perintah `execv` untuk menjalankan perintah `unzip` dengan argumen yang telah disiapkan sebelumnya
- Apabila child berjalan dengan normal, maka `pid > 0` menampilkan output bahwa unzip telah berhasil. Hasil ini merupakan proses parent yang menunggu proses child selesai dengan `waitpid`

_5. Setelah zip diekstrak dan hasilnya dipindahkan dalam direktori "task", dilanjutkan untuk mencari file berekstensi .txt. Dalam hal ini saya tidak perlu mengecek awal penamaan apakah task_id karena sudah dipastikan bahwa file dengan ekstensi .txt pasti memiliki format penamaan tersebut_
```C
void search_task(const char *path)
{
    DIR *dir;           // pointer ke directory
    struct dirent *ent; // pointer ke file/directory
    const char *ori_path = "/home/hilmifawwaz/sisop/praktikum-modul-2-d04/task-2/task";

    // membuka directory
    if ((dir = opendir(path)) != NULL)
    {
        // membaca semua file/directory dalam directory
        while ((ent = readdir(dir)) != NULL)
        {
            // cek apakah nama file bukan "." dan "..", jika bukan maka lanjutkan
            if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0)
            {
                char full_path[PATH_MAX];
                snprintf(full_path, sizeof(full_path), "%s/%s", path, ent->d_name);

                // cek apakah file adalah directory, jika iya maka rekursif panggil fungsi search_task
                if (ent->d_type == DT_DIR)
                {
                    search_task(full_path);
                    // menghapus directory jika sudah selesai
                    if (rmdir(full_path) != 0)
                    {
                        perror("Gagal menghapus directory");
                    }
                    else
                    {
                        printf("Menghapus directory: %s\n", full_path);
                    }
                }
                else
                {
                    // cek apakah file adalah .txt, jika iya maka pindahkan file ke folder task
                    if (strstr(ent->d_name, ".txt") != NULL)
                    {
                        // membuat array argumen untuk mv
                        char new_path[PATH_MAX];
                        snprintf(new_path, sizeof(new_path), "%s/%s", ori_path, ent->d_name); // path tujuan file
                        if (rename(full_path, new_path) != 0)
                        {
                            perror("Gagal memindahkan file");
                        }
                        else
                        {
                            printf("Memindahkan file: %s\n", new_path);
                        }
                    }
                    else
                    {
                        // jika bukan file .txt, maka hapus file
                        if (remove(full_path) != 0)
                        {
                            perror("Gagal menghapus file");
                        }
                        else
                        {
                            printf("Menghapus file: %s\n", full_path);
                        }
                    }
                }
            }
        }
        closedir(dir);
    }
    else
    {
        perror("Gagal membuka directory");
        exit(EXIT_FAILURE);
    }
}
```
- Program tersebut digunakan mencari file yang sesuai (.txt) dengan cara mengecek secara rekursif setiap direktori yang ada
- Jika proses rekursif selesai, direktori akan dihapus menggunakan `rmdir`
- Selama proses rekursif, apabila menemukan entri bukan sebuah direktori, maka dilakukan pengecekan. Jika hasilnya adalah sebuah file berekstensi `.txt` maka dipindahkan ke dalam direktori tujuan menggunakan `snprintf(new_path, sizeof(new_path), "%s/%s", ori_path, ent->d_name);`
- Jika bukan file berkestensi `.txt`, maka file akan dihapus menggunakan fungsi `remove()`

_6. Dokumentasi. Dokumentasi ini saya lakukan satu per satu fungsi agar terlihat hasilnya. Tetapi, aslinya dalam program ini seharusnya hanya dijalankan sekali saja untuk semua proses_
- Download file .zip dari google drive
![alt text](/resource/2a-1.png)
- Membuat direktori "task" untuk menyimpan hasil ekstrak file .zip
![alt text](/resource/2a-2.png)
- Proses ekstrak/unzip file .zip sekaligus outputnya dipindahkan ke dalam direktori "task"
![alt text](/resource/2a-3.png)
- Menghapus file .zip yang di download
![alt text](/resource/2a-4.png)
- Mencari file .txt yang sesuai
![alt text](/resource/2a-5.png)

### Problem 2b
Yuan dan Bubu membagi tugas secara efisien dengan mengerjakannya secara bersamaan (overlapping) dan membaginya sama banyak. Yuan akan mengerjakan task dari awal, sementara Bubu dari akhir. Misalnya, jika ada 20 task, Yuan akan memulai dari task0-task1-dst dan Bubu akan memulai dari task19-task18-dst. Lalu buatlah file “recap.txt” yang menyimpan log setiap kali mereka selesai melakukan task (kriteria setiap task akan dijelaskan di poin c).

**Jawab**

_1. Mempersiapkan thread pada Main Function_
```C
int main()
{
    // SOAL POIN A
    char *url = "https://drive.google.com/uc?export=download&id=1CdWKLXVAc6P26sF-5oSrlVAsSExA1PQh";
    char *output_filename = "task_sisop.zip";
    char *path = "task";
    pthread_t yuan_thread, bubu_thread; // thread identifier
    
    .....
    
    // SOAL POIN B, C
    // membuat thread
    if (pthread_create(&yuan_thread, NULL, yuan, NULL) != 0)
    {
        fprintf(stderr, "Error creating thread\n");
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&bubu_thread, NULL, bubu, NULL) != 0)
    {
        fprintf(stderr, "Error creating thread\n");
        exit(EXIT_FAILURE);
    }

    // menunggu thread selesai
    pthread_join(yuan_thread, NULL);
    pthread_join(bubu_thread, NULL);

    .....
```
- `pthread_t yuan_thread, bubu_thread;` untuk menampung identitas thread yang akan dibuat
- `pthread_create(&yuan_thread, NULL, yuan, NULL)` untuk membuat thread baru untuk memanggil fungsi `yuan` sebagai fungsi yang akan dieksekusi oleh thread tersebut
- `pthread_create(&bubu_thread, NULL, bubu, NULL)` untuk membuat thread baru yang memanggil fungsi `bubu` sebagai fungsi yang akan dieksekusi oleh thread tersebut
- `pthread_join` digunakan untuk menggabungkan thread lain. Fungsi ini akan memblokir eksekusi program hingga thread tersebut selesai. Jadi, program dapat menjalankan dua atau lebih tugas secara bersamaan (paralel)

_2. Membuat fungsi yuan untuk menjalankan tugasnya_
```C
void *yuan(void *arg)
{
    // membuat directory Yuan
    char *yuan_path = "/home/hilmifawwaz/sisop/praktikum-modul-2-d04/task-2/task/Yuan";

    // cek apakah directory sudah ada
    if (access(yuan_path, F_OK) == -1)
    {
        create_directory(yuan_path);
        printf("Folder Yuan dibuat\n");
    }
    else
    {
        printf("Folder 'Yuan' sudah ada!!!\n");
    }

    // menjalankan task untuk Yuan
    work_task_yuan(yuan_path);

    printf("Tugas Yuan Selesai\n");

    pthread_exit(0); // mengakhiri dan keluar dari thread
}
```
- `char *yuan_path` menyimpan path dari direktori yang akan dibuat untuk tugas yang berkaitan dengan Yuan
- `access(yuan_path, F_OK)` digunakan untuk memeriksa apakah direktori dengan path yang ditentukan sudah ada. Jika belum ada, fungsi `create_directory` yang saya jelaskan pada 2a dipanggil untuk membuat direktori tersebut
- Fungsi `work_task_yuan(yuan_path)` dipanggil menjalankan tugas yang berkaitan dengan Yuan
- `pthread_exit(0);` digunakan untuk mengakhiri thread saat ini dan keluar dari thread

_3. Membuat fungsi untuk download gambar sesuai tugas Yuan. Fungsi ini dipanggil dari fungsi yuan pada nomor 2_
```C
void work_task_yuan(const char *yuan_path)
{
    DIR *dir;                                                                           // pointer ke directory
    struct dirent *ent;                                                                 // pointer ke file/directory
    const char *ori_path = "/home/hilmifawwaz/sisop/praktikum-modul-2-d04/task-2/task"; // path asal
    int num_tasks = 0;                                                                  // counter task
    int num_files = 0;                                                                  // counter file
    char file_names[100][PATH_MAX];                                                     // array untuk menyimpan nama file

    // membuka directory
    if ((dir = opendir(ori_path)) != NULL)
    {
        // membaca semua file/directory dalam directory
        while ((ent = readdir(dir)) != NULL)
        {
            // cek apakah nama file dimulai dengan "task_", jika iya maka lanjutkan
            if (strncmp(ent->d_name, "task_", 5) == 0)
            {
                // mengambil nomor task dari nama file 
                int current_task_number = atoi(ent->d_name + 5);

                // cek apakah nomor task berada di range 0-9, jika iya maka lanjutkan
                if (current_task_number >= 0 && current_task_number <= 9)
                {
                    // menyimpan nama file ke dalam array
                    snprintf(file_names[num_files], sizeof(file_names[num_files]), "%s", ent->d_name);
                    num_files++;
                }
            }
        }
        closedir(dir);

        // menampilkan jumlah file
        printf("Jumlah file yang ditemukan: %d\n", num_files);

        // mengurutkan nama file secara ascending menggunakan bubble sort
        for (int i = 0; i < num_files - 1; i++)
        {
            for (int j = 0; j < num_files - i - 1; j++)
            {
                // mengambil nomor task dari nama file
                int task_number1 = atoi(file_names[j] + 5);
                int task_number2 = atoi(file_names[j + 1] + 5);
                // membandingkan nomor task, jika task_number1 > task_number2 maka tukar posisi
                if (task_number1 > task_number2)
                {
                    char temp[PATH_MAX];
                    strcpy(temp, file_names[j]);
                    strcpy(file_names[j], file_names[j + 1]);
                    strcpy(file_names[j + 1], temp);
                }
            }
        }

        // // menampilkan nama file yang sudah diurutkan
        // for (int i = 0; i < num_files; i++)
        // {
        //     printf("File %d: %s\n", i+1, file_names[i]);
        // }

        // mengakses nama file yang sudah diurutkan
        for (int i = 0; i < num_files; i++)
        {
            char full_path[PATH_MAX];
            snprintf(full_path, sizeof(file_names[i]), "%s/%s", ori_path, file_names[i]); // membangun full path file dengan menggabungkan path asal dan nama file
            printf("Membaca file: %s\n", full_path);

            // membuka file
            FILE *fp = fopen(full_path, "r");
            if (fp == NULL)
            {
                perror("Gagal membuka file");
                exit(EXIT_FAILURE);
            }
            else
            {
                // membaca dan memproses isi file
                int count;           // counter jumlah gambar yang akan didownload
                char resolution[20]; // resolusi gambar
                char categorie[100]; // kategori gambar

                fscanf(fp, "%d %s %[^\n]", &count, resolution, categorie); // membaca isi file

                printf("Count: %d\n", count);
                printf("Resolution: %s\n", resolution);
                printf("Categorie: %s\n", categorie);

                // membuat directory sesuai dengan nama file
                char dir_name[100]; // array untuk menyimpan nama directory

                // proses penaaman directory dari nama file
                char *file_name = strtok(file_names[i], "."); // memisahkan nama file dengan ekstensi
                char *underscore = strchr(file_name, '_');    // mencari underscore pertama pada nama file
                if (underscore != NULL)
                {
                    char *second_underscore = strchr(underscore + 1, '_'); // mencari underscore kedua pada nama file
                    if (second_underscore != NULL)
                    {
                        *second_underscore = '\0';
                    }
                }
                if (underscore != NULL)
                {
                    memmove(underscore, underscore + 1, strlen(underscore)); // menghapus underscore pertama
                }
                snprintf(dir_name, sizeof(dir_name), "%s", file_name); // menyimpan nama directory
                char dir_path[PATH_MAX];
                snprintf(dir_path, sizeof(dir_path), "%s/%s", yuan_path, dir_name); // membangun full path directory dengan menggabungkan path Yuan dan nama directory
                create_directory(dir_path);                                         // membuat directory

                pid_t pid;
                pid = fork();

                if (pid == 0)
                {
                    // child process: mengksekusi wget command
                    for (int i = 1; i <= count; i++)
                    {
                        char link[200];
                        snprintf(link, sizeof(link), "https://source.unsplash.com/random/%s/?%s", resolution, categorie); // membuat link download gambar

                        char filename[5000];                                                                 // array untuk menyimpan nama file gambar
                        snprintf(filename, sizeof(filename), "%s/gambar%d.png", dir_path, i);                // membangun full path file gambar
                        char *argv[] = {"wget", "-q", "--no-check-certificate", link, "-O", filename, NULL}; // array argumen untuk wget

                        pid_t pid;
                        pid = fork();

                        if (pid == 0)
                        {
                            // child process: mengksekusi wget command
                            execv("/usr/bin/wget", argv);
                            perror("wget failed");
                            exit(EXIT_FAILURE);
                        }
                        else if (pid > 0)
                        {
                            // parent process: menunggu child process selesai
                            int status;
                            waitpid(pid, &status, 0);
                            if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
                            {
                                printf("Download sukses: %s\n", filename);
                            }
                            else
                            {
                                printf("Download gagal: %s\n", filename);
                                exit(EXIT_FAILURE);
                            }
                        }
                        else
                        {
                            // gagal membuat fork
                            perror("Fork failed");
                            exit(EXIT_FAILURE);
                        }
                    }
                    exit(EXIT_SUCCESS); // keluar dari child process setelah semua file selesai didownload
                }
                else if (pid > 0)
                {
                    // parent process: menunggu child process selesai
                    int status;
                    waitpid(pid, &status, 0);

                    if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
                    {
                        printf("Horee! Download sukses\n");
                    }
                    else
                    {
                        printf("Yahh! Download gagal\n");
                        exit(EXIT_FAILURE);
                    }
                }
                else
                {
                    // gagal membuat fork
                    perror("Fork failed");
                    exit(EXIT_FAILURE);
                }

                // memindahkan file ke directory yang ditentukan
                char new_file_path[PATH_MAX];
                snprintf(new_file_path, sizeof(new_file_path), "%s/%s", yuan_path, file_names[i]); // membangun full path file tujuan

                // cek apakah file berhasil dipindahkan
                if (rename(full_path, new_file_path) != 0)
                {
                    // jika file adalah directory, maka gunakan command "mv" untuk memindahkan
                    // EISDIR adalah error code untuk menandakan bahwa file adalah directory
                    if (errno == EISDIR)
                    {
                        char *arguments[] = {"mv", full_path, new_file_path, NULL}; // array argumen untuk mv

                        pid_t pid;
                        pid = fork();

                        if (pid == 0)
                        {
                            // child process: mengksekusi mv command
                            execv("/bin/mv", arguments);
                            perror("mv failed");
                            exit(EXIT_FAILURE);
                        }
                        else if (pid > 0)
                        {
                            // parent process: menunggu child process selesai
                            int status;
                            waitpid(pid, &status, 0);
                            if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
                            {
                                printf("Pemindahan file sukses\n");
                            }
                            else
                            {
                                printf("Pemindahan file gagal\n");
                                exit(EXIT_FAILURE);
                            }
                        }
                        else
                        {
                            // gagal membuat fork
                            perror("Fork failed");
                            exit(EXIT_FAILURE);
                        }
                    }
                    else
                    {
                        perror("Gagal memindahkan file");
                        exit(EXIT_FAILURE);
                    }
                }

                fclose(fp);  // menutup file
                num_tasks++; // increment counter task

                // merekap data ke file recap.txt
                recap_data("Yuan", file_name, count, categorie, resolution);
            }
        }
    }
}
```
- Sebenarnya, program ini gabungan dari problem 2c. Namun, karena saya langsung membuatnya dalam satu fungsi yang juga berjalan pada problem 2b, maka jawaban problem 2c akan menggunakan ini
- Penjelasan program secara singkat sudah terlihat pada comment di atas
- Karena pada proses thread task_{id} yang didownload diambil secara random, saya menggunakan array `char file_names[100][PATH_MAX]` untuk menyimpan nama file yang akan diurutkan secara ascending dari task_0 sampai task_9
- Setiap file yang namanya diawali `task_` akan diproses dan disimpan dalam array `file_names`. Pemrosesan akan dilakukan menggunakan bubble sort berdasarkan {id} pada nama file
- Setiap file yang sudah diurutkan akan diproses. Direktori baru akan dibuat berdasarkan nama file
- Setiap file akan dibuka dan isinya akan diproses
Untuk setiap file, akan dilakukan fork untuk melakukan download gambar menggunakan perintah `wget` dalam child process
- Setelah download selesai, file akan dipindahkan ke direktori yang sesuai. Recap data juga dilakukan ke dalam file "recap.txt" melalui fungsi `recap_data`

_4. Pada poin ini, saya hanya mengcopykan program saya karena memang program bubu mirip dengan program yuan. Yang membedakan hanya pada sorting pada fungsi `work_task_bubu` yang dilakukan secara descendung dari task_19 sampai task_10_
```C
void *bubu(void *arg)
{
    // membuat directory Bubu
    char *bubu_path = "/home/hilmifawwaz/sisop/praktikum-modul-2-d04/task-2/task/Bubu";

    // cek apakah directory sudah ada
    if (access(bubu_path, F_OK) == -1)
    {
        create_directory(bubu_path);
        printf("Folder Bubu dibuat\n");
    }
    else
    {
        printf("Folder 'Bubu' sudah ada!!!\n");
    }

    // menjalankan task untuk Bubu
    work_task_bubu(bubu_path);

    printf("Tugas Bubu Selesai\n");

    pthread_exit(0); // mengakhiri dan keluar dari thread
}
```
```C
void work_task_bubu(const char *bubu_path)
{
    DIR *dir;                                                                           // pointer ke directory
    struct dirent *ent;                                                                 // pointer ke file/directory
    const char *ori_path = "/home/hilmifawwaz/sisop/praktikum-modul-2-d04/task-2/task"; // path asal
    int num_tasks = 0;                                                                  // counter task
    int num_files = 0;                                                                  // counter file
    char file_names[100][PATH_MAX];                                                     // array untuk menyimpan nama file

    // membuka directory
    if ((dir = opendir(ori_path)) != NULL)
    {
        // membaca semua file/directory dalam directory
        while ((ent = readdir(dir)) != NULL)
        {
            // mengambil nama file yang dimulai dengan "task_"
            int current_task_number = atoi(ent->d_name + 5);

            // cek apakah nomor task berada di range 10-19, jika iya maka lanjutkan
            if (current_task_number >= 10 && current_task_number <= 19)
            {
                // menyimpan nama file ke dalam array
                snprintf(file_names[num_files], sizeof(file_names[num_files]), "%s", ent->d_name);
                num_files++;
            }
        }
        closedir(dir);

        // menampilkan jumlah file
        printf("Jumlah file yang ditemukan: %d\n", num_files);

        // Mengurutkan nama file secara descending menggunakan bubble sort
        for (int i = 0; i < num_files - 1; i++)
        {
            for (int j = 0; j < num_files - i - 1; j++)
            {
                // mengambil nomor task dari nama file
                int task_number1 = atoi(file_names[j] + 5);
                int task_number2 = atoi(file_names[j + 1] + 5);
                // membandingkan nomor task, jika task_number1 < task_number2 maka tukar posisi
                if (task_number1 < task_number2)
                {
                    char temp[PATH_MAX];
                    strcpy(temp, file_names[j]);
                    strcpy(file_names[j], file_names[j + 1]);
                    strcpy(file_names[j + 1], temp);
                }
            }
        }

        // // menampilkan nama file yang sudah diurutkan
        // for (int i = 0; i < num_files; i++)
        // {
        //     printf("File %d: %s\n", i+1, file_names[i]);
        // }

        // mengakses nama file yang sudah diurutkan
        for (int i = 0; i < num_files; i++)
        {
            char full_path[PATH_MAX];
            snprintf(full_path, sizeof(file_names[i]), "%s/%s", ori_path, file_names[i]); // membangun full path file dengan menggabungkan path asal dan nama file
            printf("Membaca file: %s\n", full_path);

            // membuka file
            FILE *fp = fopen(full_path, "r");
            if (fp == NULL)
            {
                perror("Gagal membuka file");
                exit(EXIT_FAILURE);
            }
            else
            {
                // membaca dan memproses isi file
                int count;           // counter jumlah gambar yang akan didownload
                char resolution[20]; // resolusi gambar
                char categorie[100]; // kategori gambar

                fscanf(fp, "%d %s %[^\n]", &count, resolution, categorie); // membaca isi file

                printf("Count: %d\n", count);
                printf("Resolution: %s\n", resolution);
                printf("Categorie: %s\n", categorie);

                // membuat directory sesuai dengan nama file
                char dir_name[100]; // array untuk menyimpan nama directory

                // proses penaaman directory dari nama file
                char *file_name = strtok(file_names[i], "."); // memisahkan nama file dengan ekstensi
                char *underscore = strchr(file_name, '_');    // mencari underscore pertama pada nama file
                if (underscore != NULL)
                {
                    char *second_underscore = strchr(underscore + 1, '_'); // mencari underscore kedua pada nama file
                    if (second_underscore != NULL)
                    {
                        *second_underscore = '\0';
                    }
                }
                if (underscore != NULL)
                {
                    memmove(underscore, underscore + 1, strlen(underscore)); // menghapus underscore pertama
                }
                
                snprintf(dir_name, sizeof(dir_name), "%s", file_name); // menyimpan nama directory
                char dir_path[PATH_MAX];
                snprintf(dir_path, sizeof(dir_path), "%s/%s", bubu_path, dir_name); // membangun full path directory dengan menggabungkan path Bubu dan nama directory
                create_directory(dir_path);                                         // membuat directory

                pid_t pid;
                pid = fork();

                if (pid == 0)
                {
                    // child process: mengksekusi wget command
                    for (int i = 1; i <= count; i++)
                    {
                        char link[200];
                        snprintf(link, sizeof(link), "https://source.unsplash.com/random/%s/?%s", resolution, categorie); // membuat link download gambar

                        char filename[5000];                                                                 // array untuk menyimpan nama file gambar
                        snprintf(filename, sizeof(filename), "%s/gambar%d.png", dir_path, i);                // membangun full path file gambar
                        char *argv[] = {"wget", "-q", "--no-check-certificate", link, "-O", filename, NULL}; // array argumen untuk wget

                        pid_t pid;
                        pid = fork();

                        if (pid == 0)
                        {
                            // child process: mengksekusi wget command
                            execv("/usr/bin/wget", argv);
                            perror("wget failed");
                            exit(EXIT_FAILURE);
                        }
                        else if (pid > 0)
                        {
                            // parent process: menunggu child process selesai
                            int status;
                            waitpid(pid, &status, 0);
                            if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
                            {
                                printf("Download sukses: %s\n", filename);
                            }
                            else
                            {
                                printf("Download gagal: %s\n", filename);
                                exit(EXIT_FAILURE);
                            }
                        }
                        else
                        {
                            // gagal membuat fork
                            perror("Fork failed");
                            exit(EXIT_FAILURE);
                        }
                    }
                    exit(EXIT_SUCCESS); // keluar dari child process setelah semua file selesai didownload
                }
                else if (pid > 0)
                {
                    // parent process: menunggu child process selesai
                    int status;
                    waitpid(pid, &status, 0);

                    if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
                    {
                        printf("Horee! Download sukses\n");
                    }
                    else
                    {
                        printf("Yahh! Download failed\n");
                        exit(EXIT_FAILURE);
                    }
                }
                else
                {
                    // gagal membuat fork
                    perror("Fork failed");
                    exit(EXIT_FAILURE);
                }

                // memindahkan file ke directory yang ditentukan
                char new_file_path[PATH_MAX];
                snprintf(new_file_path, sizeof(new_file_path), "%s/%s", bubu_path, file_names[i]); // membangun full path file tujuan

                // cek apakah file berhasil dipindahkan
                if (rename(full_path, new_file_path) != 0)
                {
                    // jika file adalah directory, maka gunakan command "mv" untuk memindahkan
                    // EISDIR adalah error code untuk menandakan bahwa file adalah directory
                    if (errno == EISDIR)
                    {
                        char *arguments[] = {"mv", full_path, new_file_path, NULL}; // array argumen untuk mv

                        pid_t pid;
                        pid = fork();

                        if (pid == 0)
                        {
                            // child process: mengksekusi mv command
                            execv("/bin/mv", arguments);
                            perror("mv failed");
                            exit(EXIT_FAILURE);
                        }
                        else if (pid > 0)
                        {
                            // parent process: menunggu child process selesai
                            int status;
                            waitpid(pid, &status, 0);
                            if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
                            {
                                printf("Pemindahan file sukses\n");
                            }
                            else
                            {
                                printf("Pemindahan file gagal\n");
                                exit(EXIT_FAILURE);
                            }
                        }
                        else
                        {
                            // gaga membuat fork
                            perror("Fork failed");
                            exit(EXIT_FAILURE);
                        }
                    }

                    else
                    {

                        perror("Failed to move file");
                        exit(EXIT_FAILURE);
                    }
                }

                fclose(fp);  // menutup file
                num_tasks++; // increment counter task

                // merekap data ke file recap.txt
                recap_data("Bubu", file_name, count, categorie, resolution);
            }
        }
    }
}
```

_5. Membuat fungsi recap_data untuk menyimpan log setiap kali mereka selesai mengerjakan task_
```C
void recap_data(const char *name, const char *task, int count, const char *categorie, const char *resolution)
{
    // waktu sekarang
    time_t now = time(NULL);
    struct tm *timeinfo = localtime(&now);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);

    // mengkonversi nama task
    char task_name[100];
    strcpy(task_name, task);
    char *capital = strstr(task_name, "task"); // mencari kata "task" pada task_name
    if (capital != NULL)
    {
        *capital = 'T'; // mengganti huruf pertama dari "task" menjadi "T"
    }
    task_name[0] = toupper(task_name[0]); // mengubah huruf pertama menjadi huruf besar

    // memisahkan angka dari kata "task"
    char *number = task_name + 4; // mengambil angka setelah kata "task"
    int num = atoi(number); // mengubah string angka menjadi integer
    sprintf(task_name + 4, " %d", num); // menambahkan spasi dan angka setelah kata "Task"

    // menambahkan data ke file recap.txt
    FILE *file = fopen("recap.txt", "a");
    if (file == NULL)
    {
        perror("Gagal membuka file recap.txt");
        exit(EXIT_FAILURE);
    }

    // menambahkan data ke file recap.txt
    fprintf(file, "[%s]-[%s] %s completed, download %d %s images with %s resolution\n", timestamp, name, task_name, count, categorie, resolution);

    fclose(file);
}
```
- Fungsi `time()` digunakan untuk mendapatkan waktu saat ini dalam bentuk detik sejak epoch (00:00:00 UTC, 1 Januari 1970)
- Fungsi `localtime()` digunakan untuk mengonversi waktu yang diperoleh menjadi struktur tm yang menyimpan informasi waktu dalam format lokal
- Fungsi `strftime()` digunakan untuk memformat waktu ke dalam string dengan format tertentu, dalam hal ini digunakan format "YYYY-MM-DD HH:MM:SS"
- Proses konversi nama task yang awalnya "task1" menjadi "Task 1" untuk digunakan dalam recap data
- `fopen("recap.txt", "a")` untuk membuka file "recap.txt" dalam mode "a". Menggunakan `fprintf()` untuk menambahkan data baru ke akhir file recap.txt

_6. Dokumentasi_
- Hasil pemrosesan thread
![alt text](/resource/2b-1.png)
- Isi direktori yuan
![alt text](/resource/2b-2.png)
- Isi direktori bubu
![alt text](/resource/2b-3.png)
- Contoh isi direktori task8
![alt text](/resource/2b-4.png)
- Isi file recap.txt
```txt
[2024-04-21 22:57:00]-[Yuan] Task 0 completed, download 1 music images with 300x300 resolution
[2024-04-21 22:57:01]-[Yuan] Task 1 completed, download 1 nature images with 200x150 resolution
[2024-04-21 22:57:04]-[Bubu] Task 19 completed, download 4 city images with 250x300 resolution
[2024-04-21 22:57:06]-[Bubu] Task 18 completed, download 1 nature images with 250x150 resolution
[2024-04-21 22:57:07]-[Bubu] Task 17 completed, download 1 minimalist images with 150x250 resolution
[2024-04-21 22:57:08]-[Yuan] Task 2 completed, download 4 night images with 200x150 resolution
[2024-04-21 22:57:10]-[Yuan] Task 3 completed, download 1 fruit images with 200x150 resolution
[2024-04-21 22:57:13]-[Bubu] Task 16 completed, download 3 face images with 250x150 resolution
[2024-04-21 22:57:14]-[Yuan] Task 4 completed, download 3 night images with 300x250 resolution
[2024-04-21 22:57:25]-[Yuan] Task 5 completed, download 5 music images with 250x150 resolution
[2024-04-21 22:57:28]-[Bubu] Task 15 completed, download 4 city images with 200x200 resolution
[2024-04-21 22:57:36]-[Bubu] Task 14 completed, download 4 summer images with 250x200 resolution
[2024-04-21 22:57:39]-[Bubu] Task 13 completed, download 2 face images with 150x200 resolution
[2024-04-21 22:57:44]-[Bubu] Task 12 completed, download 3 minimalist images with 100x250 resolution
[2024-04-21 22:57:47]-[Yuan] Task 6 completed, download 5 minimalist images with 300x100 resolution
[2024-04-21 22:57:54]-[Yuan] Task 7 completed, download 5 fruit images with 200x250 resolution
[2024-04-21 22:58:01]-[Yuan] Task 8 completed, download 4 fruit images with 200x300 resolution
[2024-04-21 22:58:02]-[Bubu] Task 11 completed, download 2 fruit images with 150x250 resolution
[2024-04-21 22:58:08]-[Bubu] Task 10 completed, download 3 nature images with 300x200 resolution
[2024-04-21 22:58:29]-[Yuan] Task 9 completed, download 4 summer images with 200x250 resolution
```

### Problem 2c
Yuan dan Bubu menemukan petunjuk dalam setiap file task txt dengan format "{jumlah} {resolusi} {kategori}". Mereka harus mengunduh gambar sesuai dengan jumlah, resolusi, dan kategori yang disebutkan. Contohnya, jika task0 berisi "5 250x150 music", mereka mengunduh 5 gambar berukuran 250x150 dalam kategori music. Gambar diunduh dari unsplash.com dan disimpan bersama file txt setiap task berdasarkan orang yang mengerjakan task.

**Jawab**

Jawaban no 2c sudah include pada jawaban 2b. Dimulai pada poin 3 dan seterusnya.

### Problem 2d
Setelah semua tugas selesai, mereka menambahkan jumlah gambar dari setiap kategori ke dalam "recap.txt". dan pada baris terakhir tambahkan jumlah seluruh gambar yg di unduh.

**Jawab**

_1. Membuat fungsi total_recap_data yang digunakan untuk menjumlahkan semua gambar yang diunduh dan yang sesuai kategorinya. Tetapi sebelum itu, pada Main Function jangan lupa untuk memanggil fungsi total_recap_data_
```C
int main()
{
    .....

    // SOAL POIN D
    total_recap_data();

    .....
    return 0;
}
```
```C
void total_recap_data()
{
    FILE *file = fopen("recap.txt", "r");
    if (file == NULL)
    {
        perror("Gagal untuk membuka recap.txt");
        exit(EXIT_FAILURE);
    }

    // inisialisasi variabel
    int nature = 0;
    int night = 0;
    int fruit = 0;
    int music = 0;
    int city = 0;
    int summer = 0;
    int minimalist = 0;
    int face = 0;
    int total_images = 0;

    // membaca setiap baris file recap.txt
    char line[100];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        int count;

        // mencari kata kunci pada baris
        if (strstr(line, "nature images"))
        {
            sscanf(line, "[%*[^]]]-[%*[^]]] Task %*d completed, download %d nature images", &count);
            nature += count;
        }
        else if (strstr(line, "night images"))
        {
            sscanf(line, "[%*[^]]]-[%*[^]]] Task %*d completed, download %d night images", &count);
            night += count;
        }
        else if (strstr(line, "fruit images"))
        {
            sscanf(line, "[%*[^]]]-[%*[^]]] Task %*d completed, download %d fruit images", &count);
            fruit += count;
        }
        else if (strstr(line, "music images"))
        {
            sscanf(line, "[%*[^]]]-[%*[^]]] Task %*d completed, download %d music images", &count);
            music += count;
        }
        else if (strstr(line, "city images"))
        {
            sscanf(line, "[%*[^]]]-[%*[^]]] Task %*d completed, download %d city images", &count);
            city += count;
        }
        else if (strstr(line, "summer images"))
        {
            sscanf(line, "[%*[^]]]-[%*[^]]] Task %*d completed, download %d summer images", &count);
            summer += count;
        }
        else if (strstr(line, "minimalist images"))
        {
            sscanf(line, "[%*[^]]]-[%*[^]]] Task %*d completed, download %d minimalist images", &count);
            minimalist += count;
        }
        else if (strstr(line, "face images"))
        {
            sscanf(line, "[%*[^]]]-[%*[^]]] Task %*d completed, download %d face images", &count);
            face += count;
        }
    }

    // menghitung total images
    total_images = nature + night + fruit + music + city + summer + minimalist + face;

    fclose(file);

    // append data ke file recap.txt
    FILE *append_file = fopen("recap.txt", "a");
    if (append_file == NULL)
    {
        perror("Gagal membuka file recap.txt untuk menambahkan data");
        exit(EXIT_FAILURE);
    }

    // menambahkan data ke file recap.txt
    fprintf(append_file, "\nnature: %d images\n", nature);
    fprintf(append_file, "night: %d images\n", night);
    fprintf(append_file, "fruit: %d images\n", fruit);
    fprintf(append_file, "music: %d images\n", music);
    fprintf(append_file, "city: %d images\n", city);
    fprintf(append_file, "summer: %d images\n", summer);
    fprintf(append_file, "minimalist: %d images\n", minimalist);
    fprintf(append_file, "face: %d images\n", face);
    fprintf(append_file, "total images: %d images", total_images);

    fclose(append_file);
}
```
- Membuka file "recap.txt" dalam mode baca ("r") menggunakan fungsi `fopen("recap.txt", "r")`
- Karena kita sudah tau apa saja kategori yang didownload, kita dapat langsung melakukan inisialisasi variabel untuk merekam jumlah gambar yang terunduh untuk setiap kategori dan total keseluruhan
- `fgets` untuk membaca setiap baris dari file "recap.txt". Fungsi ini akan mencari keyword yang menandakan enis gambar yang terunduh dan mengambil jumlah gambar yang terunduh untuk setiap kategori tersebut
- Membuka kembali file "recap.txt" dalam mode append ("a") untuk menambahkan data baru berupa jumlah gambar untuk setiap kategori dan jumlah gambar keseluruhan

_2. Dokumentasi. Dokumentasi ini terekam dalam file "recap.txt" pada bagian bawah_
```txt
nature: 5 images
night: 7 images
fruit: 12 images
music: 6 images
city: 8 images
summer: 8 images
minimalist: 9 images
face: 5 images
total images: 60 images
```

### Problem 2e
Karena tugas sudah siap dikumpulkan, zip folder Yuan dan Bubu menjadi satu zip bernamakan submission.zip dan JANGAN HAPUS folder aslinya.

**Jawab**

_1. Membuat fungsi untuk mengcopy file "recap.txt" dan zip direktori "task" Tetapi sebelum itu, pada Main Function jangan lupa untuk memanggil fungsi finally()_
```C
int main()
{
    .....
    
    // SOAL POIN E
    finally();

    return 0;
}
```
```C
void finally()
{
    // copy file recap.txt ke folder task
    char *copy_command[] = {"cp", "recap.txt", "task/", NULL};

    pid_t pid;
    pid = fork();

    if (pid == 0)
    {
        // child process: mengksekusi cp command
        execv("/bin/cp", copy_command);
        perror("Copy failed");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        // parent process: menunggu child process selesai
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
        {
            printf("Copy sukses\n");
        }
        else
        {
            printf("Copy gagal\n");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        // gagal membuat fork
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    // zip folder task
    char *argz[] = {"zip", "-r", "task/submission.zip", "task/", NULL};

    pid = fork();

    if (pid == 0)
    {
        // child process: mengksekusi zip command
        execv("/usr/bin/zip", argz);
        perror("zip failed");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        // parent process: menunggu child process selesai
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
        {
            printf("Zip sukses\n");
        }
        else
        {
            printf("Zip gagal\n");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        // gagal membuat fork
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
}
```
- Menggunakan `fork()` untuk copy file dan zip direktori
- Karena pada contoh file "recap.txt" juga ikut di zip, maka saya mengcopy file tersebut ke dalam direktori "task" menggunakan command-line utility `cp`
- Setelah file di copy, maka dilanjutkan proses zip direktori
- Array `argz` dibuat yang berisi argumen untuk command `zip`
- Di dalam child process, fungsi `execv` digunakan untuk mengeksekusi command `zip` dengan argumen yang telah disiapkan sebelumnya

_2. Dokumentasi_
- Isi direktori "task"
![alt text](/resource/2e-1.png)


### Kendala

Kendala yang dialami ketika mengerjakan nomor 2 adalah kegagalan dalam mengurutkan tugas yuan dan bubu. Sebelum saya menggunakan array untuk mengurutkan proses pengerjaan task, thread mengerjakan task secara random/acak dan tidak sesuai dengan apa yang soal inginkan. Akhirnya, saya mengakali dengan menggunakan array untuk mengurutkan proses pendownloadan gambar. Yuan menggunakan ascending, sedangkan bubu menggunakan descending dan berhasil.

## 3️⃣ Soal 3
Rama adalah seorang mahasiswa yang mengambil mata kuliah Sistem Operasi. Ia diberikan proyek oleh dosennya untuk mengelola data cuaca dari berbagai kecamatan di Indonesia. Data tersebut tersedia dalam file zip yang bisa didownload disini. Karena suka tantangan, Rama melakukan analisis dengan menggunakan process di linux. Bantulah Rama untuk menyelesaikan tugas ini dengan satu program C bernama weather_manager.c.

**Jawab**
```bash
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>
#include <string.h>

#define LOG_FILE "logbook.txt"
#define ZIP_FILE "weather_data.zip"
#define WEATHER_FOLDER "weather"
#define LOG_FORMAT "[%s] %s\n"

void logbook(const char *message) {
    time_t current_time;
    struct tm *timeinfo;
    char timestamp[80];

    time(&current_time);
    timeinfo = localtime(&current_time);

    strftime(timestamp, sizeof(timestamp), "[%Y-%m-%d %H:%M:%S]", timeinfo);

    FILE *log_file = fopen(LOG_FILE, "a");
    if (log_file == NULL) {
        perror("Error opening log file");
        exit(EXIT_FAILURE);
    }
    fprintf(log_file, LOG_FORMAT, timestamp, message);
    fclose(log_file);
}

void preprocessing() {
    FILE *input_file = fopen("weather/kecamatanforecast.csv", "r");
    FILE *temp_file = fopen("weather/kecamatanforecast_temp.csv", "w");
    if (input_file == NULL || temp_file == NULL) {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }

    char line[512]; // Assuming maximum line length is 512 characters
    fgets(line, sizeof(line), input_file); // Read and ignore header line
    fputs(line, temp_file);

    while (fgets(line, sizeof(line), input_file)) {
        char temp_line[512];
        strcpy(temp_line, line);  // Create a temporary copy of the line for printing

        char *token = strtok(line, ",");
        int column = 1;
        char suhu_udara[10];
        char parameter_udara[10];
        char value_suhu_udara[10];
        while (token != NULL) {
            // Trim newline characters from the token
            char *newline = strchr(token, '\n');
            if (newline != NULL) {
                *newline = '\0';
            }

            if (column == 4) { // Adjust column index to capture "Suhu Udara"
                strcpy(suhu_udara, token);
                strcpy(value_suhu_udara, suhu_udara);
            }
            if (column == 1) { // Adjust column index to capture parameter
                strcpy(parameter_udara, token);
            }
            column++;
            token = strtok(NULL, ",");
        }
        float value_suhu = atof(suhu_udara);
        if (value_suhu <= 35) {
            // Print the line only if suhu <= 35
            // printf("%s", temp_line);

            // Write the line to the temporary file
            fputs(temp_line, temp_file);
        } else {
            char log_message[100];
            sprintf(log_message, "Successfully drop row with parameter %s value %s", parameter_udara, value_suhu_udara);
            logbook(log_message);
            // Skip writing the line to the temporary file
        }
    }

    fclose(input_file);
    fclose(temp_file);

    remove("weather/kecamatanforecast.csv");
    rename("weather/kecamatanforecast_temp.csv", "weather/kecamatanforecast.csv");

    logbook("Data preprocessing completed.");
}



void grouping() {
    // Create folder "city_group" if it doesn't exist
    mkdir("city_group", 0777);

    // Open the preprocessed dataset
    FILE *input_file = fopen("weather/kecamatanforecast.csv", "r");
    if (input_file == NULL) {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }

    char line[512]; // Assuming maximum line length is 512 characters
    fgets(line, sizeof(line), input_file); // Read and ignore header line

    FILE *file_map[10000] = {NULL}; // Array to store file pointers based on loc_id index

    while (fgets(line, sizeof(line), input_file)) {
        if (strlen(line) > 0) {
            char temp_line[sizeof(line)];
            strcpy(temp_line, line);

            char *loc_id = strtok(temp_line, ",");
            if (loc_id == NULL) {
                continue;
            }

            int index = atoi(loc_id) % 10000;

            if (!file_map[index]) {
                char grouped_file_name[256];
                snprintf(grouped_file_name, sizeof(grouped_file_name), "city_group/city_%s.csv", loc_id);
                file_map[index] = fopen(grouped_file_name, "a");
                if (!file_map[index]) {
                    perror("Error opening grouped file");
                    continue;
                }
                // Write the header line for the new city file
                fputs("0,1,6,7,8,9,10\n", file_map[index]); // Assuming the header line format
            }
            // Write the entire line to the corresponding city file
            fputs(line, file_map[index]);
        }
    }

    // Close all opened city files
    for (int i = 0; i < 10000; i++) {
        if (file_map[i]) {
            fclose(file_map[i]);
        }
    }

    fclose(input_file);

    // Log the successful grouping of data
    logbook("Successfully grouped data.\n");
}

int main() {
    pid_t pid;
    int status;

    pid = fork();
    if (pid < 0) {
        perror("Error in fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        execlp("wget", "wget", "-O", ZIP_FILE, "https://drive.google.com/uc?export=download&id=1U1-9v9pJeX5MoofwoBHCcD9nA7YWy_np", NULL);
        perror("Error in execlp");
        exit(EXIT_FAILURE);
    } else {
        wait(&status);
        if (WIFEXITED(status) && !WEXITSTATUS(status)) {
            logbook("Successfully download file..");
            pid = fork();
            if (pid < 0) {
                perror("Error in fork");
                exit(EXIT_FAILURE);
            } else if (pid == 0) {
                execlp("unzip", "unzip", ZIP_FILE, "-d", WEATHER_FOLDER, NULL);
                perror("Error in execlp");
                exit(EXIT_FAILURE);
            } else {
                wait(&status);
                if (WIFEXITED(status) && !WEXITSTATUS(status)) {
                    remove(ZIP_FILE);
                    logbook("Successfully unzip file..");

                    // Perform data preprocessing after extracting the zip file
                    preprocessing();
                    grouping();
                }
            }
        }
    }

    return 0;
}
```
### Problem 3a
Rama harus mendownload dan menyimpan file tersebut ke dalam penyimpanan lokal dan mengekstrak data cuaca tersebut pada folder yang bernama “weather”. Untuk menghemat memori Rama menghapus file zip tersebut.

**Jawab**

```bash

int main() {
    pid_t pid;  // Tipe data untuk menyimpan ID proses
    int status;  // Status untuk memantau hasil fork dan proses child

    pid = fork();  // Buat proses child
    if (pid < 0) {  // Jika fork gagal
        perror("Error in fork");  // Tampilkan pesan kesalahan
        exit(EXIT_FAILURE);  // Keluar jika gagal
    } else if (pid == 0) {  // Proses child
        execlp("wget", "wget", "-O", ZIP_FILE, "https://drive.google.com/uc?export=download&id=1U1-9v9pJeX5MoofwoBHCcD9nA7YWy_np", NULL);  // Unduh file zip
        perror("Error in execlp");  // Tampilkan pesan kesalahan jika gagal
        exit(EXIT_FAILURE);  // Keluar jika gagal
    } else {  // Proses parent
        wait(&status);  // Tunggu proses child selesai
        if (WIFEXITED(status) && !WEXITSTATUS(status)) {  // Jika proses child sukses
            logbook("Successfully download file.");  // Catat log berhasil mengunduh
            pid = fork();  // Buat proses child baru
            if (pid < 0) {
                perror("Error in fork");  // Tampilkan pesan kesalahan jika gagal
                exit(EXIT_FAILURE);  // Keluar jika gagal
            } else if (pid == 0) {  // Proses child
                execlp("unzip", "unzip", ZIP_FILE, "-d", WEATHER_FOLDER, NULL);  // Ekstrak file zip
                perror("Error in execlp");  // Tampilkan pesan kesalahan jika gagal
                exit(EXIT_FAILURE);  // Keluar jika gagal
            } else {  // Proses parent
                wait(&status);  // Tunggu proses child selesai
                if (WIFEXITED(status) && !WEXITSTATUS(status)) {  // Jika proses child sukses
                    remove(ZIP_FILE);  // Hapus file zip setelah diekstrak
                    logbook("Successfully unzip file.");  // Catat log berhasil mengekstrak
                    preprocessing();  // Lakukan pemrosesan data
                    grouping();  // Lakukan pengelompokan data
                }
            }
        }
    }

    return 0;  // Kembali ke sistem operasi
}
```

- Inisialisasi Variabel dan Fork Pertama

```bash
pid_t pid;  // Tipe data untuk menyimpan ID proses
int status;  // Variabel untuk memantau status proses child

pid = fork();  // Buat proses child
if (pid < 0) {  // Jika fork gagal
    perror("Error in fork");  // Tampilkan pesan kesalahan
    exit(EXIT_FAILURE);  // Keluar jika gagal
} 
```
Di bagian ini, program membuat proses baru (child) menggunakan fungsi fork(). Jika fork() gagal, akan ditampilkan pesan kesalahan dan program keluar dengan EXIT_FAILURE. Jika berhasil, variabel pid akan mengandung ID proses child.

 - Proses child pertama
 ```bash
    else if (pid == 0) {  // Jika ini proses child
    execlp("wget", "wget", "-O", ZIP_FILE, "https://drive.google.com/uc?export=download&id=1U1-9v9pJeX5MoofwoBHCcD9nA7YWy_np", NULL);  // Unduh file zip
    perror("Error in execlp");  // Tampilkan pesan kesalahan jika gagal
    exit(EXIT_FAILURE);  // Keluar jika gagal
}
 ``` 
 Jika pid adalah 0, berarti ini adalah proses child. Dalam kasus ini, program akan menjalankan execlp untuk mengunduh file dari URL yang diberikan menggunakan wget. Jika execlp gagal, akan ditampilkan pesan kesalahan, dan proses child keluar dengan EXIT_FAILURE.

- Proses Parent Menunggu Child Pertama
```bash 
    else {  // Jika ini adalah proses parent
    wait(&status);  // Tunggu proses child selesai
    if (WIFEXITED(status) && !WEXITSTATUS(status)) {  // Jika child berhasil
        logbook("Successfully download file.");  // Catat log berhasil mengunduh
```
Jika pid bukan 0, berarti ini adalah proses parent. Proses parent menggunakan wait(&status) untuk menunggu proses child selesai. Jika child berhasil (diperiksa dengan WIFEXITED(status) dan !WEXITSTATUS(status)), akan dicatat log bahwa pengunduhan berhasil.

- Fork Kedua dan Proses Child Kedua
```bash 
pid = fork();  // Buat proses child baru
if (pid < 0) {  // Jika fork gagal
    perror("Error in fork");  // Tampilkan pesan kesalahan jika gagal
    exit(EXIT_FAILURE);  // Keluar jika gagal
} else if (pid == 0) {  // Jika ini proses child
    execlp("unzip", "unzip", ZIP_FILE, "-d", WEATHER_FOLDER, NULL);  // Ekstrak file zip
    perror("Error in execlp");  // Tampilkan pesan kesalahan jika gagal
    exit(EXIT_FAILURE);  // Keluar jika gagal
}

```
Setelah pengunduhan berhasil, parent membuat child baru untuk mengekstrak file zip. Seperti sebelumnya, jika fork() gagal, program keluar dengan pesan kesalahan. Jika berhasil dan ini adalah proses child, execlp digunakan untuk menjalankan perintah unzip. Jika gagal, akan ada pesan kesalahan, dan proses child keluar.

- Parent Menunggu Child Kedua dan Tindakan Setelah Ekstraksi
```bash 
else {  // Jika ini adalah proses parent
    wait(&status);  // Tunggu proses child selesai
    if (WIFEXITED(status) && !WEXITSTATUS(status)) {  // Jika child berhasil
        remove(ZIP_FILE);  // Hapus file zip setelah diekstrak
        logbook("Successfully unzip file.");  // Catat log berhasil mengekstrak
        preprocessing();  // Lakukan pemrosesan data
        grouping();  // Lakukan pengelompokan data
    }
}
```
Jika proses parent, ini akan menunggu proses child kedua selesai. Jika proses child kedua berhasil, file zip yang diekstrak akan dihapus dengan remove(ZIP_FILE). Log juga dicatat untuk menunjukkan keberhasilan ekstraksi. Setelah itu, fungsi preprocessing() dijalankan untuk melakukan pemrosesan data dan grouping() untuk pengelompokan data.



> [Format : Penjelasan (sesuaikan dengan style modul 1), Dokumentasi]

### Problem 3b
Setelah berhasil, Rama melakukan data preprocessing dengan cara mencari data yang tidak masuk akal pada kolom “Suhu Udara” dan melakukan drop pada row tersebut. Lalu update dataset “kecamatanforecast.csv”.

**Jawab**


 - ```bash
    void preprocessing() {
    FILE *input_file = fopen("weather/kecamatanforecast.csv", "r");  // Buka file input
    FILE *temp_file = fopen("weather/kecamatanforecast_temp.csv", "w");  // Buka file sementara
    if (input_file == NULL || temp_file == NULL) {
        perror("Error opening input file");  // Tampilkan pesan kesalahan jika gagal
        exit(EXIT_FAILURE);  // Keluar jika gagal
    }

    char line[512];  // Buffer untuk membaca baris
    fgets(line, sizeof(line), input_file);  // Baca dan abaikan baris header
    fputs(line, temp_file);  // Tulis header ke file sementara

    while (fgets(line, sizeof(line), input_file)) {  // Baca setiap baris dari file input
        char temp_line[512];  // Buffer untuk mencatat baris sementara
        strcpy(temp_line, line);  // Buat salinan sementara dari baris

        char *token = strtok(line, ",");  // Tokenisasi dengan delimiter ","
        int column = 1;  // Variabel untuk melacak kolom
        char suhu_udara[10];
        char parameter_udara[10];
        char value_suhu_udara[10];
        
        while (token != NULL) {  // Iterasi melalui token
            char *newline = strchr(token, '\n');  // Temukan karakter newline dan hapus
            if (newline != NULL) {
                *newline = '\0';
            }

            if (column == 4) {  // Kolom suhu udara
                strcpy(suhu_udara, token);  // Simpan suhu udara
                strcpy(value_suhu_udara, suhu_udara);
            }
            if (column == 1) {  // Kolom parameter
                strcpy(parameter_udara, token);
            }
            column++;  // Naikkan indeks kolom
            token = strtok(NULL, ",");  // Ambil token berikutnya
        }

        float value_suhu = atof(suhu_udara);  // Konversi suhu udara menjadi float
        if (value_suhu <= 35) {  // Hanya tulis jika suhu kurang dari atau sama dengan 35
            fputs(temp_line, temp_file);  // Tulis baris ke file sementara
        } else {
            char log_message[100];  // Buffer untuk pesan log
            sprintf(log_message, "Successfully drop row with parameter %s value %s", parameter_udara, value_suhu_udara);  // Buat pesan log
            logbook(log_message);  // Catat pesan log
        }
    }

    fclose(input_file);  // Tutup file input
    fclose(temp_file);  // Tutup file sementara

    remove("weather/kecamatanforecast.csv");  // Hapus file lama
    rename("weather/kecamatanforecast_temp.csv", "weather/kecamatanforecast.csv");  // Ganti nama file sementara ke nama file asli

    logbook("Data preprocessing completed.");  // Catat log selesai pemrosesan
}

 - Bagian pembuka file
```bash 
FILE *input_file = fopen("weather/kecamatanforecast.csv", "r");  // Buka file input
FILE *temp_file = fopen("weather/kecamatanforecast_temp.csv", "w");  // Buka file sementara
if (input_file == NULL atau temp_file == NULL) {
    perror("Error opening input file");  // Pesan kesalahan jika file tidak bisa dibuka
    exit(EXIT_FAILURE);  // Keluar jika gagal membuka file
}
```
Bagian ini membuka file CSV yang ingin diproses dan file sementara untuk menyimpan hasil pemrosesan. Jika salah satu file tidak dapat dibuka, program akan menampilkan pesan kesalahan dan berhenti.

 - Bagian Membaca dan Menulis Header
```bash 
char line[512];  // Buffer untuk membaca baris
fgets(line, sizeof(line), input_file);  // Baca baris pertama (header)
fputs(line, temp_file);  // Tulis baris header ke file sementara
```
Bagian ini membaca baris pertama dari file CSV yang biasanya adalah header, dan menulisnya ke file sementara. Buffer berukuran 512 karakter digunakan untuk menyimpan data yang dibaca dari file.


 - Bagian pemrosesan baris
```bash 
while (fgets(line, sizeof(line), input_file)) {  // Baca setiap baris dari file input
    char temp_line[512];  // Buffer untuk menyimpan baris sementara
    strcpy(temp_line, line);  // Buat salinan sementara dari baris

    char *token = strtok(line, ",");  // Tokenisasi baris menggunakan koma sebagai delimiter
    int column = 1;  // Variabel untuk melacak kolom
    char suhu_udara[10];  // Variabel untuk menyimpan suhu udara
    char parameter_udara[10];  // Variabel untuk menyimpan parameter udara
    char value_suhu_udara[10];  // Variabel untuk menyimpan nilai suhu udara


```
Bagian ini memulai loop untuk membaca setiap baris dari file input. Baris pertama dilewati karena sudah ditulis ke file sementara. Salinan baris dibuat untuk digunakan jika perlu menulis ulang ke file sementara. Kemudian, setiap baris dipecah (tokenisasi) berdasarkan koma, yang merupakan pemisah antar-kolom.


 - Bagian ekstraksi dan penyaringan data
```bash 
    while (token != NULL) {  // Iterasi melalui setiap token
        char *newline = strchr(token, '\n');  // Hapus karakter newline jika ada
        if (newline != NULL) {
            *newline = '\0';
        }

        if (column == 4) {  // Jika ini adalah kolom ke-4 (suhu udara)
            strcpy(suhu_udara, token);  // Simpan suhu udara
            strcpy(value_suhu_udara, suhu_udara);
        }

        if (column == 1) {  // Jika ini adalah kolom pertama (parameter udara)
            strcpy(parameter_udara, token);  // Simpan parameter udara
        }

        column++;  // Naikkan indeks kolom
        token = strtok(NULL, ",");  // Ambil token berikutnya
    }

    float value_suhu = atof(suhu_udara);  // Ubah suhu udara menjadi tipe float
    if (value_suhu <= 35) {  // Jika suhu kurang dari atau sama dengan 35
        fputs(temp_line, temp_file);  // Tulis baris ke file sementara
    } else {  // Jika suhu lebih dari 35
        char log_message[100];  // Buat pesan log
        sprintf(log_message, "Successfully drop row with parameter %s value %s", parameter_udara, value_suhu_udara);  // Tulis pesan log
        logbook(log_message);  // Catat pesan log
    }
}

```
Bagian ini mengambil setiap baris yang dibaca dan mengekstrak nilai kolom ke-4 (suhu udara) dan kolom pertama (parameter udara). Jika suhu udara lebih dari 35 derajat, baris tersebut dihapus, dan pesan log dicatat untuk mencatat penghapusan ini. Jika suhu udara kurang dari atau sama dengan 35 derajat, baris tersebut ditulis ke file sementara.



 - Bagian penyelesaian pemrosesan
```bash 
fclose(input_file);  // Tutup file input
fclose(temp_file);  // Tutup file sementara

remove("weather/kecamatanforecast.csv");  // Hapus file lama
rename("weather/kecamatanforecast_temp.csv", "weather/kecamatanforecast.csv");  // Ganti nama file sementara menjadi file asli

logbook("Data preprocessing completed.");  // Catat log selesai pemrosesan

```
Bagian ini menutup file input dan file sementara. Setelah pemrosesan selesai, file input lama dihapus dan file sementara diganti namanya menjadi file asli. Setelah semua langkah selesai, pesan log dicatat untuk mencatat bahwa pemrosesan data telah selesai.


> [Format : Penjelasan (sesuaikan dengan style modul 1), Dokumentasi]

### Problem 3c
Kemudian dari dataset tersebut, Rama mengelompokkannya berdasarkan ID Lokasi dan memisahkannya menjadi dataset baru dengan format nama “city_{ID Lokasi}” contoh “city_5012725.csv”. Semua dataset baru tersebut dimasukan ke dalam folder bernama “city_group”.

**Jawab**

```bash
void grouping() {
    mkdir("city_group", 0777);  // Buat folder "city_group" jika belum ada

    FILE *input_file = fopen("weather/kecamatanforecast.csv", "r");  // Buka file input
    if (input_file == NULL) {
        perror("Error opening input file");  // Tampilkan pesan kesalahan jika gagal
        exit(EXIT_FAILURE);  // Keluar jika gagal
    }

    char line[512];  // Buffer untuk membaca baris
    fgets(line, sizeof(line), input_file);  // Baca dan abaikan baris header

    FILE *file_map[10000] = {NULL};  // Array untuk menyimpan penunjuk file berdasarkan indeks loc_id

    while (fgets(line, sizeof(line), input_file)) {  // Baca setiap baris dari file input
        if (strlen(line) > 0) {  // Jika baris tidak kosong
            char temp_line[sizeof(line)];
            strcpy(temp_line, line);

            char *loc_id = strtok(temp_line, ",");  // Dapatkan loc_id
            if (loc_id == NULL) {
                continue;  // Jika tidak ada loc_id, lanjutkan
            }

            int index = atoi(loc_id) % 10000;  // Hitung indeks untuk file_map

            if (!file_map[index]) {  // Jika file belum dibuka
                char grouped_file_name[256];  // Buffer untuk nama file yang dikelompokkan
                snprintf(grouped_file_name, sizeof(grouped_file_name), "city_group/city_%s.csv", loc_id);  // Buat nama file berdasarkan loc_id
                file_map[index] = fopen(grouped_file_name, "a");  // Buka file
                if (!file_map[index]) {
                    perror("Error opening grouped file");  // Tampilkan pesan kesalahan jika gagal
                    continue;
                }
                fputs("0,1,6,7,8,9,10\n", file_map[index]);  // Tulis header ke file baru
            }
            fputs(line, file_map[index]);  // Tulis baris ke file yang sesuai
        }
    }

    // Tutup semua file yang dibuka
    for (int i = 0; i < 10000; i++) {
        if (file_map[i]) {
            fclose(file_map[i]);  // Tutup file
        }
    }

    fclose(input_file);  // Tutup file input

    logbook("Successfully grouped data.\n");  // Catat log selesai pengelompokan
}
```

- Pembuatan Folder city_group
```bash 
mkdir("city_group", 0777);  // Buat folder "city_group" jika belum ada
```
Pada bagian ini, folder bernama city_group dibuat dengan izin 0777, yang berarti folder tersebut dapat diakses dan dimodifikasi oleh semua pengguna. Jika folder sudah ada, mkdir tidak akan membuat yang baru.

- Membuka File Input
```bash 
FILE *input_file = fopen("weather/kecamatanforecast.csv", "r");  // Buka file input
if (input_file == NULL) {
    perror("Error opening input file");  // Tampilkan pesan kesalahan jika gagal
    exit(EXIT_FAILURE);  // Keluar jika gagal
}
```
Bagian ini membuka file CSV yang berisi data cuaca. Jika file tidak ditemukan atau tidak dapat dibuka, fungsi perror akan menampilkan pesan kesalahan dan program akan keluar dengan kode kesalahan.


- Membaca Header File CSV
```bash 
char line[512];  // Buffer untuk membaca baris
fgets(line, sizeof(line), input_file);  // Baca dan abaikan baris header
```
Bagian ini membaca baris pertama (header) dari file CSV dan mengabaikannya. Buffer line digunakan untuk menyimpan baris yang dibaca.

- Inisialisasi Array Penunjuk File
```bash 
FILE *file_map[10000] = {NULL};  // Array untuk menyimpan penunjuk file berdasarkan indeks loc_id
```
Bagian ini menginisialisasi array yang akan menyimpan penunjuk file berdasarkan indeks loc_id. Indeks didasarkan pada nilai loc_id yang akan dihitung kemudian.

- Pengelompokan Data Berdasarkan loc_id
```bash 
while (fgets(line, sizeof(line), input_file)) {  // Baca setiap baris dari file input
    if (strlen(line) > 0) {  // Jika baris tidak kosong
        char temp_line[sizeof(line)];
        strcpy(temp_line, line);

        char *loc_id = strtok(temp_line, ",");  // Dapatkan loc_id
        if (loc_id == NULL) {
            continue;  // Jika tidak ada loc_id, lanjutkan
        }

        int index = atoi(loc_id) % 10000;  // Hitung indeks untuk file_map

        if (!file_map[index]) {  // Jika file belum dibuka
            char grouped_file_name[256];  // Buffer untuk nama file yang dikelompokkan
            snprintf(grouped_file_name, sizeof(grouped_file_name), "city_group/city_%s.csv", loc_id);  // Buat nama file berdasarkan loc_id
            file_map[index] = fopen(grouped_file_name, "a");  // Buka file
            if (!file_map[index]) {
                perror("Error opening grouped file");  // Tampilkan pesan kesalahan jika gagal
                continue;
            }
            fputs("0,1,6,7,8,9,10\n", file_map[index]);  // Tulis header ke file baru
        }
        fputs(line, file_map[index]);  // Tulis baris ke file yang sesuai
    }
}
```
Bagian ini membaca setiap baris dari file CSV. Jika baris tidak kosong, ia menyalin baris ke temp_line dan memecahnya dengan delimiter , untuk mendapatkan loc_id. Jika loc_id ada, indeks dihitung dengan operasi modulo 10.000 (atoi(loc_id) % 10000), sehingga nilai indeks berada dalam rentang 0-9.999.

Jika file untuk indeks tersebut belum ada, maka file baru dibuat di dalam folder city_group dengan nama berdasarkan loc_id. Jika file gagal dibuka, kesalahan ditampilkan dan iterasi dilanjutkan ke baris berikutnya. Jika file berhasil dibuka, header standar ditulis ke file tersebut. Baris kemudian ditulis ke file yang sesuai berdasarkan indeks.

- Penutupan File dan Logging
```bash 
// Tutup semua file yang dibuka
for (int i = 0; i < 10000; i++) {
    if (file_map[i]) {
        fclose(file_map[i]);  // Tutup file
    }
}

fclose(input_file);  // Tutup file input

logbook("Successfully grouped data.\n");  // Catat log selesai pengelompokan

```
Bagian ini menutup semua file yang telah dibuka untuk menghindari kebocoran memori dan file yang terbuka. Penutupan dilakukan dengan iterasi melalui seluruh array file_map dan menutup file jika tidak NULL. Setelah itu, file input juga ditutup. Terakhir, pesan log dicatat untuk menandai bahwa pengelompokan telah berhasil dilakukan.

> [Format : Penjelasan (sesuaikan dengan style modul 1), Dokumentasi]

### Problem 3d
Rama harus membuat logbook untuk laporan kepada dosen, sehingga semua activities diatas harus disimpan dalam “logbook.txt” dengan format { [YYYY-MM-dd HH:mm:ss] [message]}.

**Jawab**

```bash
void logbook(const char *message) {
    time_t current_time;
    struct tm *timeinfo;
    char timestamp[80];  // Array untuk menyimpan stempel waktu

    time(&current_time);  // Dapatkan waktu saat ini
    timeinfo = localtime(&current_time);  // Konversi ke waktu lokal

    strftime(timestamp, sizeof(timestamp), "[%Y-%m-%d %H:%M:%S]", timeinfo);  // Format stempel waktu

    FILE *log_file = fopen(LOG_FILE, "a");  // Buka file log untuk penambahan
    if (log_file == NULL) {
        perror("Error opening log file");  // Tampilkan pesan kesalahan jika gagal
        exit(EXIT_FAILURE);  // Keluar dari program jika gagal
    }
    fprintf(log_file, LOG_FORMAT, timestamp, message);  // Tulis pesan log dengan stempel waktu
    fclose(log_file);  // Tutup file log
}
```

- Menginisiasi Variabel dan Mendapatkan Waktu Saat Ini
```bash 
time_t current_time;  // Menyimpan waktu saat ini dalam format time_t
struct tm *timeinfo;  // Menyimpan informasi waktu yang terstruktur
char timestamp[80];  // Buffer untuk menyimpan stempel waktu

time(&current_time);  // Mendapatkan waktu saat ini dalam format time_t
timeinfo = localtime(&current_time);  // Mengkonversi waktu ke waktu lokal yang terstruktur
```
Pada bagian ini, variabel yang diperlukan untuk mencatat waktu dan stempel waktu diinisiasi. time(&current_time) digunakan untuk mendapatkan waktu saat ini dalam format time_t, dan localtime(&current_time) mengkonversi waktu tersebut ke struktur waktu lokal yang lebih mudah dimanipulasi.

- Penutupan File dan Logging
```bash 
strftime(timestamp, sizeof(timestamp), "[%Y-%m-%d %H:%M:%S]", timeinfo);  // Format stempel waktu

```
Stempel waktu dibuat menggunakan strftime. Ini mengubah waktu yang terstruktur menjadi string yang diformat sesuai dengan pola yang diberikan. Pola yang digunakan di sini adalah [tahun-bulan-hari jam:menit:detik], sehingga memberikan waktu yang dapat dibaca.

- Membuka File Log untuk Penambahan
```bash 
FILE *log_file = fopen(LOG_FILE, "a");  // Buka file log dalam mode penambahan
if (log_file == NULL) {
    perror("Error opening log file");  // Tampilkan pesan kesalahan jika file gagal dibuka
    exit(EXIT_FAILURE);  // Keluar dari program jika gagal
}

```
Di bagian ini, file log dibuka dalam mode penambahan dengan fopen(LOG_FILE, "a"). Mode penambahan berarti data baru akan ditambahkan di akhir file tanpa menghapus data yang ada. Jika file tidak dapat dibuka, perror menampilkan pesan kesalahan, dan exit(EXIT_FAILURE) keluar dari program untuk mencegah kerusakan lebih lanjut.

- Menulis Pesan Log dan Menutup File

```bash 
fprintf(log_file, LOG_FORMAT, timestamp, message);  // Tulis pesan log dengan stempel waktu
fclose(log_file);  // Tutup file log
```
Bagian ini menulis pesan log ke dalam file dengan format yang sudah ditentukan, termasuk stempel waktu dan pesan yang diterima sebagai argumen. fprintf digunakan untuk menulis ke file dengan format tertentu. Setelah pesan ditulis, file ditutup menggunakan fclose untuk memastikan semua data disimpan dengan benar dan untuk mencegah kebocoran sumber daya.





> [Format : Penjelasan (sesuaikan dengan style modul 1), Dokumentasi]

### Kendala

> [Format : Penjelasan Kendala + Solusi (jika ada)]

## 4️⃣ Soal 4
Sebagai mahasiswa baru informatika, satria memiliki ide untuk membuat game tic-tac-toe yang berjalan di terminal. Tic-tac-toe merupakan permainan dengan 2 pemain yang secara bergantian menempatkan tanda 'X' atau 'O' hingga membentuk garis horizontal, vertikal, atau diagonal. Untuk membuat game ini, satria membutuhkan 2 program c, yaitu game.c dan player.c
### Problem 4a
Pengguna dapat memilih tipe pemain (‘X’ atau ‘O’) berdasarkan perintah dari program ‘player.c’. Jika tipe telah dipilih oleh pengguna sebelumnya, beri pesan " has already been taken, choose other player types". Informasi terkait apakah tipe pemain telah dipilih atau tidak, dapat diakses melalui shared memory.

**Jawab**

> [Format : Penjelasan (sesuaikan dengan style modul 1), Dokumentasi]

### Problem 4b
Karena berjalan di terminal, program ini menggunakan nomor kotak (1-9) yang berurutan dari kiri atas ke kanan bawah untuk memudahkan pemilihan kotak. Program 'player.c' akan menampilkan kotak tic tac toe saat ini dan meminta input pengguna berupa nomor kotak sesuai gilirannya (player 'X' memulai terlebih dahulu). Input yang diberikan pengguna kemudian dikirimkan ke 'game.c' menggunakan message queue.

**Jawab**

> [Format : Penjelasan (sesuaikan dengan style modul 1), Dokumentasi]

### Problem 4c
Selanjutnya, ‘game.c’ harus mengecek apakah input user sesuai atau tidak berdasarkan nilai kotak permainan saat ini. Kirimkan pesan error kepada pengguna jika nomor kotak yang dipilih tidak kosong atau diluar angka 1-9. Pesan dapat dikirimkan melalui message queue dengan ‘mesg_type’ yang berbeda.

**Jawab**

> [Format : Penjelasan (sesuaikan dengan style modul 1), Dokumentasi]

### Problem 4d
Jika input valid, ‘game.c’ perlu mengubah nilai pada kotak tic-tac-toe dan memutuskan apakah permainan telah selesai atau masih berlanjut. Permainan telah selesai jika kotak telah terisi semua (seri) atau terdapat simbol (‘X’ atau ‘O’) yang membentuk garis vertikal, horizontal, maupun diagonal. Kotak tic tac toe yang telah diperbarui dan hasil akhir permainan (jika sudah berakhir) dikirimkan ke 'player.c' untuk kemudian ditampilkan kepada pengguna.

**Jawab**

> [Format : Penjelasan (sesuaikan dengan style modul 1), Dokumentasi]

### Kendala

> [Format : Penjelasan Kendala + Solusi (jika ada)]

<div align=center>

# ------- SELESAI -------
</div>
