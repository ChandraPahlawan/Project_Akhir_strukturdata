#include <iostream>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <conio.h>
#include <cctype>

using namespace std;

#define oren "\033[38;5;208m"
#define tebalCyan "\033[1m\033[36m"
#define merah "\033[31m"
#define cyan "\033[36m"
#define kuning "\033[33m"
#define putih "\033[0m"
#define hijau "\033[32m"
#define tebalhijau "\033[1;32m"
#define biru "\033[1;34m"

string nama_barang;
int harga_barang;
long stok_barang;
int pilihan;
int pilih1;

struct User {
    string username;
    string password;
    string role;
    User* next;
};

struct riwayat {
    string username;
    string nama_barang;
    int harga_satuan;
    int harga_total;
    int jumlah;
    riwayat* next;
};

riwayat* headRiwayat = nullptr;

User* usersHead = nullptr;
User* currentUser = nullptr;

string userini;

struct Barang {
    string nama_barang;
    int harga_barang;
    long stok_barang;
};

struct Node{
    Barang stuff;
    Node *next;
    Node *prev;
};

Node*head=NULL;
Node*tail=NULL;


bool isValidInput(string& input) {
    for (char c : input) {
        if (!isalnum(c)) { 
            return false;
        }
    }
    return true;
}

void save_riwayat_pembelian(string username, string nama_barang, int harga_satuan, int harga_total, int jumlah){
    ofstream file;
    file.open("databases/riwayat_pembelian.csv", ios::out | ios::app);
    if (!file.is_open()) {
        cout << "File tidak dapat dibuka" << endl;
        return;
    }

    file << username << ",";
    file << nama_barang << ",";
    file << harga_satuan << ",";
    file << harga_total << ",";
    file << jumlah << endl;

    file.close();
}

void save_barang(){
    ofstream file;
    file.open("databases/barang.csv", ios::out | ios::trunc);
    if (!file.is_open()) {
        cout << "File tidak dapat dibuka" << endl;
        return;
    }

    Node *temp = head;
    while (temp != nullptr){
        file << temp->stuff.nama_barang << ",";
        file << temp->stuff.harga_barang << ",";
        file << temp->stuff.stok_barang << endl;
        temp = temp->next;
    }
    file.close();
}

void save_pengguna(string username, string password, string role){
    ofstream file;
    file.open("databases/pengguna.csv", ios::out | ios::app);
    if (!file.is_open()) {
        cout << "File tidak dapat dibuka" << endl;
        return;
    }

    file << username << ",";
    file << password << ",";
    file << role << endl;

    file.close();
}   

void load_pengguna(){
    ifstream file;
    file.open("databases/pengguna.csv", ios::in);
    string username, password, role;
    if (file.is_open()){
        while (getline(file, username, ',') && getline(file, password, ',') && getline(file, role)){
            User *newUser = new User;
            newUser->username = username;
            newUser->password = password;
            newUser->role = role;
            newUser->next = nullptr;
            if (usersHead == nullptr){
                usersHead = newUser;
            } else {
                User *temp = usersHead;
                while (temp->next != nullptr){
                    temp = temp->next;
                }
                temp->next = newUser;
            }
        }
        file.close();
    } else {
        cout << "File tidak dapat dibuka." << endl;
    }
}

void load_barang(){
    ifstream file;
    file.open("databases/barang.csv", ios::in);
    string nama, harga, stok;
    if (file.is_open()){
        while (getline(file, nama, ',') && getline(file, harga, ',') && getline(file, stok)){
            Barang barang;
            barang.nama_barang = nama;
            barang.harga_barang = stod(harga); 
            barang.stok_barang = stol(stok);   

            Node *newNode = new Node;
            newNode->stuff = barang;
            newNode->next = nullptr;
            newNode->prev = nullptr;
            if (head == nullptr){
                head = newNode;
                tail = newNode;
            } else {
                tail->next = newNode;
                newNode->prev = tail;
                tail = newNode;
            }
        }
        file.close();
    } else {
        cout << "File tidak dapat dibuka." << endl;
    }
}

