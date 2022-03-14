﻿#if _MSC_VER >= 1600 // MSVC2015>1899,对于MSVC2010以上版本都可以使用
#pragma execution_character_set("utf-8")
#endif
#include "recommend.h"
#include "config.h"
#include "soloalbum.h"
#include "tag.h"
#include "ui_recommend.h"
#include <QDateTime>
#include <QEvent>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QMouseEvent>
#include <QMutex>
#include <QNetworkAccessManager>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTabWidget>
#include <QTimer>
#include <thread>
#include <QFont>
#include <QEventLoop>
#include <QPainter>
#include "songmenu.h"
#include "photowall/photowall.h"
Recommend::Recommend(QWidget* parent) : QWidget(parent), ui(new Ui::Recommend) {
	ui->setupUi(this);
	//照片墙
	photowall = new PhotoWall(this);
	/*添加到布局*/
	ui->verLout_pic->addWidget(photowall);
	config = new Config();
	m_tag = new M_Tag(this);
	soloalbum = new SoloAlbum(this);
	soloalbum->hide();
	recDaily = new RecommendedDaily(this);
	recDaily->hide();
	songmuen = new SongMenu(this);
	songmuen->hide();

	ui->horizontalSpacer->sizeHint().setWidth(0);
	addBtn_rec_();
	addLab_rec_();

	NetNewDisc = new QNetworkAccessManager(this);
	NetNewSong = new QNetworkAccessManager(this);
	NetAlbumPic = new QNetworkAccessManager(this);
	NetRecPlaylist = new QNetworkAccessManager(this);
	NetRecommend = new QNetworkAccessManager(this);
	Netpic = new QNetworkAccessManager(this);
	//每日推荐歌单
	QString RecPlaylistUrl{ "http://cloud-music.pl-fe.cn/personalized?limit=9" };
	request = config->setCookies();
	request->setUrl(RecPlaylistUrl);
	NetRecPlaylist->get(*request);

	connect(NetNewDisc, &QNetworkAccessManager::finished, this,
		&Recommend::on_FinshedNewDisc);

	connect(NetAlbumPic, &QNetworkAccessManager::finished, this,
		&Recommend::on_FinshedGetAlubPic);

	connect(NetRecommend, &QNetworkAccessManager::finished, this,
		&Recommend::on_FinshedNetRecommend);

	connect(NetRecPlaylist, &QNetworkAccessManager::finished, this, &Recommend::on_FinishedNetRecPlaylist);

	connect(Netpic, &QNetworkAccessManager::finished, this, [&](QNetworkReply* reply) {
		if (reply->error() == QNetworkReply::NoError && !queue.isEmpty()) {
			QPixmap pix;
			pix.loadFromData(reply->readAll());
			btn_recAll.at(index)->setIconSize(QSize(195, 195));
			btn_recAll.at(index)->setIcon(QIcon(pix));
			++index;
			//queue.pop_back();
		}
		reply->deleteLater();
		});
}

Recommend::~Recommend() {
	delete ui;
	delete config;
	config = nullptr;
	delete m_tag;
	m_tag = nullptr;
	//关闭会导致崩溃
	// delete request;
	// request = nullptr;
}

//每日推荐单曲 (需要登录)
void Recommend::on_btn_rec_1_clicked() {
	QString loggingstatus = config->GetValue("Pwd/loggingstatus");
	//未登录账号
	if (0 == loggingstatus.compare("0") || loggingstatus.isEmpty()) {
		recDaily->show();
		recDaily->Notlogin(false);
		return;
	}
	else {
		QString Url{ "http://cloud-music.pl-fe.cn/recommend/songs" };
		request = config->setCookies();
		request->setUrl(Url);
		NetRecommend->get(*request);
	}
}

void Recommend::on_btn_rec_2_clicked()
{
	if (!RecList.isEmpty()) {
		songmuen->getSongMenuID(RecList.at(0).id, RecList.at(0).trackCount);
	}
}

void Recommend::on_btn_rec_3_clicked()
{
	if (!RecList.isEmpty()) {
		songmuen->getSongMenuID(RecList.at(1).id, RecList.at(1).trackCount);
	}
}

void Recommend::on_btn_rec_4_clicked()
{
	if (!RecList.isEmpty()) {
		songmuen->getSongMenuID(RecList.at(2).id, RecList.at(2).trackCount);
	}
}

void Recommend::on_btn_rec_5_clicked()
{
	if (!RecList.isEmpty()) {
		songmuen->getSongMenuID(RecList.at(3).id, RecList.at(3).trackCount);
	}
}

void Recommend::on_btn_rec_6_clicked()
{
	if (!RecList.isEmpty()) {
		songmuen->getSongMenuID(RecList.at(4).id, RecList.at(4).trackCount);
	}
}

