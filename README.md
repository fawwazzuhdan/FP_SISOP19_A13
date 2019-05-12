# FP_SISOP19_A13

## Soal
Buatlah sebuah music player dengan bahasa C yang memiliki fitur play nama_lagu, pause, next, prev, list lagu. Selain music player juga terdapat FUSE untuk mengumpulkan semua jenis file yang berekstensi .mp3 kedalam FUSE yang tersebar pada direktori /home/user. Ketika FUSE dijalankan, direktori hasil FUSE hanya berisi file .mp3 tanpa ada direktori lain di dalamnya. Asal file tersebut bisa tersebar dari berbagai folder dan subfolder. program mp3 mengarah ke FUSE untuk memutar musik.

## Penyelesaian
### Membuat FUSE
- Membuat FUSE untuk menaruh semua file yang berenstensi .mp3 kedalam direktori hasil FUSE. Menggunakan program [fuse.c](fuse.c).
- Meng-copy semua file yang berekstensi .mp3 ke direktori "**/home/fawwaz**".
- Menganmbil semua file yang berekstensi .mp3 yang berada direktori "**/home/fawwaz**" kedalam direktori hasil FUSE
- Ketika program selesai dikerjakan makan akan menghapus seluruh file yang berekstensi .mp3 yang di-copy ke dalam direktori "**/home/fawwaz**"

### Membuat Thread untuk mp3 player
