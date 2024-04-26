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

> [Format : Penjelasan (sesuaikan dengan style modul 1), Dokumentasi]

### Problem 2b
 Yuan dan Bubu membagi tugas secara efisien dengan mengerjakannya secara bersamaan (overlapping) dan membaginya sama banyak. Yuan akan mengerjakan task dari awal, sementara Bubu dari akhir. Misalnya, jika ada 20 task, Yuan akan memulai dari task0-task1-dst dan Bubu akan memulai dari task19-task18-dst. Lalu buatlah file “recap.txt” yang menyimpan log setiap kali mereka selesai melakukan task (kriteria setiap task akan dijelaskan di poin c).

**Jawab**

> [Format : Penjelasan (sesuaikan dengan style modul 1), Dokumentasi]

### Problem 2c
Yuan dan Bubu menemukan petunjuk dalam setiap file task txt dengan format "{jumlah} {resolusi} {kategori}". Mereka harus mengunduh gambar sesuai dengan jumlah, resolusi, dan kategori yang disebutkan. Contohnya, jika task0 berisi "5 250x150 music", mereka mengunduh 5 gambar berukuran 250x150 dalam kategori music. Gambar diunduh dari unsplash.com dan disimpan bersama file txt setiap task berdasarkan orang yang mengerjakan task.

**Jawab**

> [Format : Penjelasan (sesuaikan dengan style modul 1), Dokumentasi]

### Problem 2d
Setelah semua tugas selesai, mereka menambahkan jumlah gambar dari setiap kategori ke dalam "recap.txt". dan pada baris terakhir tambahkan jumlah seluruh gambar yg di unduh.

**Jawab**

> [Format : Penjelasan (sesuaikan dengan style modul 1), Dokumentasi]

### Problem 2e
Karena tugas sudah siap dikumpulkan, zip folder Yuan dan Bubu menjadi satu zip bernamakan submission.zip dan JANGAN HAPUS folder aslinya.

**Jawab**

> [Format : Penjelasan (sesuaikan dengan style modul 1), Dokumentasi]

### Kendala

> [Format : Penjelasan Kendala + Solusi (jika ada)]

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
