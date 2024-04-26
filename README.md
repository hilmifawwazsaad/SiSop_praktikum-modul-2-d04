[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/o3jj1gXA)
<div align=center>

|    NRP     |      Name      |
| :--------: | :------------: |
| 5025221000 | Student 1 Name |
| 5025221000 | Student 2 Name |
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
### Problem 1a
Buatlah fungsi HorseFetcher, yang bertugas mengambil data kuda dari internet dan menyimpannya dalam file dengan nama horse_1.txt, horse_2.txt, horse_3.txt, dan seterusnya. Kuda dan pemilik kuda dipisahkan dengan “:”, sebagai contoh “Morioh:DJumanto”, DJumanto adalah pemilik dari kuda Morioh.

**Jawab**

> [Format : Penjelasan (sesuaikan dengan style modul 1), Dokumentasi]

### Problem 1b
Buatlah fungsi HorseRaceHooray, yang bertugas melaksanakan balapan kuda. Parameter yang diguakan adalah salah satu file kuda yang telah kalian fetch sebelumnya. Gunakan thread untuk menjalankan perlombaan tersebut. Setiap kuda akan melakukan perhitungan bilangan prima dengan angka acak antara 1 sampai 4000. Kuda yang menyelesaikan perhitungan lebih awal akan menempati posisi lebih tinggi dari kuda lainnya. Setelah perlombaan selesai, simpan hasil lomba dalam format HorseRace_(nomor lomba).txt.

**Jawab**

> [Format : Penjelasan (sesuaikan dengan style modul 1), Dokumentasi]

### Problem 1c
Buatlah fungsi HorseChampionLeaderboard, yang bertugas menampilkan pemenang dari pertandingan-pertandingan sebelumnya. Berikan opsi kepada pengguna untuk memilih lomba mana yang ingin dilihat hasilnya.

**Jawab**

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
### Problem 3a
Rama harus mendownload dan menyimpan file tersebut ke dalam penyimpanan lokal dan mengekstrak data cuaca tersebut pada folder yang bernama “weather”. Untuk menghemat memori Rama menghapus file zip tersebut.

**Jawab**

> [Format : Penjelasan (sesuaikan dengan style modul 1), Dokumentasi]

### Problem 3b
Setelah berhasil, Rama melakukan data preprocessing dengan cara mencari data yang tidak masuk akal pada kolom “Suhu Udara” dan melakukan drop pada row tersebut. Lalu update dataset “kecamatanforecast.csv”.

**Jawab**

> [Format : Penjelasan (sesuaikan dengan style modul 1), Dokumentasi]

### Problem 3c
Kemudian dari dataset tersebut, Rama mengelompokkannya berdasarkan ID Lokasi dan memisahkannya menjadi dataset baru dengan format nama “city_{ID Lokasi}” contoh “city_5012725.csv”. Semua dataset baru tersebut dimasukan ke dalam folder bernama “city_group”.

**Jawab**

> [Format : Penjelasan (sesuaikan dengan style modul 1), Dokumentasi]

### Problem 3d
Rama harus membuat logbook untuk laporan kepada dosen, sehingga semua activities diatas harus disimpan dalam “logbook.txt” dengan format { [YYYY-MM-dd HH:mm:ss] [message]}.

**Jawab**

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
