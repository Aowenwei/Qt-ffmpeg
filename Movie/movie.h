#ifndef MOVIE_H
#define MOVIE_H
#include <thread>
#include <QWidget>
#include <QNetworkReply>
#include <QPushButton>
#include "config.h"
class Config;
class QGridLayout;
class PlayerVideo;
class QMouseEvent;
class QNetworkAccessManager;
namespace Ui {
	class Movie;
}


struct _MV {
	long long id{};
	QString name{};
	QString cover{};
	QString artistName{};
	long long playCount{};
	long long artistId{};

};

//视频UI的widget
struct VideoWidget
{
	QPushButton* pic{};
	QPushButton* title{};
	QPushButton* artName{};
};

struct VideoTagId
{
	//现场
	int scene{};
	//翻唱
	int cover{};
	//舞蹈
	int dance{};
	//听BGM
	int BGM{};
	//MV
	int MV{};
	//生活
	int live{};
	//游戏
	int game{};
	//ACG音乐
	int ACG{};
	//最佳饭制
	int fanzhi{};
};

//视频组
struct _VideoGroud
{
	long long userId{};
	QString nickname{};
	QString coverUrl{};
	QString title{};
	QString description{};
	QString vid{};
	QString durationms{};
	QString playTime{};
};

class Movie : public QWidget
{
	Q_OBJECT

public:
	explicit Movie(QWidget* parent = nullptr);
	virtual  ~Movie();

	//获取视频标签
	void getVideoTag();
	//对应标签的视频（默认现场）
	void videoGroupId(const int id = 58100);
	void getNewMV(const int limit);
	void getWyMv(const int limit);
	//解析MV视频的信息
	void parseNewMV(const QJsonObject& rot, const QString);
	void addNewMV(const QPixmap& map, int index);
	//最新MV的控件集合
	void addNewMvList();
	//网易出品MV控件集合
	void addWyMv();
	//解析视频组的视频
	void ParseVideoGroud(QJsonValue& val);

	//视频UI控件
	void CreatorVideoWidget(const int index, const QPixmap& pic);
	PlayerVideo* getPlayerVideo();

signals:
	void HideTheSidebar();
	void ShowTheSidebar();
public slots:
	//请求MV相关信息
	void RequestMvMessage();
	void clicked();
protected slots:

	void mousePressEvent(QMouseEvent* event);
	void on_tabclicked(int index);
	void on_getpic();
	void on_finshedNewMv();
	void on_finshedVideoGroud();

	//最新MV里面的控件被点击
	void on_MVClicked();

	//网易出品MV里面的控件被点击
	void on_WyMVClicked();

private:
	Ui::Movie* ui;
	Config conf{};
	int videwidgetindex{};
	std::thread thread{};

	int NewMVindex{};
	int wy_index{};

	VideoTagId tagId{};

	PlayerVideo* playervideo;
	QList<VideoWidget> videwidget{};

	//最新MV
	QList<_MV> mvmsg{};
	QList<QPushButton*> mv_btn{};
	QList<QPushButton*> mv_artName{};
	QList<QPushButton*>mv_songName{};

	//网易出品
	QList<_MV> wy;
	QList<QPushButton*> wy_mv_pic{};
	QList<QPushButton*> wy_title{};

	//视频组
	QList<_VideoGroud>vglist{};

	QNetworkAccessManager* manger;
	QNetworkRequest* request{};
	//获取视频标签
	QNetworkReply* taglist{};
	QNetworkReply* NewMv{};
	QNetworkReply* WyMv{};
	QNetworkReply* pic{};
	QNetworkReply* VideoGroud{};
	QNetworkReply* getpic{};
};

#endif // MOVIE_H
