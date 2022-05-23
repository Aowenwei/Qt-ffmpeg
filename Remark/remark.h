#ifndef REMARK_H
#define REMARK_H

#include <QWidget>
#include <thread>
class QFrame;
class QNetworkReply;
class QNetworkAccessManager;

namespace Ui {
	class Remark;
}

//struct SongMSG {
//	QPixmap imge{};
//	QString title{};
//	QString Albume{};
//	QString Artist{};
//};

struct HotComments
{
	int userid{};
	QString nickname{};
	QString avatarUrl{};
	//评论
	QString content{};
	QString timeStr{};
	int likedCount{};
	bool liked{ false };
	QPixmap image{};
	QString time{};
};

class Remark : public QWidget
{
	Q_OBJECT

public:
	explicit Remark(QWidget* parent = nullptr);
	~Remark();
	void addWidget(HotComments hotmsg,int index);
	//每一个评论widget下都加入一条横线
	void addLine(int i);
	//获取歌曲评论
	void getMusicComment(int id, int offset, int limit);
	//获取专辑评论
	void getSongMenuComment(size_t id, int offset, int limit);
	//获取MV评论
	void getMvComment(long long mvid, int offset, int limit);
	//获取视频评论
	void getVideoComment(const QString& id, int offset, int limit);
	//解析歌单评论
	void ParseSongMenuComment(QJsonObject& root, QString str);
	//解析歌曲评论
	void parseMusicComment(QJsonObject& root, QString str);
	//相同的json
	void parsepublic(QList<HotComments> rhs, QJsonArray& str);
	//删除评论区内容
	void deletewidget();
	//设置评论区顶部的歌曲信息
	void setMsg(QPixmap& imge, QString title, QString Albume, QString Artist);
	//隐藏不必要的内容
	void hideWidget();
	QList<HotComments>& getcomments();
	int getToal() {return total;}
protected slots:
	//获取歌单评论
	void on_finshedSongMenuComment();
	//获取单曲评论
	void on_finshedMusicComment();
private:
	Ui::Remark* ui;
	std::thread thread;
	//评论总数
	int total{};
	//SongMSG songmsg{};
	QList<HotComments>MvComment;
	QList<HotComments>VideoComment;
	QList<HotComments> hotComments{};
	QList<HotComments> MusicComment{};
	QList<QWidget*> listwidget{};
	QList<QFrame*> line{};

	QNetworkAccessManager* manager;
	QNetworkReply* Netcomment{};
	QNetworkReply* NetMvComment{};
	QNetworkReply* NetVideoComment{};
};

#endif // REMARK_H

 