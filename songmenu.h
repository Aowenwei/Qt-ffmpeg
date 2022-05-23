#ifndef SONGMENU_H
#define SONGMENU_H

#include <QWidget>
#include "config.h"
#include <QMap>
#include "tag.h"

class Base;
class Remark;
class UserSongMuen;
class QNetworkReply;
class QNetworkAccessManager;

namespace Ui {
	class SongMenu;
}

//用户歌单
class UserSongMuen {
public:
	//歌单用户ID
	int userID{};
	//歌单ID
	long long id{};
	//歌曲数量
	int trackCount{};
	//收藏量
	size_t subscribedCount{};
	//播放量
	size_t playCount{};
	//歌单名称
	QString name{};
	//歌单封面
	QString coverImgUrl{};
	QString createTime{};
	QPixmap Userpic{};
	QString nickname{};
	//简介 
	QString description{};
	//标签
	QString tags{};
	//创建者的图片
	QString avatarUrl{};
	//评论总数
	int commentCount{};
};

class SongMenu : public QWidget
{
	Q_OBJECT
public:
	explicit SongMenu(QWidget* parent = nullptr);
	virtual ~SongMenu();
	void loadData();
	//获取我喜欢的歌曲列表ID,方便后续操作
	void RequestSongTable();
	//返回歌单名字
	QStringList getSongMenu();
	void RequestUserSongMenu();
	void SongMenuAt(const int index = 0);
	//拿到歌单之后，直接请求歌单中的歌曲
	void getSongMenuID(const size_t ID = 0, const int limit = 10);
	//创建歌单
	void CreatorSongMuen(const QString& name);
	void DeleteSongMuenu(const int ID);
	void SetInformation(QString& name, QString& crutTime, size_t sub,
		QString& _tag, int song, size_t playNum, QString& des);
	QStringList GetPlayList() { return SongID; }
	Remark* getRemark() { return remark; }
signals:
	//加载歌单列表
	void DataLoading();
	void CreatorSongMenuOk();
	void DeleteOk();
	void SongMenu_playAll(SongMenu*);
	void Nextplay(SongMenu*, const int index, const QString ID);
protected slots:
	void on_btn_playAll_clicked();
	void on_finshedNetSongMenu();
	void on_finsedNetAllSong();
	void on_finshedNetDetail();
	void on_finsedNetPic();
	void on_clickedComment();
	//QTabWidget的Item被点击
	void on_TableWidget_taBarClicked(int index);

signals:
	void C_Comment();

private:
	Ui::SongMenu* ui;
	int userId{};
	size_t songMenuID{};
	int curtableindex{};
	Base* base;
	Remark* remark;
	Config config{};
	UserSongMuen tempMuenInfo{};
	//保存用户歌单
	QList<UserSongMuen> songlistMenu{};

	M_Tag tag{};
	QList<Temptag>* taglsit;
	QStringList SongID{};
	QNetworkAccessManager* manger;
	QNetworkReply* NetSongMenu;
	QNetworkReply* NetAllSong;
	//拿到歌单的描述之类的
	QNetworkReply* NetDetail;
	QNetworkReply* NetPic;
};

#endif // SONGMENU_H