void load_riwayat_pembelian(){
    ifstream file;
    file.open("databases/riwayat_pembelian.csv", ios::in);
    string username, nama_barang, harga_satuan, harga_total, jumlah;
    if (file.is_open()){
        while (getline(file, username, ',') && getline(file, nama_barang, ',') && getline(file, harga_satuan, ',') && getline(file, harga_total, ',') && getline(file, jumlah)){
            riwayat *newRiwayat = new riwayat;
            newRiwayat->username = username;
            newRiwayat->nama_barang = nama_barang;
            newRiwayat->harga_satuan = stod(harga_satuan);
            newRiwayat->harga_total = stod(harga_total);
            newRiwayat->jumlah = stoi(jumlah);
            newRiwayat->next = nullptr;
            if (headRiwayat == nullptr){
                headRiwayat = newRiwayat;
            } else {
                riwayat *temp = headRiwayat;
                while (temp->next != nullptr){
                    temp = temp->next;
                }
                temp->next = newRiwayat;
            }
        }
        file.close();
    } else {
        cout << "File tidak dapat dibuka." << endl;
    }
}

void tampilkan_riwayat_pembelian(){
    riwayat* temp = headRiwayat;
    cout << "Riwayat Pembelian:" << endl;
    int count = 0;
    while (temp->next != nullptr) {
            cout << "---------------------------------------------" << endl;
            cout << "-- [" << count+1 << "] --------------------------------------" << endl;
            cout << "---------------------------------------------" << endl;
            cout << "Nama Pembeli: " << temp->username << endl;
            cout << "Nama Barang: " << temp->nama_barang << endl;
            cout << "Harga Satuan: " << temp->harga_satuan << endl;
            cout << "Harga Total: " << temp->harga_total << endl;
            cout << "Jumlah: " << temp->jumlah << endl;
            cout << "---------------------------------------------" << endl << endl;
        
        count++;
        temp = temp->next;
    }

    cout << endl;
    cout << "\tTekan Enter Untuk Kembali";
    getch();
    system("cls");

    if (count == 0){
        cout << "---------------------------------------------" << endl;
        cout << "--------- RIWAYAT PEMBELIAN KOSONG ----------" << endl;
        cout << "---------------------------------------------" << endl;
    } 
}

int total_stok(){
    int jumlah = 0;
    Node *temp = head;
    while (temp != nullptr){
        jumlah += temp->stuff.stok_barang;
        temp = temp->next;
    }
    cout<<hijau << "total stok:"<< jumlah <<putih <<endl;
    return jumlah;
}

Node* merge_sort_ascending(Node* left, Node* right) {
    if (!left) return right;
    if (!right) return left;

    if (left->stuff.nama_barang <= right->stuff.nama_barang) {
        left->next = merge_sort_ascending(left->next, right);
        left->next->prev = left;
        return left;
    } else {
        right->next = merge_sort_ascending(left, right->next);
        right->next->prev = right;
        return right;
    }
}
Node* mergsort_ascending(Node* head) {
    if (!head || !head->next) return head;
    Node* mid = head;
    Node* fast = head->next;
    while (fast && fast->next) {
        mid = mid->next;
        fast = fast->next->next;
    }
    Node* right = mid->next;
    mid->next = nullptr;
    if (right) right->prev = nullptr;
    return merge_sort_ascending(mergsort_ascending(head), mergsort_ascending(right));
}
int length(Node *head) {
    int count = 0;
    Node *temp = head;
    while (temp != nullptr) {
        count++;
        temp = temp->next;
    }
    return count;
}

