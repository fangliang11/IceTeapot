#include <QCoreApplication>

#include "CGetServerParameters.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString baseURL = "http://curling.cmp-dev.onesport.com.cn";
    CGetServerParameters param(baseURL);
    param.InitialConnect();
    //param.ConnectionTest(baseURL);
    CommonData test_common;
    param.ReadCommonData(test_common);




//    Task mtask;
//    if(!test_common.tasks.empty()){
//        mtask = test_common.tasks.front();
//        std::cout << "Http Server Response: common data = " << mtask.name.toStdString();
//    }
//    else{
//        std::cout << "task empty" << std::endl;
//    }


    return a.exec();
}
