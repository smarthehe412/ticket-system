#include "bpt/bpt.cpp"
#include "string.cpp"
#include <cstring>
#include <iostream>
#include <string>

const int SUCCESS=0,FAILED=-1;

class user
{
public:
    sjtu::string<20> username;
    sjtu::string<30> password,mailAddr;
    sjtu::string<20> name;
    int privilege;
    user(){privilege=-1;}
    user(sjtu::string<20> username,sjtu::string<30> password,sjtu::string<20> name,sjtu::string<30> mailAddr,int privilege):
        username(username),password(password),name(name),mailAddr(mailAddr),privilege(privilege) {}
    void print() {std::cout<<username<<" "<<name<<" "<<mailAddr<<" "<<privilege<<std::endl;}
    bool operator < (const user &a) const {return username<a.username;}
    bool operator == (const user &a) const {return username==a.username;}
};
sjtu::BPT<sjtu::string<20>,user,25,256> users("users_init","users_data");
sjtu::BPT<sjtu::string<20>,user,25,256> login("login_init","login_data");
typedef sjtu::pair<sjtu::string<20>,user> psu;
std::string s;
int sp;
std::string get_word()
{
    while(s[sp]==' ') sp++;
    int st=sp;
    while(s[sp]!=' '&&sp<s.length()) sp++;
    return s.substr(st,sp-st);
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
        users.insert(psu(username,user(username,password,name,mailAddr,privilege)));
        std::cout<<SUCCESS<<std::endl;return;
    }
    sjtu::vector<psu> ret=login.get_all(cur_username);
    if(ret.empty()) {std::cout<<FAILED<<std::endl;return;}
    sjtu::vector<psu> ret2=users.get_all(username);
    if(!ret2.empty()) {std::cout<<FAILED<<std::endl;return;}
    if(ret[0].second.privilege<=privilege) {std::cout<<FAILED<<std::endl;return;}
    users.insert(psu(username,user(username,password,name,mailAddr,privilege)));
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
    sjtu::vector<psu> ret=users.get_all(username);
    if(ret.empty()) {std::cout<<FAILED<<std::endl;return;}
    if(ret[0].second.password!=password) {std::cout<<FAILED<<std::endl;return;}
    sjtu::vector<psu> ret2=login.get_all(username);
    if(!ret2.empty()) {std::cout<<FAILED<<std::endl;return;}
    login.insert(ret[0]);
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
    sjtu::vector<psu> ret=login.get_all(username);
    if(ret.empty()) {std::cout<<FAILED<<std::endl;return;}
    login.erase(ret[0]);
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
    sjtu::vector<psu> ret=login.get_all(cur_username);
    if(ret.empty()) {std::cout<<FAILED<<std::endl;return;}
    if(cur_username==username) {ret[0].second.print();return;}
    sjtu::vector<psu> ret2=users.get_all(username);
    if(ret2.empty()) {std::cout<<FAILED<<std::endl;return;}
    if(ret[0].second.privilege<=ret2[0].second.privilege) {std::cout<<FAILED<<std::endl;return;}
    ret2[0].second.print();
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
    sjtu::vector<psu> ret=login.get_all(cur_username);
    if(ret.empty()) {std::cout<<FAILED<<std::endl;return;}
    if(ret[0].second.privilege<=privilege) {std::cout<<FAILED<<std::endl;return;}
    if(cur_username==username)
    {
        users.erase(ret[0]);
        login.erase(ret[0]);
        if(password.length()) ret[0].second.password=password;
        if(name.length()) ret[0].second.name=name;
        if(mailAddr.length()) ret[0].second.mailAddr=mailAddr;
        if(privilege!=-1) ret[0].second.privilege=privilege;
        users.insert(ret[0]);
        login.insert(ret[0]);
        return;
    }
    sjtu::vector<psu> ret2=users.get_all(username);
    if(ret2.empty()) {std::cout<<FAILED<<std::endl;return;}
    if(ret[0].second.privilege<=ret2[0].second.privilege) {std::cout<<FAILED<<std::endl;return;}
    sjtu::vector<psu> ret3=login.get_all(username);
    users.erase(ret2[0]);
    if(!ret3.empty()) login.erase(ret2[0]);
    if(password.length()) ret2[0].second.password=password;
    if(name.length()) ret2[0].second.name=name;
    if(mailAddr.length()) ret2[0].second.mailAddr=mailAddr;
    if(privilege!=-1) ret2[0].second.privilege=privilege;
    users.insert(ret2[0]);
    if(!ret3.empty()) login.erase(ret2[0]);
    ret2[0].second.print();
    return;
}
void clean()
{
    users.clear();
    login.clear();
    std::cout<<SUCCESS<<std::endl;return;
}
int main()
{
    freopen("test/1.in","r",stdin);
    freopen("test/1.out","w",stdout);
    std::ios::sync_with_stdio(false);
    sjtu::string<30> timestamp,cmd;
    while(std::getline(std::cin,s))
    {
        sp=0;
        timestamp=get_word();
        std::cout<<timestamp<<" ";
        if(timestamp=="[108]")
        {
            int debug=0;
        }
        cmd=get_word();
        if(cmd=="add_user") add_user();
        else if(cmd=="login") log_in();
        else if(cmd=="logout") log_out();
        else if(cmd=="query_profile") query_profile();
        else if(cmd=="modify_profile") modify_profile();
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