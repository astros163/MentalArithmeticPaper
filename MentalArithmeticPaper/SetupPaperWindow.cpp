#include "SetupPaperWindow.h"
#include "SetupSectionWindow.h"
#include <QLabel>

CSetupPaperWindow::CSetupPaperWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    //  工具条信号
    connect(ui.actionAdd,       //  添加
            &QAction::triggered,
            [=]()
            {
                _pLastItem = nullptr;
                CSetupSectionWindow* setup = new CSetupSectionWindow(this);
                setup->setWindowModality(Qt::WindowModal);
                setup->setAttribute(Qt::WA_DeleteOnClose);
                connect(setup,
                        &CSetupSectionWindow::signalSetupFinished,
                        this,
                        &CSetupPaperWindow::slotSectionSetupFinished);
                setup->show();
            });
    connect(ui.actionDelete,    //  删除
            &QAction::triggered,
            [=]()
            {
                delCurrentSection();
            });
    connect(ui.actionMoveUp,    //  上移
            &QAction::triggered,
            [=]()
            {
                moveCurrentSectionUp();
            });
    connect(ui.actionMoveDown,  //  下移
            &QAction::triggered,
            [=]()
            {
                moveCurrentSectionDown();
            });
    connect(ui.actionClear,     //  清除
            &QAction::triggered,
            [=]()
            {
                ui.lstQuestion->clear();
                updateCount();
            });
    connect(ui.actionYes,       //  确认
            &QAction::triggered,
            [=]()
            {
                finish();
                emit signalSetupFinished(getPaper());
                close();
            });
    connect(ui.actionCancel,    //  取消
            &QAction::triggered,
            [=]()
            {
                close();
            });
    //  试题部分列表窗口的信号
    connect(ui.lstQuestion,     //  双击某一项
            &QListWidget::itemDoubleClicked,
            [=](QListWidgetItem* item)
            {
                //  记录双击的项，试题部分设置窗口关闭时需要用到
                _pLastItem = item;
                //  取得该项对应设题部分的数据
                QVariant var = item->data(Qt::UserRole);
                CQuestionSection section = var.value<CQuestionSection>();
                //  创建新的试题部分设置窗口
                CSetupSectionWindow* setup = new CSetupSectionWindow(this);
                //  由于不能从QDialog继承，需自己实现模态
                setup->setWindowModality(Qt::WindowModal);
                //  让该窗口退出即自动删除
                setup->setAttribute(Qt::WA_DeleteOnClose);
                //  设置该窗口的试题部分设置
                setup->setSection(section);
                //  连接该窗口的信号
                connect(setup,
                        &CSetupSectionWindow::signalSetupFinished,
                        this,
                        &CSetupPaperWindow::slotSectionSetupFinished);
                //  显示窗口
                setup->show();
            });
}

CSetupPaperWindow::~CSetupPaperWindow()
{
}

void CSetupPaperWindow::setPaper(const CQuestionPaper & paper)
{
    _Paper = paper;
    refreshSections();
}

CQuestionPaper CSetupPaperWindow::getPaper() const
{
    return _Paper;
}

void CSetupPaperWindow::addSection(const CQuestionSection& qi)
{
    //  创建新的列表项
    QListWidgetItem* item = new QListWidgetItem;
    //  设置该项的内容
    setSection(item, qi);
    //  将该项加入列表窗口
    ui.lstQuestion->addItem(item);
}

void CSetupPaperWindow::delCurrentSection()
{
    //  取得当前列表项
    int row = ui.lstQuestion->currentRow();
    //  若没有选中，则返回
    if(row < 0) return;
    //  移除该列表项
    QListWidgetItem* item = ui.lstQuestion->takeItem(row);
    //  删除该项
    delete item;
    //  重新计算题目总数
    updateCount();
}

