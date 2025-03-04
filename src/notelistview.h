#ifndef NOTELISTVIEW_H
#define NOTELISTVIEW_H

#include <QListView>
#include <QScrollArea>
#include "editorsettingsoptions.h"
#include "dbmanager.h"

class TagPool;
class NoteListViewPrivate;
enum class NoteListState : uint8_t;

enum class ShowAction : uint8_t { NotInit, ShowPin, ShowBoth, ShowUnpin };

class NoteListView : public QListView
{
    Q_OBJECT

public:
    explicit NoteListView(QWidget *parent = nullptr);
    ~NoteListView() override;

    void animateAddedRow(const QModelIndexList &indexes);
    void setAnimationEnabled(bool isEnabled);
    void setCurrentRowActive(bool isActive);
    void setTheme(Theme::Value theme);
    void setTagPool(TagPool *newTagPool);
    void setIsInTrash(bool newIsInTrash);
    void setDbManager(DBManager *newDbManager);

    void setCurrentFolderId(int newCurrentFolderId);
    void openPersistentEditorC(const QModelIndex &index);
    void closePersistentEditorC(const QModelIndex &index);
    void setEditorWidget(int noteId, QWidget *w);
    void unsetEditorWidget(int noteId, QWidget *w);
    void closeAllEditor();
    void setListViewInfo(const ListViewInfo &newListViewInfo);
    bool isDragging() const;

    bool isPinnedNotesCollapsed() const;
    void setIsPinnedNotesCollapsed(bool newIsPinnedNotesCollapsed);
    void setCurrentIndexC(const QModelIndex &index);
    QModelIndexList getSelectedIndex() const;
    bool isDraggingInsidePinned() const;

public slots:
    void onCustomContextMenu(QPoint point);
    void onRemoveRowRequested(const QModelIndexList &indexes);
    void onAnimationFinished(NoteListState state);

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    bool viewportEvent(QEvent *e) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;

    // QAbstractScrollArea interface
protected:
    void scrollContentsBy(int dx, int dy) override;
    void startDrag(Qt::DropActions supportedActions) override;

public slots:
    void rowsAboutToBeMoved(const QModelIndexList &source);
    void rowsMoved(const QModelIndexList &dest);
    void onRowsInserted(const QModelIndexList &rows);

private slots:
    void init();

signals:
    void addTagRequested(const QModelIndex &index, int tadId);
    void removeTagRequested(const QModelIndex &index, int tadId);
    void deleteNoteRequested(const QModelIndexList &index);
    void restoreNoteRequested(const QModelIndexList &indexes);
    void newNoteRequested();
    void moveNoteRequested(int noteId, int folderId);
    void setPinnedNoteRequested(const QModelIndexList &indexes, bool isPinned);
    void saveSelectedNote(const QSet<int> &noteId);
    void pinnedCollapseChanged();
    void notePressed(const QModelIndexList &selected);
    void noteListViewClicked();

private:
    bool m_isScrollBarHidden;
    bool m_animationEnabled;
    bool m_isMousePressed;
    bool m_mousePressHandled;
    int m_rowHeight;
    QMenu *m_contextMenu;
    QMenu *m_tagsMenu;
    QAction *m_deleteNoteAction;
    QAction *m_restoreNoteAction;
    QAction *m_pinNoteAction;
    QAction *m_unpinNoteAction;
    QAction *m_newNoteAction;
    TagPool *m_tagPool;
    DBManager *m_dbManager;
    int m_currentFolderId;
    QVector<QAction *> m_noteTagActions;
    QVector<QAction *> m_folderActions;
    bool m_isInTrash;
    QPoint m_dragStartPosition;
    QPixmap m_dragPixmap;
    QMap<int, QVector<QWidget *>> m_openedEditor;
    QVector<int> m_needRemovedNotes;
    ListViewInfo m_listViewInfo;
    bool m_isDragging;
    bool m_isDraggingPinnedNotes;
    bool m_isPinnedNotesCollapsed;
    bool m_isDraggingInsidePinned;
    void setupSignalsSlots();
    void setupStyleSheet();

    void addNotesToTag(QSet<int> const &notesId, int tagId);
    void removeNotesFromTag(QSet<int> const &notesId, int tagId);

private:
    Q_DECLARE_PRIVATE(NoteListView)

    // QAbstractItemView interface
protected slots:
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) override;
};

#endif // NOTELISTVIEW_H
