#include "DeviceDataDetect.h"
#include "ui_DeviceDataDetect.h"
#include <QPushButton>
#include <QPainter>
#include <QString>

DeviceDataDetect::DeviceDataDetect(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeviceDataDetect)
{
    LOGOUT;
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::FramelessWindowHint);
    tip_              = NULL;
    connect(this,SIGNAL(SigReturn()),this,SLOT(MenuReturn()));

    ui->pushButton_2->hide();
    //ui->pushButtonSort->hide();

    //qmj
    abnormal = NULL;
    normal = NULL;
    db = new ETdeviceDb;
    ui->total_Num->setText("共计：12条");

    model = new QStandardItemModel();
    ui->tableView->setModel(model);
    this->curPage = 1;
    this->rowCount = 5;
    this->columnCount = 6;
    this->displayState = 0;
    this->ascendSort = true;
    //数据模型行列数
    model->setRowCount(rowCount);
    model->setColumnCount(columnCount);
    model->setHeaderData(0,Qt::Horizontal,"序号");
    model->setHeaderData(1,Qt::Horizontal,"变电站名称");
    model->setHeaderData(2,Qt::Horizontal,"设备名称");
    model->setHeaderData(3,Qt::Horizontal,"型号");
    model->setHeaderData(4,Qt::Horizontal,"维修日期");
    model->setHeaderData(5,Qt::Horizontal,"状态等级");
    ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
//    ui->tableView->horizontalHeader()->setStyleSheet("QHeaderView::section{border: 0px solid white;"
//                                                     "color: white;"
//                                                     "font: bold 32px;"
//                                                     "background-color:rgb(14, 46, 86);"
//                                                     "min-height:88px;}");

    //设置列宽
      ui->tableView->setColumnWidth(0, 210);
      ui->tableView->setColumnWidth(1, 340);
      ui->tableView->setColumnWidth(2, 340);
      ui->tableView->setColumnWidth(3, 340);
      ui->tableView->setColumnWidth(4, 340);
      ui->tableView->setColumnWidth(5, 340);

      //设置行高
      ui->tableView->setRowHeight(0,120);
      ui->tableView->setRowHeight(1,120);
      ui->tableView->setRowHeight(2,120);
      ui->tableView->setRowHeight(3,120);
      ui->tableView->setRowHeight(4,120);

      ui->tableView->setShowGrid(false);
      ui->tableView->setSelectionMode(QAbstractItemView::NoSelection);

    //隐藏行号
    ui->tableView->verticalHeader()->hide();
    //bu ke bian ji
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //交替颜色
    ui->tableView->setAlternatingRowColors(true);



    ReadDb();
    ui->total_Num->setText("共计："+QString::number(AllDynamicData.size(),10)+"条");
    //time
//    for(unsigned int i=0;i<AllStaticData.size();i++)
//    {
//        ETdeviceInfo tempinfo = AllStaticData[i];
//        ETdata_Result tempresult = AllDynamicData[i];
//        tempinfo.BuildDate = QDateTime::currentDateTime();
//        tempinfo.ProduceDate = QDateTime::currentDateTime();
//        tempresult.RepairDate = QDateTime::currentDateTime();
//        tempresult.Processed = false;
//        db->UpdateDynamicData(tempresult);
//        db->UpdateStaticData(tempinfo);
//    }

    //time
    ReadData();
    for(int i=0;i<rowCount;i++)
    {
        for(int j=0;j<columnCount-1;j++)
        {

            model->setItem(i, j, new QStandardItem);
            model->item(i, j)->setTextAlignment(Qt::AlignCenter);
        }
    }
    DisplayData();

    LOGOUT;
}

DeviceDataDetect::~DeviceDataDetect()
{
    LOGOUT;
    delete ui;
    if(db != NULL)
    {
        delete db;
        db = NULL;
    }
}

void DeviceDataDetect::on_pushButton_exit_clicked()
{
    emit BacktoMainwindow();
}


void DeviceDataDetect::MenuReturn()
{
        if(!tip_){
            tip_ = new DialogTip("确定退出检测数据？",this);
            connect(tip_,SIGNAL(Exit()),this,SIGNAL(BacktoMainwindow()));
            connect(tip_,SIGNAL(ExitTip()),this,SLOT(ExitTip()));
            tip_->move(500,300);
            tip_->show();
        }else{
            ExitTip();
        }
}


void DeviceDataDetect::ExitTip()
{
    if(tip_){
        tip_->deleteLater();
        tip_ = NULL;
    }
}

void DeviceDataDetect::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void DeviceDataDetect::mousePressEvent(QMouseEvent *event)
{
    event->accept();
    pressPosX_ = event->pos().x();
}

