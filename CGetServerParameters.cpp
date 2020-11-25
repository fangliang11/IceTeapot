#include "CGetServerParameters.h"

CGetServerParameters::CGetServerParameters(QString baseURL, QObject *parent) : QObject(parent)
{
   mainURL = baseURL;
}


void CGetServerParameters::InitialConnect()
{
    m_httpServer.Initial(mainURL);

    m_strUpdataRivalryStatisticUrl = mainURL +"/live/match/statistics";
    m_strUpdataSpecialStatisticUrl = mainURL +"/live/match/statistics";
    m_strUpdataMatchStatus         = mainURL +"/live/match/statistics";

}

//测试
void CGetServerParameters::ConnectionTest(QString strurl)
{
    m_httpServer.ConnectionTest(strurl);
}

//获取公共接口数据
void CGetServerParameters::ReadCommonData(CommonData &commondata)
{
    int nresult = m_httpServer.GetCommonMsg();
    while(!m_httpServer.response_success){
        QEventLoop eventloop;
        QTimer::singleShot(100, &eventloop, SLOT(quit()));
        eventloop.exec();
    }
    m_httpServer.response_success = false;
    qDebug() << "response code : " << nresult;

    std::vector<Task> vecTasks;
    ParsingCommonTasks(vecTasks);

    std::vector<Handle> vecHandles;
    ParsingCommonHandles(vecHandles);

    std::vector<Zone> vecZones;
    ParsingCommonZones(vecZones);

    std::vector<Point> vecPoints;
    ParsingCommonPoints(vecPoints);

    commondata.tasks = vecTasks;
    commondata.handles = vecHandles;
    commondata.zones = vecZones;
    commondata.points = vecPoints;

}

//获取专项训练数据
void CGetServerParameters::ReadSpecialMatchData(std::vector<MatchData> &matchdata)
{
    int nresult = m_httpServer.GetSpecialTrainMsg();
    while(!m_httpServer.response_success){
        QEventLoop eventloop;
        QTimer::singleShot(100, &eventloop, SLOT(quit()));
        eventloop.exec();
    }
    m_httpServer.response_success = false;
    qDebug() << "response code : " << nresult;


    QJsonObject obj = m_httpServer.m_jsonSpecialTrain;
    if(obj.contains("data")){
        QJsonArray array = obj.value("data").toArray();
        MatchData match;
        for(int i = 0; i < array.size(); i++){
            QJsonValue val = array.at(i);
            QJsonObject second_obj = val.toObject();
            match.training_code = second_obj.value("training_code").toString();
            match.team_code     = "";
            match.team_name     = "";
            match.team_logo     = "";
            match.training_event = "";
            match.training_date = second_obj.value("training_date").toString();
            match.training_time = second_obj.value("training_time").toString();
            match.venue         = second_obj.value("venue").toString();            
            if(second_obj.value("track") == 0x80) match.track_code = "";
            else match.track_code    = second_obj.value("track").toString();
            match.track_name    = "";
            if(second_obj.value("weather") == 0x80) match.weather = "";
            else match.weather  = second_obj.value("weather").toString();
            if(second_obj.value("temperature") == 0x80) match.temperature = "";
            else match.temperature  = second_obj.value("temperature").toString();
            if(second_obj.value("humidity") == 0x80) match.humidity = "";
            else match.humidity  = second_obj.value("humidity").toString();
            if(second_obj.value("ice_surface_temperature") == 0x80) match.ice_surface_temperature = "";
            else match.ice_surface_temperature  = second_obj.value("ice_surface_temperature").toString();

            std::vector<Athlete> vecAthletes;
            ParsingSpecialAthletes(second_obj, vecAthletes);
            match.athletes = vecAthletes;
            matchdata.push_back(match);
        }
    }
}

