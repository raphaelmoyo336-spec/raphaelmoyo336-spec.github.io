#include "fileexplorerwidget.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileSystemModel>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QTreeView>
#include <QVBoxLayout>

FileExplorerWidget::FileExplorerWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    auto *title = new QLabel(QStringLiteral("EXPLORER"), this);
    title->setObjectName(QStringLiteral("explorerHeader"));
    title->setContentsMargins(10, 8, 10, 8);
    layout->addWidget(title);

    auto *refreshButton = new QPushButton(QStringLiteral("Refresh"), this);
    refreshButton->setObjectName(QStringLiteral("refreshExplorerAction"));
    layout->addWidget(refreshButton);

    model_ = new QFileSystemModel(this);
    // Keep dotfiles visible so project configuration files appear in the tree.
    model_->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);
    model_->setReadOnly(false);

    tree_ = new QTreeView(this);
    tree_->setModel(model_);
    tree_->setHeaderHidden(true);
    tree_->hideColumn(1);
    tree_->hideColumn(2);
    tree_->hideColumn(3);
    tree_->setAnimated(true);
    tree_->setUniformRowHeights(true);
    tree_->setContextMenuPolicy(Qt::CustomContextMenu);
    layout->addWidget(tree_);

    connect(refreshButton, &QPushButton::clicked, this, &FileExplorerWidget::refresh);
    connect(tree_, &QTreeView::customContextMenuRequested,
            this, &FileExplorerWidget::showContextMenu);
    connect(tree_, &QTreeView::doubleClicked, this, [this](const QModelIndex &index) {
        if (model_->fileInfo(index).isFile())
            emit fileActivated(model_->filePath(index));
    });
}

bool FileExplorerWidget::setRootDirectory(const QString &directory)
{
    const QFileInfo info(directory);
    if (!info.exists() || !info.isDir())
        return false;

    rootDirectory_ = QDir(directory).absolutePath();
    tree_->setRootIndex(model_->setRootPath(rootDirectory_));
    return true;
}

QString FileExplorerWidget::rootDirectory() const
{
    return rootDirectory_;
}

QString FileExplorerWidget::directoryForIndex(const QModelIndex &index) const
{
    const QFileInfo info = index.isValid()
        ? model_->fileInfo(index)
        : QFileInfo(rootDirectory_);
    return info.isDir() ? info.absoluteFilePath() : info.absolutePath();
}

void FileExplorerWidget::showContextMenu(const QPoint &position)
{
    const QModelIndex index = tree_->indexAt(position);
    QMenu menu(this);
    QAction *newFile = menu.addAction(QStringLiteral("New File..."));
    QAction *newDirectory = menu.addAction(QStringLiteral("New Folder..."));
    QAction *rename = nullptr;
    QAction *remove = nullptr;
    if (index.isValid()) {
        menu.addSeparator();
        rename = menu.addAction(QStringLiteral("Rename..."));
        remove = menu.addAction(QStringLiteral("Delete..."));
    }

    QAction *selected = menu.exec(tree_->viewport()->mapToGlobal(position));
    if (selected == newFile)
        createFile(index);
    else if (selected == newDirectory)
        createDirectory(index);
    else if (selected == rename)
        renameEntry(index);
    else if (selected == remove)
        deleteEntry(index);
}

void FileExplorerWidget::createFile(const QModelIndex &index)
{
    const QString name = QInputDialog::getText(
        this, QStringLiteral("New File"), QStringLiteral("File name:"));
    if (name.isEmpty())
        return;
    if (QFileInfo(name).fileName() != name || name == QStringLiteral(".")
        || name == QStringLiteral("..")) {
        QMessageBox::warning(this, QStringLiteral("Invalid file name"),
                             QStringLiteral("Enter a file name, not a path."));
        return;
    }

    const QString path = QDir(directoryForIndex(index)).filePath(name);
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::NewOnly)) {
        QMessageBox::warning(this, QStringLiteral("Unable to create file"),
                             QStringLiteral("Could not create %1:\n%2")
                                 .arg(path, file.errorString()));
    }
}

void FileExplorerWidget::createDirectory(const QModelIndex &index)
{
    const QString name = QInputDialog::getText(
        this, QStringLiteral("New Folder"), QStringLiteral("Folder name:"));
    if (name.isEmpty())
        return;
    if (QFileInfo(name).fileName() != name || name == QStringLiteral(".")
        || name == QStringLiteral("..")) {
        QMessageBox::warning(this, QStringLiteral("Invalid folder name"),
                             QStringLiteral("Enter a folder name, not a path."));
        return;
    }

    const QString path = QDir(directoryForIndex(index)).filePath(name);
    if (!QDir().mkdir(path)) {
        QMessageBox::warning(this, QStringLiteral("Unable to create folder"),
                             QStringLiteral("Could not create %1. Check that the name "
                                            "is available and the location is writable.")
                                 .arg(path));
    }
}

void FileExplorerWidget::renameEntry(const QModelIndex &index)
{
    if (!index.isValid())
        return;
    const QString oldPath = model_->filePath(index);
    const QString oldName = model_->fileName(index);
    bool accepted = false;
    const QString newName = QInputDialog::getText(
        this, QStringLiteral("Rename"), QStringLiteral("New name:"),
        QLineEdit::Normal, oldName, &accepted);
    if (!accepted || newName.isEmpty() || newName == oldName)
        return;
    if (QFileInfo(newName).fileName() != newName || newName == QStringLiteral(".")
        || newName == QStringLiteral("..")) {
        QMessageBox::warning(this, QStringLiteral("Invalid name"),
                             QStringLiteral("Enter a name, not a path."));
        return;
    }
    if (!model_->setData(index, newName, Qt::EditRole)) {
        QMessageBox::warning(this, QStringLiteral("Unable to rename"),
                             QStringLiteral("Could not rename %1. Check permissions and "
                                            "whether the destination already exists.")
                                 .arg(oldPath));
        return;
    }
    const QString newPath = QDir(QFileInfo(oldPath).absolutePath()).filePath(newName);
    emit fileRenamed(oldPath, newPath);
}

void FileExplorerWidget::deleteEntry(const QModelIndex &index)
{
    if (!index.isValid())
        return;
    const QString path = model_->filePath(index);
    const QFileInfo info(path);
    const QString kind = info.isDir() ? QStringLiteral("folder") : QStringLiteral("file");
    if (QMessageBox::question(
            this, QStringLiteral("Delete %1").arg(kind),
            QStringLiteral("Permanently delete %1?\n%2").arg(kind, path),
            QMessageBox::Yes | QMessageBox::Cancel,
            QMessageBox::Cancel) != QMessageBox::Yes)
        return;

    const bool removed = info.isDir() && !info.isSymLink()
        ? QDir(path).removeRecursively()
        : QFile::remove(path);
    if (!removed) {
        QMessageBox::warning(this, QStringLiteral("Unable to delete"),
                             QStringLiteral("Could not delete %1. Check permissions and "
                                            "whether it is in use.")
                                 .arg(path));
    }
}

void FileExplorerWidget::refresh()
{
    if (!rootDirectory_.isEmpty())
        tree_->setRootIndex(model_->setRootPath(rootDirectory_));
}