void DeviceDataDetect::mouseReleaseEvent(QMouseEvent *event)
{
    event->accept();
    int offsetX = pressPosX_ - event->pos().x();
    if(offsetX > CLOSEOFFSETX || offsetX < -CLOSEOFFSETX){
        emit SigReturn();
    }
}


void DeviceDataDetect::currentShow()
{
    if(abnormal)
    {
        abnormal->close();
        delete abnormal;
        abnormal = NULL;
        this->setVisible(true);
    }
    if(normal)
    {
        normal->close();
        delete normal;
        normal = NULL;
        this->setVisible(true);
    }
}

void DeviceDataDetect::on_in1_clicked()
{
    if(StaticData.size()>=1)
    {
        if(DynamicData[0].RunStatus==0)
        {
            if(!normal)
            {
                normal = new DeviceDataNormal;
                normal->UpdateData(DynamicData[0], StaticData[0]);
                connect(normal, SIGNAL(backToDetect()), this, SLOT(currentShow()));
                this->setVisible(false);
                normal->show();
            }
        }
        else
        {
            if(!abnormal)
            {
                abnormal = new DeviceDataAbnormal;
                abnormal->UpdateData(DynamicData[0], StaticData[0]);
                connect(abnormal, SIGNAL(backToDetect()), this, SLOT(currentShow()));
                this->setVisible(false);
                abnormal->show();
            }
        }
    }

}

void DeviceDataDetect::on_in2_clicked()
{
    if(StaticData.size()>=2)
    {
        if(DynamicData[1].RunStatus==0)
        {
            if(!normal)
            {
                normal = new DeviceDataNormal;
                normal->UpdateData(DynamicData[1], StaticData[1]);
                connect(normal, SIGNAL(backToDetect()), this, SLOT(currentShow()));
                this->setVisible(false);
                normal->show();
            }
        }
        else
        {
            if(!abnormal)
            {
                abnormal = new DeviceDataAbnormal;
                abnormal->UpdateData(DynamicData[1], StaticData[1]);
                connect(abnormal, SIGNAL(backToDetect()), this, SLOT(currentShow()));
                this->setVisible(false);
                abnormal->show();
            }
        }
    }
}


void DeviceDataDetect::on_in3_clicked()
{
    if(StaticData.size()>=3)
    {
        if(DynamicData[2].RunStatus==0)
        {
            if(!normal)
            {
                normal = new DeviceDataNormal;
                normal->UpdateData(DynamicData[2], StaticData[2]);
                connect(normal, SIGNAL(backToDetect()), this, SLOT(currentShow()));
                this->setVisible(false);
                normal->show();
            }
        }
        else
        {
            if(!abnormal)
            {
                abnormal = new DeviceDataAbnormal;
                abnormal->UpdateData(DynamicData[2], StaticData[2]);
                connect(abnormal, SIGNAL(backToDetect()), this, SLOT(currentShow()));
                this->setVisible(false);
                abnormal->show();
            }
        }
    }
}


void DeviceDataDetect::on_in4_clicked()
{
    if(StaticData.size()>=4)
    {
        if(DynamicData[3].RunStatus==0)
        {
            if(!normal)
            {
                normal = new DeviceDataNormal;
                normal->UpdateData(DynamicData[3], StaticData[3]);
                connect(normal, SIGNAL(backToDetect()), this, SLOT(currentShow()));
                this->setVisible(false);
                normal->show();
            }
        }
        else
        {
            if(!abnormal)
            {
                abnormal = new DeviceDataAbnormal;
                abnormal->UpdateData(DynamicData[3], StaticData[3]);
                connect(abnormal, SIGNAL(backToDetect()), this, SLOT(currentShow()));
                this->setVisible(false);
                abnormal->show();
            }
        }
    }
}

void DeviceDataDetect::on_in5_clicked()//zai ci pan duan normal haishi abnormal
{
    if(StaticData.size()>=5)
    {
        if(DynamicData[4].RunStatus==0)
        {
            if(!normal)
            {
                normal = new DeviceDataNormal;
                normal->UpdateData(DynamicData[4], StaticData[4]);
                connect(normal, SIGNAL(backToDetect()), this, SLOT(currentShow()));
                this->setVisible(false);
                normal->show();
            }
        }
        else
        {
            if(!abnormal)
            {
                abnormal = new DeviceDataAbnormal;
                abnormal->UpdateData(DynamicData[4], StaticData[4]);
                connect(abnormal, SIGNAL(backToDetect()), this, SLOT(currentShow()));
                this->setVisible(false);
                abnormal->show();
            }
        }
    }
}