//获取对抗训练数据
void CGetServerParameters::ReadRivalryMatchData(std::vector<MatchData> &matchdata)
{
    int nresult = m_httpServer.GettRivalryTrainMsg();
    while(!m_httpServer.response_success){
        QEventLoop eventloop;
        QTimer::singleShot(100, &eventloop, SLOT(quit()));
        eventloop.exec();
    }
    m_httpServer.response_success = false;
    qDebug() << "response code : " << nresult;

    QJsonObject obj = m_httpServer.m_jsonRivalryTrain;
    if(obj.contains("data")){
        QJsonArray array = obj.value("data").toArray();
        MatchData match;
        for(int i = 0; i < array.size(); i++){
            QJsonValue val = array.at(i);
            QJsonObject second_obj = val.toObject();
            match.training_code = second_obj.value("training_code").toString();
            match.team_code     = second_obj.value("team_code").toString();
            match.team_name     = second_obj.value("team_name").toString();
            if(second_obj.value("team_logo") == 0x80) match.team_logo = "";
            else match.team_logo  = second_obj.value("team_logo").toString();
            match.training_event = second_obj.value("training_event").toString();
            match.training_date = second_obj.value("training_date").toString();
            match.training_time = second_obj.value("training_time").toString();
            match.venue         = second_obj.value("venue").toString();
            match.track_code    = second_obj.value("track_code").toString();
            match.track_name    = second_obj.value("track_name").toString();
            if(second_obj.value("weather") == 0x80) match.weather = "";
            else match.weather  = second_obj.value("weather").toString();
            if(second_obj.value("temperature") == 0x80) match.temperature = "";
            else match.temperature  = second_obj.value("temperature").toString();
            if(second_obj.value("humidity") == 0x80) match.humidity = "";
            else match.humidity  = second_obj.value("humidity").toString();
            if(second_obj.value("ice_surface_temperature") == 0x80) match.ice_surface_temperature = "";
            else match.ice_surface_temperature  = second_obj.value("ice_surface_temperature").toString();

            std::vector<Athlete> vecAthletes;
            ParsingVerseAthletes(second_obj, vecAthletes);
            match.athletes = vecAthletes;
            matchdata.push_back(match);
        }
    }
}

//上传
void CGetServerParameters::UpdataVerseMatchStatisticData()
{
    QJsonObject obj = m_jsonRivalryStatistics;

    int nresult = m_httpServer.PostStatisticsMsg(m_strUpdataRivalryStatisticUrl, obj);
    while(!m_httpServer.response_success){
        QEventLoop eventloop;
        QTimer::singleShot(100, &eventloop, SLOT(quit()));
        eventloop.exec();
    }
    m_httpServer.response_success = false;
    qDebug() << "response code : " << nresult;
}

void CGetServerParameters::UpdataVerseMatchStatisticData(MatchStatistic matchdata)
{
    QJsonObject obj = GenerateRivalryMatchStatisticData(matchdata);


    int nresult = m_httpServer.PostStatisticsMsg(m_strUpdataRivalryStatisticUrl, obj);
    while(!m_httpServer.response_success){
        QEventLoop eventloop;
        QTimer::singleShot(100, &eventloop, SLOT(quit()));
        eventloop.exec();
    }
    m_httpServer.response_success = false;
    qDebug() << "response code : " << nresult;

}

void CGetServerParameters::UpdataSpecialMatchStatisticData()
{
    QJsonObject obj = m_jsonSpecialStatistics;


    int nresult = m_httpServer.PostStatisticsMsg(m_strUpdataSpecialStatisticUrl, obj);
    while(!m_httpServer.response_success){
        QEventLoop eventloop;
        QTimer::singleShot(100, &eventloop, SLOT(quit()));
        eventloop.exec();
    }
    m_httpServer.response_success = false;
    qDebug() << "response code : " << nresult;


}

