#if _MSC_VER >= 1600 // MSVC2015>1899,对于MSVC2010以上版本都可以使用
#pragma execution_character_set("utf-8")
#endif

#include "movie.h"
#include <QMouseEvent>
#include "playervideo.h"
#include "ui_movie.h"
#include <QNetworkAccessManager>
#include <QDebug>
#include <QNetworkRequest>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QGridLayout>

Movie::Movie(QWidget* parent) : QWidget(parent), ui(new Ui::Movie)
{
	ui->setupUi(this);
	manger = new QNetworkAccessManager(this);
	request = conf.setCookies();
	qDebug()<< "\n\nrequest = " << request->CookieHeader;
	playervideo = new PlayerVideo(this);
	playervideo->setVisible(false);
	ui->gridLayout_9->addWidget(playervideo);
	ui->stackedWidget->setCurrentWidget(playervideo);

	//获取视频标签
	getVideoTag();

	connect(ui->tabWidget, &QTabWidget::tabBarClicked, this, &Movie::on_tabclicked);

	connect(ui->stackedWidget, &QStackedWidget::currentChanged, this, [&](int index) {
		if (index == 0)
			emit ShowTheSidebar();


		if (index == 1)
			emit HideTheSidebar();
		});

	connect(playervideo, &PlayerVideo::back, this, [&] {ui->stackedWidget->setCurrentIndex(0); });
}
Movie::~Movie()
{
	delete ui;
}

void Movie::CreatorVideoWidget(const int index, const QPixmap& pix)
{
	static int row{}, column{};
	VideoWidget vwidget{};
	QVBoxLayout* vbox = new QVBoxLayout();
	QPushButton* pic = new QPushButton(ui->widget);
	pic->setObjectName(QString("pic_%1").arg(index));
	pic->setMinimumSize(260, 135);
	pic->setMaximumSize(260, 135);
	pic->setIconSize(QSize(260, 135));
	pic->setIcon(pix);
	pic->setStyleSheet("background-color:red; border-image: url(D:/Music/Userpix/user.png); "
		"QPushButton#pic:hover{color:#cfcfcf};");

	QPushButton* title = new QPushButton(ui->widget);
	title->setObjectName(QString("title_%1").arg(index));
	title->setMinimumSize(240, 20);
	title->setMaximumSize(240, 20);
	title->setText(vglist.at(index).title);
	title->setStyleSheet("background-color:blue;QPushButton#title:hover{color:#cfcfcf}");

	QPushButton* artname = new QPushButton(ui->widget);
	artname->setObjectName(QString("artname_%1").arg(index));
	artname->setMinimumSize(240, 20);
	artname->setMaximumSize(240, 20);
	title->setText(vglist.at(index).nickname);
	artname->setStyleSheet("background-color:yellow;QPushButton#artname:hover{color:#cfcfcf}");

	vbox->addWidget(pic);
	vbox->addWidget(title);
	vbox->addWidget(artname);

	if (0 == index % 3) {
		++row;
		column = 0;
	}
	++column;
	ui->VideoGrid->addLayout(vbox, row, column, 1, 1);

	vwidget.pic = pic;
	vwidget.title = title;
	vwidget.artName = artname;
	videwidget.push_back(vwidget);
	connect(pic, &QPushButton::clicked, this, [&] {});
	connect(title, &QPushButton::clicked, this, [&] {});
	connect(artname, &QPushButton::clicked, this, [&] {});
}

PlayerVideo* Movie::getPlayerVideo() { return playervideo; }

void Movie::mousePressEvent(QMouseEvent* event) {
	//后退被按下
	if (event->button() == Qt::XButton1) {
		qDebug() << "*********后退被按下\n";
	}

	//前进被按下
	if (event->button() == Qt::XButton2) {
		qDebug() << "*********前进被按下\n";
	}
}

void Movie::RequestMvMessage()
{
	addNewMvList();
	getNewMV(6);

	addWyMv();
	getWyMv(6);
}