void DeviceDataDetect::ReadDb()
{
    std::vector<ETdeviceInfo> temp_StaticData;
    this->db->GetStaticDatas(temp_StaticData, 0, db->StaticDataSize);
    this->AllStaticData.clear();
    this->AllDynamicData.clear();
    this->NormalStaticData.clear();
    this->NormalDynamicData.clear();
    this->WarningStaticData.clear();
    this->WarningDynamicData.clear();
    this->UrgentStaticData.clear();
    this->UrgentDynamicData.clear();
    this->sortedDynamicData.clear();
    this->sortedStaticData.clear();
    this->urgentNum = 0;
    for(unsigned int i=0;i<temp_StaticData.size();i++)
    {
        std::vector<ETdata_Result> temp_result;
        db->GetDynamicData(temp_result, temp_StaticData[i].ETdeviceID);
        for(unsigned int k=0;k<temp_result.size();k++)
        {
            this->AllDynamicData.push_back(temp_result[k]);
            this->AllStaticData.push_back(temp_StaticData[i]);
            if(temp_result[k].RunStatus == 0)//normal
            {
                this->NormalDynamicData.push_back(temp_result[k]);
                this->NormalStaticData.push_back(temp_StaticData[i]);
            }
            else if(temp_result[k].RunStatus == 1)//warning
            {
                this->WarningDynamicData.push_back(temp_result[k]);
                this->WarningStaticData.push_back(temp_StaticData[i]);
            }
            else if(temp_result[k].RunStatus == 2)//urgent
            {
                this->UrgentDynamicData.push_back(temp_result[k]);
                this->UrgentStaticData.push_back(temp_StaticData[i]);
                this->urgentNum++;
            }
        }

    }
    this->sortedDynamicData = this->AllDynamicData;
    sort(this->sortedDynamicData.begin(), this->sortedDynamicData.end(), MyCompareRepairDateLittler());
    for(unsigned int j=0;j<this->sortedDynamicData.size();j++)
    {
        ETdeviceInfo info;
        info.ETdeviceID = this->sortedDynamicData[j].ETdeviceID;
        db->GetStaticData(info, this->sortedDynamicData[j].ETdeviceID);
        this->sortedStaticData.push_back(info);
    }

    ui->UrgentNum_label->setText("紧急："+QString::number(this->urgentNum,10)+"条");
}


void DeviceDataDetect::ReadData()
{
    this->StaticData.clear();
    this->DynamicData.clear();
    if(this->displayState == 0)//all
    {
        for(unsigned int i=(this->curPage-1)*5;(i<(this->curPage-1)*5+5)&&(i<this->AllStaticData.size());i++)
        {
            this->StaticData.push_back(this->AllStaticData[i]);
            this->DynamicData.push_back(this->AllDynamicData[i]);
        }
    }
    else if(this->displayState == 1)//normal
    {
        for(unsigned int i=(this->curPage-1)*5;(i<(this->curPage-1)*5+5)&&(i<this->NormalDynamicData.size());i++)
        {
            this->StaticData.push_back(this->NormalStaticData[i]);
            this->DynamicData.push_back(this->NormalDynamicData[i]);
        }
    }
    else if(this->displayState == 2)//warning
    {
        for(unsigned int i=(this->curPage-1)*5;(i<(this->curPage-1)*5+5)&&(i<this->WarningDynamicData.size());i++)
        {
            this->StaticData.push_back(this->WarningStaticData[i]);
            this->DynamicData.push_back(this->WarningDynamicData[i]);
        }
    }
    else if(this->displayState == 3)//urgent
    {
        for(unsigned int i=(this->curPage-1)*5;(i<(this->curPage-1)*5+5)&&(i<this->UrgentDynamicData.size());i++)
        {
            this->StaticData.push_back(this->UrgentStaticData[i]);
            this->DynamicData.push_back(this->UrgentDynamicData[i]);
        }
    }
    else if(this->displayState == 5)//search
    {
        for(unsigned int i=(this->curPage-1)*5;(i<(this->curPage-1)*5+5)&&(i<this->searchDynamicData.size());i++)
        {
            this->StaticData.push_back(this->searchStaticData[i]);
            this->DynamicData.push_back(this->searchDynamicData[i]);
        }

    }
    else if(this->displayState == 6)//sort
    {
        for(unsigned int i=(this->curPage-1)*5;(i<(this->curPage-1)*5+5)&&(i<this->sortedDynamicData.size());i++)
        {
            this->StaticData.push_back(this->sortedStaticData[i]);
            this->DynamicData.push_back(this->sortedDynamicData[i]);
        }

    }
}