void CGetServerParameters::UpdataSpecialMatchStatisticData(MatchStatistic matchdata)
{
    QJsonObject obj = GenerateSpecialMatchStatisticData(matchdata);


    int nresult = m_httpServer.PostStatisticsMsg(m_strUpdataSpecialStatisticUrl, obj);
    while(!m_httpServer.response_success){
        QEventLoop eventloop;
        QTimer::singleShot(100, &eventloop, SLOT(quit()));
        eventloop.exec();
    }
    m_httpServer.response_success = false;
    qDebug() << "response code : " << nresult;

}

void CGetServerParameters::UpdataMatchStatus(int nstatus)
{
    QJsonObject obj;
    obj.insert("status", nstatus);


    int nresult = m_httpServer.PostStatisticsMsg(m_strUpdataMatchStatus, obj);
    while(!m_httpServer.response_success){
        QEventLoop eventloop;
        QTimer::singleShot(100, &eventloop, SLOT(quit()));
        eventloop.exec();
    }
    m_httpServer.response_success = false;
    qDebug() << "response code : " << nresult;


}

// 解析
void CGetServerParameters::ParsingCommonTasks(std::vector<Task> &value)
{
    QJsonObject obj = m_httpServer.m_jsonCommon;
    if(obj.contains("data")){
        QJsonObject second_obj = obj.value("data").toObject();
        if(second_obj.contains("tasks")){
            QJsonArray array = second_obj.value("tasks").toArray();
            Task task;
            for(int i = 0; i < array.size(); i++){
                QJsonValue val = array.at(i);
                QJsonObject third_obj = val.toObject();
                task.code = third_obj.value("code").toString();
                task.name = third_obj.value("name").toString();
                task.name_en = third_obj.value("name_en").toString();
                value.push_back(task);
                qDebug() << i << " task:code = " << task.code << ",  name = "
                         << task.name << ",  name_en = " << task.name_en;
            }
        }
    }
}

void CGetServerParameters::ParsingCommonHandles(std::vector<Handle> &value)
{
    QJsonObject obj = m_httpServer.m_jsonCommon;
    if(obj.contains("data")){
        QJsonObject second_obj = obj.value("data").toObject();
        if(second_obj.contains("handles")){
            QJsonArray array = second_obj.value("handles").toArray();
            Handle handle;
            for(int i = 0; i < array.size(); i++){
                QJsonValue val = array.at(i);
                QJsonObject third_obj = val.toObject();
                handle.code = third_obj.value("code").toString();
                handle.name = third_obj.value("name").toString();
                handle.name_en = third_obj.value("name_en").toString();
                value.push_back(handle);
                qDebug() << i << " handle: code = " << handle.code << ",  name = "
                         << handle.name << ",  name_en = " << handle.name_en;
            }
        }
    }
}

void CGetServerParameters::ParsingCommonZones(std::vector<Zone> &value)
{
    QJsonObject obj = m_httpServer.m_jsonCommon;
    if(obj.contains("data")){
        QJsonObject second_obj = obj.value("data").toObject();
        if(second_obj.contains("zones")){
            QJsonArray array = second_obj.value("zones").toArray();
            Zone zone;
            for(int i = 0; i < array.size(); i++){
                QJsonValue val = array.at(i);
                QJsonObject third_obj = val.toObject();
                zone.code = third_obj.value("code").toString();
                zone.name = third_obj.value("name").toString();
                zone.name_en = third_obj.value("name_en").toString();
                value.push_back(zone);
                qDebug() << i << " zone: code = " << zone.code << ",  name = "
                         << zone.name << ",  name_en = " << zone.name_en;
            }
        }
    }
}

