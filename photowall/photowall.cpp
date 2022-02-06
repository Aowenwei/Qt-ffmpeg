#include "photowall.h"
#include "ui_photowall.h"
#include "picturebutton.h"
#include "pictureview.h"
#include "qpixmap.h"
#include <QPixmap>
#include <QPainterPath>
#include <QGraphicsView>
#include <QDebug>
#include <QQueue>
#include <QGraphicsItemAnimation>
#include <QTimeLine>
#include <cmath>
#include <ctime>
#include <QTransform>
#include <QButtonGroup>
#include <QMap>
#include <QTimer>
#include <QButtonGroup>
#include <QGraphicsScene>
#include <QButtonGroup>
#include <QGraphicsScene>
#include <QGraphicsItemAnimation>

static int dir = 0;//��¼����
static QList<qreal> spaceList;
static QList<qreal> unitList;
static QList<qreal> transScaleList;//���ű�����
static QSize pictrueBigSize = RAW_VIEW_SIZE / SCALE_VIEW_PIXMAP;
static QSize pictrueSmallSize = RAW_VIEW_SIZE / SCALE_VIEW_PIXMAP / SCALE_BIG_SMALL;
static QList<pictureItem*> itemList;
static QList<int> finishList;
static QMap<int, pictureItem*> mapLink;  //��ťid �� ͼƬ��Դ��ӳ��
static int startNum = 0;
static QList<QPointF> pointA;

PhotoWall::PhotoWall(QWidget* parent) :
	QWidget(parent),
	ui(new Ui::PhotoWall)
{
	ui->setupUi(this);
	m_timer = new QTimer(this),
		m_scene = new QGraphicsScene(this);
	m_index = 0;
	m_currentRule = RuleA;
	m_rollCount = 0;
	btnMoveEnable = true;

	for (int i = 0; i < 10; i++) {
		item[i] = new QGraphicsItemAnimation(this);
	}

	setAttribute(Qt::WA_StyledBackground); //������ʽ��
	setButtonGroup(); //���ð�ť��
	setInitList();
	setPictureScreen();
	setTimerAndConnect();

	m_newT = new QTimer(this);
	m_newT->setInterval(5000);
	connect(m_newT, &QTimer::timeout, [this]() {
		on_btnR_clicked();
		});
	m_newT->start();
}

PhotoWall::~PhotoWall()
{
	delete ui;
}
void PhotoWall::setButtonGroup()
{
	m_BtnGroup = new QButtonGroup(this);
	m_BtnGroup->addButton(ui->btnPic0, 0);
	m_BtnGroup->addButton(ui->btnPic1, 1);
	m_BtnGroup->addButton(ui->btnPic2, 2);
	m_BtnGroup->addButton(ui->btnPic3, 3);
	m_BtnGroup->addButton(ui->btnPic4, 4);
	m_BtnGroup->addButton(ui->btnPic5, 5);
	m_BtnGroup->addButton(ui->btnPic6, 6);
	m_BtnGroup->addButton(ui->btnPic7, 7);
	m_BtnGroup->addButton(ui->btnPic8, 8);
	m_BtnGroup->addButton(ui->btnPic9, 9);
	m_BtnGroup->setExclusive(true);
	m_BtnGroup->button(1)->setChecked(true);
	for (int i = 0; i < 10; i++) {
		static_cast<pictureButton*>(m_BtnGroup->button(i))->setId(i);
	}
}

void PhotoWall::setInitList()
{
	m_PointList << P1 << P2 << P3 << P4 << P5 << P6 << P7 << P8 << P9 << P10;
	m_ZValueList << 1 << 2 << 1 << 0 << 0 << 0 << 0 << 0 << 0 << 0;
	m_PixmapScaleList << 0.8 << 1 << 0.8 << 0.8 << 0.8 << 0.8 << 0.8 << 0.8 << 0.8 << 0.8;
}

void PhotoWall::setPictureScreen()
{
	//������ʽ �ޱ�߿�
	ui->graphicsView->setStyleSheet("background: transparent; padding: 0px; border: 0px;");
	ui->graphicsView->setScene(m_scene);
	m_scene->setSceneRect(0, 0, RAW_VIEW_SIZE.width(), RAW_VIEW_SIZE.height());

	m_MidLine.setPoints(QPointF(0, 0), \
		QPointF(RAW_VIEW_SIZE.width(), 0));

	//��Ӷ�ӦͼƬ
	for (int i = 1; i <= 10; i++) {
		m_PixmapList.append(QPixmap(QString(":/photowall/%1.png").arg(i)));
	}
	for (int i = 0; i < 10; i++) {
		itemList.append(new pictureItem(m_PixmapList[i].scaled(pictrueBigSize,
			Qt::KeepAspectRatio, Qt::SmoothTransformation)));
		itemList[i]->setScale(m_PixmapScaleList[i]);
		itemList[i]->setType(i);
		itemList[i]->setItemId(i);
		itemList[i]->setOffset(QPointF(0, 0));
	}
	//ͼԪ���ͼƬ
	for (int i = 0; i < 10; i++)
	{
		m_scene->addItem(itemList[i]);//���ͼԪ
		itemList[i]->setPos(m_MidLine.pointAt(m_PointList[i]));//����λ��
		itemList[i]->setZValue(m_ZValueList[i]);//������ʾ���ȼ�
		itemList[i]->setTransformationMode(Qt::SmoothTransformation);//��������ģʽ
		if (i != 1)
		{
			itemList[i]->setPos(itemList[i]->x(), RAW_VIEW_SIZE.height() / 10);
		}
		pointA.append(itemList[i]->pos());  //���뵽��Ӧλ��
		qDebug() << pointA[i].x() << ": " << pointA[i].y();
	}
	//����ӳ��map��
	for (int i = 0; i < 10; i++)
	{
		mapLink.insert(static_cast<pictureButton*>(m_BtnGroup->button(i))->id(), itemList[i]);
	}

	//��ӡ��Ϣ
	//    QMap<int,pictureItem *>::const_iterator it;

	//    for (it = mapLink.constBegin();it!=mapLink.constEnd();it++) {
	//        qDebug()<< it.key()<< ": " << it.value();
	//    }

}

