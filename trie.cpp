#include "trie.h"

Trie::Trie() {}

void MainWindow::buildTrieFromModel()
{
    // 获取姓名所在列的索引（假设姓名列为第一列）
    int nameColumn = 0; // 假设姓名列为第一列

    // 获取模型中的行数
    int rowCount = model->rowCount();

    // 遍历模型中的每一行，并将姓名插入到 Trie 中
    for (int row = 0; row < rowCount; ++row) {
        QModelIndex index = model->index(row, nameColumn);
        QString name = model->data(index).toString();
        trie->insert(name);
    }
}
