#include "trie.h"

TrieNode::TrieNode() {
    isEndOfWord = false;
}

Trie::Trie() {
    root = new TrieNode();
}

Trie::~Trie() {
    // 释放字典树的内存，防止内存泄漏
    delete root;
}

void Trie::insert(const QString& word) {
    TrieNode* node = root;
    for (const QChar& ch : word) {
        if (node->children.find(ch) == node->children.end()) {
            node->children[ch] = new TrieNode();
        }
        node = node->children[ch];
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
