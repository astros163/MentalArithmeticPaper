#include "SetupSectionWindow.h"
#include <QLabel>

CSetupSectionWindow::CSetupSectionWindow(QWidget *parent)
    : QMainWindow(parent),
    _pItemDlg(new CQuestionItemDialog)
{
    ui.setupUi(this);
    //
    static QString CheckInfo[] =
    {
        QString::fromLocal8Bit("·ñ"),
        QString::fromLocal8Bit("ÊÇ"),
    };
    //
    ui.cmbColumns->addItem("1");
    ui.cmbColumns->addItem("2");
    ui.cmbColumns->addItem("3");
    ui.cmbColumns->addItem("4");
    ui.cmbColumns->setCurrentIndex(3);
    //
    connect(ui.actionAdd,
            &QAction::triggered,
            [=]()
            {
                if(_pItemDlg->exec() == QDialog::Accepted)
                {
                    addItem(CQuestionSet(_pItemDlg->getType(),
                                         _pItemDlg->getCount()));
                    //
                    updateCount();
                }
            });
    connect(ui.actionDelete,
            &QAction::triggered,
            [=]()
            {
                delCurrentItem();
            });
    connect(ui.actionMoveUp,
            &QAction::triggered,
            [=]()
            {
                moveCurrentItemUp();
            });
    connect(ui.actionMoveDown,
            &QAction::triggered,
            [=]()
            {
                moveCurrentItemDown();
            });
    connect(ui.actionDisorder,
            &QAction::triggered,
            [=]()
            {
                ui.labDisorder->setText(CheckInfo[ui.actionDisorder->isChecked()]);
            });
    connect(ui.actionClear,
            &QAction::triggered,
            [=]()
            {
                ui.lstQuestion->clear();
                updateCount();
            });
    connect(ui.actionYes,
            &QAction::triggered,
            [=]()
            {
                finish();
                emit signalSetupFinished(getSection());
                close();
            });
    connect(ui.actionCancel,
            &QAction::triggered,
            [=]()
            {
                close();
            });
    connect(ui.lstQuestion,
            &QListWidget::itemDoubleClicked,
            [=](QListWidgetItem* item)
            {
                QVariant var = item->data(Qt::UserRole);
                CQuestionSet si = var.value<CQuestionSet>();
                _pItemDlg->setType(si.Type);
                _pItemDlg->setCount(si.Count);
                if(_pItemDlg->exec() == QDialog::Accepted)
                {
                    setItem(item,
                            CQuestionSet(_pItemDlg->getType(),
                                         _pItemDlg->getCount()));
                    //
                    updateCount();
                }
            });
}

CSetupSectionWindow::~CSetupSectionWindow()
{
    delete _pItemDlg;
}

void CSetupSectionWindow::setSection(const CQuestionSection & questions)
{
    _Section = questions;
}

CQuestionSection CSetupSectionWindow::getSection() const
{
    return _Section;
}

void CSetupSectionWindow::addItem(const CQuestionSet& qi)
{
    QListWidgetItem* item = new QListWidgetItem;
    //
    setItem(item, qi);
    //
    ui.lstQuestion->addItem(item);
}

void CSetupSectionWindow::delCurrentItem()
{
    int row = ui.lstQuestion->currentRow();
    if(row < 0) return;
    QListWidgetItem* item = ui.lstQuestion->takeItem(row);
    delete item;
    //
    updateCount();
}

void CSetupSectionWindow::moveCurrentItemUp()
{
    int row = ui.lstQuestion->currentRow();
    if(row < 1) return;
    QListWidgetItem* item = ui.lstQuestion->takeItem(row);
    ui.lstQuestion->insertItem(row - 1, item);
    ui.lstQuestion->setCurrentRow(row - 1);
}

void CSetupSectionWindow::moveCurrentItemDown()
{
    int row = ui.lstQuestion->currentRow();
    if(row < 0 || row > ui.lstQuestion->count() - 2) return;
    QListWidgetItem* item = ui.lstQuestion->takeItem(row);
    ui.lstQuestion->insertItem(row + 1, item);
    ui.lstQuestion->setCurrentRow(row + 1);
}

void CSetupSectionWindow::refreshItems()
{
    //
    ui.lstQuestion->clear();
    for(int i = 0; i < _Section._Sets.size(); ++i)
    {
        addItem(_Section._Sets[i]);
    }
    //
    ui.cmbColumns->setCurrentIndex(_Section.getColumns() - 1);
    //
    ui.actionDisorder->setChecked(_Section._Disorder);
}

void CSetupSectionWindow::setItem(QListWidgetItem * item, const CQuestionSet & qi)
{
    if(item == nullptr) return;
    item->setText(qi.Type
                  + " ("
                  + QString::number(qi.Count)
                  + ")");
    QVariant var = QVariant::fromValue(qi);
    item->setData(Qt::UserRole, var);
}

void CSetupSectionWindow::updateCount()
{
    int count = 0;
    for(int i = 0; i < ui.lstQuestion->count(); ++i)
    {
        QVariant var = ui.lstQuestion->item(i)->data(Qt::UserRole);
        CQuestionSet item = var.value<CQuestionSet>();
        count += item.Count;
    }
    ui.labQuestionCount->setText(QString::number(count));
}

void CSetupSectionWindow::finish()
{
    //
    _Section._Sets.clear();
    for(int i = 0; i < ui.lstQuestion->count(); ++i)
    {
        QVariant var = ui.lstQuestion->item(i)->data(Qt::UserRole);
        CQuestionSet item = var.value<CQuestionSet>();
        _Section._Sets.push_back(item);
    }
    _Section._Disorder = ui.actionDisorder->isChecked();
    _Section.setCount(ui.spinCount->value());
    _Section.setColumns(ui.cmbColumns->currentIndex() + 1);
}

void CSetupSectionWindow::showEvent(QShowEvent * event)
{
    //
    refreshItems();
}
