#include <iostream>
#include <numeric>
#include <chrono>
#include <fstream>
#include <vector>
#include "Hashmap.h"

int additiveHash(std::string key, int table_size) {
    int sum = 0;
    for (int i = 0 ; i < key.size() ; i++) {
        int c = (unsigned char)key[i];
        sum += c;
    }
    return sum%table_size;
}

int polynomialHash(std::string key, int table_size) {
    int p = 131;
    long long sum = 0;
    long long power = 1;
    for (int i = 0 ; i < key.size() ; i++) {
        int c = (unsigned char)key[i]+1;
        sum = (sum + (c*power))%table_size;
        power = (power * p)%table_size;
    }
    return sum;
}

int djb2Hash(std::string key, int table_size) {
    unsigned long long hash_val = 5381; // *magic number*
    for (int i = 0 ; i < key.size() ; i++) {
        int c = (unsigned char)key[i];
        hash_val = ((hash_val << 5) + hash_val) + c;
    }
    return hash_val%table_size;
}

bool nextPermutation(std::string& s) {
    int n = s.size();
    int i = n - 2;

    while (i >= 0 && s[i] >= s[i + 1]) i--;

    if (i < 0) return false; 

    int j = n - 1;
    while (s[j] <= s[i]) j--;

    char temp = s[i];
    s[i] = s[j];
    s[j] = temp;

    int left = i + 1;
    int right = n - 1;
    while (left < right) {
        char t = s[left];
        s[left] = s[right];
        s[right] = t;
        left++;
        right--;
    }
    return true;
}

void nextKey(std::string& key, bool reset = false) {
    static int c1 = 1;
    static int c2 = 1;
    static int c3 = 1;

    if (reset) {
        c1 = 1;
        c2 = 1;
        c3 = 1;
        return;
    }

    c1++;
    if (c1 > 255) {
        c1 = 1;
        c2++;
        if (c2 > 255) {
            c2 = 1;
            c3++;  
        }
    }

    key[0] = (char)c1;
    key[1] = (char)c2;
    key[2] = (char)c3;
}

int main() {
    
    int amount = 100000;
    std::string hashfn = "djb2Hash"; //"additiveHash", "polynomialHash", "djb2Hash"
    std::string folders = "data/";

    int liczba_serii = 10;

    for (int seria = 1; seria <= liczba_serii; seria++) {
        std::cout << "Seria " << seria << "/" << liczba_serii << "\n";
    {
        //pesymistyczny
        HashMap<std::string, int(*)(std::string, int)>* map = new HashMap<std::string, int(*)(std::string, int)>(amount, djb2Hash); //"additiveHash", "polynomialHash", "djb2Hash"
        std::vector<std::string> keys;
        keys.reserve(amount);
        std::string base = "abcdefghi";
        std::string fileName1 = folders + "pesymistyczny" + "_" + hashfn + "_" + "insert.csv";
        std::string fileName2 = folders + "pesymistyczny" + "_" + hashfn + "_" + "remove.csv";
        std::ofstream File1(fileName1, std::ios::app);
        std::ofstream File2(fileName2, std::ios::app);


        for(int i = 0 ; i < amount ; i++) {
            if((nextPermutation(base))) {
                keys.push_back(base);
                auto start = std::chrono::high_resolution_clock::now();
                map->insert(base,std::to_string(i));
                auto stop = std::chrono::high_resolution_clock::now();
                auto czas = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();
                File1<<seria<<";"<<i<<";"<<czas<<"\n";
            }
        }
         for(int i = amount-1 ; i > -1 ; i--) {
            auto start = std::chrono::high_resolution_clock::now();
            map->remove(keys[i]);
            auto stop = std::chrono::high_resolution_clock::now();
            auto czas = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();
            File2<<seria<<";"<<i<<";"<<czas<<"\n";
        }

        delete map;
    }
    {
        // optymistyczny
        HashMap<std::string, int(*)(std::string, int)>* map = new HashMap<std::string, int(*)(std::string, int)>(amount, djb2Hash); //"additiveHash", "polynomialHash", "djb2Hash"
        std::vector<std::string> keys;
        keys.reserve(amount);
        std::string fileName1 = folders + "optymistyczny" + "_" + hashfn + "_" + "insert.csv";
        std::string fileName2 = folders + "optymistyczny" + "_" + hashfn + "_" + "remove.csv";
        std::ofstream File3(fileName1, std::ios::app);
        std::ofstream File4(fileName2, std::ios::app);
        std::string s1 = "123";

        nextKey(s1, true);

        for(int i = 0 ; i < amount ; i++) {
            nextKey(s1);
            keys.push_back(s1);
            auto start = std::chrono::high_resolution_clock::now();
            map->insert(s1,std::to_string(i));
            auto stop = std::chrono::high_resolution_clock::now();
            auto czas = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();
            File3<<seria<<";"<<i<<";"<<czas<<"\n";
        }
        for(int i = amount-1 ; i > -1 ; i--) {
            auto start = std::chrono::high_resolution_clock::now();
            map->remove(keys[i]);
            auto stop = std::chrono::high_resolution_clock::now();
            auto czas = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();
            File4<<seria<<";"<<i<<";"<<czas<<"\n";
        }
        delete map;
    }
    {
        // sredni
        HashMap<std::string, int(*)(std::string, int)>* map = new HashMap<std::string, int(*)(std::string, int)>(amount, djb2Hash); //"additiveHash", "polynomialHash", "djb2Hash"
        std::vector<std::string> keys;
        keys.reserve(amount);
        std::string fileName1 = folders + "sredni" + "_" + hashfn + "_" + "insert.csv";
        std::string fileName2 = folders + "sredni" + "_" + hashfn + "_" + "remove.csv";
        std::ofstream File5(fileName1, std::ios::app);
        std::ofstream File6(fileName2, std::ios::app);

        int inserted = 0;
        for (int a = 0; a < 26 && inserted < amount; a++) {
            for (int b = 0; b < 26 && inserted < amount; b++) {
                for (int c = 0; c < 26 && inserted < amount; c++) {
                    for (int d = 0; d < 26 && inserted < amount; d++) {
                        std::string base = "    ";
                        base[0] = 'a' + a;
                        base[1] = 'a' + b;
                        base[2] = 'a' + c;
                        base[3] = 'a' + d;

                        keys.push_back(base);
                        auto start = std::chrono::high_resolution_clock::now();
                        map->insert(base, std::to_string(inserted));
                        auto stop = std::chrono::high_resolution_clock::now();
                        File5 << seria << ";" << inserted << ";" << std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count() << "\n";
                        inserted++;
                    }
                }
            }
        }
        for (int i = inserted - 1; i > -1; i--) {
            auto start = std::chrono::high_resolution_clock::now();
            map->remove(keys[i]);
            auto stop = std::chrono::high_resolution_clock::now();
            File6 << seria << ";" << i << ";" << std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count() << "\n";
        }
        delete map;
    }
    }
}
