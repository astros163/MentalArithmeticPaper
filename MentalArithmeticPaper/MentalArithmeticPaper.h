#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MentalArithmeticPaper.h"
#include "SetupPaperWindow.h"
#include <QtPrintSupport/QPrinter>

class MentalArithmeticPaper : public QMainWindow
{
    Q_OBJECT

public:
    MentalArithmeticPaper(QWidget *parent = Q_NULLPTR);
    virtual ~MentalArithmeticPaper();
private:
    Ui::MentalArithmeticPaperMain ui;
    CSetupPaperWindow*  _pSetupWindow;
    CQuestionPaper      _Paper;
    QPrinter            _Printer;

private slots:
    //! @brief  打印
    //! @return void
    void doPrint();
    //! @brief  打印预览
    //! @return void
    void doPrintPreview();
    //! @brief  打印预览回调函数
    //! @return void
    void printPreview(QPrinter *printer);
    //! @brief  打印到PDF文件
    //! @return void
    void printToPDF();
    //! @brief  打印页面设置
    //! @return void
    //! ***** 实际测试中无法设置页边距
    void doPageSetup();
    //! @brief  重新绘制试卷
    //! @return void
    void refresh();

protected:
    //! @brief  窗口尺寸变化事件
    //! @return void
    //! 用于在窗口尺寸变化后自动重绘
    //! ***** 尺寸变化可能导致页面设置发生变化，使得打印出现偏移，原因未明
    virtual void resizeEvent(QResizeEvent *event) override;
    //! @brief  窗口关闭事件
    //! @return void
    //! 用于在窗口关闭时保存最后的试卷设置
    virtual void closeEvent(QCloseEvent *event) override;
};