void CGetServerParameters::ParsingCommonPoints(std::vector<Point> &value)
{
    QJsonObject obj = m_httpServer.m_jsonCommon;
    if(obj.contains("data")){
        QJsonObject second_obj = obj.value("data").toObject();
        if(second_obj.contains("points")){
            QJsonArray array = second_obj.value("points").toArray();
            Point point;
            for(int i = 0; i < array.size(); i++){
                QJsonValue val = array.at(i);
                QJsonObject third_obj = val.toObject();
                point.code = third_obj.value("code").toString();
                point.name = third_obj.value("name").toString();
                point.name_en = third_obj.value("name_en").toString();
                point.percent = third_obj.value("percent").toDouble();
                point.point = third_obj.value("point").toString();
                value.push_back(point);
                qDebug() << i << " point: code = " << point.code << ",  name = "
                         << point.name << ",  name_en = " << point.name_en
                         << ",  percent = " << point.percent << ",  point = " << point.point;
            }
        }
    }
}

// 解析运动员信息
void CGetServerParameters::ParsingVerseAthletes(QJsonObject obj, std::vector<Athlete> &value)
{
    QJsonArray array = obj.value("athletes").toArray();
    Athlete athlete;
    for(int i = 0; i < array.size(); i++){
        QJsonValue val = array.at(i);
        QJsonObject second_obj = val.toObject();
        athlete.athlete_code = second_obj.value("athlete_code").toString();
        athlete.athlete_name = second_obj.value("athlete_name").toString();
        athlete.role_num     = second_obj.value("role_num").toInt();
        athlete.gender       = second_obj.value("gender").toString();
        athlete.age          = second_obj.value("age").toInt();
        if(second_obj.value("height") == 0x80) athlete.height = 0;
        else athlete.height  = second_obj.value("height").toInt();
        if(second_obj.value("weight") == 0x80) athlete.weight = 0;
        else athlete.weight  = second_obj.value("weight").toInt();
        athlete.photo        = second_obj.value("photo").toString();
        if(second_obj.value("is_skip") == 0x80) athlete.is_skip = false;
        else athlete.is_skip  = second_obj.value("is_skip").toBool();
        value.push_back(athlete);
    }
}

void CGetServerParameters::ParsingSpecialAthletes(QJsonObject obj, std::vector<Athlete> &value)
{
    QJsonArray array = obj.value("athletes").toArray();
    Athlete athlete;
    for(int i = 0; i < array.size(); i++){
        QJsonValue val = array.at(i);
        QJsonObject second_obj = val.toObject();
        athlete.athlete_code = second_obj.value("athlete_code").toString();
        athlete.athlete_name = second_obj.value("athlete_name").toString();
        athlete.role_num     = 0;
        athlete.gender       = second_obj.value("gender").toString();
        athlete.age          = second_obj.value("age").toInt();
        if(second_obj.value("height") == 0x80) athlete.height = 0;
        else athlete.height  = second_obj.value("height").toInt();
        if(second_obj.value("weight") == 0x80) athlete.weight = 0;
        else athlete.weight  = second_obj.value("weight").toInt();
        athlete.photo        = second_obj.value("photo").toString();
        athlete.is_skip      = false;
        value.push_back(athlete);
    }
}

