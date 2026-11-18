#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
#include <cctype>
#include <cstddef>
using namespace std;

#if __cplusplus < 201103L
#define nullptr NULL
#endif

////////////////////////////////////////

class Vehicle {
private:
    int vehicleID;
    string vehicleType;
    int entryTime;
    int parkingDuration;
    double parkingFee;

public:
    Vehicle();
    Vehicle(int id, const string& type, int entry, int duration);

    int getVehicleID() const;
    const string& getVehicleType() const;
    int getEntryTime() const;
    int getParkingDuration() const;
    double getParkingFee() const;

    void setVehicleID(int id);
    void setVehicleType(const string& type);
    void setEntryTime(int entry);
    void setParkingDuration(int duration);

    double calculateFee();
    void display() const;
};

Vehicle::Vehicle()
    : vehicleID(0), vehicleType(""), entryTime(0), parkingDuration(0), parkingFee(0) {}

Vehicle::Vehicle(int id, const string& type, int entry, int duration)
    : vehicleID(id), vehicleType(type), entryTime(entry), parkingDuration(duration), parkingFee(0) {
    calculateFee();
}

int Vehicle::getVehicleID() const { return vehicleID; }
const string& Vehicle::getVehicleType() const { return vehicleType; }
int Vehicle::getEntryTime() const { return entryTime; }
int Vehicle::getParkingDuration() const { return parkingDuration; }
double Vehicle::getParkingFee() const { return parkingFee; }

void Vehicle::setVehicleID(int id) { vehicleID = id; }
void Vehicle::setVehicleType(const string& type) { vehicleType = type; calculateFee(); }
void Vehicle::setEntryTime(int entry) { entryTime = entry; calculateFee(); }
void Vehicle::setParkingDuration(int duration) { parkingDuration = duration; calculateFee(); }

double Vehicle::calculateFee() {
    double gia = (vehicleType == "Car") ? 6.0 : (vehicleType == "Motorcycle") ? 4.0 : 2.0;
    bool caoDiem = (entryTime >= 6 && entryTime <= 9) || (entryTime >= 15 && entryTime <= 18);
    double tong = 0;
    for (int h = 0; h < parkingDuration; ++h) {
        if (h < 2) tong += gia * (caoDiem ? 1.30 : 0.95);
        else       tong += gia * 0.90;
    }
    parkingFee = tong;
    return parkingFee;
}

void Vehicle::display() const {
    cout << "  ID: " << vehicleID << " | " << vehicleType
         << " | " << entryTime << "h | " << parkingDuration << "h"
         << " | Phi: $" << fixed << setprecision(2) << parkingFee << "\n";
}

////////////////////////////////////////

class ParkingLot {
private:
    int parkID;
    char size;
    int capacity;
    int currentVehicleCount;
    Vehicle* vehicles;

    static int initialCapacity(char s) {
        return (s == 'S') ? 1 : (s == 'M') ? 2 : 3;
    }

    void resize(int newCapacity);
    void copyFrom(const ParkingLot& other);
    void clear();

public:
    ParkingLot();
    ParkingLot(int id, char s);
    ParkingLot(const ParkingLot& other);
    ParkingLot& operator=(const ParkingLot& other);
    ~ParkingLot();

    int getParkID() const;
    bool isFull() const;
    bool addVehicle(const Vehicle& v);
    bool removeVehicle(int vID);
    bool hasVehicle(int vID) const;

    void display() const;
    void displayByType(const string& type) const;
    bool statsByType(const string& type, int& count, double& total) const;
};

ParkingLot::ParkingLot()
    : parkID(0), size('S'), capacity(0), currentVehicleCount(0), vehicles(nullptr) {}

ParkingLot::ParkingLot(int id, char s)
    : parkID(id), size(s), capacity(initialCapacity(s)), currentVehicleCount(0), vehicles(nullptr) {
    vehicles = new Vehicle[capacity];
}