void Recommend::on_btn_rec_7_clicked()
{
	if (!RecList.isEmpty()) {
		songmuen->getSongMenuID(RecList.at(5).id, RecList.at(5).trackCount);
	}
}

void Recommend::on_btn_rec_8_clicked()
{
	if (!RecList.isEmpty()) {
		songmuen->getSongMenuID(RecList.at(6).id, RecList.at(6).trackCount);
	}
}

void Recommend::on_btn_rec_9_clicked()
{
	if (!RecList.isEmpty()) {
		songmuen->getSongMenuID(RecList.at(7).id, RecList.at(7).trackCount);
	}
}

void Recommend::on_btn_rec_10_clicked()
{
	if (!RecList.isEmpty()) {
		songmuen->getSongMenuID(RecList.at(8).id, RecList.at(8).trackCount);
	}
}

void Recommend::addBtn_rec_()
{
	QWidget* widget = ui->scrollArea->widget();
	if (widget != nullptr) {
		for (quint32 i = 2; i != 11; ++i) {
			QPushButton* btn = widget->findChild<QPushButton*>(QString("btn_rec_" + QString::number(i)));
			btn_recAll.push_back(btn);
		}
		for (int x = 0; x != 9; ++x) {
			QLabel* lab = new QLabel("0万", btn_recAll.at(x));
			lab->setMaximumSize(65, 25);
			lab->setStyleSheet("color:#fbfbfc");
			lab->setFont(QFont("QFont::Bold"));
			lab->setAlignment(Qt::AlignRight);

			//拿到所在位置
			QRect rect = btn_recAll.at(x)->geometry();
			//设置lab的显示位置
			lab->setGeometry(rect.x() + 125, rect.y(), 65, 25);
			lab_title.push_back(lab);
		}
	}

}

void Recommend::addLab_rec_()
{
	QWidget* widget = ui->scrollArea->widget();
	if (widget != nullptr) {
		for (int i = 2; i != 11; ++i)
		{
			QLabel* lab = widget->findChild<QLabel*>("lab_rec_" + QString::number(i));
			//文字太多，自动换行
			lab->setWordWrap(true);
			lab_recAll.push_back(lab);
		}
	}
}


bool Recommend::eventFilter(QObject* obj, QEvent* event) {
	//	if (obj == ui->lab_1) {
	//		//鼠标进入事件
	//		if (event->type() == QEvent::Enter) {
	//		}
	//		else if (event->type() == QEvent::Leave) {
	//		}

	//		// label被点击
	//		if (event->type() == QEvent::MouseButtonPress) {
	//			QMouseEvent* mouse = static_cast<QMouseEvent*>(event);

	//			if (mouse->button() == Qt::LeftButton) {
	//				if (!strcmp(targetlist.at(index - 1).typeTitle.toUtf8().data(),
	//					"新碟首发")) {
	//					fprintf(stdout, "enter Disc\n");
	//					NetNewDisc->get(
	//						QNetworkRequest(QString("http://cloud-music.pl-fe.cn/album?id=%1")
	//							.arg(targetlist.at(index - 1).targetId)));
	//				}
	//				else {
	//					NetNewSong->get(QNetworkRequest(
	//						QString("http://cloud-music.pl-fe.cn/song/detail?ids=%1")
	//						.arg(targetlist.at(index).targetId)));
	//				}
	//			}
	//		}
	//	}
	return QWidget::eventFilter(obj, event);
}

//返回专辑Ui界面
SoloAlbum* Recommend::getAlbumUi() {
	return soloalbum;
}

SongMenu* Recommend::getSoungMenu()
{
	return songmuen;
}

RecommendedDaily* Recommend::getRecDailyUi() { return recDaily; }

//发布的新歌
void Recommend::on_FinshedNewSong(QNetworkReply* reply) {}

void Recommend::on_FinshedGetAlubPic(QNetworkReply* reply) {
	if (reply->error() == QNetworkReply::NoError) {
		QPixmap pixmap;
		pixmap.loadFromData(reply->readAll());
		//设置专辑封面
		soloalbum->setlab_AlubPic(pixmap);
	}
	reply->deleteLater();
}

//每日推荐单曲
void Recommend::on_FinshedNetRecommend(QNetworkReply* reply) {
	if (reply->error() == QNetworkReply::NoError) {
		QJsonParseError error_t{};
		QJsonDocument doucment =
			QJsonDocument::fromJson(reply->readAll(), &error_t);
		if (error_t.error == QJsonParseError::NoError) {
			QJsonObject rot = doucment.object();
			QJsonObject datarot = rot.value("data").toObject();
			if (true == m_tag->ParseDetailsSong(datarot, "dailySongs")) {
				//加载数据
				recDaily->loadData(m_tag->getTag());
				recDaily->show();
			}
		}
	}
	reply->deleteLater();
}