void Movie::getVideoTag()
{
	//获取cookie
	QString Url = QString("http://localhost:3000/video/category/list");
	request->setUrl(Url);
	taglist = manger->get(*request);
	connect(taglist, &QNetworkReply::finished, this, [&]() {
		if (taglist->error() == QNetworkReply::NoError) {
			QJsonParseError err_t{};
			QJsonDocument docment = QJsonDocument::fromJson(taglist->readAll(), &err_t);
			if (err_t.error == QJsonParseError::NoError) {
				QJsonObject rot = docment.object();
				qDebug() << "rot = " << rot;
				QJsonValue val = rot.value("data");

				if (val.isArray()) {
					QJsonArray ary = val.toArray();
					tagId.scene = ary.at(0).toObject().value("id").toInt();
					tagId.cover = ary.at(1).toObject().value("id").toInt();
					tagId.dance = ary.at(2).toObject().value("id").toInt();
					tagId.BGM = ary.at(3).toObject().value("id").toInt();
					tagId.MV = ary.at(4).toObject().value("id").toInt();
					tagId.live = ary.at(5).toObject().value("id").toInt();
					tagId.game = ary.at(6).toObject().value("id").toInt();
					tagId.ACG = ary.at(7).toObject().value("id").toInt();
					tagId.fanzhi = ary.at(8).toObject().value("id").toInt();

					ui->btn_scene->setText(ary.at(0).toObject().value("name").toString());
					ui->btn_cover->setText(ary.at(1).toObject().value("name").toString());
					ui->btn_dance->setText(ary.at(2).toObject().value("name").toString());
					ui->btn_bgm->setText(ary.at(3).toObject().value("name").toString());
					ui->btn_MV->setText(ary.at(4).toObject().value("name").toString());
					ui->btn_live->setText(ary.at(5).toObject().value("name").toString());
					ui->btn_game->setText(ary.at(6).toObject().value("name").toString());
					ui->btn_acg->setText(ary.at(7).toObject().value("name").toString());
					ui->btn_clip->setText(ary.at(8).toObject().value("name").toString());

				}
			}
		}
		});
}

void Movie::videoGroupId(const int id)
{
	QString url = QString("http://localhost:3000/video/group?id=%1").arg(id);
	request->setUrl(url);
	VideoGroud = manger->get(*request);
	connect(VideoGroud, &QNetworkReply::finished, this, &Movie::on_finshedVideoGroud);
}
void Movie::on_finshedVideoGroud()
{
	qDebug() << "Request Url = " << VideoGroud->url();

	if (VideoGroud->error() == QNetworkReply::NoError) {
		QJsonParseError err_t{};
		QJsonDocument docment = QJsonDocument::fromJson(VideoGroud->readAll(), &err_t);
		if (err_t.error == QJsonParseError::NoError) {
			QJsonObject rot = docment.object();
			qDebug() << rot;
			QJsonValue val = rot.value("datas");

			ParseVideoGroud(val);
		}
	}
}

void Movie::ParseVideoGroud(QJsonValue& val) {
	if (val.isArray()) {
		QJsonArray ary = val.toArray();
		_VideoGroud vg{};
		foreach(const auto && rhs, ary) {
			if (rhs.isObject()) {
				QJsonObject obj = rhs.toObject();
				QJsonValue Creator = obj.value("creator");
				if (Creator.isObject()) {
					QJsonObject creatObj = Creator.toObject();
					vg.userId = creatObj.value("userId").toVariant().toLongLong();
					vg.nickname = creatObj.value("nickname").toString();
				}

				QJsonValue Valdata = obj.value("data");
				if (Valdata.isObject()) {
					auto dataobj = Valdata.toObject();
					vg.coverUrl = dataobj.value("coverUrl").toString();
					vg.title = dataobj.value("title").toString();
				}
				vg.vid = obj.value("vid").toString();
				auto x = obj.value("durationms").toVariant().toLongLong();
				auto playtime = obj.value("playTime").toVariant().toLongLong();
				//vg.durationms = x;

				vglist.push_back(vg);
			}
		}
		thread = std::thread{ [&]() {
		QEventLoop loop{};
		foreach(auto && rhs,vglist) {
		getpic = manger->get(QNetworkRequest(rhs.coverUrl));
		connect(getpic, &QNetworkReply::finished, this, [&] {
			if (getpic->error() == QNetworkReply::NoError) {
				QPixmap pix{};
				pix.loadFromData(getpic->readAll());
				CreatorVideoWidget(videwidgetindex,pix);
				++videwidgetindex;
		}
		});
		connect(getpic, &QNetworkReply::finished, &loop, &QEventLoop::quit);
		loop.exec();
		}
		}
		};
		//不必等待线程结束
		thread.detach();
	}
}


