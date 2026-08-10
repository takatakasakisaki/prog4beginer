#include <iostream>
#include <string>
#include <algorithm>

/**
 * @brief 学生の個人情報を格納する構造体
 */
struct Student 
{
    int id;             //!< 学生ID（一意の識別子）
    std::string name;   //!< 学生の名前（木のキーとして使用）
    int age;            //!< 学生の年齢
};

/**
 * @brief AVL木の各ノードを表す構造体
 */
struct AVLNode 
{
    Student data;       //!< 格納するデータ本体
    AVLNode* left;      //!< 左の子ノードへのポインタ
    AVLNode* right;     //!< 右の子ノードへのポインタ
    int height;         //!< このノードを根とする部分木の高さ

    /**
     * @brief 新しいノードを初期化するコンストラクタ
     * @param s 格納する学生データ
     */
    AVLNode(const Student& s) : data(s), left(nullptr), right(nullptr), height(1) 
    {
    }
};

/**
 * @brief AVL木（平衡二分探索木）を管理するクラス
 */
class AVLTree 
{
private:
    AVLNode* m_root;    //!< 木の根（ルート）ノードへのポインタ

    /**
     * @brief ノードの高さを安全に取得する
     * @param node 対象のノードへのポインタ
     * @return ノードの高さ（nullptrの場合は0）
     */
    int getHeight(AVLNode* node) 
    {
        return (node == nullptr) ? 0 : node->height;
    }

    /**
     * @brief 左右のサブ木の高さの差（バランスファクタ）を計算する
     * @param node 対象のノードへのポインタ
     * @return 左右の高さの差（左の高さ - 右の高さ）
     */
    int getBalance(AVLNode* node) 
    {
        return (node == nullptr) ? 0 : getHeight(node->left) - getHeight(node->right);
    }

    /**
     * @brief ノードの高さ情報を最新の状態に更新する
     * @param node 対象のノードへのポインタ
     */
    void updateHeight(AVLNode* node) 
    {
        if (node != nullptr) 
        {
            node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
        }
    }

    /**
     * @brief 右回転（Right Rotate）を実行する
     * @details 左に偏った木を右に回転させ、バランスを戻す
     * @param y 回転の軸となるノードへのポインタ
     * @return 回転後の新しい根ノードへのポインタ
     */
    AVLNode* rotateRight(AVLNode* y) 
    {
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;

        x->right = y;
        y->left = T2;

        updateHeight(y);
        updateHeight(x);

        return x;
    }

    /**
     * @brief 左回転（Left Rotate）を実行する
     * @details 右に偏った木を左に回転させ、バランスを戻す
     * @param x 回転の軸となるノードへのポインタ
     * @return 回転後の新しい根ノードへのポインタ
     */
    AVLNode* rotateLeft(AVLNode* x) 
    {
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;

        y->left = x;
        x->right = T2;

        updateHeight(x);
        updateHeight(y);

        return y;
    }

    /**
     * @brief 挿入処理を行う再帰補助関数
     * @details nameをキーとして辞書順に配置、かつ単一出口の原則に従う
     * @param node 現在探索中のノードへのポインタ
     * @param student 挿入する学生データ（定数参照渡し）
     * @return 更新された部分木の根ノードへのポインタ
     */
    AVLNode* insertNode(AVLNode* node, const Student& student) 
    {
        AVLNode* resultNode = nullptr;

        if (node == nullptr) 
        {
            resultNode = new AVLNode(student);
        }
        else 
        {
            // キーを student.id から student.name（文字列の辞書順）に変更
            if (student.name < node->data.name) 
            {
                AVLNode* nextNode = insertNode(node->left, student);
                node->left = nextNode;
            }
            else if (student.name > node->data.name) 
            {
                AVLNode* nextNode = insertNode(node->right, student);
                node->right = nextNode;
            }
            else 
            {
                // 同姓同名（重複）の場合はなにもしない
            }

            updateHeight(node);

            int balance = getBalance(node);

            // リバランスの条件判定も student.name の比較に修正
            if (balance > 1 && student.name < node->left->data.name) 
            {
                AVLNode* rotatedNode = rotateRight(node);
                node = rotatedNode;
            }
            else if (balance < -1 && student.name > node->right->data.name) 
            {
                AVLNode* rotatedNode = rotateLeft(node);
                node = rotatedNode;
            }
            else if (balance > 1 && student.name > node->left->data.name) 
            {
                AVLNode* firstRotatedNode = rotateLeft(node->left);
                node->left = firstRotatedNode;
                
                AVLNode* secondRotatedNode = rotateRight(node);
                node = secondRotatedNode;
            }
            else if (balance < -1 && student.name < node->right->data.name) 
            {
                AVLNode* firstRotatedNode = rotateRight(node->right);
                node->right = firstRotatedNode;
                
                AVLNode* secondRotatedNode = rotateLeft(node);
                node = secondRotatedNode;
            }
            else 
            {
                // 回転が不要な場合
            }

            resultNode = node;
        }

        return resultNode;
    }