//  对抗 statistics
QJsonObject CGetServerParameters::GenerateRivalryMatchStatisticData(MatchStatistic matchdata)
{
    QJsonObject obj;
    obj.insert("training_code", matchdata.training_code);
    QJsonArray arry_team;
    for(int i=0;i<(int)matchdata.teams_statistics.size(); i++){
        QJsonObject teams_statistics_obj;
        teams_statistics_obj.insert("team_code", matchdata.teams_statistics.at(i).team_code);
        teams_statistics_obj.insert("score", matchdata.teams_statistics.at(i).score);
        arry_team.insert(i, teams_statistics_obj);
    }
    obj.insert("teams_statistics", arry_team);

    QJsonArray arry_statistics;
    for(int i=0; i<(int)matchdata.statistics.size(); i++){
        QJsonObject statistics_obj;
        statistics_obj.insert("athlete_code", matchdata.statistics.at(i).athlete_code);
        statistics_obj.insert("order", matchdata.statistics.at(i).order);
        statistics_obj.insert("task_code", matchdata.statistics.at(i).task_code);
        statistics_obj.insert("task_remark", matchdata.statistics.at(i).task_remark);
        statistics_obj.insert("handle_code", matchdata.statistics.at(i).handle_code);
        statistics_obj.insert("point_code", matchdata.statistics.at(i).point_code);
        statistics_obj.insert("comment", matchdata.statistics.at(i).comment);
        //statistics_obj.insert("zone_code", matchdata.statistics.at(i).zone_code);
        statistics_obj.insert("is_outside", matchdata.statistics.at(i).is_outside);
        statistics_obj.insert("image", matchdata.statistics.at(i).image);
        statistics_obj.insert("initial_speed", matchdata.statistics.at(i).initial_speed);
        statistics_obj.insert("spin", matchdata.statistics.at(i).spin);
        statistics_obj.insert("running_speed", matchdata.statistics.at(i).running_speed);
        statistics_obj.insert("acceleration", matchdata.statistics.at(i).acceleration);
        statistics_obj.insert("angular_velocity", matchdata.statistics.at(i).angular_velocity);
        statistics_obj.insert("coefficient_of_friction", matchdata.statistics.at(i).coefficient_of_friction);
        statistics_obj.insert("long_clock", matchdata.statistics.at(i).long_clock);
        statistics_obj.insert("short_clock", matchdata.statistics.at(i).short_clock);
        statistics_obj.insert("video_url", matchdata.statistics.at(i).video_url);
        statistics_obj.insert("track_video_url", matchdata.statistics.at(i).track_video_url);
        QJsonObject curloc;
        curloc.insert("abscissa", matchdata.statistics.at(i).current_location.abscissa);
        curloc.insert("ordinate", matchdata.statistics.at(i).current_location.ordinate);
        curloc.insert("real_abscissa", matchdata.statistics.at(i).current_location.real_abscissa);
        curloc.insert("real_ordinate", matchdata.statistics.at(i).current_location.real_ordinate);
        curloc.insert("coordinate_plotting_scale", matchdata.statistics.at(i).current_location.coordinate_plotting_scale);
        statistics_obj.insert("current_location", curloc);
        QJsonArray arryloc;
        for(int j=0; j<(int)matchdata.statistics.at(i).locations.size(); j++){
            QJsonObject loc_obj;
            loc_obj.insert("abscissa", matchdata.statistics.at(i).locations.at(j).abscissa);
            loc_obj.insert("ordinate", matchdata.statistics.at(i).locations.at(j).ordinate);
            loc_obj.insert("real_abscissa", matchdata.statistics.at(i).locations.at(j).real_abscissa);
            loc_obj.insert("real_ordinate", matchdata.statistics.at(i).locations.at(j).real_ordinate);
            loc_obj.insert("coordinate_plotting_scale", matchdata.statistics.at(i).locations.at(j).coordinate_plotting_scale);
            loc_obj.insert("colour", matchdata.statistics.at(i).locations.at(j).colour);
            arryloc.insert(j, loc_obj);
        }
        statistics_obj.insert("locations", arryloc);
    }
    obj.insert("statistics", arry_statistics);

    return obj;
}

