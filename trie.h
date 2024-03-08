#ifndef TRIE_H
#define TRIE_H

#include <QString>
#include <QStringList>
#include <unordered_map>

class TrieNode {
public:
    std::unordered_map<QChar, TrieNode*> children;
    bool isEndOfWord;

    TrieNode();
};

class Trie {
private:
    TrieNode* root;

public:
    Trie();
    ~Trie();

    void insert(const QString& word);
    QStringList search(const QString& prefix);

private:
    void findAllWords(TrieNode* node, const QString& prefix, QStringList& results);
};

#endif // TRIE_H
