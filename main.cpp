#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <set>
#include <map>
using namespace std;

string lowerStr(string s){
    transform(s.begin(),s.end(),s.begin(),[](unsigned char c){return tolower(c);});
    return s;
}
void clearIn(){ cin.clear(); cin.ignore(numeric_limits<streamsize>::max(),'\n'); }
vector<string> split(const string& s,char d){
    vector<string> r; string x; stringstream ss(s);
    while(getline(ss,x,d)) r.push_back(x);
    return r;
}
string join(const vector<string>& v,char d=','){
    string r; for(size_t i=0;i<v.size();++i){ if(i) r+=d; r+=v[i]; } return r;
}

struct User{
    string username,password,name,education,experience,interest;
    vector<string> skills;
};
struct Career{
    string name,description;
    vector<string> skills,interests;
};
struct Job{
    int id; string title,company,location,career,experience;
    vector<string> skills;
};
struct Course{
    int id; string title,skill,level;
};
struct Application{
    string username; int jobId; string status;
};

class System{
    vector<User> users;
    vector<Career> careers;
    vector<Job> jobs;
    vector<Course> courses;
    vector<Application> applications;

    template<class T> bool has(const vector<T>&, const string&) const { return false; }

    bool userHas(const User& u,const string& s) const{
        for(auto &x:u.skills) if(lowerStr(x)==lowerStr(s)) return true;
        return false;
    }
    double skillMatch(const User& u,const vector<string>& req) const{
        if(req.empty()) return 0;
        int m=0; for(auto &s:req) if(userHas(u,s)) ++m;
        return 100.0*m/req.size();
    }

public:
    System(){ seed(); loadUsers(); loadApplications(); }

    void seed(){
        careers={
            {"Web Developer","Builds websites and web applications.",
             {"html","css","javascript","sql","git"},{"web development","frontend","backend","website"}},
            {"Software Developer","Designs and develops software applications.",
             {"c++","java","python","oop","sql","git"},{"software development","programming"}},
            {"Data Analyst","Uses data to produce useful insights.",
             {"python","sql","excel","statistics","pandas"},{"data","analytics","statistics"}},
            {"Cyber Security Analyst","Protects systems and networks.",
             {"networking","linux","python","cybersecurity","security"},{"cyber security","security","networking"}},
            {"Database Administrator","Manages databases and data systems.",
             {"sql","mysql","database","backup","security"},{"database","sql","data"}},
            {"Mobile App Developer","Creates mobile applications.",
             {"java","kotlin","flutter","dart","oop"},{"mobile","android","apps"}},
            {"AI / ML Developer","Builds intelligent data-driven systems.",
             {"python","statistics","machine learning","numpy","pandas"},{"ai","artificial intelligence","machine learning","data"}}
        };
        jobs={
            {1,"Junior Web Developer","Tech Solutions","Karachi","Web Developer","Beginner",{"html","css","javascript"}},
            {2,"Front-End Developer","Digital Pakistan","Lahore","Web Developer","Beginner",{"html","css","javascript","git"}},
            {3,"C++ Software Developer","Software House","Islamabad","Software Developer","Beginner",{"c++","oop","git"}},
            {4,"Junior Data Analyst","DataWorks","Karachi","Data Analyst","Beginner",{"python","sql","excel"}},
            {5,"Cyber Security Intern","SecureNet","Islamabad","Cyber Security Analyst","Beginner",{"networking","linux","security"}},
            {6,"Database Assistant","PakTech","Lahore","Database Administrator","Beginner",{"sql","mysql","database"}},
            {7,"AI Intern","FutureAI","Karachi","AI / ML Developer","Beginner",{"python","numpy","pandas"}}
        };
        courses={
            {1,"HTML & CSS Basics","html","Beginner"},{2,"JavaScript Fundamentals","javascript","Beginner"},
            {3,"SQL for Beginners","sql","Beginner"},{4,"Git & GitHub","git","Beginner"},
            {5,"C++ OOP","oop","Intermediate"},{6,"Python Programming","python","Beginner"},
            {7,"Linux Essentials","linux","Beginner"},{8,"Networking Fundamentals","networking","Beginner"},
            {9,"Statistics for Data Analysis","statistics","Intermediate"},{10,"Machine Learning Basics","machine learning","Advanced"},
            {11,"MySQL Database","mysql","Intermediate"},{12,"Cybersecurity Fundamentals","cybersecurity","Intermediate"}
        };
    }

    void loadUsers(){
        ifstream f("data/users.txt"); string line;
        while(getline(f,line)){
            auto a=split(line,'|'); if(a.size()<7) continue;
            User u{a[0],a[1],a[2],a[3],a[4],a[5],split(a[6],',')}; users.push_back(u);
        }
    }
    void saveUsers(){
        ofstream f("data/users.txt");
        for(auto &u:users) f<<u.username<<"|"<<u.password<<"|"<<u.name<<"|"<<u.education<<"|"
            <<u.experience<<"|"<<u.interest<<"|"<<join(u.skills)<<'\n';
    }
    void loadApplications(){
        ifstream f("data/applications.txt"); string line;
        while(getline(f,line)){
            auto a=split(line,'|'); if(a.size()<3) continue;
            applications.push_back({a[0],stoi(a[1]),a[2]});
        }
    }
    void saveApplications(){
        ofstream f("data/applications.txt");
        for(auto &a:applications) f<<a.username<<"|"<<a.jobId<<"|"<<a.status<<'\n';
    }