void fibMonaccianSearch() {
    int x;
    while (true) {
        cout << "Masukkan stok barang yang ingin dicari: ";
        cin >> x;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            system("cls");
            cout << "Input tidak valid. Silakan masukkan angka yang valid." << endl;
        } else if (x < 0) {
            system("cls");
            cout << "Stok barang tidak bisa negatif. Harus angka positif." << endl;
        } else {
            break;
        }
    }

    if (head == nullptr) {
        cout << "Vending machine kosong, tidak ada barang untuk dicari." << endl;
        return;
    }

    int n = length(head); 
    int fibMMm2 = 0; // (m-2)'th Fibonacci number
    int fibMMm1 = 1; // (m-1)'th Fibonacci number
    int fibM = fibMMm2 + fibMMm1;  // m'th Fibonacci number

    // Membangun Fibonacci terbesar yang <= n
    while (fibM < n) {
        fibMMm2 = fibMMm1;
        fibMMm1 = fibM;
        fibM = fibMMm2 + fibMMm1;
    }

    int offset = -1; // Inisialisasi offset
    Node* temp = nullptr;

    // Fibonacci Search
    while (fibM > 1) {
        int i = min(offset + fibMMm2, n - 1);

        // Navigasi ke node yang relevan
        temp = head;
        for (int j = 0; j < i && temp != nullptr; j++) {
            temp = temp->next;
        }

        if (temp == nullptr) break;

        if (temp->stuff.stok_barang < x) {
            fibM = fibMMm1;
            fibMMm1 = fibMMm2;
            fibMMm2 = fibM - fibMMm1;
            offset = i;
        } else if (temp->stuff.stok_barang > x) {
            fibM = fibMMm2;
            fibMMm1 = fibMMm1 - fibMMm2;
            fibMMm2 = fibM - fibMMm1;
        } else {
            system("cls");
            cout << "Barang dengan stok " << x << " ditemukan pada indeks: " << i << endl;
            return;
        }
    }

    // Memeriksa elemen terakhir
    temp = head;
    for (int j = 0; j <= offset + 1 && temp != nullptr; j++) {
        temp = temp->next;
    }

    if (temp != nullptr && temp->stuff.stok_barang == x) {
        system("cls");
        cout << "Barang dengan stok " << x << " ditemukan pada indeks: " << offset + 1 << endl;
    } else {
        system("cls");
        cout << "Barang dengan stok " << x << " tidak ditemukan dalam vending machine." << endl;
    }
}


Node* findNode(Node *head, int idx)
{
    Node *temp = head;
    for (int i = 0; i < idx; i++)
    {
        if (temp == nullptr) {
            return nullptr; 
        }
        temp = temp->next;
    }
    return temp; 
}

int jumpSearch(Node *head, int x, int n)
{
    int step = sqrt(n); 
    int prev = 0;
    Node *current = head;
    while (current != nullptr && current->stuff.harga_barang < x)
    {
        prev = step;
        for (int i = 0; i < step; i++) {
            if (current == nullptr) break;
            current = current->next;
        }
        if (current == nullptr) break;
    }
    while (current != nullptr && current->stuff.harga_barang < x)
    {
        prev++;
        current = current->next;
        if (current == nullptr) break;
    }
    if (current != nullptr && current->stuff.harga_barang == x)
    {
        return prev; 
    }

    return -1;  
}
void cari_dengan_jump_search() {
    int harga;
    while(true){
    cout << "Masukkan harga barang yang ingin dicari: ";
    cin >> harga;
    if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            system("cls");
            cout << "Input tidak valid. Silakan masukkan angka yang valid." << endl;
        } else if (harga < 0) {
            system("cls");
            cout << "Harga tidak boleh negatif." << endl;
        } else {
            break;
        }
    }  
    int n = length(head); 

    if (n == 0) {
        system("cls");
        cout << "Daftar barang kosong." << endl;
        return;
    }

    int result = jumpSearch(head, harga, n);
    if (result != -1) {
        system("cls");
        cout << "Barang dengan harga RP" << harga << " ditemukan pada indeks: " << result << endl;
    } else {
        system("cls");
        cout << "Barang dengan harga RP" << harga << " tidak ditemukan dalam vending machine." << endl;
    }
}

int boyerMooreSearch(Node *head, string nama){
    int n = length(head);
    Node *temp = head;

    for (int i = 0; i < n; i++) {
        if (temp->stuff.nama_barang == nama){
            return i;
        }
        temp = temp->next;
    }
    return -1;
}
void cari_dengan_boyer_moore() {
    string nama;
    cout << "Masukkan nama barang yang ingin dicari: ";
    cin.ignore();
    getline(cin, nama);
    int result = boyerMooreSearch(head, nama);
    if (result != -1) {
        system("cls");
        cout << "Barang dengan nama \"" << nama << "\" ditemukan pada indeks: " << result << endl;
    } else {
        system("cls");
        cout << "Barang dengan nama \"" << nama << "\" tidak ditemukan dalam vending machine." << endl;
    }
}
void shellsort_descending(Node** head) {
    int n = 0;
    Node* temp = *head;
    while (temp) {
        n++;
        temp = temp->next;
    }
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            Node* temp1 = *head;
            Node* temp2 = *head;
            for (int j = 0; j < i; j++) temp1 = temp1->next;
            for (int j = 0; j < i - gap; j++) temp2 = temp2->next;
            if (temp1->stuff.nama_barang > temp2->stuff.nama_barang) {
                swap(temp1->stuff, temp2->stuff);
            }
        }
    }
}

