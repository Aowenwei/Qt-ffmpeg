#include "playervideo.h"
#include "Remark/remark.h"
#include "ui_playervideo.h"
#include <QNetworkRequest>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

//http://localhost:3000/

PlayerVideo::PlayerVideo(QWidget* parent) :QWidget(parent),
ui(new Ui::PlayerVideo)
{
	ui->setupUi(this);
	//ui->lab_pix->setWordWrap(true);
	player = new QMediaPlayer(this);
	//视频播放控件
	videowidget = new QVideoWidget(ui->video);
	addBtn_Title();
	remark = new Remark(ui->wid_comment);
	remark->hideWidget();

	manger = new QNetworkAccessManager(this);
}

PlayerVideo::~PlayerVideo()
{
	delete ui;
}



void PlayerVideo::getVideoComment()
{
}

void PlayerVideo::getMovieUrl(const long long& id)
{
	remark->getMvComment(id, 1, 10);
	MVID = QString::number(id);
	QString URL = QString("http://localhost:3000/mv/url?id=%1").arg(id);
	VideoURL = manger->get(QNetworkRequest(URL));
	connect(VideoURL, &QNetworkReply::finished, this, [&]() {
		if (VideoURL->error() == QNetworkReply::NoError) {
			QJsonParseError err_t{};
			QJsonDocument docm = QJsonDocument::fromJson(VideoURL->readAll(), &err_t);
			if (err_t.error == QJsonParseError::NoError) {
				QJsonObject rot = docm.object();
				QJsonValue val = rot.value("data");
				if (val.isObject()) {
					QJsonObject obj = val.toObject();
					QString	mvURL = obj.value("url").toString();
					qDebug() << "MVURL = " << mvURL;
					player->setMedia(QUrl(mvURL));
					//设置视频输出控件
					player->setVideoOutput(videowidget);

					videowidget->resize(ui->video->size());
					player->play();

					//获取推荐的相关视频
					getRelatedVideo(MVID);
				}
			}
		}
		});
}

void PlayerVideo::getMovieUrl(const QString& _id)
{
	//获取评论
	remark->getVideoComment(_id,1,20);
	QString URL = QString("http://localhost:3000/video/url?id=%1").arg(_id);
	VideoURL = manger->get(QNetworkRequest(URL));
	connect(VideoURL, &QNetworkReply::finished, this, [&]() {
		if (VideoURL->error() == QNetworkReply::NoError) {
			QJsonParseError err_t{};
			QJsonDocument docm = QJsonDocument::fromJson(VideoURL->readAll(), &err_t);
			if (err_t.error == QJsonParseError::NoError) {
				QJsonObject rot = docm.object();
				QJsonValue value = rot.value("urls");
				if (value.isArray()) {
					QJsonArray ary = value.toArray();
					MVID = ary.at(0).toObject().value("id").toString();

					player->setMedia(QUrl(ary.at(0).toObject().value("url").toString()));
					//设置视频输出控件
					player->setVideoOutput(videowidget);

					videowidget->resize(ui->video->size());
					player->play();
					//获取推荐的相关视频
					getRelatedVideo(MVID);
				}
			}
		}
		});
}

//设置视频详情
void PlayerVideo::setMessage(const long long artid, const QString& cover, const QString& art,
	const QString& title)
{
	ui->btn_artName->setText(art);
	qDebug() << "Cover = " << cover;
	artpix = manger->get(QNetworkRequest(cover));
	connect(artpix, &QNetworkReply::finished, this, [&] {
		if (artpix->error() == QNetworkReply::NoError) {
			QPixmap pix{};
			pix.loadFromData(artpix->readAll());

			ui->lab_pix->setPixmap(pix);
		}
		});
}


void PlayerVideo::getRelatedVideo(const QString& str)
{
	QString URL = QString("http://localhost:3000/related/allvideo?id=%1").arg(str);
	//QString URL = QString("http://localhost:3000/simi/mv?mvid=%1").arg(str);
	qDebug() << "ID = " << URL;
	RelatedVideo = manger->get(QNetworkRequest(URL));
	connect(RelatedVideo, &QNetworkReply::finished, this, &PlayerVideo::on_finshedRelatedVideo);
}

