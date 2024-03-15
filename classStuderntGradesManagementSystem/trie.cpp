#include "trie.h"

TrieNode::TrieNode() {
    isEndOfWord = false;
}

Trie::Trie() {
    root = new TrieNode();  // 创建一个新的TrieNode对象作为根节点
}

Trie::~Trie() {
    // 释放字典树的内存，防止内存泄漏
    delete root;
}

void Trie::insert(const QString& word) {  // 插入一个单词到字典树中
    TrieNode* node = root;           // 从根节点开始插入
    for (const QChar& ch : word) {    // 遍历单词中的每个字符
        if (node->children.find(ch) == node->children.end()) {  //
            node->children[ch] = new TrieNode();   // 创建一个新的子节点，并添加到当前节点的children中
        }
        node = node->children[ch];   // 节点迭代
    }
    node->isEndOfWord = true;
}

QStringList Trie::search(const QString& prefix) {
    QStringList results;
    TrieNode* node = root;
    for (const QChar& ch : prefix) {
        if (node->children.find(ch) == node->children.end()) {
            return results;
        }
        node = node->children[ch];
    }
    findAllWords(node, prefix, results);
    return results;
}

void Trie::findAllWords(TrieNode* node, const QString& prefix, QStringList& results) {
    if (node->isEndOfWord) {
        results.append(prefix);
    }

    for (auto it = node->children.begin(); it != node->children.end(); ++it) {
        QChar ch = it->first;
        TrieNode* childNode = it->second;
        QString newPrefix = prefix;
        newPrefix.append(ch);
        findAllWords(childNode, newPrefix, results);
    }
}
