#ifndef QUESTION_ITEM_DIALOG
#define QUESTION_ITEM_DIALOG

#include <QDialog>
#include "ui_QuestionItemDialog.h"

class CQuestionItemDialog : public QDialog
{
    Q_OBJECT

public:
    CQuestionItemDialog(QWidget *parent = Q_NULLPTR);
    virtual ~CQuestionItemDialog();
    QString getType() const;
    int     getCount() const;
    void    setType(const QString& type);
    void    setCount(int count);

private:
    Ui::QuestionItemDialog ui;

};
#endif // !QUESTION_ITEM_DIALOG
