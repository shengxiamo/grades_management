#ifndef TRIE_H
#define TRIE_H

#include <QString>
#include <QStringList>
#include <QMap>

// 定义 Trie 节点结构
struct TrieNode {
    bool isEnd;
    QMap<QChar, TrieNode*> children;

    TrieNode() : isEnd(false) {}
};

class Trie {
public:
    Trie() {
        root = new TrieNode();
    }

    // 插入姓名到 Trie 树中
    void insert(const QString& name) {
        TrieNode* node = root;
        for (const QChar& ch : name) {
            if (!node->children.contains(ch)) {
                node->children[ch] = new TrieNode();
            }
            node = node->children[ch];
        }
        node->isEnd = true;
    }

    // 根据输入前缀搜索匹配的姓名
    QStringList search(const QString& prefix) {
        QStringList results;
        TrieNode* node = root;
        for (const QChar& ch : prefix) {
            if (!node->children.contains(ch)) {
                return results; // 没有匹配的前缀
            }
            node = node->children[ch];
        }
        // 从当前节点开始深度优先搜索匹配的姓名
        dfsSearch(prefix, node, results);
        return results;
    }

    TrieNode* getRoot() const {
        return root;
    }
private:
    TrieNode* root;

    // 深度优先搜索匹配的姓名
    void dfsSearch(const QString& prefix, TrieNode* node, QStringList& results) {
        if (node->isEnd) {
            results.append(prefix);
        }
        for (auto it = node->children.constBegin(); it != node->children.constEnd(); ++it) {
            QChar ch = it.key();
            dfsSearch(prefix + ch, it.value(), results);
        }
    }
};


#endif // TRIE_H
