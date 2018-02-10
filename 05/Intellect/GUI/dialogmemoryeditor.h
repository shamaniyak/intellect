#ifndef DIALOGMEMORYEDITOR_H
#define DIALOGMEMORYEDITOR_H

#include <memory>
#include <QWidget>
#include "Src/Memory/memorywrapper.h"
#include "Src/GUI/ScriptEditor/highlighter.h"

class Intellect;

namespace Ui {
class Editor;
}

class DialogMemoryEditor : public QWidget
{
  Q_OBJECT

public:
  explicit DialogMemoryEditor(QWidget *parent = 0);
  ~DialogMemoryEditor();

  void Init();

  Intellect *intellect() const;
  void setIntellect(Intellect *intellect);

  void LoadSettings(MEWrapper *meSettings);
  void SaveSettings(MEWrapper *meSettings);

  QAction *getActUndo() const;

  QAction *getActRedo() const;

  QAction *getActAbout() const;

  QAction *getActSave() const;

signals:
  signalKeyPress(QObject *obj, int key);

protected:
  bool eventFilter(QObject *obj, QEvent *ev);

  void ShowMemory();

  void ShowMeValues(MEWrapper *me);

  void AddResult(const QString &str);

  void ClearValues();

  void UpdateUI();

  void CreateMainMenu();

  void ShowTxtVal(const QString &val);

  QSize sizeHint() const;

  void SaveChanges(QObject *obj);

  void SetChanged(bool val = true);

  bool focusEvent(QObject *obj, QEvent *ev);
  bool keyEvent(QObject *obj, QEvent *ev);

  void SetupEditor();

  void checkItemChanged(QObject *obj);

private slots:

  void on_btnAdd_clicked();

  void on_btnEdit_clicked();

  void on_btnDelete_clicked();

  void on_btnCompile_clicked();

  void memory_change(MEWrapper *me, EMemoryChange idMsg);

  void on_txtVal_textChanged();

  void on_addResult(const QString &str);

  void on_edtName_textEdited(const QString &arg1);

  void on_txtVal_modificationChanged(bool arg1);

  void edit_undo_triggered(bool checked);

  void edit_redo_triggered(bool checked);

  void actSave_triggered(bool checked);

private:
  Ui::Editor *ui;
  Intellect *intellect_ = 0;
  MEWrapper *selected_ = 0;
  QSize baseSize_;
  bool selected_changed_ = false;
  Highlighter *highlighter_ = 0;
  QObject *focussed_object_ = 0;

  QAction *actUndo = 0;
  QAction *actRedo = 0;
  QAction *actAbout = 0;
  QAction *actSave = 0;
};

#endif // DIALOGMEMORYEDITOR_H
