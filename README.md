# Sisop-4-2024-MH-IT26

## ***KELOMPOK IT 26***
  | Nama      | NRP         |
  |-----------|-------------|
  | Rafael Jonathan Arnoldus   | 5027231006  |
  | Muhammad Abhinaya Al-Faruqi  | 5027231011  |  
  | Danendra Fidel Khansa  | 5027231063  |

## Bagian Pengerjaan Soal 
+ Soal 1 = Rafael Jonathan Arnoldus
+ Soal 2 = Muhammad Abhinaya Al-Faruqi
+ Soal 3 = Danendra Fidel Khansa

## ***MODUL 4***
  Berikut untuk struktur repository pengumpulan dari hasil praktikum sistem operasi modul 4 :
```
—soal_1:
        — inikaryakita.c
                                    
—soal_2:
        — pastibisa.c
				
—soal_3:
        — archeology.c
```				

## ***SOAL 1 (Rafael)***

Adfi merupakan seorang CEO agency creative bernama Ini Karya Kita. Ia sedang melakukan inovasi pada manajemen project photography Ini Karya Kita. Salah satu ide yang dia kembangkan adalah tentang pengelolaan foto project dalam sistem arsip Ini Karya Kita. Dalam membangun sistem ini, Adfi tidak bisa melakukannya sendirian, dia perlu bantuan mahasiswa Departemen Teknologi Informasi angkatan 2023 untuk membahas konsep baru yang akan mengubah project fotografinya lebih menarik untuk dilihat. Adfi telah menyiapkan portofolio hasil project fotonya yang bisa didownload dan diakses di `www.inikaryakita.id` . Silahkan eksplorasi web Ini Karya Kita dan temukan halaman untuk bisa mendownload projectnya. Setelah kalian download terdapat folder **gallery** dan **bahaya**.

- Pada folder “**gallery**”:
  + Membuat folder dengan prefix "**wm**." Dalam folder ini, setiap gambar yang dipindahkan ke dalamnya akan diberikan watermark bertuliskan **inikaryakita.id**.

