# 🔐 PKCS#11 Crypto Service (C Library + REST API)

Proyek ini merupakan **library C modular** dan **REST API server** untuk melakukan enkripsi dan dekripsi menggunakan **PKCS#11**. Cocok digunakan untuk integrasi dengan perangkat keras seperti **HSM (Hardware Security Module)**.

## 📁 Struktur Direktori
.
├── include/           # Header file modular
├── src/               # Implementasi fungsi utama
├── server/            # Server HTTP menggunakan Ulfius
├── Makefile
└── README.md

## 🧩 Komponen

### 1. `error_handling.c`
Menangani error dari fungsi PKCS#11 dan menampilkan kode kesalahan jika ada.

### 2. `module_loader.c`
Memuat modul PKCS#11 (biasanya file `.so`) dan mengambil daftar fungsi (`C_GetFunctionList`).

### 3. `session_manager.c`
Membuka sesi dengan token PKCS#11 dan login menggunakan PIN pengguna.

### 4. `key_manager.c`
Membuat kunci AES sementara (tidak disimpan permanen di token).

### 5. `crypto_engine.c`
Melakukan enkripsi dan dekripsi menggunakan AES CBC dengan padding.

### 6. `libpkcs11_crypto.c`
Fungsi utama `pkcs11_encrypt_decrypt()` yang menggabungkan semua langkah: load modul, login, enkripsi, dekripsi, dan cleanup.

### 7. `server.c`
REST API server berbasis Ulfius yang menerima:
- `POST /crypto`
- JSON body: `{ "data": "...", "pin": "...", "module": "..." }`

Respons:
```json
{
  "encrypted": "<base64>",
  "decrypted": "<plaintext>"
}
```
contoh valid:
```
curl -X POST http://localhost:8080/crypto \
  -H "Content-Type: application/json" \
  -d '{
    "data": "panglima polim",
    "pin": "1234",
    "module": "/usr/lib/softhsm/libsofthsm2.so"
}'

respons:

{"encrypted":"GQJHMSDuGngmQYnuhS3mUg==","decrypted":"panglima polim"}
```

contoh invalid:
```
curl -X POST http://localhost:8080/crypto \
  -H "Content-Type: application/json" \
  -d '{
    "data": "panglima polim",
    "pin": "1234",
    "module": "/usr/lib/softhsm/libsofthsm2a.so"
}'

respons
Encryption/Decryption failed%
```
---

## ⚙️ Build System

Proyek ini menggunakan beberapa Makefile modular untuk membangun library, server REST API, dan unit test.
sebelum build perlu ada instalasi berikut
```
sudo apt install libpkcs11-helper1-dev softhsm2 libulfius-dev uwsc libcunit1-dev libcunit1  cmocka-doc libcmocka-dev
```
---

### cara menjalankan proyek ini
- pada root folder jalankan `make` untuk build library proyek ini
- lalu masuk ke dalam folder `server` untuk menjalankan dan build http server lakukan hal di bawah
```
make
./server
```
- untuk melakukan unit testing, masuk ke dalam folder `test` lalu build dan jalankan test dengan cara di bawah
```
make run
```

## sebelum menjalankan program
perlu diketahui ini perupakan progam yang berajalan dengan softhsm, dimana dilakukan virtualisasi terhadap token dengan cara berikut
```
mkdir -p ~/softhsm/tokens

chmod 700 ~/softhsm/tokens

ls -ld ~/softhsm/tokens

cat > ~/softhsm/softhsm2.conf <<EOF
directories.tokendir = ~/softhsm/tokens
objectstore.backend = file
EOF


softhsm2-util --init-token --slot 0 --label "MyToken" --pin 1234
```