    void registerUser(){
        User u;
        cout<<"\n--- Registration ---\nUsername: "; cin>>u.username;
        for(auto &x:users) if(x.username==u.username){cout<<"Username already exists.\n";return;}
        cout<<"Password: "; cin>>u.password; clearIn();
        cout<<"Full name: "; getline(cin,u.name);
        cout<<"Education: "; getline(cin,u.education);
        cout<<"Experience (Beginner/Intermediate/Advanced): "; getline(cin,u.experience);
        cout<<"Main interest: "; getline(cin,u.interest);
        int n; cout<<"Number of skills: "; cin>>n; clearIn();
        for(int i=0;i<n;i++){string s; cout<<"Skill "<<i+1<<": ";getline(cin,s);u.skills.push_back(lowerStr(s));}
        users.push_back(u); saveUsers(); cout<<"Registration successful.\n";
    }

    int login(){
        string u,p; cout<<"\nUsername: ";cin>>u;cout<<"Password: ";cin>>p;
        for(int i=0;i<(int)users.size();i++) if(users[i].username==u&&users[i].password==p){cout<<"Login successful.\n";return i;}
        cout<<"Invalid credentials.\n"; return -1;
    }

    vector<pair<string,double>> careerMatches(const User& u) const{
        vector<pair<string,double>> r;
        for(auto &c:careers){
            double score=skillMatch(u,c.skills)*0.7;
            for(auto &i:c.interests) if(lowerStr(u.interest).find(lowerStr(i))!=string::npos){score+=30;break;}
            r.push_back({c.name,score});
        }
        sort(r.begin(),r.end(),[](auto&a,auto&b){return a.second>b.second;});
        return r;
    }

    const Career* findCareer(const string& n) const{
        for(auto &c:careers) if(c.name==n) return &c;
        return nullptr;
    }
    const Job* findJob(int id) const{
        for(auto &j:jobs) if(j.id==id) return &j;
        return nullptr;
    }

    void profile(User& u){
        cout<<"\n--- PROFILE ---\nName: "<<u.name<<"\nEducation: "<<u.education
            <<"\nExperience: "<<u.experience<<"\nInterest: "<<u.interest<<"\nSkills: "<<join(u.skills)<<'\n';
    }

    void recommendations(User& u){
        auto r=careerMatches(u);
        cout<<"\n--- CAREER RECOMMENDATIONS ---\n";
        for(int i=0;i<(int)min<size_t>(5,r.size());i++)
            cout<<i+1<<". "<<r[i].first<<" - "<<fixed<<setprecision(1)<<r[i].second<<"% match\n";
    }

    void skillGap(User& u){
        auto r=careerMatches(u); if(r.empty()) return;
        auto c=findCareer(r[0].first); if(!c) return;
        double ready=skillMatch(u,c->skills);
        cout<<"\n--- SKILL GAP ANALYSIS ---\nBest career: "<<c->name<<"\nReadiness: "<<fixed<<setprecision(1)<<ready<<"%\n";
        cout<<"You have: "; for(auto&s:c->skills) if(userHas(u,s)) cout<<s<<", "; cout<<"\nMissing: ";
        bool any=false; for(auto&s:c->skills) if(!userHas(u,s)){cout<<s<<", ";any=true;} if(!any) cout<<"None";
        cout<<"\n\nLEARNING ROADMAP:\n";
        int step=1;
        for(auto&s:c->skills) if(!userHas(u,s)){
            cout<<step++<<". "<<s;
            for(auto&co:courses) if(lowerStr(co.skill)==lowerStr(s)) cout<<" -> "<<co.title<<" ("<<co.level<<")";
            cout<<'\n';
        }
    }

    void jobsFor(User& u){
        cout<<"\n--- JOB RECOMMENDATIONS ---\n";
        auto r=careerMatches(u); set<string> good;
        for(auto &x:r) if(x.second>=40) good.insert(x.first);
        for(auto &j:jobs) if(good.count(j.career)){
            double s=skillMatch(u,j.skills);
            cout<<j.id<<". "<<j.title<<" | "<<j.company<<" | "<<j.location
                <<" | "<<j.career<<" | "<<fixed<<setprecision(1)<<s<<"% skill match\n";
        }
    }

    void searchJobs(User& u){
        string key; clearIn(); cout<<"Search by title/company/location/skill: ";getline(cin,key);
        cout<<"\n--- SEARCH RESULTS ---\n"; bool found=false;
        for(auto&j:jobs){
            string all=j.title+" "+j.company+" "+j.location+" "+j.career+" "+join(j.skills);
            if(lowerStr(all).find(lowerStr(key))!=string::npos){
                cout<<j.id<<". "<<j.title<<" | "<<j.company<<" | "<<j.location<<" | "<<j.career<<'\n'; found=true;
            }
        }
        if(!found) cout<<"No jobs found.\n";
    }

