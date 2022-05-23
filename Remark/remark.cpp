#if _MSC_VER >= 1600 // MSVC2015>1899,对于MSVC2010以上版本都可以使用
#pragma execution_character_set("utf-8")
#endif
#include "remark.h"
#include "ui_remark.h"
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QLabel>
#include <QLineEdit>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPushButton>
#include <QSpacerItem>
#include <QVBoxLayout>
Remark::Remark(QWidget* parent) : QWidget(parent), ui(new Ui::Remark) {
	ui->setupUi(this);

	manager = new QNetworkAccessManager(this);
	ui->widget->resize(1024, 280);
	/*for (int i = 0; i != 20; i++) {
					addWidget();
					QFrame* Hline = new QFrame(ui->widget);
					Hline->move(0, listwidget.at(i)->width() + 10);
					Hline->setFrameShape(QFrame::HLine);
					ui->verticalLayout_2->addWidget(Hline);
	}*/
}

Remark::~Remark() { delete ui; }

void Remark::addWidget(HotComments hotmsg, int index) {
	QWidget* widget = new QWidget(ui->widget);
	widget->setObjectName(QString("widget_%1").arg(index));
	widget->setMinimumSize(16777215, 55);
	QGridLayout* grid = new QGridLayout(widget);
	QPushButton* imge = new QPushButton(widget);
	imge->setIconSize(imge->size());
	imge->setIcon(QIcon(hotmsg.image));
	imge->setStyleSheet("border-radius:30px");

	/*imge->setStyleSheet(
					"border-image:url(:/1.jpg);background-color:blue;border-radius:30px");*/
	imge->setMaximumSize(50, 50);
	imge->setMinimumSize(50, 50);
	grid->addWidget(imge, 0, 0, 2, 1);

	//名字
	QPushButton* name = new QPushButton(hotmsg.nickname + ":", widget);
	name->setStyleSheet("text-align: left;");
	name->setMaximumWidth(205);
	name->setMinimumHeight(30);
	name->adjustSize();
	//评论内容
	QLabel* lab = new QLabel(hotmsg.content, widget);
	lab->setAlignment(Qt::AlignLeft);
	lab->setStyleSheet("border-width:0;border-style:outset;color:#373737;");
	lab->setWordWrap(true);
	lab->adjustSize();
	// QLineEdit* linedit = new QLineEdit(hotmsg.content);
	// linedit->setStyleSheet("QLineEdit{border-width:0;border-style:outset;color:#373737}");

	//禁止编辑
	// linedit->setEnabled(false);
	// linedit->setMinimumWidth(23);
	QHBoxLayout* hbox = new QHBoxLayout();
	//横向布局两个控件
	hbox->addWidget(name);
	hbox->addWidget(lab);
	grid->addLayout(hbox, 0, 1, 1, 1);

	//下面部分布局
	//评论时间
	QLabel* date = new QLabel(hotmsg.time, widget);
	date->setMinimumSize(20, 15);

	QSpacerItem* hitem =
		new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

	//点赞人数
	QPushButton* like =
		new QPushButton(QString::number(hotmsg.likedCount), widget);
	like->setMinimumSize(20, 15);

	QFrame* vline_1 = new QFrame(widget);
	vline_1->setMaximumSize(QSize(16777215, 40));
	vline_1->setFrameShape(QFrame::VLine);
	vline_1->setFrameShadow(QFrame::Sunken);

	QPushButton* shared = new QPushButton(widget);
	shared->setMinimumSize(20, 15);
	shared->setIcon(QIcon(":/image/shered_h.png"));
	shared->setStyleSheet("border-radius:30px;");

	QFrame* vline_2 = new QFrame(widget);
	vline_2->setMaximumSize(QSize(16777215, 40));
	vline_2->setFrameShape(QFrame::VLine);
	vline_2->setFrameShadow(QFrame::Sunken);

	QPushButton* msg = new QPushButton(widget);
	msg->setMinimumSize(20, 15);
	msg->setIcon(QIcon(":/image/msg_h.png"));
	msg->setStyleSheet("border-radius:30px;");

	QHBoxLayout* hbox_1 = new QHBoxLayout();
	hbox_1->addWidget(date);
	hbox_1->addItem(hitem);
	hbox_1->addWidget(like);
	hbox_1->addWidget(vline_1);
	hbox_1->addWidget(shared);
	hbox_1->addWidget(vline_2);
	hbox_1->addWidget(msg);
	grid->addLayout(hbox_1, 1, 1, 1, 1);
	//    widget ->setStyleSheet("background-color:blue");
	widget->setMaximumSize(16777215, 150);
	widget->setMinimumSize(16777215, 75);
	ui->verticalLayout_3->addWidget(widget);
	listwidget.push_back(widget);

	QFrame* Hline = new QFrame(ui->widget);
	Hline->move(0, listwidget.at(index)->width() + 10);
	Hline->setFrameShape(QFrame::HLine);
	ui->verticalLayout_3->addWidget(Hline);
	line.push_back(Hline);

	qDebug() << "addWidget() *****************";
}