void lihat_barang_teratas(){
    if (head == nullptr) {
        cout << merah << "Vending Machine kosong."<<putih << endl;
    } else {
        cout<< kuning << "Barang teratas dalam Vending Machine:" << endl;
        cout << "Nama: " << head->stuff.nama_barang
             << ", Harga: RP" << head->stuff.harga_barang
             << ", Stok: " << head->stuff.stok_barang << putih << endl;
    }
}

void tampilkan_barang_dalam_vending_machine() {
    system("cls");
    int pilih1;
    do {
        cout << kuning << "Pilih tampilan yang diinginkan:\n";
        cout << "1. Ascending\n";
        cout << "2. Descending\n";
        cout << "3. Tampilan Biasa\n";
        cout << "4. Kembali ke menu utama\n";
        cout << "Pilih menu di atas: " << putih;
        cin >> pilih1;

        if (cin.fail()) { 
            cin.clear();
            cin.ignore(1000, '\n');
            system("cls");
            cout << merah << "Input tidak valid. Masukkan angka antara 1 dan 4.\n" << putih;
            continue;
        }

        if (head == nullptr) { 
            system("cls");
            cout << merah << "Tidak ada barang dalam Vending Machine." << putih << endl;
            return;
        }

        if (pilih1 == 1 || pilih1 == 2) {
            Node* savedHead = head;
            if (pilih1 == 1) {
                head = mergsort_ascending(head);
            } else if (pilih1 == 2) {
                shellsort_descending(&head);
            }
            system("cls");
            cout << hijau << "Barang dalam Vending Machine (Setelah Sorting):" << putih << endl;
            Node* temp = head;
            while (temp != nullptr) {
                cout << kuning << "Nama: " << temp->stuff.nama_barang
                     << ", Harga: RP" << temp->stuff.harga_barang
                     << ", Stok: " << temp->stuff.stok_barang << putih << endl;
                temp = temp->next;
            }
            head = savedHead;
            total_stok();
            return;
        }
        if (pilih1 == 3) { 
            system("cls");
            cout << hijau << "Barang dalam Vending Machine (Tampilan Biasa):" << putih << endl;
            Node* temp = head;
            while (temp != nullptr) {
                cout << kuning << "Nama: " << temp->stuff.nama_barang
                     << ", Harga: RP" << temp->stuff.harga_barang
                     << ", Stok: " << temp->stuff.stok_barang << putih << endl;
                temp = temp->next;
            }
            total_stok();
            return;
        }

        if (pilih1 == 4) { 
            cout << "Kembali ke menu utama..." << endl;
            return;
        }
        system("cls");
        cout << merah << "Pilihan tidak sesuai. Silakan coba lagi." << putih << endl;
    } while (pilih1 != 4);
    }

void registrasi() {
    User* newUser = new User;
   while (true) {
        cout << "Masukkan username baru: ";
        cin >> newUser->username;
        if (isValidInput(newUser->username)) {
            break; 
        } else {
            cout << "Username hanya boleh mengandung huruf dan angka. Silakan coba lagi.\n";
        }
    }
    while (true) {
        cout << "Masukkan password: ";
        cin >> newUser->password;

        if (isValidInput(newUser->password)) {
            break;
        } else {
            cout << "Password hanya boleh mengandung huruf dan angka. Silakan coba lagi.\n";
        }
    }
    while (true) {
        cout << "Pilih role (admin/biasa): ";
        cin >> newUser->role;
        if (newUser->role == "admin" || newUser->role == "biasa") {
            break; 
        } else {
            cout << "Role hanya bisa diisi dengan 'admin' atau 'biasa'. Silakan coba lagi.\n";
        }
    }
    newUser->next = nullptr;
    save_pengguna(newUser->username, newUser->password, newUser->role);  
    if (usersHead == nullptr) {
        usersHead = newUser;
    } else {
        User* temp = usersHead;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newUser;
    }
    cout << hijau << "Registrasi berhasil!" << putih << endl;
}