    void coursesFor(User& u){
        auto r=careerMatches(u); if(r.empty())return;
        auto c=findCareer(r[0].first); cout<<"\n--- COURSES FOR YOUR SKILL GAPS ---\n";
        for(auto&s:c->skills) if(!userHas(u,s))
            for(auto&co:courses) if(lowerStr(co.skill)==lowerStr(s))
                cout<<co.id<<". "<<co.title<<" | Skill: "<<co.skill<<" | "<<co.level<<'\n';
    }

    void apply(User&u){
        jobsFor(u); int id; cout<<"\nEnter Job ID to apply (0 cancel): ";cin>>id;if(id==0)return;
        if(!findJob(id)){cout<<"Invalid job.\n";return;}
        for(auto&a:applications) if(a.username==u.username&&a.jobId==id){cout<<"Already applied.\n";return;}
        applications.push_back({u.username,id,"Applied"});saveApplications();cout<<"Application saved.\n";
    }

    void applicationsFor(User&u){
        cout<<"\n--- MY APPLICATIONS ---\n"; bool f=false;
        for(auto&a:applications) if(a.username==u.username){
            auto*j=findJob(a.jobId); if(j){cout<<j->id<<". "<<j->title<<" | "<<j->company<<" | "<<a.status<<'\n';f=true;}
        }
        if(!f)cout<<"No applications yet.\n";
    }

    void resume(User&u){
        string file="data/"+u.username+"_CV.txt"; ofstream f(file);
        f<<"CURRICULUM VITAE\n=================\nName: "<<u.name<<"\nEducation: "<<u.education
         <<"\nExperience: "<<u.experience<<"\nInterest: "<<u.interest<<"\nSkills: "<<join(u.skills)<<'\n';
        auto r=careerMatches(u); if(!r.empty())f<<"Recommended Career: "<<r[0].first<<" ("<<fixed<<setprecision(1)<<r[0].second<<"%)\n";
        f.close(); cout<<"CV generated: "<<file<<'\n';
    }

    void userMenu(int idx){
        while(true){
            cout<<"\n===== STUDENT DASHBOARD =====\n1 Profile\n2 Career recommendations\n3 Skill-gap + roadmap\n4 Job recommendations\n5 Search jobs\n6 Courses for skill gaps\n7 Apply for job\n8 My applications\n9 Generate CV\n0 Logout\nChoice: ";
            int c;cin>>c;
            switch(c){
                case 1:profile(users[idx]);break; case 2:recommendations(users[idx]);break;
                case 3:skillGap(users[idx]);break; case 4:jobsFor(users[idx]);break;
                case 5:searchJobs(users[idx]);break; case 6:coursesFor(users[idx]);break;
                case 7:apply(users[idx]);break; case 8:applicationsFor(users[idx]);break;
                case 9:resume(users[idx]);break; case 0:return; default:cout<<"Invalid choice.\n";
            }
        }
    }

    void admin(){
        string u,p;cout<<"Admin username: ";cin>>u;cout<<"Admin password: ";cin>>p;
        if(u!="admin"||p!="admin123"){cout<<"Invalid admin login.\n";return;}
        while(true){
            cout<<"\n===== ADMIN PANEL =====\n1 Users\n2 Careers\n3 Jobs\n4 Courses\n5 Update application status\n0 Logout\nChoice: ";
            int c;cin>>c;
            if(c==0)return;
            if(c==1){for(auto&x:users)cout<<x.username<<" | "<<x.name<<" | "<<x.education<<'\n';}
            else if(c==2){for(auto&x:careers)cout<<x.name<<" | "<<join(x.skills)<<'\n';}
            else if(c==3){for(auto&x:jobs)cout<<x.id<<". "<<x.title<<" | "<<x.company<<" | "<<x.location<<'\n';}
            else if(c==4){for(auto&x:courses)cout<<x.id<<". "<<x.title<<" | "<<x.skill<<" | "<<x.level<<'\n';}
            else if(c==5){
                string user;int id;string st;cout<<"Username: ";cin>>user;cout<<"Job ID: ";cin>>id;
                cout<<"Status (Applied/Interview/Selected/Rejected): ";cin>>st;
                bool ok=false;for(auto&a:applications)if(a.username==user&&a.jobId==id){a.status=st;ok=true;}
                saveApplications();cout<<(ok?"Updated.\n":"Application not found.\n");
            } else cout<<"Invalid choice.\n";
        }
    }

    void run(){
        while(true){
            cout<<"\n============================================\n JOB & SKILL RECOMMENDATION SYSTEM\n============================================\n1 Register\n2 Student Login\n3 Admin Login\n0 Exit\nChoice: ";
            int c;cin>>c;
            if(c==0){cout<<"Goodbye!\n";break;}
            if(c==1)registerUser();
            else if(c==2){int i=login();if(i>=0)userMenu(i);}
            else if(c==3)admin();
            else cout<<"Invalid choice.\n";
        }
    }
};

int main(){
    System app;
    app.run();
    return 0;
}
