#include <iostream>
#include <string>
#include <stdio.h>
#include <omp.h>

// 1. 格納する構造体（データ本体）
struct Student 
{
    int id;
    std::string name;
    int age;
};

// 2. ハッシュテーブルの各マス（バケット）に置くノード
struct HashNode 
{
    Student data;       // 構造体データ
    HashNode* next;     // 衝突時に次のノードを指すポインタ

    HashNode(Student s) : data(s), next(nullptr) {}
};

// 3. ハッシュテーブルクラス
class MyHashTable 
{
private:
    static const int TABLE_SIZE = 10; // テーブルのサイズ（配列の要素数）
    HashNode* table[TABLE_SIZE];      // ノードのポインタ配列

    // 【ハッシュ関数】IDからインデックス（配列の部屋番号）を計算する
    int hashFunction(int id) {
        return id % TABLE_SIZE; // シンプルな剰余（あまり）計算
    }

public:
    // コンストラクタ（配列をすべて空ポインタで初期化）
    MyHashTable() 
	{
        for (int i = 0; i < TABLE_SIZE; i++) {
            table[i] = nullptr;
        }
    }

    // デストラクタ（メモリ解放）
    ~MyHashTable() 
	{
        for (int i = 0; i < TABLE_SIZE; i++) {
            HashNode* current = table[i];
            while (current != nullptr) {
                HashNode* nextNode = current->next;
                delete current;
                current = nextNode;
            }
        }
    }

    // データの追加（インサート）
    void insert(Student student) 
	{
        int index = hashFunction(student.id); // 1. ハッシュ値を計算
        HashNode* newNode = new HashNode(student); // 2. 新しいノードを作成

        // 3. 衝突対策（チェイン法）：配列の先頭に追加する
        newNode->next = table[index];
        table[index] = newNode;
    }

    // データの検索（サーチ）
    Student* search(int id) 
	{
        int index = hashFunction(id); // 1. ハッシュ値を計算
        HashNode* current = table[index]; // 2. 該当するインデックスの先頭を取得

        // 3. 連結リストをたどってIDが一致するものを探す
        while (current != nullptr) {
            if (current->data.id == id) {
                return &(current->data); // 見つかったら構造体のポインタを返す
            }
            current = current->next;
        }
        return nullptr; // 見つからなかった場合
    }

    // デバッグ用：テーブル全体の構造を表示
    void display() 
	{
        for (int i = 0; i < TABLE_SIZE; i++) {
            std::cout << "[" << i << "]: ";
            HashNode* current = table[i];
            while (current != nullptr) {
                std::cout << "-> (ID:" << current->data.id 
                          << ", " << current->data.name << ") ";
                current = current->next;
            }
            std::cout << "nullptr" << std::endl;
        }
    }
};

int main() 
{
    MyHashTable myMap;

    // データの追加（あえて同じハッシュ値「3」になるIDも入れる）
    myMap.insert({103, "Taro", 18});  
    myMap.insert({12, "Jiro", 18});  
    myMap.insert({13, "Saburo", 18}); 
    myMap.insert({14, "Shiro", 18}); 
    myMap.insert({55, "Goro", 18});  
    myMap.insert({205, "Hiromi", 19});
    myMap.insert({203, "Hideki", 17});  
    myMap.insert({301, "Kikaidar", 20});  
    myMap.insert({302, "Hakaidar", 30});  

    // テーブルの内部構造を表示
    std::cout << "--- ハッシュテーブルの内部構造 ---" << std::endl;
    myMap.display();
    std::cout << std::endl;

    // データの検索
    int targetId = 203;
    std::cout << "--- ID " << targetId << " の検索結果 ---" << std::endl;
    Student* result = myMap.search(targetId);
    if (result != nullptr) {
        std::cout << "名前: " << result->name << ", 年齢: " << result->age << std::endl;
    } else {
        std::cout << "見つかりませんでした。" << std::endl;
    }

    return 0;
}

