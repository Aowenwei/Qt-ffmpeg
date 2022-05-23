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
	//����
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
	//ÿһ������widget�¶�����һ������
	void addLine(int i);
	//��ȡ��������
	void getMusicComment(int id, int offset, int limit);
	//��ȡר������
	void getSongMenuComment(size_t id, int offset, int limit);
	//��ȡMV����
	void getMvComment(long long mvid, int offset, int limit);
	//��ȡ��Ƶ����
	void getVideoComment(const QString& id, int offset, int limit);
	//�����赥����
	void ParseSongMenuComment(QJsonObject& root, QString str);
	//������������
	void parseMusicComment(QJsonObject& root, QString str);
	//��ͬ��json
	void parsepublic(QList<HotComments> rhs, QJsonArray& str);
	//ɾ������������
	void deletewidget();
	//���������������ĸ�����Ϣ
	void setMsg(QPixmap& imge, QString title, QString Albume, QString Artist);
	//���ز���Ҫ������
	void hideWidget();
	QList<HotComments>& getcomments();
	int getToal() {return total;}
protected slots:
	//��ȡ�赥����
	void on_finshedSongMenuComment();
	//��ȡ��������
	void on_finshedMusicComment();
private:
	Ui::Remark* ui;
	std::thread thread;
	//��������
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

 