void ParkingLot::copyFrom(const ParkingLot& other) {
    parkID = other.parkID;
    size = other.size;
    capacity = other.capacity;
    currentVehicleCount = other.currentVehicleCount;
    vehicles = nullptr;
    if (other.vehicles != nullptr) {
        vehicles = new Vehicle[capacity];
        for (int i = 0; i < currentVehicleCount; ++i)
            vehicles[i] = other.vehicles[i];
    }
}

void ParkingLot::clear() {
    delete[] vehicles;
    vehicles = nullptr;
    capacity = 0;
    currentVehicleCount = 0;
}

ParkingLot::ParkingLot(const ParkingLot& other) : vehicles(nullptr) {
    copyFrom(other);
}

ParkingLot& ParkingLot::operator=(const ParkingLot& other) {
    if (this != &other) {
        clear();
        copyFrom(other);
    }
    return *this;
}

ParkingLot::~ParkingLot() {
    clear();
}

int ParkingLot::getParkID() const { return parkID; }
bool ParkingLot::isFull() const { return currentVehicleCount == capacity; }

void ParkingLot::resize(int newCapacity) {
    if (newCapacity <= capacity) return;
    Vehicle* newArr = new Vehicle[newCapacity];
    for (int i = 0; i < currentVehicleCount; ++i) newArr[i] = vehicles[i];
    delete[] vehicles;
    vehicles = newArr;
    capacity = newCapacity;
    cout << "[Bai " << parkID << " tu dong mo rong suc chua len " << capacity << "]\n";
}

bool ParkingLot::addVehicle(const Vehicle& v) {
    if (vehicles == nullptr) return false;
    if (currentVehicleCount == capacity) resize(capacity * 2);
    vehicles[currentVehicleCount] = v;
    ++currentVehicleCount;
    return true;
}

bool ParkingLot::removeVehicle(int vID) {
    for (int i = 0; i < currentVehicleCount; ++i) {
        if (vehicles[i].getVehicleID() == vID) {
            for (int j = i; j < currentVehicleCount - 1; ++j)
                vehicles[j] = vehicles[j + 1];
            vehicles[currentVehicleCount - 1] = Vehicle();
            --currentVehicleCount;
            return true;
        }
    }
    return false;
}

bool ParkingLot::hasVehicle(int vID) const {
    for (int i = 0; i < currentVehicleCount; ++i)
        if (vehicles[i].getVehicleID() == vID) return true;
    return false;
}

void ParkingLot::display() const {
    cout << "Bai " << parkID << " [" << size << "] "
         << currentVehicleCount << "/" << capacity << " xe\n";
    for (int i = 0; i < currentVehicleCount; ++i)
        vehicles[i].display();
}

void ParkingLot::displayByType(const string& type) const {
    cout << "Bai " << parkID << " [" << size << "]\n";
    for (int i = 0; i < currentVehicleCount; ++i)
        if (vehicles[i].getVehicleType() == type)
            vehicles[i].display();
}

bool ParkingLot::statsByType(const string& type, int& count, double& total) const {
    count = 0; total = 0;
    for (int i = 0; i < currentVehicleCount; ++i) {
        if (vehicles[i].getVehicleType() == type) {
            ++count;
            total += vehicles[i].getParkingFee();
        }
    }
    return count > 0;
}

////////////////////////////////////////

class SystemInterface {
private:
    int lotCapacity;
    int currentLotCount;
    ParkingLot* parkingLots;

    void copyFrom(const SystemInterface& other);
    void clear();
    void resizeSystem(int newCapacity);

