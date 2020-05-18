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
    //! @brief  ��ӡ
    //! @return void
    void doPrint();
    //! @brief  ��ӡԤ��
    //! @return void
    void doPrintPreview();
    //! @brief  ��ӡԤ���ص�����
    //! @return void
    void printPreview(QPrinter *printer);
    //! @brief  ��ӡ��PDF�ļ�
    //! @return void
    void printToPDF();
    //! @brief  ��ӡҳ������
    //! @return void
    //! ***** ʵ�ʲ������޷�����ҳ�߾�
    void doPageSetup();
    //! @brief  ���»����Ծ�
    //! @return void
    void refresh();

protected:
    //! @brief  ���ڳߴ�仯�¼�
    //! @return void
    //! �����ڴ��ڳߴ�仯���Զ��ػ�
    //! ***** �ߴ�仯���ܵ���ҳ�����÷����仯��ʹ�ô�ӡ����ƫ�ƣ�ԭ��δ��
    virtual void resizeEvent(QResizeEvent *event) override;
    //! @brief  ���ڹر��¼�
    //! @return void
    //! �����ڴ��ڹر�ʱ���������Ծ�����
    virtual void closeEvent(QCloseEvent *event) override;
};
