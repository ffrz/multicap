multicap
========

PANDUAN INSTALASI DAN KONFIGURASI
---------------------------------

- Copy paste direktori ini ke sebuah lokasi misal di D:\multicap

- Jalankan multicapsettings.exe lalu set pengaturan:
   - Service
     - Address
       Alamat listen layanan multicapd, tidak perlu diubah. Defaultnya adalah 0.0.0.0
       
     - Port
       Port layanan multicapd, defaultnya 4321. Ubah HANYA jika bentrok dengan aplikasi lain.
       
   - Capture
     - Destination Directory
       Folder untuk save hasil capture, bisa ditujukan ke komputer lain di jaringan,
       misal untuk disimpan di PC sendiri, set  D:\capture 
       
     - Device 0 sampai Device 3
       Ini adalah kamera dari sistem operasi yang akan digunakan oleh service multicapd, maksimal 4 kamera.
       Silahkan tentukan masing-masing device, Device yang dipilih TIDAK BOLEH ADA YANG SAMA atau jika ada
       yang sama, aplikasi akan CRASH!
       
- Terapkan pengaturan dengan mengklik tombol [Apply]
- Tutup pengaturan multicap klik tombol [X] atau [Close]
- Jalankan layanan multicapd.exe,
   - PASTIKAN layanan multicapd.exe sudah berjalan, buka Windows Task Manager dan pastikan multicapd.exe ada di daftar proses
   - Untuk menghentikan layanan multicapd.exe, buka Windows Task Manager dan pilih proses multicapd.exe, lalu klik [ End Process ]
- Silahkan coba layanan dengan membuka file excel "multicap_test.xlsm"
   - Klik tombol Multi Capture Test
   - PASTIKAN hasil capture tersimpan sesuai pengaturan "Destination Directory" pada aplikasi multicapsettings

PANDUAN PENGGUNAAN MULTICAP DARI VBA
---------------------------------
- Panggil multicap.exe diikuti parameter id transaksi.
Contoh Kode VBA:
  Dim id_transaksi AS String
  
  'Simpan transaksi terlebih dahulu, lalu gunakan id transaksi untuk menyimpan foto
  id_transaksi = 1
  
  'Panggil multicap client diikuti id_transaksi
  Shell("D:\multicap\multicap.exe " & id_transaksi)

TROUBLESHOOT
------------
Untuk sementara hubungi yang bikin, :D