    int findLotIndex(int id) const {
        for (int i = 0; i < currentLotCount; ++i)
            if (parkingLots[i].getParkID() == id) return i;
        return -1;
    }
    bool vehicleExists(int id) const {
        for (int i = 0; i < currentLotCount; ++i)
            if (parkingLots[i].hasVehicle(id)) return true;
        return false;
    }
    bool readInt(int& x) {
        if (cin >> x) return true;
        if (cin.eof()) return false;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }

public:
    SystemInterface();
    SystemInterface(const SystemInterface& other);
    SystemInterface& operator=(const SystemInterface& other);
    ~SystemInterface();

    void runMenu();
};

SystemInterface::SystemInterface()
    : lotCapacity(5), currentLotCount(0), parkingLots(nullptr) {
    parkingLots = new ParkingLot[lotCapacity];
}

void SystemInterface::copyFrom(const SystemInterface& other) {
    lotCapacity = other.lotCapacity;
    currentLotCount = other.currentLotCount;
    parkingLots = new ParkingLot[lotCapacity];
    for (int i = 0; i < currentLotCount; ++i)
        parkingLots[i] = other.parkingLots[i];
}

void SystemInterface::clear() {
    delete[] parkingLots;
    parkingLots = nullptr;
    lotCapacity = 0;
    currentLotCount = 0;
}

SystemInterface::SystemInterface(const SystemInterface& other) : parkingLots(nullptr) {
    copyFrom(other);
}

SystemInterface& SystemInterface::operator=(const SystemInterface& other) {
    if (this != &other) {
        clear();
        copyFrom(other);
    }
    return *this;
}

SystemInterface::~SystemInterface() {
    clear();
}

void SystemInterface::resizeSystem(int newCapacity) {
    if (newCapacity <= lotCapacity) return;
    ParkingLot* newArr = new ParkingLot[newCapacity];
    for (int i = 0; i < currentLotCount; ++i) newArr[i] = parkingLots[i];
    delete[] parkingLots;
    parkingLots = newArr;
    lotCapacity = newCapacity;
    cout << "[He thong tu dong mo rong suc chua bai do len " << lotCapacity << "]\n";
}

