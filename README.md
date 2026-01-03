# C++ Multi-Threaded Web Server

Bu proje, Windows platformu üzerinde C++ ve Winsock kütüphanesi kullanılarak sıfırdan geliştirilmiş bir web sunucusudur. Herhangi bir hazır web framework'ü kullanılmamıştır.

## Özellikler
- **Socket Programlama:** Winsock API ile TCP/IP bağlantıları yönetimi.
- **Multi-threading:** `std::thread` kullanılarak aynı anda birden fazla istemciye hizmet verme.
- **Dinamik İçerik:** C++ verilerini HTML şablonlarına (`{{TABLO}}`) enjekte etme.
- **Thread Safety:** `std::mutex` ile paylaşılan verilere güvenli erişim.
- **File I/O:** Statik HTML dosyalarını diskten okuma ve sunma.

## Nasıl Çalıştırılır?
1. Repoyu bilgisayarınıza indirin.
2. `g++ src/main.cpp -o server -lws2_32` komutuyla derleyin.
3. `server.exe` dosyasını çalıştırın.
4. Tarayıcınızdan `localhost:8080` adresine gidin.
