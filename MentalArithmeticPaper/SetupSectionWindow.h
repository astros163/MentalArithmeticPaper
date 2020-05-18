#ifndef SETUP_FRAME_H
#define SETUP_FRAME_H

#include <QtWidgets/QMainWindow>
#include "ui_SetupSectionWindow.h"
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QComboBox>
#include "Questions.h"
#include "QuestionItemDialog.h"

class CSetupSectionWindow : public QMainWindow
{
    Q_OBJECT

public:
    CSetupSectionWindow(QWidget *parent = Q_NULLPTR);
    ~CSetupSectionWindow();

private:
    Ui::SetupSectionWindow ui;
    CQuestionSection        _Section;
    CQuestionItemDialog*    _pItemDlg;
public:
    void                setSection(const CQuestionSection& questions);
    CQuestionSection    getSection()    const;
private:
    void            addItem(const CQuestionSet& qi);
    void            delCurrentItem();
    void            moveCurrentItemUp();
    void            moveCurrentItemDown();
    void            refreshItems();
    void            setItem(QListWidgetItem* item, const CQuestionSet& qi);
    void            updateCount();
    void            finish();
protected:
    virtual void    showEvent(QShowEvent *event);
signals:
    void signalSetupFinished(const CQuestionSection& section);
};

#endif // !SETUP_FRAME_H
