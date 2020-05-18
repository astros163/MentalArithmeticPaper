#ifndef SETUP_PAPER_FRAME_H
#define SETUP_PAPER_FRAME_H

#include <QtWidgets/QMainWindow>
#include "ui_SetupPaperWindow.h"
#include <QtWidgets/QLabel>
#include "SetupSectionWindow.h"

class CSetupPaperWindow : public QMainWindow
{
    Q_OBJECT

public:
    CSetupPaperWindow(QWidget *parent = Q_NULLPTR);
    ~CSetupPaperWindow();

private:
    Ui::SetupPaperWindow ui;
    CQuestionPaper      _Paper;
    QListWidgetItem*    _pLastItem;
public:
    //! @brief  �趨�Ծ�����
    //! @param  paper   const CQuestionPaper&   [in]�Ծ�����
    //! @return void
    //! �趨���Ծ����û��ڴ�����ʾ��ʱ�����
    void            setPaper(const CQuestionPaper& paper);
    //! @brief  ȡ���Ծ�����
    //! @return CQuestionPaper&                 �Ծ�����
    CQuestionPaper  getPaper()    const;
private:
    //! @brief  ����µ����ⲿ��
    //! @param  qi      const CQuestionSection& [in]���ⲿ��
    //! @return void
    void            addSection(const CQuestionSection& qi);
    //! @brief  ɾ����ǰѡ�е����ⲿ��
    //! @return void
    void            delCurrentSection();
    //! @brief  ����ǰѡ�е����ⲿ������
    //! @return void
    void            moveCurrentSectionUp();
    //! @brief  ����ǰѡ�е����ⲿ������
    //! @return void
    void            moveCurrentSectionDown();
    //! @brief  ˢ�¸������ⲿ��
    //! @return void
    //! �����ݴ洢���������ã��������ý�����ʾ
    void            refreshSections();
    //! @brief  �������ⲿ��
    //! @param  item    QListWidgetItem*        [in]�б��ڵ��б��������ʾ�ò���
    //! @param  qi      const CQuestionSection& [in]���ⲿ��
    //! @return void
    void            setSection(QListWidgetItem* item, const CQuestionSection& qi);
    //! @brief  ˢ����������
    //! @return void
    void            updateCount();
    //! @brief  ������ã�������������ź�
    //! @return void
    void            finish();
protected:
    //! @brief  ��ʾ�¼��������ڵ�һ����ʾʱ��������
    //! @return void
    virtual void    showEvent(QShowEvent *event);
signals:
    //  �Ծ���������ź�
    void signalSetupFinished(const CQuestionPaper& paper);
protected slots:
    //  �������ⲿ��������ɵĲ�
    void slotSectionSetupFinished(const CQuestionSection& section);
};

#endif // !SETUP_PAPER_FRAME_H