bool login() {
    string username, password;
    cout << "Masukkan username: ";
    cin >> username;
    cout << "Masukkan password: ";
    cin >> password;

    User* temp = usersHead;
    while (temp != nullptr) {
        if (temp->username == username && temp->password == password) {
            currentUser = temp;
            userini = temp->username;
            system("cls");
            cout << hijau << "Login berhasil! Selamat datang, " << username << "." << putih << endl;
            Sleep(1000);
            system("cls");
            return true;
        }
        temp = temp->next;
    }
    system("cls");
    cout << merah << "Username atau password salah." << putih << endl;
    return false;
}

void logout() {
    currentUser = nullptr;
    system("cls");
    cout << "Logout berhasil." << endl;
}

void tampilkan_menu_utama() {
    int pilihan;
    do {
        cout << biru << "\n==== Menu Utama ====\n" << putih;
        cout << "1. Login\n";
        cout << "2. Registrasi\n";
        cout << "3. Keluar\n";
        cout << "Pilih: ";
        cin >> pilihan;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000,'\n');
            system("cls");
            cout << merah << "Inputan harus berupa angka!" << putih << endl;
        } else{
        switch (pilihan) {
            case 1:
                system("cls");
                if (login()) return; 
                break;
            case 2:
                system("cls");
                registrasi();
                break;
            case 3:
                system("cls");
                cout << "Keluar dari aplikasi." << endl;
                exit(0);
            default:
                system("cls");
                cout << merah << "Pilihan tidak valid." << putih << endl;
                break;
                
        }}
    } while (pilihan != 3);
}

void menu_CRUD() {
    cout << tebalCyan << "[>>>>--------> Menu CRUD Vending Machine <--------<<<<]" << putih << endl;
    cout << biru << "|1. Menambahkan barang di rak depan Vending Machine   |" << endl;
    cout << "|2. Menambahkan barang di rak belakang Vending Machine|" << endl;
    cout << "|3. Tampilkan barang teratas                          |" << endl;
    cout << "|4. Tampilkan barang pada Vending Machine             |" << endl;
    cout << "|5. Perbarui harga dan stok dalam Vending Machine     |" << endl;
    cout << "|6. Hapus barang di rak depan Vending Machine         |" << endl;
    cout << "|7. Cari Barang                                       |" << endl;
    cout << "|8. Keluar                                            |" << putih << endl;
    cout << tebalCyan << "[xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx]\n" << putih << endl;
}

void beli_barang() {
    string nama_barang;
    int jumlah;
    tampilkan_barang_dalam_vending_machine();
    cout << "Masukkan nama barang yang ingin dibeli: ";
    cin.ignore();
    getline(cin, nama_barang);
    do {
        cout << "Masukkan jumlah barang yang ingin dibeli: ";
        cin >> jumlah;
        if (cin.fail()) { 
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Input tidak valid. Harap masukkan angka positif.\n";
        } else if (jumlah <= 0) {
            cout << "Jumlah harus lebih dari 0. Harap coba lagi.\n";
        }
    } while (cin.fail() || jumlah <= 0); 
    Node* temp = head;
    while (temp != nullptr) {
        if (temp->stuff.nama_barang == nama_barang) {
            if (temp->stuff.stok_barang >= jumlah) {

                riwayat* newRiwayat = new riwayat;
                newRiwayat->username = currentUser->username;
                newRiwayat->nama_barang = temp->stuff.nama_barang;
                newRiwayat->harga_satuan = temp->stuff.harga_barang;
                newRiwayat->harga_total = temp->stuff.harga_barang * jumlah;
                newRiwayat->jumlah = jumlah;
                newRiwayat->next = nullptr;
                if (headRiwayat == nullptr) {
                    headRiwayat = newRiwayat;

                } else {
                    riwayat *temp = headRiwayat;
                    while (temp->next != nullptr) {
                        temp = temp->next;
                    }
                    temp->next = newRiwayat;
                }
                temp->stuff.stok_barang -= jumlah;
                save_riwayat_pembelian(newRiwayat->username, newRiwayat->nama_barang, newRiwayat->harga_satuan, newRiwayat->harga_total, newRiwayat->jumlah);
                save_barang();
                system("cls");
                cout << hijau << "Pembelian berhasil. Jumlah stok tersisa: " << temp->stuff.stok_barang << putih << endl;
                return;
            } else {
                system("cls");
                cout << merah << "Stok barang tidak mencukupi." << putih << endl;
                return;
            }
        }
        temp = temp->next;
    }
    system("cls");
    cout << merah << "Barang tidak ditemukan." << putih << endl;
}

