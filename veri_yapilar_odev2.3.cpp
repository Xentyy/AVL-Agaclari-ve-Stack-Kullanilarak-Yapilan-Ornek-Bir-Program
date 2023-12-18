#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

class Dugum {
public:
    int veriAvl;
    Dugum* sol;
    Dugum* sag;
    int yukseklik;

    Dugum(int veri) : veriAvl(veri), sol(nullptr), sag(nullptr), yukseklik(1) {}
};

class YigitDugum {
public:
    Dugum* veriYigit;
    YigitDugum* sonraki;

    YigitDugum(Dugum* veri) : veriYigit(veri), sonraki(nullptr) {}
};

class Yigit {
public:
    YigitDugum* ust;

    Yigit() : ust(nullptr) {}

    void Push(Dugum* veri) {
        YigitDugum* yeniDugum = new YigitDugum(veri);
        yeniDugum->sonraki = ust;
        ust = yeniDugum;
    }

    Dugum* Pop() {
        if (ust == nullptr)
            return nullptr;

        YigitDugum* temp = ust;
        ust = ust->sonraki;
        Dugum* poppedData = temp->veriYigit;
        delete temp;
        return poppedData;
    }

    bool Bos() {
        return ust == nullptr;
    }

    void Yazdir() {
        YigitDugum* temp = ust;
        while (temp != nullptr) {
            std::cout << temp->veriYigit->veriAvl << " ";
            temp = temp->sonraki;
        }
        std::cout << std::endl;
    }

    int TepeSayisi() {
        if (ust != nullptr) {
            return ust->veriYigit->veriAvl;
        }
        return -1; // Yığın boşsa -1 döndürülebilir veya uygun bir değer seçilebilir.
    }

};

class AvlAgac {
public:
    Dugum* kok;

    AvlAgac() : kok(nullptr) {}

    int Yukseklik(Dugum* dugum) {
        if (dugum == nullptr)
            return 0;
        return dugum->yukseklik;
    }

    int Max(int a, int b) {
        return (a > b) ? a : b;
    }

    int dengeFaktoru(Dugum* dugum) {
        if (dugum == nullptr)
            return 0;
        return Yukseklik(dugum->sol) - Yukseklik(dugum->sag);
    }

    Dugum* SagaDondur(Dugum* y) {
        Dugum* x = y->sol;
        Dugum* T2 = x->sag;

        x->sag = y;
        y->sol = T2;

        y->yukseklik = Max(Yukseklik(y->sol), Yukseklik(y->sag)) + 1;
        x->yukseklik = Max(Yukseklik(x->sol), Yukseklik(x->sag)) + 1;

        return x;
    }

    Dugum* SolaDondur(Dugum* x) {
        Dugum* y = x->sag;
        Dugum* T2 = y->sol;

        y->sol = x;
        x->sag = T2;

        x->yukseklik = Max(Yukseklik(x->sol), Yukseklik(x->sag)) + 1;
        y->yukseklik = Max(Yukseklik(y->sol), Yukseklik(y->sag)) + 1;

        return y;
    }

    Dugum* Ekle(Dugum* kok, int veri) {
        if (kok == nullptr)
            return new Dugum(veri);

        if (veri < kok->veriAvl)
            kok->sol = Ekle(kok->sol, veri);
        else if (veri > kok->veriAvl)
            kok->sag = Ekle(kok->sag, veri);
        else
            return kok; // Aynı elemanı eklemeyiz.

        kok->yukseklik = 1 + Max(Yukseklik(kok->sol), Yukseklik(kok->sag));

        int denge = dengeFaktoru(kok);

        // Sol Sol Durumu
        if (denge > 1 && veri < kok->sol->veriAvl)
            return SagaDondur(kok);

        // Sag Sag Durumu
        if (denge < -1 && veri > kok->sag->veriAvl)
            return SolaDondur(kok);

        // Sol Sag Durumu
        if (denge > 1 && veri > kok->sol->veriAvl) {
            kok->sol = SolaDondur(kok->sol);
            return SagaDondur(kok);
        }

        // Sag Sol Durumu
        if (denge < -1 && veri < kok->sag->veriAvl) {
            kok->sag = SagaDondur(kok->sag);
            return SolaDondur(kok);
        }

        return kok;
    }

    void Ekle(int veri) {
        kok = Ekle(kok, veri);
    }

    void YapraklariStackeEkle(Dugum* kok, Yigit& yaprakYigit) {
        if (kok != nullptr) {
            if (kok->sol == nullptr && kok->sag == nullptr) {
                yaprakYigit.Push(kok);
            }
            YapraklariStackeEkle(kok->sol, yaprakYigit);
            YapraklariStackeEkle(kok->sag, yaprakYigit);
        }
    }

    void YapraklariStackeEkle(Yigit& yaprakYigit) {
        YapraklariStackeEkle(kok, yaprakYigit);
    }

    int CocuguOlanDugumleriTopla(Dugum* kok) {
        if (kok == nullptr)
            return 0;

        int toplam = 0;

        if (kok->sol != nullptr || kok->sag != nullptr)
            toplam += kok->veriAvl;

        toplam += CocuguOlanDugumleriTopla(kok->sol);
        toplam += CocuguOlanDugumleriTopla(kok->sag);

        return toplam;
    }

    int CocuguOlanDugumleriTopla() {
        return CocuguOlanDugumleriTopla(kok);
    }

    int ASCIIKarakteri(Dugum* kok) {
        int toplamDeger = CocuguOlanDugumleriTopla(kok);
        return toplamDeger % (90 - 65 + 1) + 65;
    }

    char ASCIIKarakteri() {
        return static_cast<char>(ASCIIKarakteri(kok));
    }
};



int main() {
    std::ifstream dosya("Veri.txt");
    if (dosya.is_open()) {
        std::string satir;
        int satirSayisi = 0;

        while (getline(dosya, satir)) {
            satirSayisi++;
            AvlAgac agac;
            std::istringstream iss(satir);
            Yigit yaprakYigit;

            int sayi;
            while (iss >> sayi) {
                agac.Ekle(sayi);
            }

            agac.YapraklariStackeEkle(yaprakYigit);

            int asciiDeger = agac.ASCIIKarakteri();
            char karakter = static_cast<char>(asciiDeger);

            std::cout <<"Karakter:" << karakter << " Yiginlar:";
            yaprakYigit.Yazdir();
            
            
        }

        dosya.close();
    }
    else {
        std::cout << "Dosya açılamadı!" << std::endl;
    }

    return 0;
}