void Remark::addLine(int i) {
	QFrame* vline_3 = new QFrame(this);
	vline_3->setGeometry(
		QRect(0, listwidget.at(i)->height() + 2, listwidget.at(i)->width(), 10));
	vline_3->setFrameShape(QFrame::HLine);
	vline_3->setFrameShadow(QFrame::Sunken);
}

// offset: 偏移数量 , 用于分页 , 如 :(评论页数 -1）*20， 其中 20 为 limit 的值
void Remark::getMusicComment(int id, int offset, int limit) {
	QString url =
		QString("http://localhost:3000/comment/music?id=%1&limit=%2&offset=%3")
		.arg(id)
		.arg(offset)
		.arg(limit);
	Netcomment = manager->get(QNetworkRequest(url));
	connect(Netcomment, &QNetworkReply::finished, this,
		&Remark::on_finshedMusicComment);
}

void Remark::getSongMenuComment(size_t id, int offset, int limit) {
	ui->btn_image->hide();
	ui->lab_title->hide();
	ui->btn_album->hide();
	ui->btn_artistname->hide();
	ui->label_2->hide();
	ui->label_4->hide();

	QString url =
		QString("http://localhost:3000/comment/playlist?id=%1&limit=%2&offset=%3")
		.arg(id)
		.arg(limit)
		.arg(offset);
	Netcomment = manager->get(QNetworkRequest(url));
	connect(Netcomment, &QNetworkReply::finished, this,
		&Remark::on_finshedSongMenuComment);
}

void Remark::getMvComment(long long mvid, int offset, int limit)
{
	QString Url = QString("http://localhost:3000/comment/mv?id=%1&limit=%2&offset=%3").arg(mvid).arg(limit)
		.arg(offset);
	qDebug() << Url;
	NetMvComment = manager->get(QNetworkRequest(Url));
	connect(NetMvComment, &QNetworkReply::finished, this, [&]() {
		if (NetMvComment->error() == QNetworkReply::NoError) {
			QJsonParseError err_t{};
			QJsonDocument docment = QJsonDocument::fromJson(NetMvComment->readAll(), &err_t);
			if (err_t.error == QJsonParseError::NoError) {
				QJsonObject rot = docment.object();
				QJsonValue val = rot.value("hotComments");
				if (val.isArray()) {
					QJsonArray ary = val.toArray();
					parsepublic(MvComment, ary);
				}

				//if (val.isArray()) {
				//    HotComments hot{};
				//    MvComment.clear();
					//QJsonArray ary = val.toArray();
				//    foreach(const auto && rhs, ary) {
				//        if (rhs.isObject()) {
				//            QJsonObject obj = rhs.toObject();
				//            QJsonValue UserVal = obj.value("user");
				//            if (UserVal.isObject()) {
				//              QJsonObject userobj=  UserVal.toObject();
				//            hot.userid= userobj.value("userId").toVariant().toLongLong();
				//            hot.avatarUrl = userobj.value("avatarUrl").toString();
				//            hot.nickname = userobj.value("nickname").toString();
				//            }

				//            //继续解析

				//        }
				//    }
				//}

			}
		}
		});
}

void Remark::getVideoComment(const QString& id, int offset, int limit)
{
	QString Url = QString("http://localhost:3000/comment/video?id=%1").arg(id).arg(limit)
		.arg(offset);
	NetVideoComment = manager->get(QNetworkRequest(Url));
	connect(NetVideoComment, &QNetworkReply::finished, this, [&] {

		if (NetVideoComment->error() == QNetworkReply::NoError) {
			QJsonParseError err_t{};
			QJsonDocument docment = QJsonDocument::fromJson(NetVideoComment->readAll(), &err_t);
			if (err_t.error == QJsonParseError::NoError) {
				QJsonObject rot = docment.object();
				QJsonValue val = rot.value("hotComments");
				if (val.isArray()) {
					QJsonArray ary = val.toArray();
					parsepublic(VideoComment, ary);
				}
			}
		}
		});
}

void Remark::parseMusicComment(QJsonObject& root, QString str) {
	QJsonValue val = root.value(str);
	if (val.isObject()) {
		QJsonObject rot = val.toObject();
	}
}

void Remark::deletewidget() {
	foreach(auto && rhs, listwidget) { delete rhs; }

	foreach(auto && rhs, line) { delete rhs; }

	line.clear();
	listwidget.clear();
}

void Remark::setMsg(QPixmap& _imge, QString _title, QString _Albume,
	QString _Artist) {
	ui->btn_image->setIconSize(_imge.size());
	ui->btn_image->setIcon(QIcon(_imge));
	ui->btn_album->setText(_Albume);
	ui->btn_artistname->setText(_Artist);
}

