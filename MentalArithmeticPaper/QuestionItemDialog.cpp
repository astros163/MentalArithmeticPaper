#include "QuestionItemDialog.h"
#include "Questions.h"

CQuestionItemDialog::CQuestionItemDialog(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    //  ��ʼ��QComboBox��Ԫ��
    ui.cmbType->addItems(CQuestionTypes::instance().getTypes());
    //  ����QComboBox�ĵ�ǰ��������ĳ���
    if(ui.cmbType->count() > 0)
        ui.cmbType->setCurrentIndex(0);
    //  �ź�
    connect(ui.okButton,
            &QPushButton::clicked,
            this,
            &QDialog::accept);
    connect(ui.cancelButton,
            &QPushButton::clicked,
            this,
            &QDialog::reject);
}

CQuestionItemDialog::~CQuestionItemDialog()
{
}

QString CQuestionItemDialog::getType() const
{
    return ui.cmbType->currentText();;
}

int CQuestionItemDialog::getCount() const
{
    return ui.spinCount->value();
}

void CQuestionItemDialog::setType(const QString & type)
{
    int idx = ui.cmbType->findText(type);
    if(idx == -1)
    {
        idx = 0;
    }
    ui.cmbType->setCurrentIndex(idx);
}

void CQuestionItemDialog::setCount(int count)
{
    ui.spinCount->setValue(count);
}