void Movie::getNewMV(const int limit)
{
	QString url = QString("http://localhost:3000/mv/first?limit=%1").arg(limit);
	NewMv = manger->get(QNetworkRequest(url));
	connect(NewMv, &QNetworkReply::finished, this, &Movie::on_finshedNewMv);
}

void Movie::getWyMv(const int limit)
{
	QString url = QString("http://localhost:3000/mv/exclusive/rcmd?limit=%1").arg(limit);
	WyMv = manger->get(QNetworkRequest(url));
	connect(WyMv, &QNetworkReply::finished, this, [&]() {
		if (NewMv->error() == QNetworkReply::NoError) {
			QJsonParseError err_t{};
			QJsonDocument document = QJsonDocument::fromJson(WyMv->readAll(), &err_t);
			if (err_t.error == QJsonParseError::NoError) {
				QJsonObject obj = document.object();
				QJsonValue val = obj.value("data");
				if (val.isArray()) {
					_MV mv{};
					wy.clear();
					QJsonArray ary = val.toArray();
					foreach(const auto && rhs, ary) {
						if (rhs.isObject()) {
							QJsonObject obj = rhs.toObject();
							mv.id = obj.value("id").toVariant().toLongLong();
							QString cover = obj.value("cover").toString();
							mv.cover = cover;
							mv.name = obj.value("name").toString();
							mv.playCount = obj.value("playCount").toVariant().toLongLong();
							mv.artistName = obj.value("artistName").toString();
							mv.artistId = obj.value("artistId").toVariant().toLongLong();

							QEventLoop loop{};
							pic = manger->get(QNetworkRequest(cover));
							connect(pic, &QNetworkReply::finished, this, [&]() {
								QPixmap map{};
								map.loadFromData(pic->readAll());
								wy_mv_pic.at(wy_index)->setIconSize(QSize(300, 160));
								wy_mv_pic.at(wy_index)->setIcon(QIcon(map));

								wy_title.at(wy_index)->setText(mv.name);
								wy_title.at(wy_index)->setStyleSheet("text-align:left;");
								wy_title.at(wy_index)->adjustSize();

								++wy_index;
								});
							connect(pic, &QNetworkReply::finished, &loop, &QEventLoop::quit);
							loop.exec();
							wy.push_back(mv);
						}
					}
				}
			}
		}
		});
}

void Movie::clicked()
{
	//第一次进入
	static bool click = true;
	//默认现场
	videoGroupId(58100);

	/*for (int x = 0; x != 57; x++)
		CreatorVideoWidget(x);*/
}

void Movie::on_tabclicked(int index) {
	/*if (index == 1) {

	}*/
}

void Movie::on_finshedNewMv() {
	if (NewMv->error() == QNetworkReply::NoError) {
		QJsonParseError err_t{};
		QJsonDocument document = QJsonDocument::fromJson(NewMv->readAll(), &err_t);
		if (err_t.error == QJsonParseError::NoError) {
			QJsonObject obj = document.object();
			parseNewMV(obj, "data");
		}
	}
}

void Movie::on_MVClicked()
{
	if (mv_btn.isEmpty())return;
	const QObject* btn = sender();
	QString name = btn->objectName();
	int i = 0;
	for (i = 0; i != mv_btn.length(); ++i) {
		if (mv_btn.at(i)->objectName().compare(name) == 0) {
			break;
		}
	}
	qDebug() << "index  =" << i << " data = " << mvmsg.at(i).artistName;
	playervideo->setMessage(mvmsg.at(i).artistId, mvmsg.at(i).cover, mvmsg.at(i).artistName, mvmsg.at(i).name);
	//打开播放UI
	ui->stackedWidget->setCurrentIndex(1);
	playervideo->setVisible(true);
	//获取MV的ID
	playervideo->getMovieUrl(mvmsg.at(i).id);

}

