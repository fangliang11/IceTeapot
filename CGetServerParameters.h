#ifndef CGETSERVERPARAMETERS_H
#define CGETSERVERPARAMETERS_H

#include <QObject>
#include "Chttp.h"



struct KeyValue
{
    int index;
    QString strvalue;
};

struct Task
{
    QString code;
    QString name;
    QString name_en;
};

struct Handle
{
    QString code;
    QString name;
    QString name_en;
};

struct Zone
{
    QString code;
    QString name;
    QString name_en;
};

struct Point
{
    QString code;
    QString name;
    QString name_en;
    float   percent;
    QString point;
};

//   common  data
struct CommonData
{
    std::vector<Task> tasks;
    std::vector<Handle> handles;
    std::vector<Zone> zones;
    std::vector<Point> points;
};


struct Athlete
{
    QString athlete_code;
    QString athlete_name;
    int     role_num;
    QString gender;
    int     age;
    int     height;
    int     weight;
    QString photo;    //  QString to QImage
    bool    is_skip;
};

//   match  data
struct MatchData
{
    QString training_code;
    QString team_code;
    QString team_name;
    QString team_logo;
    QString training_event;
    QString training_date;
    QString training_time;
    QString venue;
    QString track_code;
    QString track_name;
    QString weather;
    QString temperature;
    QString humidity;
    QString ice_surface_temperature;
    std::vector<Athlete> athletes;
};

struct TeamStatistics
{
    QString team_code;
    int     score;
};


struct Location
{
    float     abscissa;
    float     ordinate;
    float real_abscissa;
    float real_ordinate;
    float coordinate_plotting_scale;
    QString colour;
};

struct Statistic
{
    QString athlete_code;
    int     order;
    QString task_code;
    QString task_remark;
    QString handle_code;
    QString point_code;
    QString comment;
    QString zone_code;
    bool    is_outside;
    QString image;
    float   initial_speed;
    float   spin;
    float   running_speed;
    float   acceleration;
    float   angular_velocity;
    float   coefficient_of_friction;
    float   long_clock;
    float   short_clock;
    QString video_url;
    QString track_video_url;
    Location current_location;
    std::vector<Location> locations;
};

//  statistics  data
struct StatisticsData
{
    QString training_code;
    std::vector<TeamStatistics> teams_statistics;
    std::vector<Statistic> statistics;
};

struct TrainTemplate
{
    QString template_image;
    std::vector<Location> locations;
};

// match statistics data
struct MatchStatistic
{
    QString training_code;
    std::vector<TeamStatistics> teams_statistics;
    TrainTemplate _template;
    std::vector<Statistic> statistics;
};



class CGetServerParameters : public QObject
{
    Q_OBJECT
public:
    explicit CGetServerParameters(QString baseURL, QObject *parent = nullptr);


    QString m_strUpdataRivalryStatisticUrl;
    QString m_strUpdataSpecialStatisticUrl;
    QString m_strUpdataMatchStatus;
    QJsonObject m_jsonRivalryStatistics;
    QJsonObject m_jsonSpecialStatistics;


    void InitialConnect();
    void ConnectionTest(QString strurl);

    void ReadCommonData(CommonData &commondata);
    void ReadSpecialMatchData(std::vector<MatchData> &matchdata);
    void ReadRivalryMatchData(std::vector<MatchData> &matchdata);

    void UpdataVerseMatchStatisticData(MatchStatistic matchdata);
    void UpdataVerseMatchStatisticData();
    void UpdataSpecialMatchStatisticData(MatchStatistic matchdata);
    void UpdataSpecialMatchStatisticData();
    void UpdataMatchStatus(int nstatus);




protected:

    //  common
    void ParsingCommonTasks(std::vector<Task> &value);
    void ParsingCommonHandles(std::vector<Handle> &value);
    void ParsingCommonZones(std::vector<Zone> &value);
    void ParsingCommonPoints(std::vector<Point> &value);
    //   match
    void ParsingVerseAthletes(QJsonObject obj, std::vector<Athlete> &value);
    void ParsingSpecialAthletes(QJsonObject obj, std::vector<Athlete> &value);

    //  statistics
    QJsonObject GenerateRivalryMatchStatisticData(MatchStatistic matchdata);
    QJsonObject GenerateSpecialMatchStatisticData(MatchStatistic matchdata);

private:
    QString mainURL;
    CHttp m_httpServer;

signals:

};

#endif // CGETSERVERPARAMETERS_H