void SystemInterface::runMenu() {
    int chon;
    while (true) {
        cout << "\n===== QUAN LY BAI DO XE =====\n";
        cout << "1. Tao bai do\n";
        cout << "2. Them xe\n";
        cout << "3. Xoa bai do\n";
        cout << "4. Lay xe ra\n";
        cout << "5. Hien thi tat ca bai do\n";
        cout << "6. Hien thi xe theo bai\n";
        cout << "7. Liet ke xe theo loai\n";
        cout << "8. Bao cao phi trung binh\n";
        cout << "9. Thoat\n";
        cout << "Chon: ";
        if (cin.eof()) break;
        if (!readInt(chon)) {
            if (cin.eof()) break;
            cout << "Vui long nhap so!\n";
            continue;
        }

        switch (chon) {
        case 1: {
            int ma; char kt;
            cout << "Ma bai: ";
            if (!readInt(ma)) { cout << "Du lieu khong hop le!\n"; break; }
            if (findLotIndex(ma) != -1) { cout << "Ma bai da ton tai!\n"; break; }
            cout << "Kich thuoc (S/M/L): ";
            cin >> kt;
            kt = static_cast<char>(toupper(static_cast<unsigned char>(kt)));
            if (kt != 'S' && kt != 'M' && kt != 'L') { cout << "Kich thuoc khong hop le!\n"; break; }
            if (currentLotCount == lotCapacity) resizeSystem(lotCapacity * 2);
            parkingLots[currentLotCount] = ParkingLot(ma, kt);
            ++currentLotCount;
            cout << "Da tao bai " << ma << "\n";
            break;
        }
        case 2: {
            int ma, gio, tg; string loai;
            if (currentLotCount == 0) { cout << "Chua co bai do nao!\n"; break; }
            cout << "Ma xe: ";
            if (!readInt(ma)) { cout << "Du lieu khong hop le!\n"; break; }
            if (vehicleExists(ma)) { cout << "Ma xe da ton tai!\n"; break; }
            cout << "Loai (Car/Motorcycle/Bicycle): ";
            cin >> loai;
            if (loai != "Car" && loai != "Motorcycle" && loai != "Bicycle") {
                cout << "Loai xe khong hop le!\n"; break;
            }
            cout << "Gio vao (0-23): ";
            if (!readInt(gio) || gio < 0 || gio > 23) { cout << "Gio vao khong hop le!\n"; break; }
            cout << "Thoi luong (1-23): ";
            if (!readInt(tg) || tg < 1 || tg > 23) { cout << "Thoi luong khong hop le!\n"; break; }

            Vehicle v(ma, loai, gio, tg);

            int target = 0;
            for (int i = 0; i < currentLotCount; ++i) {
                if (!parkingLots[i].isFull()) { target = i; break; }
            }
            parkingLots[target].addVehicle(v);
            cout << "Da them xe " << ma << " vao bai " << parkingLots[target].getParkID()
                 << " | Phi: $" << fixed << setprecision(2) << v.getParkingFee() << "\n";
            break;
        }
        case 3: {
            int ma;
            cout << "Ma bai can xoa: ";
            if (!readInt(ma)) { cout << "Du lieu khong hop le!\n"; break; }
            int idx = findLotIndex(ma);
            if (idx == -1) { cout << "Khong tim thay bai\n"; break; }
            for (int j = idx; j < currentLotCount - 1; ++j)
                parkingLots[j] = parkingLots[j + 1];
            parkingLots[currentLotCount - 1] = ParkingLot();
            --currentLotCount;
            cout << "Da xoa bai " << ma << "\n";
            break;
        }
        case 4: {
            int ma;
            cout << "Ma xe can lay ra: ";
            if (!readInt(ma)) { cout << "Du lieu khong hop le!\n"; break; }
            bool ok = false;
            for (int i = 0; i < currentLotCount && !ok; ++i)
                ok = parkingLots[i].removeVehicle(ma);
            cout << (ok ? "Da lay xe ra\n" : "Khong tim thay xe\n");
            break;
        }
        case 5:
            if (currentLotCount == 0) cout << "Chua co bai do nao!\n";
            for (int i = 0; i < currentLotCount; ++i)
                parkingLots[i].display();
            break;
        case 6: {
            int ma;
            cout << "Ma bai: ";
            if (!readInt(ma)) { cout << "Du lieu khong hop le!\n"; break; }
            int idx = findLotIndex(ma);
            if (idx == -1) cout << "Khong tim thay bai\n";
            else parkingLots[idx].display();
            break;
        }
        case 7: {
            string loai;
            cout << "Loai xe: ";
            cin >> loai;
            int dem; double tong;
            bool thay = false;
            for (int i = 0; i < currentLotCount; ++i) {
                if (parkingLots[i].statsByType(loai, dem, tong)) {
                    parkingLots[i].displayByType(loai);
                    thay = true;
                }
            }
            if (!thay) cout << "Khong co xe loai " << loai << "\n";
            break;
        }
        case 8: {
            const string loaiArr[3] = {"Car", "Motorcycle", "Bicycle"};
            for (int t = 0; t < 3; ++t) {
                int dem = 0; double tong = 0;
                for (int i = 0; i < currentLotCount; ++i) {
                    int d; double s;
                    if (parkingLots[i].statsByType(loaiArr[t], d, s)) {
                        dem += d; tong += s;
                    }
                }
                cout << loaiArr[t] << ": ";
                if (dem == 0) cout << "khong co xe\n";
                else cout << dem << " xe | TB: $" << fixed << setprecision(2) << tong / dem << "\n";
            }
            break;
        }
        case 9:
            cout << "Dang giai phong bo nho va thoat...\n";
            return;
        default:
            cout << "Lua chon khong hop le!\n";
        }
    }
}

int main() {
    SystemInterface sys;
    sys.runMenu();
    return 0;
}
