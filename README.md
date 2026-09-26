# Smartgarden

Kebun pintar pakai ESP8266. Baca suhu sama kelembapan, terus nyalain kipas
atau lampu sendiri kalau udah lewat batas. Semua bisa dipantau dari HP
lewat Blynk.

Ini proyek lama (2023), tapi kodenya masih jalan dan masih kepakai.

## Isinya apa

| File | Buat apa |
|---|---|
| `Codeganador.ino` | Kode utama buat ESP8266 |
| `index.html` + `css/` | Halaman pantauan sederhana |
| `Image/` | Logo sama background |
| `Arduino_ALLCOMPONEN.zip` | Kumpulan library yang dibutuhin |

## Yang dibutuhin

**Perangkat keras**
- ESP8266 (NodeMCU / Wemos D1 mini)
- Sensor DHT22 — suhu + kelembapan
- LCD I2C 16x2 (alamat `0x27`)
- 3 buah relay modul
- Kabel jumper, breadboard

**Perangkat lunak**
- Arduino IDE + board manager ESP8266
- Library: `Blynk`, `DHT sensor library`, `LiquidCrystal_I2C`

Semua library udah ada di `Arduino_ALLCOMPONEN.zip`, tinggal extract ke
folder `libraries` Arduino lo.

## Sambungan kabel

```
DHT22    →  D6
Relay 1  →  D5
Relay 2  →  D7
Relay 3  →  D4
LCD I2C  →  SDA / SCL (default ESP8266)
```

## Cara pakai

1. **Bikin file `secrets.h`** — copy dari `secrets.h.example`, isi 3 baris:

   ```c
   #define BLYNK_AUTH_TOKEN "token-blynk-lo"
   #define WIFI_SSID        "nama-wifi-lo"
   #define WIFI_PASS        "password-wifi-lo"
   ```

   File `secrets.h` udah masuk `.gitignore`, jadi nol bakal ke-upload.

2. Buka `Codeganador.ino` di Arduino IDE.
3. Pilih board **NodeMCU 1.0 (ESP-12E)** sama port yang bener.
4. Upload. Kalau berhasil, LCD nyala terus nampilin `Suhu :` sama `Lembab:`.

## Cara kerjanya

Baca sensor tiap putaran loop, terus:

- Suhu **di atas 31°C** → relay 2 nyala. Turun di bawah **30°C** → mati.
- Suhu **di bawah 27°C** → relay 3 nyala. Naik di atas **28°C** → mati.

Ada jeda 1 derajat di antara nyala sama mati. Ini sengaja — biar relay nol
kederak-kerdut pas suhu goyang dikit di sekitar batas.

Nilai suhu sama kelembapan juga dikirim ke Blynk (pin `V5` sama `V6`), jadi
bisa diliat dari HP.

## Yang belum jalan

- **Nol ada autentikasi** di halaman `index.html`. Buat nampilin doang.
- **Batas suhu di-hardcode** di dalam kode. Kalau mau bisa diatur dari jauh,
  harus ditambah dulu.
- **Nol ada penanganan kalau WiFi putus.** ESP8266 bakal nyoba terus, tapi
  relay tetap di posisi terakhir.
- **Belum diuji** sama versi Blynk terbaru. Kalau error, paling gampang
  turunin versi library Blynk-nya.

## Kalau mau ngembangin

- Batas suhu dibikin bisa diubah lewat Blynk (`V1` misalnya)
- Tambah sensor kelembapan tanah biar bisa nyiram sendiri
- Log datanya ke SD card atau Google Sheets

---

```
 _        _    _        _
| |      / \  | |      / \
| |     / _ \ | |     / _ \
| |___ / ___ \| |___ / ___ \
|_____/_/   \_\_____/_/   \_\
```

Dibikin sama **LALA** — tools yang langsung jalan, nol ribet.