void DeviceDataDetect::DisplayData()
{
    QLabel *labels[5];
    labels[0] = ui->status_label_1;
    labels[1] = ui->status_label_2;
    labels[2] = ui->status_label_3;
    labels[3] = ui->status_label_4;
    labels[4] = ui->status_label_5;
    QPushButton *buttons[5];
    buttons[0] = ui->in1;
    buttons[1] = ui->in2;
    buttons[2] = ui->in3;
    buttons[3] = ui->in4;
    buttons[4] = ui->in5;


    QString statusColor;
    for(int i=0;i<rowCount;i++)
        {
            if(i<StaticData.size())//数据满格
            {
                if((curPage-1)*5+i+1<10)
                {
                    model->item(i, 0)->setText("0"+QString("%1").arg((curPage-1)*5+i+1));
                }
                else
                {
                    model->item(i, 0)->setText(QString("%1").arg((curPage-1)*5+i+1));
                }
                model->item(i, 1)->setText(StaticData[i].ETstationName);
                model->item(i, 2)->setText(StaticData[i].DeviceName);
                model->item(i, 3)->setText(StaticData[i].ProduceNum);
                model->item(i, 4)->setText(DynamicData[i].RepairDate.toString("yyyy-MM-dd"));

                labels[i]->setVisible(true);
                buttons[i]->setVisible(true);
                if(DynamicData[i].RunStatus==0)//normal
                {
                    statusColor = "#13AABF";
                    buttons[i]->setText("正常");
                }
                else if(DynamicData[i].RunStatus==1)//warning
                {
                    statusColor = "#F7931E";
                    buttons[i]->setText("预警");
                }
                else
                {
                    statusColor = "#FF0000";
                    buttons[i]->setText("紧急");
                }
                labels[i]->setStyleSheet("background-color:"+statusColor);
            }
           else
           {
                model->item(i, 0)->setText("");
                model->item(i, 1)->setText("");
                model->item(i, 2)->setText("");
                model->item(i, 3)->setText("");
                model->item(i, 4)->setText("");

                labels[i]->setVisible(false);
                buttons[i]->setVisible(false);
           }
        }
    ui->UrgentNum_label->setText("紧急："+QString::number(urgentNum,10)+"条");
}
void DeviceDataDetect::on_lastPage_clicked()
{
    if(this->curPage>1)
    {
        this->curPage--;
    }
    this->ReadData();
    this->DisplayData();
}



void DeviceDataDetect::on_nextPage_clicked()
{
    if(StaticData.size()==rowCount)
    {
        this->curPage++;
        this->ReadData();
        if(StaticData.size()>0)
        {
            this->DisplayData();
        }
        else
        {
            this->curPage--;
        }
    }
}


void DeviceDataDetect::on_pushButton_2_clicked()
{
    emit poptest();
}

void DeviceDataDetect::on_comboBox_currentIndexChanged(const QString &arg1)
{
    //状态筛选代码 全部 正常 预警 紧急
    if(arg1 == "全部")
    {
        this->displayState = 0;
        ui->total_Num->setText("共计："+QString::number(AllDynamicData.size(),10)+"条");
    }
    else if(arg1 == "正常")
    {
        this->displayState = 1;
        ui->total_Num->setText("共计："+QString::number(NormalDynamicData.size(),10)+"条");
    }
    else if(arg1 == "预警")
    {
        this->displayState = 2;
        ui->total_Num->setText("共计："+QString::number(WarningDynamicData.size(),10)+"条");
    }
    else if(arg1 == "紧急")
    {
        this->displayState = 3;
        ui->total_Num->setText("共计："+QString::number(UrgentDynamicData.size(),10)+"条");
    }
    curPage = 1;
    ReadData();
    DisplayData();
}
//搜索
void DeviceDataDetect::on_search_clicked()
{
//    QString ret = ui->lineEdit->text();
//    this->searchStaticData.clear();
//    this->searchDynamicData.clear();
//    std::vector<ETdeviceInfo> temp_searchStaticData;
//    this->db->LookforStaticData(temp_searchStaticData, ret);
//    for(unsigned int i=0;i<temp_searchStaticData.size();i++)
//    {
//        std::vector<ETdata_Result> temp_result;
//        db->GetDynamicData(temp_result, temp_searchStaticData[i].ETdeviceID);
//        for(unsigned int j=0;j<temp_result.size();j++)
//        {
//            this->searchDynamicData.push_back(temp_result[j]);
//            this->searchStaticData.push_back(temp_searchStaticData[i]);
//        }

//    }
//    this->displayState = 5;
//    ui->total_Num->setText("共计："+QString::number(searchStaticData.size(),10)+"条");
//    curPage = 1;
//    ReadData();
//    DisplayData();
}


void DeviceDataDetect::on_pushButtonSort_clicked()
{
    this->displayState = 6;
    ascendSort = !ascendSort;
    if(ascendSort)
    {
        ui->pushButtonSort->setIcon(QPixmap(":/images/icon_sortup.png"));
    }
    else
    {
        ui->pushButtonSort->setIcon(QPixmap(":/images/icon_sortdown.png"));
    }
    reverse(this->sortedStaticData.begin(), this->sortedStaticData.end());
    reverse(this->sortedDynamicData.begin(), this->sortedDynamicData.end());
    curPage = 1;
    ReadData();
    DisplayData();
}
