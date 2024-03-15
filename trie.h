#ifndef TRIE_H
#define TRIE_H

#include <QString>
#include <QStringList>
#include <unordered_map>

class TrieNode {
public:
    std::unordered_map<QChar, TrieNode*> children; // 存储子节点的无序映射，键是字符，值是子节点的指针
    bool isEndOfWord; // 标记当前节点是否是一个单词的结尾

    TrieNode();
};

class Trie {
private:
    TrieNode* root;   // 指向字典树根节点的指针

public:
    Trie();
    ~Trie();

    void insert(const QString& word);
    QStringList search(const QString& prefix);

private:
    void findAllWords(TrieNode* node, const QString& prefix, QStringList& results);
};

#endif // TRIE_H
