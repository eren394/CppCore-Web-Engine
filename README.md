# CppCore-Web-Engine

Bu proje, Windows platformu üzerinde Winsock API kullanılarak sıfırdan geliştirilmiş, yüksek performanslı ve çok iş parçacıklı (multi-threaded) bir HTTP web sunucusu motorudur. Herhangi bir dış framework kullanılmadan, düşük seviyeli ağ programlama prensipleriyle inşa edilmiştir.

##  Teknik Özellikler

- **Multi-Threaded Mimari:** `std::thread` kullanılarak her istemci bağlantısı için ayrı bir iş parçacığı oluşturulur. Bu sayede sunucu eşzamanlı olarak onlarca isteği yanıtlayabilir.
- **Thread-Safety (Veri Güvenliği):** Paylaşılan kaynaklara (envanter verisi vb.) erişim, Race Condition riskini önlemek amacıyla `std::mutex` ve `std::lock_guard` ile koruma altına alınmıştır.
- **Dinamik Template Engine:** Statik HTML dosyaları (`index.html`, `envanter.html`) diskten okunur ve çalışma zamanında dinamik veriler (C++ vektör içerikleri) özel işaretleyiciler (`{{TABLO}}`) aracılığıyla HTML içerisine enjekte edilir.
- **Soket Optimizasyonu:** `SO_REUSEADDR` soket seçeneği ile sunucunun hızlıca yeniden başlatılması sağlanmış ve port çakışmaları minimize edilmiştir.
- **Logging:** Bağlanan her istemcinin IP adresi, kullandığı Thread ID ve HTTP istek satırı terminal üzerinden gerçek zamanlı olarak izlenebilir.

##  Proje Yapısı
- `main.cpp`: Sunucu çekirdeği, thread yönetimi ve yönlendirme mantığı.
- `index.html`: Ana sayfa şablonu.
- `envanter.html`: Veri listeleme şablonu.

##  Kurulum ve Çalıştırma

1. Projeyi klonlayın:
   ```bash
   git clone [https://github.com/KULLANICI_ADIN/REPO_ISMIN.git](https://github.com/KULLANICI_ADIN/REPO_ISMIN.git)
2. G++ veya MSVC ile derleyin (Winsock kütüphanesini bağlamayı unutmayın):
   '''bash
   -'g++ main.cpp -o server -lws2_32'
3. Sunucuyu çalıştırın ve localhostunuza bağlanın:
   './sunucu.exe'
   'http://localhost:8080'
