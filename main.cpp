#include "bpt/bpt.cpp"
#include "string.cpp"
#include "date.cpp"
#include <cstring>
#include <iostream>
#include <ostream>
#include <string>
#include <algorithm>

const int SUCCESS=0,FAILED=-1;

struct USER
{
    sjtu::string<20> username;
    sjtu::string<30> password,mailAddr;
    sjtu::string<20> name;
    int privilege;
    USER(){privilege=-1;}
    USER(sjtu::string<20> username,sjtu::string<30> password,sjtu::string<20> name,sjtu::string<30> mailAddr,int privilege):
        username(username),password(password),name(name),mailAddr(mailAddr),privilege(privilege) {}
    void print() {std::cout<<username<<" "<<name<<" "<<mailAddr<<" "<<privilege<<std::endl;}
    bool operator < (const USER &a) const {return username<a.username;}
    bool operator == (const USER &a) const {return username==a.username;}
};
sjtu::BPT<sjtu::string<20>,USER,25,256> users("users_init","users_data","users_value");
sjtu::BPT<sjtu::string<20>,USER,25,256> login("login_init","login_data","login_value");
std::string s;
int sp;
std::string get_word()
{
    while(s[sp]==' ') sp++;
    int st=sp;
    while(s[sp]!=' '&&sp<s.length()) sp++;
    return s.substr(st,sp-st);
}
sjtu::vector<std::string> get_piece(const std::string &ts)
{
    sjtu::vector<std::string> ret;
    char *cs=new char(ts.length()+1);
    strcpy(cs,ts.c_str());
    char *tmp=strtok(cs,"|");
    while(tmp!=NULL)
    {
        ret.push_back(std::string(tmp));
        tmp=strtok(cs,"|");
    }
    return ret;
}
char get_key()
{
    std::string tmp=get_word();
    return tmp[1];
}
void add_user()
{
    sjtu::string<20> cur_username,username,name;
    sjtu::string<30> password,mailAddr;
    int privilege;
    while(sp<s.length())
    {
        switch(get_key())
        {
            case 'c': cur_username=get_word();break;
            case 'u': username=get_word();break;
            case 'p': password=get_word();break;
            case 'n': name=get_word();break;
            case 'm': mailAddr=get_word();break;
            case 'g': privilege=stoi(get_word());break;
            default: std::cout<<FAILED<<std::endl;return;
        }
    }
    if(users.empty())
    {
        privilege=10;
        users.insert(username,USER(username,password,name,mailAddr,privilege));
        std::cout<<SUCCESS<<std::endl;return;
    }
    USER ret=login.query(cur_username);
    if(ret.privilege==-1) {std::cout<<FAILED<<std::endl;return;}
    USER ret2=users.query(username);
    if(ret2.privilege!=-1) {std::cout<<FAILED<<std::endl;return;}
    if(ret.privilege<=privilege) {std::cout<<FAILED<<std::endl;return;}
    users.insert(username,USER(username,password,name,mailAddr,privilege));
    std::cout<<SUCCESS<<std::endl;return;
}
void log_in()
{
    sjtu::string<20> username;
    sjtu::string<30> password;
    while(sp<s.length())
    {
        switch(get_key())
        {
            case 'u': username=get_word();break;
            case 'p': password=get_word();break;
            default: std::cout<<FAILED<<std::endl;return;
        }
    }
    USER ret=users.query(username);
    if(ret.privilege==-1) {std::cout<<FAILED<<std::endl;return;}
    if(ret.password!=password) {std::cout<<FAILED<<std::endl;return;}
    USER ret2=login.query(username);
    if(ret2.privilege!=-1) {std::cout<<FAILED<<std::endl;return;}
    login.insert(username,ret);
    std::cout<<SUCCESS<<std::endl;return;
}
void log_out()
{
    sjtu::string<20> username;
    while(sp<s.length())
    {
        switch(get_key())
        {
            case 'u': username=get_word();break;
            default: std::cout<<FAILED<<std::endl;return;
        }
    }
    USER ret=login.query(username);
    if(ret.privilege==-1) {std::cout<<FAILED<<std::endl;return;}
    login.erase(username);
    std::cout<<SUCCESS<<std::endl;return;
}
void query_profile()
{
    sjtu::string<20> cur_username,username;
    while(sp<s.length())
    {
        switch(get_key())
        {
            case 'c': cur_username=get_word();break;
            case 'u': username=get_word();break;
            default: std::cout<<FAILED<<std::endl;return;
        }
    }
    USER ret=login.query(cur_username);
    if(ret.privilege==-1) {std::cout<<FAILED<<std::endl;return;}
    if(cur_username==username) {ret.print();return;}
    USER ret2=users.query(username);
    if(ret2.privilege==-1) {std::cout<<FAILED<<std::endl;return;}
    if(ret.privilege<=ret2.privilege) {std::cout<<FAILED<<std::endl;return;}
    ret2.print();
    return;
}
void modify_profile()
{
    sjtu::string<20> cur_username,username,name;
    sjtu::string<30> password,mailAddr;
    int privilege=-1;
    while(sp<s.length())
    {
        switch(get_key())
        {
            case 'c': cur_username=get_word();break;
            case 'u': username=get_word();break;
            case 'p': password=get_word();break;
            case 'n': name=get_word();break;
            case 'm': mailAddr=get_word();break;
            case 'g': privilege=stoi(get_word());break;
            default: std::cout<<FAILED<<std::endl;return;
        }
    }
    USER ret=login.query(cur_username);
    if(ret.privilege==-1) {std::cout<<FAILED<<std::endl;return;}
    if(ret.privilege<=privilege) {std::cout<<FAILED<<std::endl;return;}
    if(cur_username==username)
    {
        users.erase(username);
        login.erase(username);
        if(password.length()) ret.password=password;
        if(name.length()) ret.name=name;
        if(mailAddr.length()) ret.mailAddr=mailAddr;
        if(privilege!=-1) ret.privilege=privilege;
        users.insert(username,ret);
        login.insert(username,ret);
        return;
    }
    USER ret2=users.query(username);
    if(ret2.privilege==-1) {std::cout<<FAILED<<std::endl;return;}
    if(ret.privilege<=ret2.privilege) {std::cout<<FAILED<<std::endl;return;}
    if(password.length()) ret2.password=password;
    if(name.length()) ret2.name=name;
    if(mailAddr.length()) ret2.mailAddr=mailAddr;
    if(privilege!=-1) ret2.privilege=privilege;
    USER ret3=login.query(username);
    users.erase(username),users.insert(username,ret2);
    if(ret3.privilege!=-1) login.erase(username),login.insert(username,ret2);
    ret2.print();
    return;
}
struct TRAIN
{
    sjtu::string<20> trainID;
    int stationNum,seatNum;
    sjtu::vector<sjtu::string<30>> station;
    sjtu::vector<int> prices,travelTimes,stopoverTimes;
    sjtu::TIME startTime;
    sjtu::DATE saleDate_l,saleDate_r;
    char type;
    TRAIN(){type='n',stationNum=seatNum=0;}
    TRAIN(sjtu::string<20> trainID,int stationNum,int seatNum,sjtu::vector<sjtu::string<30>> station,sjtu::vector<int> prices,
    sjtu::vector<int> travelTimes,sjtu::vector<int> stopoverTimes,sjtu::TIME startTime,sjtu::DATE saleDate_l,sjtu::DATE saleDate_r,char type):
    trainID(trainID),stationNum(stationNum),seatNum(seatNum),station(station),prices(prices),travelTimes(travelTimes),stopoverTimes(stopoverTimes),
    startTime(startTime),saleDate_l(saleDate_l),saleDate_r(saleDate_r),type(type) {}
    int get_price(int l,int r)
    {
        int ret=0;
        for(int i=l;i<=r;i++) ret+=prices[i];
        return ret;
    }
};
struct TRAIN_DAY
{
    sjtu::string<20> trainID;
    sjtu::DATE day;
    TRAIN_DAY(){}
    TRAIN_DAY(sjtu::string<20> trainID,sjtu::DATE day): trainID(trainID),day(day) {}
    bool operator < (const TRAIN_DAY &a) const
    {
        if(day==a.day) return trainID<a.trainID;
        return day<a.day;
    }
    bool operator == (const TRAIN_DAY &a) const
    {
        return day==a.day&&trainID==a.trainID;
    }
};
struct STOP
{
    sjtu::string<20> trainID;
    sjtu::DATE saleDate_l,saleDate_r;
    sjtu::TIME arriveTime,leaveTime;
    int stationPlace;
    STOP(){}
    STOP(sjtu::string<20> trainID,sjtu::DATE saleDate_l,sjtu::DATE saleDate_r,sjtu::TIME arriveTime,sjtu::TIME leaveTime,int stationPlace):
        trainID(trainID),saleDate_l(saleDate_l),saleDate_r(saleDate_r),arriveTime(arriveTime),leaveTime(leaveTime),stationPlace(stationPlace) {}
    bool operator < (const STOP &a) const {return trainID<a.trainID;}
    bool operator == (const STOP &a) const {return trainID==a.trainID;}
};
struct SEAT
{
    int seatNum;
    sjtu::vector<int> rem;
    SEAT(){seatNum=-1;}
    SEAT(int seatNum,sjtu::vector<int> rem): seatNum(seatNum),rem(rem) {}
    int min(int l,int r)
    {
        int ret=seatNum;
        for(int i=l;i<=r;i++) if(rem[i]<ret) ret=rem[i];
        return ret;
    }
    void modify(int l,int r,int v)
    {
        for(int i=l;i<=r;i++) rem[i]+=v;
    }
};
struct QUERY
{
    int timestamp,price,cnt,l,r,type;
    sjtu::string<20> username;
    TRAIN_DAY trainDay;
    sjtu::string<30> from,to;
    sjtu::DATE_TIME st,ed;
    QUERY(){timestamp=-1,cnt=-1,l=r=0,type=-1;}
    QUERY(int timestamp,sjtu::string<20> username,TRAIN_DAY trainDay,sjtu::string<30> from,sjtu::string<30> to,
    sjtu::DATE_TIME st,sjtu::DATE_TIME ed,int price,int cnt,int l,int r,int type):
        timestamp(timestamp),username(username),trainDay(trainDay),from(from),to(to),st(st),ed(ed),price(price),cnt(cnt),l(l),r(r),type(type) {}
    bool operator < (const QUERY &a) const {return timestamp<a.timestamp;}
    bool operator == (const QUERY &a) const {return timestamp==a.timestamp;}
};
struct TICKET
{
    sjtu::string<20> trainID;
    int price,seat;
    sjtu::DATE_TIME st,ed;
    TICKET(){price=seat=0;}
    TICKET(sjtu::string<20> trainID,int price,int seat,sjtu::DATE_TIME st,sjtu::DATE_TIME ed):
        trainID(trainID),price(price),seat(seat),st(st),ed(ed) {}
};
bool cmpc(const TICKET &a,const TICKET &b)
{
    if(a.price==b.price) return a.trainID<b.trainID;
    return a.price<b.price;
}
bool cmpt(const TICKET &a,const TICKET &b)
{
    if((a.ed-a.st)==(b.ed-b.st)) return a.trainID<b.trainID;
    return (a.ed-a.st)<(b.ed-b.st);
}
sjtu::BPT<sjtu::string<20>,TRAIN,25,256> released("released_init","released_data","released_value");
sjtu::BPT<sjtu::string<20>,TRAIN,25,256> trains("trains_init","trains_data","trains_value");
sjtu::BPT<TRAIN_DAY,SEAT,25,256> seats("seats_init","seats_data","seats_value");
sjtu::multiBPT<sjtu::string<30>,STOP,25,256> stops("stops_init","stops_data");
sjtu::multiBPT<TRAIN_DAY,QUERY,25,256> pendings("pendings_init","pendings_data");
sjtu::multiBPT<sjtu::string<20>,QUERY,25,256> orders("orders_init","orders_data");
void add_train()
{
    sjtu::string<20> trainID;
    int stationNum,seatNum;
    sjtu::vector<sjtu::string<30>> station;
    sjtu::vector<int> prices,travelTimes,stopoverTimes;
    sjtu::TIME startTime;
    sjtu::DATE saleDate_l,saleDate_r;
    char type;
    while(sp<s.length())
    {
        switch(get_key())
        {
            case 'i':trainID=get_word();break;
            case 'n':stationNum=stoi(get_word());break;
            case 'm':seatNum=stoi(get_word());break;
            case 's':
            {
                sjtu::vector<std::string> tmp=get_piece(get_word());
                for(auto i=tmp.begin();i!=tmp.end();i++) station.push_back(sjtu::string<30>(*i));
                break;
            }
            case 'p':
            {
                sjtu::vector<std::string> tmp=get_piece(get_word());
                for(auto i=tmp.begin();i!=tmp.end();i++) prices.push_back(stoi(*i));
                break;
            }
            case 'x':startTime=sjtu::TIME(get_word());break;
            case 't':
            {
                sjtu::vector<std::string> tmp=get_piece(get_word());
                for(auto i=tmp.begin();i!=tmp.end();i++) travelTimes.push_back(stoi(*i));
                break;
            }
            case 'o':
            {
                std::string ts=get_word();
                if(ts=="_") break;
                sjtu::vector<std::string> tmp=get_piece(ts);
                for(auto i=tmp.begin();i!=tmp.end();i++) travelTimes.push_back(stoi(*i));
                break;
            }
            case 'd':
            {
                sjtu::vector<std::string> tmp=get_piece(get_word());
                saleDate_l=sjtu::DATE(tmp[0]),saleDate_r=sjtu::DATE(tmp[1]);
                break;
            }
            case 'y':type=get_word()[0];break;
            default: std::cout<<FAILED<<std::endl;return;
        }
    }
    TRAIN ret=trains.query(trainID);
    if(ret.type!='n') {std::cout<<FAILED<<std::endl;return;}
    trains.insert(trainID, TRAIN(trainID,stationNum,seatNum,station,prices,travelTimes,stopoverTimes,startTime,saleDate_l,saleDate_r,type));
    std::cout<<SUCCESS<<std::endl;return;
}
void delete_train()
{
    sjtu::string<20> trainID;
    while(sp<s.length())
    {
        switch(get_key())
        {
            case 'i':trainID=get_word();break;
            default: std::cout<<FAILED<<std::endl;return;
        }
    }
    TRAIN ret=released.query(trainID);
    if(ret.type!='n') {std::cout<<FAILED<<std::endl;return;}
    TRAIN ret2=trains.query(trainID);
    if(ret2.type=='n') {std::cout<<FAILED<<std::endl;return;}
    trains.erase(trainID);
    std::cout<<SUCCESS<<std::endl;return;
}
void release_train()
{
    sjtu::string<20> trainID;
    while(sp<s.length())
    {
        switch(get_key())
        {
            case 'i':trainID=get_word();break;
            default: std::cout<<FAILED<<std::endl;return;
        }
    }
    TRAIN ret=released.query(trainID);
    if(ret.type!='n') {std::cout<<FAILED<<std::endl;return;}
    TRAIN ret2=trains.query(trainID);
    if(ret2.type=='n') {std::cout<<FAILED<<std::endl;return;}
    released.insert(trainID,ret);
    sjtu::DATE now(ret.saleDate_l);
    sjtu::vector<int> ini;
    for(int i=0;i<ret.stationNum-1;i++) ini.push_back(ret.seatNum);
    for(;now<ret.saleDate_r||now==ret.saleDate_r;now++) seats.insert(TRAIN_DAY(ret.trainID,now),SEAT(ret.seatNum,ini));
    sjtu::TIME st=ret.startTime;
    for(int i=0;i<ret.stationNum;i++)
    {
        int wait=0;
        if(i&&i<ret.stationNum-1) wait=ret.stopoverTimes[i-1];
        stops.insert(ret.station[i],STOP(ret.trainID,ret.saleDate_l,ret.saleDate_r,st,st+wait,i));
        if(i==ret.stationNum-1) break;
        st+=ret.travelTimes[i]+wait;
    }
    std::cout<<SUCCESS<<std::endl;return;
}
void query_train()
{
    sjtu::string<20> trainID;
    sjtu::DATE day;
    while(sp<s.length())
    {
        switch(get_key())
        {
            case 'i':trainID=get_word();break;
            case 'd':day=sjtu::DATE(get_word());break;
            default: std::cout<<FAILED<<std::endl;return;
        }
    }
    TRAIN ret=trains.query(trainID);
    if(ret.type=='n') {std::cout<<FAILED<<std::endl;return;}
    SEAT ret2=seats.query(TRAIN_DAY(trainID,day));
    if(ret2.seatNum==-1) for(int i=0;i<ret.stationNum-1;i++) ret2.rem.push_back(ret.seatNum);
    std::cout<<trainID<<" "<<ret.type<<std::endl;
    sjtu::DATE_TIME now=sjtu::DATE_TIME(day,ret.startTime);int sumPrice=0;
    for(int i=0;i<ret.stationNum;i++)
    {
        if(i>1) now+=ret.stopoverTimes[i-2];
        std::cout<<ret.station[i]<<" ";
        if(!i) std::cout<<"xx-xx xx:xx ";
        else std::cout<<now<<" ";
        std::cout<<"-> ";
        if(i==ret.stationNum-1) std::cout<<"xx-xx xx:xx ";
        else
        {
            if(i) now+=ret.travelTimes[i-1];
            std::cout<<now<<" ";
        }
        std::cout<<sumPrice<<" ";
        if(i<ret.stationNum-1) sumPrice+=ret.prices[i];
        if(i<ret.stationNum-1) std::cout<<ret2.rem[i]<<std::endl;
        else std::cout<<"x"<<std::endl;
    }
}
void query_ticket()
{
    sjtu::string<30> from,to;
    sjtu::DATE day;
    std::string type="cost";
    while(sp<s.length())
    {
        switch(get_key())
        {
            case 's':from=get_word();break;
            case 't':to=get_word();break;
            case 'd':day=sjtu::DATE(get_word());break;
            case 'p':type=get_word();break;
            default: std::cout<<FAILED<<std::endl;return;
        }
    }
    sjtu::vector<STOP> ret1=stops.query(from),ret2=stops.query(to);
    sjtu::vector<TICKET> tickets; 
    auto j=ret2.begin();
    for(auto i=ret1.begin();i!=ret1.end();i++)
    {
        STOP tmp1=*i;
        while(j!=ret2.end()&&*j<tmp1) j++;
        if(j==ret2.end()) break;
        STOP tmp2=*j;
        if(!(tmp1==tmp2)) continue;
        if(tmp2.stationPlace<tmp1.stationPlace) continue;
        sjtu::DATE_TIME l1(tmp1.saleDate_l,tmp1.leaveTime),r1(tmp1.saleDate_r,tmp1.leaveTime);
        if(day<l1.date||r1.date<day) continue;
        TRAIN tr=released.query(tmp1.trainID);
        int price=tr.get_price(tmp1.stationPlace,tmp2.stationPlace-1);
        sjtu::DATE_TIME init(day,l1.time);
        init+=tr.startTime-tmp1.leaveTime;
        SEAT st=seats.query(TRAIN_DAY(tr.trainID,init.date));
        int seat=st.min(tmp1.stationPlace,tmp2.stationPlace-1);
        tickets.push_back(TICKET(tr.trainID,price,seat,init+(tmp1.leaveTime-tr.startTime),init+(tmp2.arriveTime-tr.startTime)));
    }
    if(type=="cost") std::sort(tickets.begin(),tickets.end(),cmpc);
    else std::sort(tickets.begin(),tickets.end(),cmpt);
    std::cout<<tickets.size()<<std::endl;
    for(auto i=tickets.begin();i!=tickets.end();i++)
    {
        TICKET tmp=*i;
        std::cout<<tmp.trainID<<" "<<from<<" "<<tmp.st<<" -> "<<to<<" "<<tmp.ed<<" "<<tmp.price<<" "<<tmp.seat<<std::endl;
    }
}
void query_transfer()
{
    sjtu::string<30> from,to;
    sjtu::DATE day;
    std::string type="cost";
    while(sp<s.length())
    {
        switch(get_key())
        {
            case 's':from=get_word();break;
            case 't':to=get_word();break;
            case 'd':day=sjtu::DATE(get_word());break;
            case 'p':type=get_word();break;
            default: std::cout<<FAILED<<std::endl;return;
        }
    }

}
void buy_ticket(int timestamp)
{
    sjtu::string<20> username,trainID;
    sjtu::DATE day;
    sjtu::string<30> from,to;
    bool ispend=false;
    int cnt;
    while(sp<s.length())
    {
        switch(get_key())
        {
            case 'u':username=get_word();break;
            case 'i':trainID=get_word();break;
            case 'd':day=sjtu::DATE(get_word());break;
            case 'n':cnt=stoi(get_word());break;
            case 'f':from=get_word();break;
            case 't':to=get_word();break;
            case 'q':if(get_word()=="true") ispend=true;break;
            default: std::cout<<FAILED<<std::endl;return;
        }
    }
    USER ret=login.query(username);
    if(ret.privilege==-1) {std::cout<<FAILED<<std::endl;return;}
    TRAIN train=released.query(trainID);
    if(train.type=='n'||cnt>train.seatNum) {std::cout<<FAILED<<std::endl;return;}
    sjtu::DATE_TIME init(train.saleDate_l,train.startTime),now,st,ed;
    SEAT seat;
    int l=train.stationNum,r=-1,tcnt=0;
    for(int i=0;i<train.stationNum;i++)
    {
        if(i&&i<train.stationNum-1) init+=train.stopoverTimes[i-1],tcnt+=train.stopoverTimes[i-1];
        if(train.station[i]==from)
        {
            l=i;
            st=sjtu::DATE_TIME(day,init.time);
            now=st+(-tcnt);
            seat=seats.query(TRAIN_DAY(trainID,now.date));
        }
        if(train.station[i]==to)
        {
            r=i;
            ed=now+tcnt;
            break;
        }
        if(i<train.stationNum-1) init+=train.travelTimes[i-1],tcnt+=train.travelTimes[i-1];
    }
    if(l>r) {std::cout<<FAILED<<std::endl;return;}
    int rem=seat.min(l,r-1);
    if(rem>=cnt)
    {
        seat.modify(l,r-1,-cnt);
        int price=train.get_price(l,r-1);
        seats.erase(TRAIN_DAY(trainID,now.date));
        seats.insert(TRAIN_DAY(trainID,now.date),seat);
        orders.insert(username,QUERY(timestamp,username,TRAIN_DAY(trainID,now.date),from,to,st,ed,price,cnt,l,r,0));
        std::cout<<price*cnt<<std::endl;
    }
    else
    {
        if(!ispend) {std::cout<<FAILED<<std::endl;return;}
        int price=train.get_price(l,r-1);
        pendings.insert(TRAIN_DAY(trainID,now.date),QUERY(timestamp,username,TRAIN_DAY(trainID,now.date),from,to,st,ed,price,cnt,l,r,1));
        orders.insert(username,QUERY(timestamp,username,TRAIN_DAY(trainID,now.date),from,to,st,ed,price,cnt,l,r,1));
        std::cout<<"queue"<<std::endl;
    }
}
void query_order()
{
    sjtu::string<20> username;
    while(sp<s.length())
    {
        switch(get_key())
        {
            case 'u':username=get_word();break;
            default: std::cout<<FAILED<<std::endl;return;
        }
    }
    USER ret=login.query(username);
    if(ret.privilege==-1) {std::cout<<FAILED<<std::endl;return;}
    sjtu::vector<QUERY> queries=orders.query(username);
    std::cout<<queries.size()<<std::endl;
    for(auto i=queries.begin();i!=queries.end();i++)
    {
        QUERY tmp=*i;
        if(tmp.type==0) std::cout<<"[success] ";
        else if(tmp.type==1) std::cout<<"[pending] ";
        else std::cout<<"[refunded] ";
        std::cout<<tmp.trainDay.trainID<<" "<<tmp.from<<" "<<tmp.st<<" -> "<<tmp.to<<" "<<tmp.ed<<" "<<tmp.price<<" "<<tmp.cnt<<std::endl;
    }
}
void refund_ticket()
{
    sjtu::string<20> username;
    int num=1;
    while(sp<s.length())
    {
        switch(get_key())
        {
            case 'u':username=get_word();break;
            case 'n':num=stoi(get_word());break;
            default: std::cout<<FAILED<<std::endl;return;
        }
    }
    USER ret=login.query(username);
    if(ret.privilege==-1) {std::cout<<FAILED<<std::endl;return;}
    sjtu::vector<QUERY> queries=orders.query(username);
    if(num>queries.size()||queries[num-1].type==2) {std::cout<<FAILED<<std::endl;return;}
    QUERY tmp=queries[num-1];
    if(tmp.type==0)
    {
        SEAT seat=seats.query(tmp.trainDay);
        seat.modify(tmp.l,tmp.r-1,tmp.cnt);
        sjtu::vector<QUERY> pends=pendings.query(tmp.trainDay);
        for(auto i=pends.begin();i!=pends.end();i++)
        {
            QUERY tp=*i;
            int rem=seat.min(tp.l,tp.r-1);
            if(rem>=tp.cnt)
            {
                seat.modify(tp.l,tp.r-1,-tp.cnt);
                pendings.erase(tp.trainDay,tp);
                orders.erase(tp.username,tp);
                tp.type=0;
                orders.insert(tp.username,tp);
            }
        }
        seats.erase(tmp.trainDay);
        seats.insert(tmp.trainDay,seat);
    }
    else pendings.erase(tmp.trainDay,tmp);
    orders.erase(username,tmp);
    tmp.type=2;
    orders.insert(username,tmp);
    std::cout<<SUCCESS<<std::endl;return;
}
void clean()
{
    users.clear();login.clear();
    trains.clear();released.clear();
    seats.clear();stops.clear();
    pendings.clear();orders.clear();
    std::cout<<SUCCESS<<std::endl;return;
}
int main()
{
    freopen("test/1.in","r",stdin);
    freopen("test/1.out","w",stdout);
    std::ios::sync_with_stdio(false);
    std::string timestamp,cmd;
    while(std::getline(std::cin,s))
    {
        sp=0;
        timestamp=get_word();
        std::cout<<timestamp<<" ";
        cmd=get_word();
        if(cmd=="add_user") add_user();
        else if(cmd=="login") log_in();
        else if(cmd=="logout") log_out();
        else if(cmd=="query_profile") query_profile();
        else if(cmd=="modify_profile") modify_profile();
        else if(cmd=="add_train") add_train();
        else if(cmd=="delete_train") delete_train();
        else if(cmd=="release_train") release_train();
        else if(cmd=="query_train") query_train();
        else if(cmd=="query_ticket") query_ticket();
        else if(cmd=="query_transfer") query_transfer();
        else if(cmd=="buy_ticket") buy_ticket(stoi(timestamp.substr(1,timestamp.length()-2)));
        else if(cmd=="query_order") query_order();
        else if(cmd=="refund_ticket") refund_ticket();
        else if(cmd=="clean") clean();
        else if(cmd=="exit")
        {
            std::cout<<"bye"<<std::endl;
            login.clear();
            return 0;
        }
        s.clear();
    }
    return 0;
}