void PhotoWall::setTimerAndConnect()
{
	//���ó���ʱ���֡���������ʱʱ��,����ʱ��/��֡��*����ʱ�䣩�������1000��תΪ��
	m_timer->setInterval(DURATION_MS / (FPS * DURATION_MS / 1000));
	connect(m_timer, &QTimer::timeout, this, &PhotoWall::timerOutFunc);
	for (int i = 0; i < 10; i++)
	{
		connect(itemList[i], &pictureItem::clickedId, this, &PhotoWall::clickedItemRoll);

		void (pictureButton:: * funcPtr)(int) = &pictureButton::entered;
		connect(static_cast<pictureButton*>(m_BtnGroup->button(i)), funcPtr, [this](int id) {
			pictureItem* p = mapLink.value(id); //��ȡid ��ͼƬֵ
			btnMoveEnable = false;
			clickedItemRoll(p->type());
			qDebug() << p->type();
			});
		//        connect(static_cast<pictureButton*>(m_BtnGroup->button(i)),&pictureButton::stop,[this](){
		//            m_timer->stop();
		//            //            m_timer->
		//            qDebug()<<"��ͣ\n";
		//        });
	}

}

//int MainWindow::getIndexByRules(int oldIndex, int rule)
//{
//    qDebug()<<"rule = "<<rule;
//    switch (rule) {
//    case 1:
//        return  (oldIndex+1)/10;
//    case -1:
//        return oldIndex==0?9:oldIndex-1;
//    default:
//        return 0;
//    }

//}

void PhotoWall::rollItem(int rollDir, unsigned rollCount)
{
	rollCount = 0;
	if (m_timer->isActive())
		return;
	//��ȡ�µ�����
	//��Ҫλ��  ��λ��
	int nbegin = rollDir;
	startNum = getrightN(nbegin);
	m_timer->start();
}

int PhotoWall::getrightN(int num)
{
	qDebug() << "num = " << num;
	if (num == -1)
	{
		num = 9;
	}
	if (num == 10)
	{
		num = 0;
	}

	return num;
}

void PhotoWall::timerOutFunc()
{
	//    QVector<QGraphicsItemAnimation *>item(10);

	//    for (int i = 0; i < 10; i++) {
	//        item[i] = new QGraphicsItemAnimation();
	//    }

	QTimeLine* timeline = new QTimeLine(200);
	timeline->setLoopCount(1); //����3��
	int first = getrightN(startNum - 1);
	for (int i = 0; i < 10; i++) {
		itemList[i] = mapLink.value(first % 10);
		first++;
		itemList[i]->setScale(m_PixmapScaleList[i]);
		itemList[i]->setZValue(m_ZValueList[i]);//������ʾ���ȼ�
		itemList[i]->setTransformationMode(Qt::SmoothTransformation);//��������ģʽ
		item[i]->setItem(itemList[i]);
		item[i]->setTimeLine(timeline);
		item[i]->setPosAt(1, pointA[i]);
		itemList[i]->setPos(pointA[i]);
	}
	timeline->start();
	m_scene->invalidate();
	m_timer->stop();
}

void PhotoWall::clickedItemRoll(int type)
{
	//�鿴��ʱ���Ƿ�����
	if (m_timer->isActive())
		return;
	rollItem(type, 0); //�õ����µ�id ��ťλ��
}

//����˶�
void PhotoWall::on_btnL_clicked()
{
	int id = m_BtnGroup->checkedId();
	if (id - 1 < 0)
	{
		id = 8;
	}
	else {
		id = id - 1;
	}
	m_BtnGroup->button(id)->setChecked(true);
	rollItem(id, 0);
}
//�ұ��˶�
void PhotoWall::on_btnR_clicked()
{
	int id = m_BtnGroup->checkedId();
	id = (id + 1) % 10;
	m_BtnGroup->button(id)->setChecked(true);
	rollItem(id, 0);
}

template<typename T>
void PhotoWall::rollList(QList<T>& oldList, int dir, int count)
{
	for (int i = 0; i < count; i++)
	{
		if (dir == 1)
		{
			oldList.prepend(oldList.last());
			oldList.removeLast();
		}
		else if (dir == -1)
		{
			oldList.append(oldList.first());
			oldList.removeFirst();
		}
	}

}