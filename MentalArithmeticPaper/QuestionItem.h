#ifndef QUESTION_ITEM_H
#define QUESTION_ITEM_H

#include <QtWidgets/QWidget>
#include "ui_QuestionItemWidget.h"


class CQuestionItem : public QWidget
{
    Q_OBJECT

public:
    CQuestionItem(QWidget *parent = Q_NULLPTR);

private:
    Ui::QuestionItemForm ui;
};

#endif // !QUESTION_ITEM_H
