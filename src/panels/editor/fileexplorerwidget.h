#pragma once

#include <QWidget>

class QFileSystemModel;
class QString;
class QTreeView;
class QModelIndex;

class FileExplorerWidget final : public QWidget
{
    Q_OBJECT

public:
    explicit FileExplorerWidget(QWidget *parent = nullptr);

    bool setRootDirectory(const QString &directory);
    QString rootDirectory() const;

signals:
    void fileActivated(const QString &filePath);
    void fileRenamed(const QString &oldPath, const QString &newPath);

private:
    void showContextMenu(const QPoint &position);
    QString directoryForIndex(const QModelIndex &index) const;
    void createFile(const QModelIndex &index);
    void createDirectory(const QModelIndex &index);
    void renameEntry(const QModelIndex &index);
    void deleteEntry(const QModelIndex &index);
    void refresh();

    QFileSystemModel *model_ = nullptr;
    QTreeView *tree_ = nullptr;
    QString rootDirectory_;
};