void Movie::on_WyMVClicked()
{
	QString signaeName = sender()->objectName();
	for (int i = 0; i != wy_mv_pic.length(); ++i) {
		if (wy_mv_pic.at(i)->objectName().compare(signaeName) == 0) {
			qDebug() << "index = " << i << " ObjectName = " << wy_mv_pic.at(i)->objectName();
			playervideo->setMessage(wy.at(i).artistId, wy.at(i).cover, wy.at(i).artistName, wy.at(i).name);
			ui->stackedWidget->setCurrentIndex(1);
			playervideo->setVisible(true);
			//获取MV的ID
			playervideo->getMovieUrl(wy.at(i).id);
		}
	}
}

void Movie::parseNewMV(const QJsonObject& rot, const QString str)
{
	QJsonValue val = rot.value(str);
	if (val.isArray()) {
		_MV mv{};
		NewMVindex = 0;
		mvmsg.clear();
		QJsonArray ary = val.toArray();
		foreach(const auto && rhs, ary) {
			if (rhs.isObject()) {
				QJsonObject obj = rhs.toObject();
				mv.id = obj.value("id").toVariant().toLongLong();
				QString cover = obj.value("cover").toString();

				mv.name = obj.value("name").toString();
				mv.playCount = obj.value("playCount").toVariant().toLongLong();
				mv.artistName = obj.value("artistName").toString();
				mv.artistId = obj.value("artistId").toVariant().toLongLong();
				mvmsg.push_back(mv);

				QEventLoop loop;
				pic = manger->get(QNetworkRequest(cover));
				connect(pic, &QNetworkReply::finished, this, &Movie::on_getpic);
				connect(pic, &QNetworkReply::finished, &loop, &QEventLoop::quit);
				loop.exec();
			}
		}
	}
}


void Movie::on_getpic() {
	if (pic->error() == QNetworkReply::NoError) {
		QPixmap pix{};
		pix.loadFromData(pic->readAll());
		//添加图片
		addNewMV(pix, NewMVindex);

		++NewMVindex;
	}
}

//最新 MV UI 贴图
void Movie::addNewMV(const QPixmap& pix, int index)
{
	mv_btn.at(index)->setIconSize(mv_btn.at(index)->size());
	mv_btn.at(index)->setIcon(pix);

	mv_artName.at(index)->setText(mvmsg.at(index).artistName);

	mv_songName.at(index)->setText(mvmsg.at(index).name);
}

void Movie::addNewMvList()
{
	if (ui->widget_2 != nullptr) {
		for (int i = 0; i != 6; ++i) {
			QPushButton* btn = ui->widget_2->findChild<QPushButton*>(QString("btn_pic_%1").arg(i));
			mv_btn.push_back(btn);

			QPushButton* art = ui->widget_2->findChild<QPushButton*>(QString("btn_artName_%1").arg(i));
			art->setStyleSheet("text-align:left;");
			mv_artName.push_back(art);

			QPushButton* songName = ui->widget_2->findChild<QPushButton*>(QString("btn_songName_%1").arg(i));
			songName->setStyleSheet("text-align:left;");
			mv_songName.push_back(songName);

			connect(btn, &QPushButton::clicked, this, &Movie::on_MVClicked);
		}
	}
}

void Movie::addWyMv()
{
	if (ui->widget_3 != nullptr) {
		for (int i = 0; i != 6; ++i) {
			QPushButton* pic = ui->widget_3->findChild<QPushButton*>(QString("wy_pic_%1").arg(i));
			wy_mv_pic.push_back(pic);
			connect(pic, &QPushButton::clicked, this, &Movie::on_WyMVClicked);
			QPushButton* title = ui->widget_3->findChild<QPushButton*>(QString("wy_title_%1").arg(i));
			title->setStyleSheet("text-align:left;");
			wy_title.push_back(title);
			connect(title, &QPushButton::clicked, this, &Movie::on_WyMVClicked);
		}
	}
}


