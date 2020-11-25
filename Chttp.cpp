#include "Chttp.h"
#include <QDebug>

CHttp::CHttp()
{

}

CHttp::~CHttp()
{

}


void CHttp::Initial(QString url)
{
    manager=new QNetworkAccessManager(this);
    m_mainURL = url;

    //查看支持的协议
    //qDebug()<<manager->supportedSchemes();
    //这里可能是我没安装openssl，所以没有https
    //("ftp", "file", "qrc", "http", "data")

    //QNetworkAccessManager是异步的, 将接受信号和槽函数连接
    //connect(manager, &QNetworkAccessManager::finished, this, &CHttp::receiveReply);

    //定时器拿来验证是否阻塞界面
    //connect(&timer,&QTimer::timeout,this,[this](){ui->editTime->setTime(QTime::currentTime());});
    //timer.start(1000); //定时1s

    //是否收到响应体
    connect(this, SIGNAL(getResponse()), this, SLOT(connectResponse()));

}


void CHttp::GenerateCommonUrl(QUrl &url)
{
    // URL
    QString baseUrl = "http://curling.cmp-dev.onesport.com.cn/live/common/get";
    //QString baseUrl = "http://httpbin.org/get";
    QUrl tempurl(baseUrl);

    // key-value 对
    QUrlQuery query;
    query.addQueryItem("tasks", "Draw");
//    query.addQueryItem("tasks", "Takeout");
//    query.addQueryItem("handles", "clockwise(in-turn)");
//    query.addQueryItem("zones", "centre-zone");
//    query.addQueryItem("points", "4(100%)");
//    tempurl.setQuery(query);

//    // Json 数据
//    QJsonObject json;
//    json.insert("User", "Qter");
//    json.insert("Password", "123456");

    url = tempurl;
}


//获取公共参数
int CHttp::GetCommonMsg()
{
    QString baseUrl = m_mainURL + "/live/common";
    QUrl url(baseUrl);

    //构建请求对象Request(包含：请求行、请求头、空行、请求体)
    QNetworkRequest request;
    request.setUrl(url);

    //设置请求头
    //已知头类型参见文档QNetworkRequest::KnownHeaders
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    //自定义类型
    request.setRawHeader("remarks","common");

    //发送请求
    manager->get(request);

    //开启一个局部的事件循环，等待响应，不会阻塞线程，否则为阻塞方式
    QNetworkReply *reply=manager->get(request);
    QEventLoop eventLoop;
    connect(manager, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    //接收 response
    int nresult = receiveReply(reply);
    eventLoop.exit();

    //发射收到响应体的信号
    emit getResponse();

    return nresult;
}

//获取专项训练数据
int CHttp::GetSpecialTrainMsg()
{
    QString baseUrl = m_mainURL + "/live/match";
    QUrl url(baseUrl);
    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    request.setRawHeader("remarks","SpecialTrainMsg");
    manager->get(request);

    QNetworkReply *reply=manager->get(request);
    QEventLoop eventLoop;
    connect(manager, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    int nresult = receiveReply(reply);
    eventLoop.exit();
    emit getResponse();

    return nresult;
}

//获取对抗训练数据
int CHttp::GettRivalryTrainMsg()
{
    QString baseUrl = m_mainURL + "/live/match";
    QUrl url(baseUrl);
    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    request.setRawHeader("remarks","RivalryTrainMsg");
    manager->get(request);

    QNetworkReply *reply=manager->get(request);
    QEventLoop eventLoop;
    connect(manager, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    int nresult = receiveReply(reply);
    eventLoop.exit();
    emit getResponse();

    return nresult;
}

//上传统计信息
int CHttp::PostStatisticsMsg(QString subURL, QJsonObject obj)
{
    QNetworkRequest request;
    QString baseUrl = m_mainURL + subURL;
    QUrl url(baseUrl);
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");

    // 将json构造为字节流
    QJsonDocument document;
    document.setObject(obj);
    QByteArray arryStatistics = document.toJson(QJsonDocument::Compact);

    manager->post(request, arryStatistics);

    QNetworkReply *reply=manager->get(request);
    QEventLoop eventLoop;
    connect(manager, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    int nresult = receiveReply(reply);
    eventLoop.exit();
    emit getResponse();

    return nresult;
}

//上传单个文件
void CHttp::PostFile()
{

}

//上传多个文件
void CHttp::PostMultiFiles()
{

}

//保存JSON文件到本地
void CHttp::SaveJsonFile()
{


}


void CHttp::connectResponse()
{
    response_success = true;
}

//响应回调函数
int CHttp::receiveReply(QNetworkReply *reply)
{
    int status_code = 0;
    //Response响应结构包括：状态行、响应头、空行、响应体
    if(reply->error()!=QNetworkReply::NoError){
        //处理中的错误信息
        qDebug()<<"reply error:"<<reply->errorString();
        return -1;
    }
    else{
        //请求方式
        //对应QNetworkAccessManager的Operation枚举
        //qDebug()<<"operation:"<<reply->operation();
        //qDebug()<<"url:"<<reply->url();
        //响应代码：200表示成功，3xx表示重定向，4xx表示客户端发送的请求有错误，5xx表示服务器端处理时发生了错误
        status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        qDebug()<<"status code:"<< status_code;
        //重定向
        /*if (status_code == 301 || status_code == 302){
              // Or the target URL if it was a redirect:
              QVariant redirectionTargetUrl =reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
              //qDebug() << "redirection Url is " << redirectionTargetUrl.toString();
              QUrl url(redirectionTargetUrl.toString());
              manager->get(QNetworkRequest(url));
        }*/

        if(status_code == 200){
            qDebug() << "connect success";
            //获取响应信息并解析
            const QByteArray response_data=reply->readAll();
            //解析json
            QJsonParseError json_error;
            QJsonDocument doucment = QJsonDocument::fromJson(response_data, &json_error);
            if (json_error.error == QJsonParseError::NoError) {
                if (doucment.isObject()) {
                    QJsonObject obj = doucment.object();
                    //qDebug()<<obj;
                    if(obj.contains("remarks")){
                        QString remark_value = obj.value("remarks").toString();
                        if(remark_value == "common"){
                            m_jsonCommon = obj;;
                        }
                        else if(remark_value == "match_special"){
                            m_jsonSpecialTrain = obj;
                        }
                        else if(remark_value == "match_verse"){
                            m_jsonRivalryTrain = obj;
                        }
                        else if(remark_value == "update"){
                            qDebug() << "update statistics successful";
                        }
                        qDebug() << "Remarks = " << remark_value;
                    }
                }
            }
            else{
                qDebug()<<"json error:"<<json_error.errorString();
            }
        }
        else if(status_code == 500){
            qDebug() << "connect fail";
        }
    }
    reply->deleteLater();
    return status_code;
}


int CHttp::ConnectionTest(QString strurl)
{
    QNetworkRequest request;
    request.setUrl(strurl);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    request.setRawHeader("Test-Msg","test message");
    manager->get(request);

    QNetworkReply *reply=manager->get(request);
    QEventLoop eventLoop;
    connect(manager, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    int nresult = receiveReply(reply);
    eventLoop.exit();

    return nresult;
}