// 专项 statistics
QJsonObject CGetServerParameters::GenerateSpecialMatchStatisticData(MatchStatistic matchdata)
{
    QJsonObject obj;
    obj.insert("training_code", matchdata.training_code);
    QJsonArray arry_statistics;
    for(int i=0; i<(int)matchdata.statistics.size(); i++){
        QJsonObject statistics_obj;
        statistics_obj.insert("athlete_code", matchdata.statistics.at(i).athlete_code);
        statistics_obj.insert("order", matchdata.statistics.at(i).order);
        statistics_obj.insert("task_code", matchdata.statistics.at(i).task_code);
        statistics_obj.insert("task_remark", matchdata.statistics.at(i).task_remark);
        statistics_obj.insert("handle_code", matchdata.statistics.at(i).handle_code);
        statistics_obj.insert("point_code", matchdata.statistics.at(i).point_code);
        statistics_obj.insert("comment", matchdata.statistics.at(i).comment);
        statistics_obj.insert("zone_code", matchdata.statistics.at(i).zone_code);
        statistics_obj.insert("is_outside", matchdata.statistics.at(i).is_outside);
        statistics_obj.insert("image", matchdata.statistics.at(i).image);
        statistics_obj.insert("initial_speed", matchdata.statistics.at(i).initial_speed);
        statistics_obj.insert("spin", matchdata.statistics.at(i).spin);
        statistics_obj.insert("running_speed", matchdata.statistics.at(i).running_speed);
        statistics_obj.insert("acceleration", matchdata.statistics.at(i).acceleration);
        statistics_obj.insert("angular_velocity", matchdata.statistics.at(i).angular_velocity);
        statistics_obj.insert("coefficient_of_friction", matchdata.statistics.at(i).coefficient_of_friction);
        statistics_obj.insert("long_clock", matchdata.statistics.at(i).long_clock);
        statistics_obj.insert("short_clock", matchdata.statistics.at(i).short_clock);
        statistics_obj.insert("video_url", matchdata.statistics.at(i).video_url);
        statistics_obj.insert("track_video_url", matchdata.statistics.at(i).track_video_url);
        QJsonObject curloc;
        curloc.insert("abscissa", matchdata.statistics.at(i).current_location.abscissa);
        curloc.insert("ordinate", matchdata.statistics.at(i).current_location.ordinate);
        curloc.insert("real_abscissa", matchdata.statistics.at(i).current_location.real_abscissa);
        curloc.insert("real_ordinate", matchdata.statistics.at(i).current_location.real_ordinate);
        curloc.insert("coordinate_plotting_scale", matchdata.statistics.at(i).current_location.coordinate_plotting_scale);
        statistics_obj.insert("current_location", curloc);
        QJsonArray arryloc;
        for(int j=0; j<(int)matchdata.statistics.at(i).locations.size(); j++){
            QJsonObject loc_obj;
            loc_obj.insert("abscissa", matchdata.statistics.at(i).locations.at(j).abscissa);
            loc_obj.insert("ordinate", matchdata.statistics.at(i).locations.at(j).ordinate);
            loc_obj.insert("real_abscissa", matchdata.statistics.at(i).locations.at(j).real_abscissa);
            loc_obj.insert("real_ordinate", matchdata.statistics.at(i).locations.at(j).real_ordinate);
            loc_obj.insert("coordinate_plotting_scale", matchdata.statistics.at(i).locations.at(j).coordinate_plotting_scale);
            loc_obj.insert("colour", matchdata.statistics.at(i).locations.at(j).colour);
            arryloc.insert(j, loc_obj);
        }
        statistics_obj.insert("locations", arryloc);
    }
    obj.insert("statistics", arry_statistics);
    QJsonObject template_obj;
    template_obj.insert("template_image", matchdata._template.template_image);
    QJsonArray arryloc;
    for(int m=0;m<(int)matchdata._template.locations.size(); m++){
        QJsonObject loc_obj;
        loc_obj.insert("abscissa", matchdata._template.locations.at(m).abscissa);
        loc_obj.insert("ordinate", matchdata._template.locations.at(m).ordinate);
        loc_obj.insert("real_abscissa", matchdata._template.locations.at(m).real_abscissa);
        loc_obj.insert("real_ordinate", matchdata._template.locations.at(m).real_ordinate);
        loc_obj.insert("coordinate_plotting_scale", matchdata._template.locations.at(m).coordinate_plotting_scale);
        loc_obj.insert("colour", matchdata._template.locations.at(m).colour);
        arryloc.insert(m, loc_obj);
    }
    template_obj.insert("locations", arryloc);
    obj.insert("template", template_obj);

    return obj;
}