void menu_berdasarkan_role() {
    if (currentUser->role == "admin") {
        menu_CRUD();
    } else {
        cout << tebalCyan << "[>>>>--------> Menu Pembelian <--------<<<<]" << putih << endl;
        cout << biru << "|1. Lihat Barang di Vending Machine        |" << endl;
        cout << "|2. Beli Barang                            |" << endl;
        cout << "|3. Lihat Riwayat Pembelian                |" << endl;
        cout << "|4. logout                                 |" << putih << endl;
        cout << tebalCyan << "[xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx]\n" << putih << endl;
    }
}

void tambah_barang_depan() {
    Node* newNode = new Node;
    do {
        cout << "Masukkan nama barang: ";
        cin.ignore(1000, '\n');
        getline(cin, newNode->stuff.nama_barang);
    } while (newNode->stuff.nama_barang.empty());
    
    do {
        cout << "Masukkan harga baru: ";
        if (!(cin >>newNode->stuff.harga_barang)) {
            cout << merah << "Inputan tidak valid, harus menggunakan angka!" << putih << endl;
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        break;
    } while (true);

    do {
        cout << "Masukkan stok baru: ";
        if (!(cin >> newNode->stuff.stok_barang)) {
            cout << merah << "Inputan tidak valid, harus menggunakan angka!" << putih << endl;
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        break;
    } while (true);

    newNode->next = head;
    newNode->prev = nullptr;
    if (head != nullptr) {
        head->prev = newNode;
    }
    head = newNode;
    if (tail == nullptr) {
        tail = newNode;
    }
    system("cls");
    save_barang();
            cout << kuning << "Barang berhasil ditambahkan." << putih << endl;
}

void hapus_dari_depan() {
    if (head == nullptr) {
        cout << merah << "Barang kosong!" << putih << endl;
        return;
    }

    Node* temp = head;
    head = head->next;   

    delete temp;          
    cout << kuning << "Barang berhasil dihapus " << putih << endl;
}

void tambah_barang_belakang() { 
    Node* newNode = new Node;
     cin.ignore(1000, '\n');
    do {
        cout << "Masukkan nama barang: ";
        getline(cin, newNode->stuff.nama_barang);
    } while (newNode->stuff.nama_barang.empty());

    do {
        cout << "Masukkan harga baru: ";
        if (!(cin >>newNode->stuff.harga_barang)) {
            cout << merah << "Inputan tidak valid, harus menggunakan angka!" << putih << endl;
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        break;
    } while (true);

    do {
        cout << "Masukkan stok baru: ";
        if (!(cin >> newNode->stuff.stok_barang)) {
            cout << merah << "Inputan tidak valid, harus menggunakan angka!" << putih << endl;
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        break;
    } while (true);
        system("cls");
            newNode->next = nullptr;  
            newNode->prev = tail;     

            if (tail == nullptr) {     
                head = newNode;
                tail = newNode;
            } else {
                tail->next = newNode; 
                tail = newNode;        
            }
            save_barang(); 
            cout << kuning << "Barang berhasil ditambahkan." << putih << endl;
}

void perbarui_barang() {
    if (head == nullptr) {
        cout << merah << "Barang kosong" << putih << endl;
        return;
    }
    cin.ignore(1000, '\n');  
    string nama_barang;
    Node *temp = head;


    cout << "Masukkan nama barang yang ingin diperbarui: ";
    getline(cin, nama_barang);

    while (temp != nullptr && temp->stuff.nama_barang != nama_barang) {
        temp = temp->next;
    }

    if (temp == nullptr) {
        cout << merah << "Barang tidak ditemukan" << putih << endl;
        return;
    }
    do {
        cout << "Masukkan harga baru: ";
        if (!(cin >> temp->stuff.harga_barang)) {  
            cout << merah << "Input tidak valid, harus angka!" << putih << endl;
            cin.clear();  
            cin.ignore(1000, '\n');  
            continue;
        }
        break;  
    } while (true);
    do {
        cout << "Masukkan stok baru: ";
        if (!(cin >> temp->stuff.stok_barang)) {  
            cout << merah << "Input tidak valid, harus angka!" << putih << endl;
            cin.clear();  
            cin.ignore(1000, '\n');  
            continue;
        }
        break;  
    } while (true);
    system("cls");
    cout << kuning << "Barang berhasil diperbarui." << putih << endl;
    save_barang();
}


void hapus_barang_depan() {
    system("cls");
    Node* temp =head;
    if (head == nullptr) {
        cout << "Barang kosong"<< endl;
        return ;
    }
    
    cin.ignore(1000, '\n');
    head = head->next;
     if (head != nullptr) {
        head->prev = nullptr;  
    } else {
        tail = nullptr;  
    }

    system("cls");
    cout << kuning << "Barang berhasil dihapus." << temp->stuff.nama_barang << putih << endl;
    delete temp;
    save_barang();
    }

void opsi_cari(){
    int pilih3;
    do{
    cout<< kuning << "Pilih:"<< endl;
    cout << "1. Cari Stok barang"<< endl;
    cout << "2. Cari Harga barang"<< endl;
    cout << "3. Cari Nama barang"<< endl;
    cout << "4. Kembali"<<endl;
    cout << ":"<< putih <<endl;
    cin >> pilih3;
    switch (pilih3){
    case 1: 
        system("cls");
        fibMonaccianSearch();
       
        break;
    case 2:
        system("cls");
        cari_dengan_jump_search();
      
        break;
    case 3:
        system("cls");
        cari_dengan_boyer_moore();  
        
        break;
    case 4:
        break;
    default:
    cout << "pilihan salah" << endl;
    break;
    }
    }while (pilih3 != 4);
}

int main() {
    load_barang();
    load_pengguna();
    load_riwayat_pembelian();
    system("cls");
    Node *head = nullptr;
    tampilkan_menu_utama();
    do {
        if (currentUser->role == "admin") {
            menu_CRUD();
        } else {
            menu_berdasarkan_role();
        }
        cout << "Pilih menu yang diinginkan: ";
        if (!(cin >> pilihan)) {
            cin.clear();
            cin.ignore(1000, '\n');
            system("cls");
            cout << merah << "Inputan harus berupa angka!" << putih << endl;
        } else {
            if (currentUser->role == "admin") {
                switch (pilihan) {
                    case 1:
                        system("cls");
                        tambah_barang_depan();
                        break;
                    case 2:
                        system("cls");
                        tambah_barang_belakang();
                        break;
                    case 3:
                        system("cls");
                        lihat_barang_teratas();
                        break;
                    case 4: 
                        system("cls");
                        tampilkan_barang_dalam_vending_machine();
                        break;
                    case 5:
                        system("cls");
                        tampilkan_barang_dalam_vending_machine();
                        perbarui_barang();
                        break;
                    case 6:
                        system("cls");
                        hapus_barang_depan();
                        break;
                    case 7:
                        system("cls");
                        opsi_cari();
                        break;
                    case 8:
                        system("cls");
                        tampilkan_menu_utama();
                        break;
                    default:
                        system("cls");
                        cout << merah << "Inputan salah, silahkan dicoba ulang." << putih << endl;
                        break;
                }
            } else {
                switch (pilihan) {
                    case 1:
                        system("cls");
                        tampilkan_barang_dalam_vending_machine();
                        break;
                    case 2:
                        system("cls");
                        beli_barang();
                        break;
                    case 3:
                        system("cls");
                        tampilkan_riwayat_pembelian();
                        break;
                    case 4:
                        system("cls"); 
                        tampilkan_menu_utama();
                        break;
                    default:
                        system("cls");
                        cout << merah << "Inputan salah, silahkan dicoba ulang." << putih << endl;
                        break;
                }
            }
        }
    } while (pilihan != 8);
    return 0;
}