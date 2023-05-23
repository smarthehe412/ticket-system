#include <iostream>
namespace sjtu
{
const int days[13]={0,0,31,59,90,120,151,181,212,243,273,304,334};
struct DATE
{
    int month,day;
    DATE(){month=day=0;}
    DATE(int month,int day): month(month),day(day) {}
    DATE(const std::string &s)
    {
        month=(s[0]-'0')*10+s[1]-'0';
        day=(s[3]-'0')*10+s[4]-'0';
    }
    bool operator < (const DATE &a) const
    {
        if(month!=a.month) return month<a.month;
        return day<a.day;
    }
    bool operator == (const DATE &a) const
    {
        return month==a.month&&day==a.day;
    }
    int operator - (const DATE &a) const
    {
        return (days[month]+day-days[a.month]-a.day)*24*60;
    }
    DATE& operator ++ (int)
    {
        int now=(month==12?31:days[month+1]-days[month]);
        if(day==now) month=month%12+1,day=0;
        day++;return *this;
    }
    DATE& operator -- (int)
    {
        int now=(month==1?31:days[month]-days[month-1]);
        if(day==0) {month--;if(month==0) month=12;day=now+1;}
        day--;return *this;
    }
    DATE& operator += (const int &a)
    {
        day+=a;
        int now=(month==12?31:days[month+1]-days[month]);
        while(now<day)
        {
            month=month%12+1,day-=now;
            now=(month==12?31:days[month+1]-days[month]);
        }
        now=(month==1?31:days[month]-days[month-1]);
        while(day<0)
        {
            month--;if(month==0) month=12;
            day+=now;
            now=(month==1?31:days[month]-days[month-1]);
        }
        return *this;
    }
    friend std::ostream& operator << (std::ostream &s, const DATE &a);
};
std::ostream& operator << (std::ostream &s, const DATE &a)
{
    s<<a.month<<"-"<<a.day;
    return s;
}
struct TIME
{
    int hour,minute;
    TIME(){hour=minute=0;}
    TIME(int hour,int minute): hour(hour),minute(minute) {}
    TIME(const std::string &s)
    {
        hour=(s[0]-'0')*10+s[1]-'0';
        minute=(s[3]-'0')*10+s[4]-'0';
    }
    bool operator < (const TIME &a) const
    {
        if(hour!=a.hour) return hour<a.hour;
        return minute<a.minute;
    }
    bool operator == (const TIME &a) const
    {
        return hour==a.hour&&minute==a.minute;
    }
    int operator - (const TIME &a) const
    {
        return (hour-a.hour)*60+minute-a.minute;
    }
    TIME operator + (const int &a)
    {
        TIME ret(*this);
        ret.minute+=a;
        ret.hour+=ret.minute/60;
        ret.minute%=60;
        return ret;
    }
    TIME& operator += (const int &a)
    {
        minute+=a;
        hour+=minute/60;
        minute%=60;
        if(minute<0) minute+=60,hour--;
        return *this;
    }
    friend std::ostream& operator << (std::ostream &s, const TIME &a);
};
std::ostream& operator << (std::ostream &s, const TIME &a)
{
    s<<a.hour<<":"<<a.minute;
    return s;
}
struct DATE_TIME
{
    DATE date;
    TIME time;
    DATE_TIME(){}
    DATE_TIME(DATE date,TIME time): date(date),time(time)
    {
        date+=time.hour/24;
        time.hour%=24;
        if(time.hour<0) time.hour+=24,date--;
    }
    DATE_TIME operator + (const int &c) const
    {
        DATE_TIME ret=*this;
        ret.time+=c;
        ret.date+=ret.time.hour/24;
        ret.time.hour%=24;
        if(ret.time.hour<0) ret.time.hour+=24,ret.date--;
        return ret;
    }
    int operator - (const DATE_TIME &a) const
    {
        return (date-a.date)+(time-a.time);
    }
    DATE_TIME operator += (const int &c)
    {
        time+=c;
        date+=time.hour/24;
        time.hour%=24;
        if(time.hour<0) time.hour+=24,date--;
        return *this;
    }
    friend std::ostream& operator << (std::ostream &s, const DATE_TIME &a);
};
std::ostream& operator << (std::ostream &s, const DATE_TIME &a)
{
    s<<a.date<<" "<<a.time;
    return s;
}
}