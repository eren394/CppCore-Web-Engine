#include <winsock2.h>
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <fstream>
#include <sstream>
#include <mutex>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

struct Urun {
    int id;
    string ad;
    double fiyat;
};

vector<Urun> envanter = {
    {1, "Laptop", 15000.0},
    {2, "Mouse", 250.0},
    {3, "Klavye", 500.0}
};

mutex envanterKilidi;

string dosya_oku(string dosyaAdi) {
    ifstream dosya(dosyaAdi);
    if (!dosya.is_open()) return "";
    stringstream buffer;
    buffer << dosya.rdbuf();
    return buffer.str();
}

void istemciyi_isle(SOCKET istemciSoketi, sockaddr_in istemciBilgisi) {
    char tampon[4096] = {0};
    int okunan = recv(istemciSoketi, tampon, 4096, 0);

    if (okunan > 0) {
        cout << "\n[BAGLANTI] IP: " 
             << (int)(istemciBilgisi.sin_addr.S_un.S_un_b.s_b1) << "."
             << (int)(istemciBilgisi.sin_addr.S_un.S_un_b.s_b2) << "."
             << (int)(istemciBilgisi.sin_addr.S_un.S_un_b.s_b3) << "."
             << (int)(istemciBilgisi.sin_addr.S_un.S_un_b.s_b4) 
             << " | Thread: " << this_thread::get_id() << endl;
        
        string fullIstek(tampon);
        cout << "Istek Satiri: " << fullIstek.substr(0, fullIstek.find("\r\n")) << endl;
    }

    string istek(tampon);
    string cevap;
    string icerik;

    if (istek.find("GET /envanter") != string::npos) {
        icerik = dosya_oku("envanter.html");
        
        lock_guard<mutex> kilit(envanterKilidi);
        
        string tablo = "<table><tr><th>ID</th><th>Ad</th><th>Fiyat</th></tr>";
        for (const auto& u : envanter) {
            tablo += "<tr><td>" + to_string(u.id) + "</td><td>" + u.ad + "</td><td>" + to_string(u.fiyat) + "</td></tr>";
        }
        tablo += "</table>";

        size_t yer = icerik.find("{{TABLO}}");
        if (yer != string::npos) {
            icerik.replace(yer, 9, tablo);
        }

        cevap = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n" + icerik;
    } 
    else if (istek.find("GET / ") != string::npos || istek.find("GET /index.html") != string::npos) {
        icerik = dosya_oku("index.html");
        cevap = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n" + icerik;
    } 
    else {
        cevap = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n<h1>404 Sayfa Bulunamadi</h1>";
    }

    send(istemciSoketi, cevap.c_str(), (int)cevap.length(), 0);
    closesocket(istemciSoketi);
}

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET dinlemeSoketi = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in sunucuAdresi;
    sunucuAdresi.sin_family = AF_INET;
    sunucuAdresi.sin_addr.s_addr = INADDR_ANY;
    sunucuAdresi.sin_port = htons(8080);

    bind(dinlemeSoketi, (sockaddr*)&sunucuAdresi, sizeof(sunucuAdresi));
    listen(dinlemeSoketi, SOMAXCONN);

    cout << "------------------------------------------" << endl;
    cout << "  C++ WEB SERVER CALISIYOR (PORT: 8080)   " << endl;
    cout << "------------------------------------------" << endl;

    while (true) {
        sockaddr_in istemciBilgisi;
        int istemciBoyutu = sizeof(istemciBilgisi);
        SOCKET istemciSoketi = accept(dinlemeSoketi, (sockaddr*)&istemciBilgisi, &istemciBoyutu);

        if (istemciSoketi != INVALID_SOCKET) {
            thread(istemciyi_isle, istemciSoketi, istemciBilgisi).detach();
        }
    }

    closesocket(dinlemeSoketi);
    WSACleanup();
    return 0;
}