**Ex**: "mv ikk.jpeg wm-foto/" 
```
Output: 
Before: (tidak ada watermark bertuliskan inikaryakita.id)
```
![Cuplikan layar 2024-05-23 193329](https://github.com/Rafjonath/Sisop-4-2024-MH-IT26/assets/150430084/ca946189-bc39-41e9-bda0-59fab43b80bd)
```
After: (terdapat watermark tulisan inikaryakita.id)
```
![Cuplikan layar 2024-05-23 193336](https://github.com/Rafjonath/Sisop-4-2024-MH-IT26/assets/150430084/bae8ca21-7743-495e-8d1d-a3b79d668bf1)

- Pada folder "**bahaya**," terdapat file bernama "script.sh." Adfi menyadari pentingnya menjaga keamanan dan integritas data dalam folder ini. 

  + Mereka harus mengubah permission pada file "script.sh" agar bisa dijalankan, karena jika dijalankan maka dapat menghapus semua dan isi dari  "gallery"
  + Adfi dan timnya juga ingin menambahkan fitur baru dengan membuat file dengan prefix "test" yang ketika disimpan akan mengalami pembalikan (reverse) isi dari file tersebut.

 ## ***PENGERJAAN***

 ## ***PENJELASAN PENGERJAAN***

 ## ***Dokumentasi***

## ***SOAL 2 (Abhinaya)***

Masih dengan Ini Karya Kita, sang CEO ingin melakukan tes keamanan pada folder sensitif Ini Karya Kita. Karena Teknologi Informasi merupakan departemen dengan salah satu fokus di Cyber Security, maka dia kembali meminta bantuan mahasiswa Teknologi Informasi angkatan 2023 untuk menguji dan mengatur keamanan pada folder sensitif tersebut. Untuk mendapatkan folder sensitif itu, mahasiswa IT 23 harus kembali mengunjungi website Ini Karya Kita pada `www.inikaryakita.id/schedule` . Silahkan isi semua formnya, tapi pada form subject isi dengan nama kelompok_SISOP24 , ex: **IT01_SISOP24** . Lalu untuk form Masukkan Pesanmu, ketik “**Mau Foldernya**” . Tunggu hingga 1x24 jam, maka folder sensitif tersebut akan dikirimkan melalui email kalian. Apabila folder tidak dikirimkan ke email kalian, maka hubungi sang `CEO` untuk meminta bantuan.

- Pada folder "**pesan**" Adfi ingin meningkatkan kemampuan sistemnya dalam mengelola berkas-berkas teks dengan menggunakan **fuse**.
  + Jika sebuah file memiliki prefix "**base64**," maka sistem akan langsung mendekode isi file tersebut dengan algoritma Base64.
  + Jika sebuah file memiliki prefix "**rot13**," maka isi file tersebut akan langsung di-decode dengan algoritma ROT13.
  + Jika sebuah file memiliki prefix "**hex**," maka isi file tersebut akan langsung di-decode dari representasi heksadesimalnya.
  + Jika sebuah file memiliki prefix "**rev**," maka isi file tersebut akan langsung di-decode dengan cara membalikkan teksnya.
  + Contoh:

    File yang belum didecode/ dienkripsi rot_13
![Cuplikan layar 2024-05-23 194421](https://github.com/Rafjonath/Sisop-4-2024-MH-IT26/assets/150430084/a10b739f-47d5-4615-aff6-59a506091a9a)

    File yang sudah didecode/ dienkripsi rot_13
![Cuplikan layar 2024-05-23 194429](https://github.com/Rafjonath/Sisop-4-2024-MH-IT26/assets/150430084/a6513c9f-75ed-4a2e-81f4-fa035ec99f8c)

- Pada folder “**rahasia-berkas**”, Adfi dan timnya memutuskan untuk menerapkan kebijakan khusus. Mereka ingin memastikan bahwa folder dengan prefix "rahasia" tidak dapat diakses tanpa izin khusus. 
  + Jika seseorang ingin mengakses folder dan file pada “rahasia”, mereka harus memasukkan sebuah password terlebih dahulu (password bebas). 
- Setiap proses yang dilakukan akan tercatat pada **logs-fuse.log** dengan format :

**[SUCCESS/FAILED]::dd/mm/yyyy-hh:mm:ss::[tag]::[information]**
```
Ex:
[SUCCESS]::01/11/2023-10:43:43::[moveFile]::[File moved successfully]
```

 ## ***PENGERJAAN***

 ## ***PENJELASAN PENGERJAAN***

 ## ***Dokumentasi***

 ## ***SOAL 3 (Fidel)***

 Seorang arkeolog menemukan sebuah gua yang didalamnya tersimpan banyak relik dari zaman praaksara, sayangnya semua barang yang ada pada gua tersebut memiliki bentuk yang terpecah belah akibat bencana yang tidak diketahui. Sang arkeolog ingin menemukan cara cepat agar ia bisa menggabungkan relik-relik yang terpecah itu, namun karena setiap pecahan relik itu masih memiliki nilai tersendiri, ia memutuskan untuk membuat sebuah file system yang mana saat ia mengakses file system tersebut ia dapat melihat semua relik dalam keadaan utuh, sementara relik yang asli tidak berubah sama sekali.

Ketentuan :

a. Buatlah sebuah direktori dengan ketentuan seperti pada tree berikut
```
.
├── [nama_bebas]
├── relics
│   ├── relic_1.png.000
│   ├── relic_1.png.001
│   ├── dst dst…
│   └── relic_9.png.010
└── report
```
b. Direktori [**nama_bebas**] adalah direktori FUSE dengan direktori asalnya adalah direktori relics. Ketentuan Direktori [**nama_bebas**] adalah sebagai berikut :
   - Ketika dilakukan listing, isi dari direktori [**nama_bebas**] adalah semua relic dari relics yang telah tergabung.
     ![Cuplikan layar 2024-05-23 195252](https://github.com/Rafjonath/Sisop-4-2024-MH-IT26/assets/150430084/56b17690-64e1-4492-aaa6-2c501e48587f)
   -Ketika dilakukan copy (dari direktori [**nama_bebas**] ke tujuan manapun), file yang disalin adalah file dari direktori **relics** yang sudah tergabung.
    ![Cuplikan layar 2024-05-21 004410](https://github.com/Rafjonath/Sisop-4-2024-MH-IT26/assets/150430084/951977d7-3300-444d-9b07-ec75e92c219d)
   - Ketika ada file dibuat, maka pada direktori asal (direktori **relics**) file tersebut akan dipecah menjadi sejumlah pecahan dengan ukuran maksimum tiap pecahan adalah 10kb.
     ![Cuplikan layar 2024-05-21 004416](https://github.com/Rafjonath/Sisop-4-2024-MH-IT26/assets/150430084/ef0eee20-0d53-4ddf-a6c4-071e8bfaad9b)
   - File yang dipecah akan memiliki nama **[namafile].000** dan seterusnya sesuai dengan jumlah pecahannya.
   - Ketika dilakukan penghapusan, maka semua pecahannya juga ikut terhapus.

c. Direktori **report** adalah direktori yang akan dibagikan menggunakan Samba File Server. Setelah kalian berhasil membuat direktori [**nama_bebas**], jalankan FUSE dan salin semua isi direktori [**nama_bebas**] pada direktori **report**.
![Cuplikan layar 2024-05-21 004427](https://github.com/Rafjonath/Sisop-4-2024-MH-IT26/assets/150430084/2ffc6872-eccd-4e35-8681-b97e95cf2ca8)

d. Catatan:
   - pada contoh terdapat 20 relic, namun pada zip file hanya akan ada 10 relic
   - [**nama_bebas**] berarti namanya dibebaskan
   - pada soal 3c, cukup salin secara manual. File Server hanya untuk membuktikan bahwa semua file pada direktori [**nama_bebas**] dapat dibuka dengan baik.

e. discoveries.zip

Changelog:
  - [18/05/2024] memperjelas keterangan soal no. 3, ada tambahan catatan.

 ## ***PENGERJAAN***

 ## ***PENJELASAN PENGERJAAN***

 ## ***Dokumentasi***



 
   