    /**
     * @brief 検索処理を行う再帰補助関数
     * @details nameをキーとして辞書順に検索、かつ単一出口の原則に従う
     * @param node 現在探索中のノードへのポインタ
     * @param name 検索対象の学生の名前（定数参照渡し）
     * @return 見つかった学生データへのポインタ（見つからない場合はnullptr）
     */
    Student* searchNode(AVLNode* node, const std::string& name) 
    {
        Student* resultStudent = nullptr;

        if (node == nullptr) 
        {
            resultStudent = nullptr;
        }
        else if (node->data.name == name) 
        {
            resultStudent = &(node->data);
        }
        else 
        {
            if (name < node->data.name) 
            {
                Student* foundStudent = searchNode(node->left, name);
                resultStudent = foundStudent;
            }
            else 
            {
                Student* foundStudent = searchNode(node->right, name);
                resultStudent = foundStudent;
            }
        }

        return resultStudent;
    }

    /**
     * @brief 木の構造をデバッグ用に横向きで画面に出力する
     * @param node 現在のノードへのポインタ
     * @param space インデント用の空白の数
     */
    void printTreeStructure(AVLNode* node, int space) 
    {
        if (node == nullptr) 
        {
            return;
        }
        
        space += 10; // 文字列表示のために幅を少し広げて調整
        printTreeStructure(node->right, space);
        std::cout << std::endl;
        
        for (int i = 10; i < space; i++) 
        {
            std::cout << " ";
        }
        
        // IDの代わりにnameを出力
        std::cout << node->data.name << "(h:" << node->height << ")" << std::endl;
        printTreeStructure(node->left, space);
    }

    /**
     * @brief 後順走査によりすべてのノードをメモリから解放する
     * @param node 解放する部分木の根ノードへのポインタ
     */
    void clear(AVLNode* node) 
    {
        if (node != nullptr) 
        {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

public:
    /**
     * @brief AVLTreeクラスのコンストラクタ
     */
    AVLTree() : m_root(nullptr) 
    {
    }

    /**
     * @brief AVLTreeクラスのデストラクタ（メモリ解放を実行）
     */
    ~AVLTree() 
    {
        clear(m_root);
    }

    /**
     * @brief 木に新しい学生データを挿入する
     * @param student 挿入する学生データ（定数参照渡し）
     */
    void insert(const Student& student) 
    {
        m_root = insertNode(m_root, student);
    }

    /**
     * @brief 指定した名前の学生データを検索する
     * @param name 検索したい学生の名前（定数参照渡し）
     * @return 見つかった学生データへのポインタ（見つからない場合はnullptr）
     */
    Student* search(const std::string& name) 
    {
        Student* foundStudent = searchNode(m_root, name);
        return foundStudent;
    }

    /**
     * @brief 木の内部構造を視覚的に表示する
     */
    void display() 
    {
        printTreeStructure(m_root, 0);
    }
};

/**
 * @brief メイン関数（動作検証用）
 * @return 終了ステータス
 */
int main() 
{
    AVLTree tree;

    // アルファベット順にデータが自動配置される
    std::cout << "--- データを挿入（名前の辞書順で平衡を維持） ---" << std::endl;
    tree.insert({12, "Jiro", 18});  
    tree.insert({13, "Saburo", 18}); 
    tree.insert({14, "Shiro", 18}); 
    tree.insert({55, "Goro", 18});  
    tree.insert({205, "Hiromi", 19});
    tree.insert({203, "Hideki", 17});  
    tree.insert({301, "Kikaidar", 20});  
    tree.insert({302, "Hakaidar", 30});  

    tree.display();
    std::cout << "\n----------------------------------------" << std::endl;

    // 文字列キーによる検索テスト
    std::string targetName = "Jiro";
    std::cout << "--- 名前 \"" << targetName << "\" の検索結果 ---" << std::endl;
    Student* result = tree.search(targetName);
    
    if (result != nullptr) 
    {
        std::cout << "ID: " << result->id << "," <<result->name <<",年齢: " << result->age << "歳" << std::endl;
    }
    else 
    {
        std::cout << "見つかりませんでした。" << std::endl;
    }

    return 0;
}

