#ifndef DEVICEDATADETECT_H
#define DEVICEDATADETECT_H

#include <QWidget>
#include <QMouseEvent>
#include <QStandardItemModel>
#include "Public/DialogTip.h"
#include "DeviceDataAbnormal.h"
#include "DeviceDataNormal.h"
#include "etdatastruct.h"
#include "ETdeviceDb.h"



namespace Ui {
class DeviceDataDetect;
}

class DeviceDataDetect : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceDataDetect(QWidget *parent = 0);
    ~DeviceDataDetect();
    //qmj
    //所有数据库数据
    std::vector<ETdeviceInfo> AllStaticData;
    std::vector<ETdata_Result> AllDynamicData;
    //正常
    std::vector<ETdeviceInfo> NormalStaticData;
    std::vector<ETdata_Result> NormalDynamicData;
    //预警
    std::vector<ETdeviceInfo> WarningStaticData;
    std::vector<ETdata_Result> WarningDynamicData;
    //紧急
    std::vector<ETdeviceInfo> UrgentStaticData;
    std::vector<ETdata_Result> UrgentDynamicData;
    //搜索
    std::vector<ETdeviceInfo> searchStaticData;
    std::vector<ETdata_Result> searchDynamicData;
    //排序
    std::vector<ETdeviceInfo> sortedStaticData;
    std::vector<ETdata_Result> sortedDynamicData;
    //当前显示数据
    std::vector<ETdeviceInfo> StaticData;
    std::vector<ETdata_Result> DynamicData;
    //qmj
signals:
    void BacktoMainwindow();
    void SigReturn();
    void poptest();

private slots:

    void MenuReturn();
    void ExitTip();
    //qmj
    void on_pushButton_exit_clicked();

    void on_in1_clicked();
    void currentShow();

    void on_in2_clicked();

    void on_in3_clicked();

    void on_in4_clicked();

    void on_in5_clicked();

    //qmj
    void on_lastPage_clicked();

    void on_nextPage_clicked();

    void on_pushButton_2_clicked();

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_search_clicked();


    void on_pushButtonSort_clicked();

protected:
    void paintEvent(QPaintEvent *);
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    Ui::DeviceDataDetect *ui;
    DialogTip           * tip_;
    DeviceDataAbnormal  * abnormal;
    DeviceDataNormal    * normal;
    int                   pressPosX_;
    ETdeviceDb          * db;
    QStandardItemModel  *model;

    void ReadData();
    void DisplayData();
    void ReadDb();

    int curPage;
    int rowCount;
    int columnCount;
    int urgentNum;
    int displayState;//显示状态 0全部 1正常 2预警 3紧急 5搜索 6排序
    bool ascendSort;
};

#endif // DEVICEDATADETECT_H
