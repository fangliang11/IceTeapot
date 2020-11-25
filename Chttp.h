#ifndef CHTTP_H
#define CHTTP_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QHttpPart>
#include <QHttpMultiPart>
#include <QEventLoop>

#include <QUrlQuery>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include <QMutex>
#include <QWaitCondition>
#include <QFile>
#include <QTimer>


class CHttp : public QObject
{
    Q_OBJECT
public:
    CHttp();
    ~CHttp();

    QString m_mainURL;
    QJsonObject m_jsonCommon;
    QJsonObject m_jsonSpecialTrain;
    QJsonObject m_jsonRivalryTrain;

    bool response_success = false;

    void Initial(QString url);
    int GetCommonMsg();
    int GetSpecialTrainMsg();
    int GettRivalryTrainMsg();
    int PostStatisticsMsg(QString subURL, QJsonObject obj);
    void PostFile();
    void PostMultiFiles();

    int ConnectionTest(QString strurl);
    void SaveJsonFile();

private:

    void GenerateCommonUrl(QUrl &url);

signals:
    void getResponse();

public slots:
    void connectResponse();
    int receiveReply(QNetworkReply *reply);

private:
    QNetworkAccessManager *manager;
    QTimer timer;

};

#endif // CHTTP_H