void PlayerVideo::on_finshedRelatedVideo()
{
	if (RelatedVideo->error() == QNetworkReply::NoError) {
		QJsonParseError err_t{};
		QJsonDocument docm = QJsonDocument::fromJson(RelatedVideo->readAll(), &err_t);
		if (err_t.error == QJsonParseError::NoError) {
			QJsonObject rot = docm.object();
			//获取相似MV
			//QJsonValue val = rot.value("mvs");
			//if (val.isArray()) {
			//	QJsonArray ary = val.toArray();
			//	Related_Video reVideo{};
			//	ListRealte.clear();
			//	int i = 0;
			//	QSize picSize = btn_RelatedPic.at(0)->size();
			//	foreach(auto && rhs, ary) {
			//		if (rhs.isObject()) {
			//			QJsonObject obj = rhs.toObject();
			//			reVideo.MvId = obj.value("id").toVariant().toLongLong();
			//			reVideo.title = obj.value("name").toString();
			//			reVideo.playCount = obj.value("playCount").toVariant().toLongLong();
			//			reVideo.userName = obj.value("artistName").toString();
			//			reVideo.userId = obj.value("artistId").toVariant().toLongLong();
			// 
			//			//	获取视频封面
			//			QString url = obj.value("cover").toString();
			//			QEventLoop loop{};
			//			QNetworkReply* cover = manger->get(QNetworkRequest(url));
			//			connect(cover, &QNetworkReply::finished, this, [&]() {
			//				if (cover->error() == QNetworkReply::NoError) {
			//					QPixmap pix{};
			//					pix.loadFromData(cover->readAll());
			//					//设置相关推荐图片
			//					btn_RelatedPic.at(i)->setIconSize(picSize);
			//					btn_RelatedPic.at(i)->setIcon(pix);
			//					//设置相关推荐TItle
			//					QString str = reVideo.title.insert(12, "\n");
			//					btn_Title.at(i)->setText(str);
			//					btn_Title.at(i)->adjustSize();
			//					//设置相关推荐的UserName
			//					btn_UserName.at(i)->setText("by " + reVideo.userName);
			//					btn_UserName.at(i)->adjustSize();
			//					++i;
			//				}
			//				});
			//			connect(cover, &QNetworkReply::finished, &loop, &QEventLoop::quit);
			//			loop.exec();
			//			ListRealte.push_back(reVideo);
			//		}
			//	}
			//}

			//相关视频推荐
			QJsonValue val = rot.value("data");
			if (val.isArray()) {
				QJsonArray ary = val.toArray();
				Related_Video reVideo{};
				ListRealte.clear();
				int i = 0;
				QSize picSize = btn_RelatedPic.at(0)->size();
				foreach(const QJsonValue & rhs, ary) {
					if (rhs.isObject()) {
						QJsonObject datarot = rhs.toObject();
						reVideo.title = datarot.value("title").toString();
						reVideo.MvId = datarot.value("vid").toString();
						reVideo.cover = datarot.value("coverUrl").toString();
						//获取用户信息
						if (datarot.value("creator").isArray()) {
							auto&& lhs = datarot.value("creator").toArray();
							if (lhs.at(0).isObject()) {
								QJsonObject obj = lhs.at(0).toObject();
								reVideo.userId = obj.value("userId").toVariant().toLongLong();
								reVideo.userName = obj.value("userName").toString();
							}
						}

						//获取视频封面
						QString url = datarot.value("coverUrl").toString();
						QEventLoop loop{};
						QNetworkReply* cover = manger->get(QNetworkRequest(url));
						connect(cover, &QNetworkReply::finished, this, [&]() {
							if (cover->error() == QNetworkReply::NoError) {
								QPixmap pix{};
								pix.loadFromData(cover->readAll());
								//设置相关推荐图片
								btn_RelatedPic.at(i)->setIconSize(picSize);
								btn_RelatedPic.at(i)->setIcon(pix);
								//设置相关推荐TItle
								QString str = reVideo.title.insert(12, "\n");
								btn_Title.at(i)->setText(str);
								btn_Title.at(i)->adjustSize();
								//设置相关推荐的UserName
								btn_UserName.at(i)->setText("by " + reVideo.userName);
								btn_UserName.at(i)->adjustSize();
								++i;
							}
							});
						connect(cover, &QNetworkReply::finished, &loop, &QEventLoop::quit);
						loop.exec();
						ListRealte.push_back(reVideo);
					}
				}
			}
		}
	}
}

void PlayerVideo::on_finshedVideoComment() {}

void PlayerVideo::on_btn_back_clicked()
{
	emit back();
}

void PlayerVideo::addBtn_Title()
{
	auto&& rhs = this->findChildren<QPushButton*>();
	int count = 0;
	for (int i = 0; i != rhs.length(); ++i) {
		{
			QString name = rhs.at(i)->objectName();
			rhs.at(i)->setStyleSheet("text-align : left;");
			if (rhs.at(i)->objectName().compare(QString("Related_pic_%1").arg(count)) == 0) {
				btn_RelatedPic.push_back(rhs.at(i));
				connect(rhs.at(i), &QPushButton::clicked, this, &PlayerVideo::on_PicClicked);
			}

			if (rhs.at(i)->objectName().compare(QString("btn_title_%1").arg(count)) == 0) {
				btn_Title.push_back(rhs.at(i));
				connect(rhs.at(i), &QPushButton::clicked, this, &PlayerVideo::on_TitleClicked);
			}

			if (rhs.at(i)->objectName().compare(QString("btn_art_%1").arg(count)) == 0) {
				btn_UserName.push_back(rhs.at(i));
				connect(rhs.at(i), &QPushButton::clicked, this, &PlayerVideo::on_UserNameClicked);
				//最后一次匹配之后在自增
				++count;
			}
		}
	}
}


void PlayerVideo::CommentWidget(long long id)
{
//	ui->wid_comment->getMusicComment(id, 1, 20);
}


//按钮槽函数
void PlayerVideo::on_PicClicked() {
	QString objname = sender()->objectName();
	for (int i = 0; i != 5; ++i) {
		if (btn_RelatedPic.at(i)->objectName().compare(objname) == 0) {
			getMovieUrl(ListRealte.at(i).MvId);
			setMessage(ListRealte.at(i).userId, ListRealte.at(i).cover, 
				ListRealte.at(i).userName, ListRealte.at(i).title);
		}
	}

}

void PlayerVideo::on_TitleClicked() {
	QString objname = sender()->objectName();
	for (int i = 0; i != 5; ++i) {
		if (btn_Title.at(i)->objectName().compare(objname) == 0) {
			getMovieUrl(ListRealte.at(i).MvId);
			setMessage(ListRealte.at(i).userId, ListRealte.at(i).cover,
				ListRealte.at(i).userName, ListRealte.at(i).title);
		}
	}
}
void PlayerVideo::on_UserNameClicked() {
	QString objname = sender()->objectName();
	for (int i = 0; i != 5; ++i) {
		if (btn_UserName.at(i)->objectName().compare(objname) == 0) {
			getMovieUrl(ListRealte.at(i).MvId);
			setMessage(ListRealte.at(i).userId, ListRealte.at(i).cover,
				ListRealte.at(i).userName, ListRealte.at(i).title);
		}
	}
}