void CSetupPaperWindow::moveCurrentSectionUp()
{
    //  取得当前列表项
    int row = ui.lstQuestion->currentRow();
    //  若没有选中，或者选中第一个，则返回
    if(row < 1) return;
    //  暂时移除该列表项
    QListWidgetItem* item = ui.lstQuestion->takeItem(row);
    //  插入到前一个位置
    ui.lstQuestion->insertItem(row - 1, item);
    //  当前列表项仍然是该项
    ui.lstQuestion->setCurrentRow(row - 1);
}

void CSetupPaperWindow::moveCurrentSectionDown()
{
    //  取得当前列表项
    int row = ui.lstQuestion->currentRow();
    //  若没有选中，或者选中最后一个，则返回
    if(row < 0 || row > ui.lstQuestion->count() - 2) return;
    //  暂时移除该列表项
    QListWidgetItem* item = ui.lstQuestion->takeItem(row);
    //  插入到后一个位置
    ui.lstQuestion->insertItem(row + 1, item);
    //  当前列表项仍然是该项
    ui.lstQuestion->setCurrentRow(row + 1);
}

void CSetupPaperWindow::refreshSections()
{
    //  清除列表窗口
    ui.lstQuestion->clear();
    //  从后台数据中重新设置列表窗口
    for(int i = 0; i < _Paper._Sections.size(); ++i)
    {
        addSection(_Paper._Sections[i]);
    }
}

void CSetupPaperWindow::setSection(QListWidgetItem * item, const CQuestionSection & qi)
{
    //  列表项的显示内容，QT列表项的内容支持换行
    QString caption = QString::fromLocal8Bit("题目类型数量：")
        + QString::number(qi._Sets.count())
        + QString::fromLocal8Bit("\t题目总数：")
        + QString::number(qi.getCount() > 0 ? qi.getCount() : qi.getFixedCount())
        + QString::fromLocal8Bit("\t\t分栏：")
        + QString::number(qi.getColumns())
        + QString::fromLocal8Bit("\t\t乱序：")
        + (qi._Disorder ? QString::fromLocal8Bit("是") : QString::fromLocal8Bit("否"))
        + "\n";
    //  添加各试题集合的信息
    for(const CQuestionSet& s : qi._Sets)
    {
        caption += "\t" + s.Type + ":\t" + QString::number(s.Count) + "\n";
    }
    //  设置为列表项的文本
    item->setText(caption);
    //  同时记录该列表项的数据为试题部分的设置
    QVariant var = QVariant::fromValue(qi);
    item->setData(Qt::UserRole, var);
}

void CSetupPaperWindow::updateCount()
{
    int count = 0;
    //  循环添加每个试题部分
    for(int i = 0; i < ui.lstQuestion->count(); ++i)
    {
        //  取得关联的试题部分的设置
        QVariant var = ui.lstQuestion->item(i)->data(Qt::UserRole);
        CQuestionSection item = var.value<CQuestionSection>();
        //  若未设置总的试题数量，则使用设定的非0试题数量
        count += item.getCount() > 0 ? item.getCount() : item.getFixedCount();
    }
    ui.labQuestionCount->setText(QString::number(count));
}

void CSetupPaperWindow::finish()
{
    //  结束设置，将界面数据更新到后台
    _Paper._Sections.clear();
    for(int i = 0; i < ui.lstQuestion->count(); ++i)
    {
        QVariant var = ui.lstQuestion->item(i)->data(Qt::UserRole);
        CQuestionSection item = var.value<CQuestionSection>();
        _Paper._Sections.push_back(item);
    }
}

void CSetupPaperWindow::showEvent(QShowEvent * event)
{
    //  刷新，用后台数据重设界面
    refreshSections();
}

void CSetupPaperWindow::slotSectionSetupFinished(const CQuestionSection& section)
{
    //  根据记录的列表项，确定不同的调用背景
    if(_pLastItem == nullptr)
    {
        //  列表项指针为空，新添加试题部分
        addSection(section);
    }
    else
    {
        //  列表项不为空，修改试题部分
        setSection(_pLastItem, section);
    }
    //  更新试题总数
    updateCount();
}