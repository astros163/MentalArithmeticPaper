#include "SetupPaperWindow.h"
#include "SetupSectionWindow.h"
#include <QLabel>

CSetupPaperWindow::CSetupPaperWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    //  �������ź�
    connect(ui.actionAdd,       //  ���
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
    connect(ui.actionDelete,    //  ɾ��
            &QAction::triggered,
            [=]()
            {
                delCurrentSection();
            });
    connect(ui.actionMoveUp,    //  ����
            &QAction::triggered,
            [=]()
            {
                moveCurrentSectionUp();
            });
    connect(ui.actionMoveDown,  //  ����
            &QAction::triggered,
            [=]()
            {
                moveCurrentSectionDown();
            });
    connect(ui.actionClear,     //  ���
            &QAction::triggered,
            [=]()
            {
                ui.lstQuestion->clear();
                updateCount();
            });
    connect(ui.actionYes,       //  ȷ��
            &QAction::triggered,
            [=]()
            {
                finish();
                emit signalSetupFinished(getPaper());
                close();
            });
    connect(ui.actionCancel,    //  ȡ��
            &QAction::triggered,
            [=]()
            {
                close();
            });
    //  ���ⲿ���б��ڵ��ź�
    connect(ui.lstQuestion,     //  ˫��ĳһ��
            &QListWidget::itemDoubleClicked,
            [=](QListWidgetItem* item)
            {
                //  ��¼˫��������ⲿ�����ô��ڹر�ʱ��Ҫ�õ�
                _pLastItem = item;
                //  ȡ�ø����Ӧ���ⲿ�ֵ�����
                QVariant var = item->data(Qt::UserRole);
                CQuestionSection section = var.value<CQuestionSection>();
                //  �����µ����ⲿ�����ô���
                CSetupSectionWindow* setup = new CSetupSectionWindow(this);
                //  ���ڲ��ܴ�QDialog�̳У����Լ�ʵ��ģ̬
                setup->setWindowModality(Qt::WindowModal);
                //  �øô����˳����Զ�ɾ��
                setup->setAttribute(Qt::WA_DeleteOnClose);
                //  ���øô��ڵ����ⲿ������
                setup->setSection(section);
                //  ���Ӹô��ڵ��ź�
                connect(setup,
                        &CSetupSectionWindow::signalSetupFinished,
                        this,
                        &CSetupPaperWindow::slotSectionSetupFinished);
                //  ��ʾ����
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
    //  �����µ��б���
    QListWidgetItem* item = new QListWidgetItem;
    //  ���ø��������
    setSection(item, qi);
    //  ����������б���
    ui.lstQuestion->addItem(item);
}

void CSetupPaperWindow::delCurrentSection()
{
    //  ȡ�õ�ǰ�б���
    int row = ui.lstQuestion->currentRow();
    //  ��û��ѡ�У��򷵻�
    if(row < 0) return;
    //  �Ƴ����б���
    QListWidgetItem* item = ui.lstQuestion->takeItem(row);
    //  ɾ������
    delete item;
    //  ���¼�����Ŀ����
    updateCount();
}

void CSetupPaperWindow::moveCurrentSectionUp()
{
    //  ȡ�õ�ǰ�б���
    int row = ui.lstQuestion->currentRow();
    //  ��û��ѡ�У�����ѡ�е�һ�����򷵻�
    if(row < 1) return;
    //  ��ʱ�Ƴ����б���
    QListWidgetItem* item = ui.lstQuestion->takeItem(row);
    //  ���뵽ǰһ��λ��
    ui.lstQuestion->insertItem(row - 1, item);
    //  ��ǰ�б�����Ȼ�Ǹ���
    ui.lstQuestion->setCurrentRow(row - 1);
}

void CSetupPaperWindow::moveCurrentSectionDown()
{
    //  ȡ�õ�ǰ�б���
    int row = ui.lstQuestion->currentRow();
    //  ��û��ѡ�У�����ѡ�����һ�����򷵻�
    if(row < 0 || row > ui.lstQuestion->count() - 2) return;
    //  ��ʱ�Ƴ����б���
    QListWidgetItem* item = ui.lstQuestion->takeItem(row);
    //  ���뵽��һ��λ��
    ui.lstQuestion->insertItem(row + 1, item);
    //  ��ǰ�б�����Ȼ�Ǹ���
    ui.lstQuestion->setCurrentRow(row + 1);
}

void CSetupPaperWindow::refreshSections()
{
    //  ����б���
    ui.lstQuestion->clear();
    //  �Ӻ�̨���������������б���
    for(int i = 0; i < _Paper._Sections.size(); ++i)
    {
        addSection(_Paper._Sections[i]);
    }
}

void CSetupPaperWindow::setSection(QListWidgetItem * item, const CQuestionSection & qi)
{
    //  �б������ʾ���ݣ�QT�б��������֧�ֻ���
    QString caption = QString::fromLocal8Bit("��Ŀ����������")
        + QString::number(qi._Sets.count())
        + QString::fromLocal8Bit("\t��Ŀ������")
        + QString::number(qi.getCount() > 0 ? qi.getCount() : qi.getFixedCount())
        + QString::fromLocal8Bit("\t\t������")
        + QString::number(qi.getColumns())
        + QString::fromLocal8Bit("\t\t����")
        + (qi._Disorder ? QString::fromLocal8Bit("��") : QString::fromLocal8Bit("��"))
        + "\n";
    //  ��Ӹ����⼯�ϵ���Ϣ
    for(const CQuestionSet& s : qi._Sets)
    {
        caption += "\t" + s.Type + ":\t" + QString::number(s.Count) + "\n";
    }
    //  ����Ϊ�б�����ı�
    item->setText(caption);
    //  ͬʱ��¼���б��������Ϊ���ⲿ�ֵ�����
    QVariant var = QVariant::fromValue(qi);
    item->setData(Qt::UserRole, var);
}

void CSetupPaperWindow::updateCount()
{
    int count = 0;
    //  ѭ�����ÿ�����ⲿ��
    for(int i = 0; i < ui.lstQuestion->count(); ++i)
    {
        //  ȡ�ù��������ⲿ�ֵ�����
        QVariant var = ui.lstQuestion->item(i)->data(Qt::UserRole);
        CQuestionSection item = var.value<CQuestionSection>();
        //  ��δ�����ܵ�������������ʹ���趨�ķ�0��������
        count += item.getCount() > 0 ? item.getCount() : item.getFixedCount();
    }
    ui.labQuestionCount->setText(QString::number(count));
}

void CSetupPaperWindow::finish()
{
    //  �������ã����������ݸ��µ���̨
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
    //  ˢ�£��ú�̨�����������
    refreshSections();
}

void CSetupPaperWindow::slotSectionSetupFinished(const CQuestionSection& section)
{
    //  ���ݼ�¼���б��ȷ����ͬ�ĵ��ñ���
    if(_pLastItem == nullptr)
    {
        //  �б���ָ��Ϊ�գ���������ⲿ��
        addSection(section);
    }
    else
    {
        //  �б��Ϊ�գ��޸����ⲿ��
        setSection(_pLastItem, section);
    }
    //  ������������
    updateCount();
}