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
    //! @brief  设定试卷设置
    //! @param  paper   const CQuestionPaper&   [in]试卷设置
    //! @return void
    //! 设定的试卷设置会在窗口显示的时候更新
    void            setPaper(const CQuestionPaper& paper);
    //! @brief  取得试卷设置
    //! @return CQuestionPaper&                 试卷设置
    CQuestionPaper  getPaper()    const;
private:
    //! @brief  添加新的试题部分
    //! @param  qi      const CQuestionSection& [in]试题部分
    //! @return void
    void            addSection(const CQuestionSection& qi);
    //! @brief  删除当前选中的试题部分
    //! @return void
    void            delCurrentSection();
    //! @brief  将当前选中的试题部分上移
    //! @return void
    void            moveCurrentSectionUp();
    //! @brief  将当前选中的试题部分下移
    //! @return void
    void            moveCurrentSectionDown();
    //! @brief  刷新各个试题部分
    //! @return void
    //! 即根据存储的试题设置，重新设置界面显示
    void            refreshSections();
    //! @brief  设置试题部分
    //! @param  item    QListWidgetItem*        [in]列表窗口的列表项，用于显示该部分
    //! @param  qi      const CQuestionSection& [in]试题部分
    //! @return void
    void            setSection(QListWidgetItem* item, const CQuestionSection& qi);
    //! @brief  刷新试题总数
    //! @return void
    void            updateCount();
    //! @brief  完成设置，发送设置完成信号
    //! @return void
    void            finish();
protected:
    //! @brief  显示事件，用于在第一次显示时更新数据
    //! @return void
    virtual void    showEvent(QShowEvent *event);
signals:
    //  试卷设置完成信号
    void signalSetupFinished(const CQuestionPaper& paper);
protected slots:
    //  处理试题部分设置完成的槽
    void slotSectionSetupFinished(const CQuestionSection& section);
};

#endif // !SETUP_PAPER_FRAME_H