void Remark::hideWidget()
{
	ui->btn_image->setVisible(false);
	ui->horizontalLayout_2->deleteLater();
	//  ui->horizontalLayout_2->changeSize(0, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
}

QList<HotComments>& Remark::getcomments() {
    //	if (hotComments.isEmpty()) {
    //		return QList<HotComments>();
    //	}
    return hotComments;
}

void Remark::on_finshedMusicComment() {
	if (Netcomment->error() == QNetworkReply::NoError) {
		total = 0;
		QJsonParseError err_t{};
		QJsonDocument docment =
			QJsonDocument::fromJson(Netcomment->readAll(), &err_t);
		if (err_t.error == QJsonParseError::NoError) {
			QJsonObject root = docment.object();
			//评论总数
			total = root.value("total").toInt();
			if (root.value("comments").isArray()) {
				QJsonArray ary = root.value("comments").toArray();
				parsepublic(MusicComment, ary);
			}
		}
	}
}

void Remark::on_finshedSongMenuComment() {
	if (Netcomment->error() == QNetworkReply::NoError) {
		total = 0;
		QJsonParseError errt_t{};
		QJsonDocument docment =
			QJsonDocument::fromJson(Netcomment->readAll(), &errt_t);
		if (errt_t.error == QJsonParseError::NoError) {
			QJsonObject root = docment.object();
			//评论总数
			total = root.value("total").toInt();
			// ParseSongMenuComment(root, "comments");
			if (root.value("comments").isArray()) {
				QJsonArray ary = root.value("comments").toArray();
				parsepublic(hotComments, ary);
			}
		}
	}
}

//解析歌曲评论json
void Remark::ParseSongMenuComment(QJsonObject& root, QString str) {
	QJsonValue val = root.value(str);
	qDebug() << val;
	if (val.isArray()) {
		QJsonArray ary = val.toArray();
		HotComments hot{};
		deletewidget();
		hotComments.clear();
		int index = 0;
		foreach(const QJsonValue & rhs, ary) {
			if (rhs.isObject()) {
				QJsonObject rot = rhs.toObject();
				if (rot.value("user").isObject()) {
					QJsonObject userobj = rot.value("user").toObject();
					hot.userid = userobj.value("userId").toInt();
					hot.avatarUrl = userobj.value("avatarUrl").toString();

					hot.nickname = userobj.value("nickname").toString();
				}
				hot.content = rot.value("content").toString();
				hot.timeStr = rot.value("timeStr").toString();
				hot.likedCount = rot.value("likedCount").toInt();
				hot.liked = rot.value("liked").toBool();
				long long dur = rot.value("time").toVariant().toULongLong();
				QDateTime data = QDateTime::fromMSecsSinceEpoch(dur);
				QString time = data.toString("yyyy-MM-dd hh:mm").toUtf8();
				hot.time = time;

				hotComments.push_back(hot);

				//拿到评论者的头像
				QEventLoop loop;
				QNetworkReply* reply = manager->get(QNetworkRequest(hot.avatarUrl));
				connect(reply, &QNetworkReply::finished, this, [&]() {
					if (reply->error() == QNetworkReply::NoError) {
						hot.image.loadFromData(reply->readAll());
						addWidget(hot, index);
						++index;
					}
					});
				connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
				loop.exec();
			}
		}
	}
}

void Remark::parsepublic(QList<HotComments> lhs, QJsonArray& Ary) {
	HotComments hot{};
	deletewidget();
	lhs.clear();
	int index = 0;
	foreach(const QJsonValue & rhs, Ary) {
		if (rhs.isObject()) {
			QJsonObject rot = rhs.toObject();
			if (rot.value("user").isObject()) {
				QJsonObject userobj = rot.value("user").toObject();
				hot.userid = userobj.value("userId").toInt();
				hot.avatarUrl = userobj.value("avatarUrl").toString();
				hot.nickname = userobj.value("nickname").toString();
			}
			hot.content = rot.value("content").toString();
			hot.timeStr = rot.value("timeStr").toString();
			hot.likedCount = rot.value("likedCount").toInt();
			hot.liked = rot.value("liked").toBool();
			long long dur = rot.value("time").toVariant().toULongLong();
			QDateTime data = QDateTime::fromMSecsSinceEpoch(dur);
			QString time = data.toString("yyyy-MM-dd hh:mm").toUtf8();
			hot.time = time;

			lhs.push_back(hot);

			//拿到评论者的头像
			QEventLoop loop;
			QNetworkReply* reply = manager->get(QNetworkRequest(hot.avatarUrl));
			connect(reply, &QNetworkReply::finished, this, [&]() {
				if (reply->error() == QNetworkReply::NoError) {
					hot.image.loadFromData(reply->readAll());
					addWidget(hot, index);
					++index;
				}
				});
			connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
			loop.exec();
		}
	}
}
