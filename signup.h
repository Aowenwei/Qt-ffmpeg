#ifndef SIGNUP_H
#define SIGNUP_H
#include <QDialog>
class QTimer;
class QNetworkAccessManager;
class QNetworkReply;
class QStackedWidget;
namespace Ui {
class Signup;
}

class Signup : public QDialog {
  Q_OBJECT

protected slots:
  void on_btn_back_clicked();
  void on_btn_signin_clicked();

public:
  explicit Signup(QDialog *parent = nullptr);

  ~Signup();

protected slots:
  void on_btn_back_2_clicked();
  void on_btn_regain_clicked();
  void on_btn_countrieslist_clicked();
  void on_finshedSingou();
  void on_finshedCaptcha();

  QStackedWidget *getstackedwidget();
signals:
  void back();
  void singin();

private:
  Ui::Signup *ui;
  QString phone{};
  QString pwd{};
  QString tip_text;
  QTimer *time;
  QNetworkAccessManager* manger;
  QNetworkReply *NetSignou;
  QNetworkReply*Sentcaptcha;
};

#endif // SIGNUP_H