void Recommend::on_FinishedNetRecPlaylist(QNetworkReply* reply)
{
	if (reply->error() == QNetworkReply::NoError) {
		QJsonParseError err_t{};
		QJsonDocument doucment = QJsonDocument::fromJson(reply->readAll(), &err_t);
		int index = 0;
		if (err_t.error == QJsonParseError::NoError) {
			QJsonObject rot = doucment.object();
			QJsonValue value = rot.value("result");
			if (value.isArray()) {
				RecList.clear();
				RecPlaylist recplay;
				QJsonArray resAry = value.toArray();
				foreach(const QJsonValue & rhs, resAry) {
					static int n = 0;
					if (rhs.isObject()) {
						QJsonObject obj = rhs.toObject();
						recplay.id = obj.value("id").toVariant().toLongLong();

						lab_recAll.at(index)->setText(obj.value("name").toString());
						QString picUrl = obj.value("picUrl").toString();

						//用队列来获取图片

						queue.push_back(Netpic->get(QNetworkRequest(picUrl)));
						qDebug() << "push_back";
						recplay.playCount = obj.value("playCount").toVariant().toLongLong();
						recplay.trackCount = obj.value("trackCount").toVariant().toLongLong();
						lab_title.at(index)->setText(QString::number(recplay.playCount));

						if (obj.value("canDislike").toBool() == false) {
							QLabel* lab = new QLabel(this);
							lab->setPixmap(QPixmap(":/images/lab_cloud.png"));
							QRect rect = lab_recAll.at(index)->geometry();
							lab->setGeometry(rect.x(), rect.y(), 20, 20);
						}
						//保存歌单ID和歌曲数量
						RecList.push_back(recplay);
					}
					++index;

					++n;
				}
			}
		}
	}
	index = 0;
	queue.pop_back();
	reply->deleteLater();
}

//专辑
void Recommend::on_FinshedNewDisc(QNetworkReply* reply) {
	if (reply->error() == QNetworkReply::NoError) {
		QByteArray byt = reply->readAll();
		QJsonParseError error_t{};
		Albumtag tag;
		QJsonDocument docm = QJsonDocument::fromJson(byt, &error_t);
		if (error_t.error == QJsonParseError::NoError) {
			QJsonObject root = docm.object();
			QJsonValue songs = root.value("songs");
			if (songs.isArray()) {
				QJsonArray songAry = songs.toArray();
				foreach(const auto & i, songAry) {
					if (i.isObject()) {
						auto songRot = i.toObject();
						//解析歌手和id
						QJsonValue ar = songRot.value("ar");
						if (ar.isArray()) {
							QJsonArray atAry = ar.toArray();
							QJsonObject arrot = atAry.at(0).toObject();
							tag.Artist_id = arrot.value("id").toInt();
							tag.Artist = arrot.value("name").toString();
						}

						//专辑名称
						QJsonValue al = songRot.value("al");
						if (al.isObject()) {
							QJsonObject alobj = al.toObject();
							tag.Album_id = alobj.value("id").toInt();
							tag.Album = alobj.value("name").toString();
						}
						//解析歌曲title
						tag.Title = songRot.value("name").toString();
						tag.Song_id = songRot.value("id").toInt();

						//歌曲时长
						int dt = songRot.value("dt").toInt();
						tag.Duration = QString("%1 : %2")
							.arg(dt / 1000 / 60, 2, 10, QChar('0'))
							.arg(dt % 60, 2, 10, QChar('0'));
					}

					//解析的数据储存起来，方便以后使用
					soloalbum->setAlbumtag(tag);
				}
			}
			//专辑描述
			QJsonValue album = root.value("album");
			if (album.isObject()) {
				QJsonObject albobj = album.toObject();
				//获取专辑描述
				soloalbum->setDescription(albobj.value("description").toString());
				//获取发布时间
				long long pt = albobj.value("publishTime").toVariant().toLongLong();
				QDateTime dd = QDateTime::fromMSecsSinceEpoch(pt);
				QString s = dd.toString("yyyy-MM-dd");
				soloalbum->setPublishTime(dd.toString("yyyy-MM-dd"));
				//设置歌手
				soloalbum->setlab_title(albobj.value("name").toString());
				QString url = albobj.value("picUrl").toString();
				NetAlbumPic->get(QNetworkRequest(url));

				//获取该专辑的歌手
				QJsonValue art = albobj.value("artists");
				if (art.isArray()) {
					QJsonArray artAry = art.toArray();
					foreach(const QJsonValue & rhs, artAry) {
						if (rhs.isObject()) {
							QJsonObject artobj = rhs.toObject();
							QString artist = artobj.value("name").toString();
							soloalbum->setlab_Artist(artist);
						}
					}
				}
			}
			soloalbum->LoadData();
		}
	}
	reply->deleteLater();
}
