#include<bits/stdc++.h>
using namespace std;

int T,st_lif,city_n,tim,hour,
	summon_perm[2][5]={{3,4,5,2,1},{4,1,2,3,5}};
string typ_nam[9]={"","dragon","ninja","iceman","lion","wolf"},
	weapon_nam[9]={"sword","bomb","arrow"};
void pr_time(int min){ cout<<setw(3)<<setfill('0')<<hour<<':'<<setw(2)<<setfill('0')<<min<<' '; }

class city{
public:
	int lif,win_cnt,flag;
	void init(int _lif){ lif=_lif, flag=win_cnt=0; }
}red,blu,city[44];

class warrior{
public:
	bool keep;
protected:
	int num,hp,atk,pos,dir,typ,t_hp,
		sword_atk,arrow_cnt,bomb_sta;
	static int red_tot,blu_tot;
	void setnum(){ num=dir==1?++red_tot:++blu_tot; }
	void setpos(){ pos=dir==1?0:city_n+1; }
	void rec_hp(){ t_hp=hp; }
	void _init(int _dir,int typ_num,int _hp,int _atk){
		keep=sword_atk=arrow_cnt=bomb_sta=t_hp=0,
		dir=_dir, setnum(), setpos(), typ=typ_num, hp=_hp, atk=_atk; }
	void weap_init(int w_num){
		switch(w_num){
			case 0: sword_atk=atk/5; break;
			case 1: bomb_sta=1; break;
			case 2: arrow_cnt=3; break;
		}
	}
public:
	static int arrow_atk;
	static void initot(){ red_tot=blu_tot=0; }
	virtual void init(int _dir,int cur_lif) =0;
	
	static int gettot(int _dir){ return _dir==1?red_tot:blu_tot; }
	virtual int getsthp() const =0;
	int gethp() const{ return hp; }
	int getnum() const{ return num; }
	int getpos() const{ return pos; }
	int get_sword_atk() const{ return sword_atk; }
	int get_arrow_cnt() const{ return arrow_cnt; }
	int get_bomb_sta() const{ return bomb_sta; }
	string s_dir() const{ return dir==1?"red":"blue"; }
	string s_typ() const{ return typ_nam[typ]; }

	//
	int shoot(warrior *to){
		if(!arrow_cnt)return -1;
		to->t_hp=0, to->hp-=arrow_atk, --arrow_cnt;
		return to->hp<=0;
	}
	int test_bomb(warrior *to){ //to防守，返回1代表to用，-1代表this用
		int t1=hp,t2=to->hp; if(t1<=0||t2<=0) return 0;
		t2-=atk+sword_atk; if(t2<=0) return to->bomb_sta?hp=to->hp=-99,1:0;
		if(to->typ!=2) t1-=to->atk/2+to->sword_atk;
		if(t1<=0) return bomb_sta?hp=to->hp=-99,-1:0; return 0;
	}
	void attack(warrior *to){ rec_hp(), to->rec_hp(), to->hp-=atk+sword_atk, sword_atk=sword_atk*4/5; }
	void counter(warrior *to){ to->hp-=atk/2+sword_atk, sword_atk=sword_atk*4/5; }
	friend void all_fight();

	virtual int las_hp(){ return 0; }
	virtual void yell(){}
	virtual void d_loyal(){}
	virtual void rob(warrior *p){}
	virtual void check_loyal(){ ; }
	void _march();
	virtual void march(){ _march(); }
	void pr_march(){
		pr_time(10);
		if(pos==0){
			cout<<"blue "<<s_typ()<<' '<<num<<" reached red headquarter with "<<hp<<" elements and force "<<atk<<endl;
			if(++red.win_cnt==2) pr_time(10), cout<<"red headquarter was taken"<<endl; }
		else if(pos==city_n+1){
			cout<<"red "<<s_typ()<<' '<<num<<" reached blue headquarter with "<<hp<<" elements and force "<<atk<<endl;
			if(++blu.win_cnt==2) pr_time(10), cout<<"blue headquarter was taken"<<endl; }
		else cout<<s_dir()<<' '<<s_typ()<<' '<<num<<" marched to city "<<pos<<" with "<<hp<<" elements and force "<<atk<<endl; 
	}
	void report(){
		bool ot=false; pr_time(55), cout<<s_dir()<<' '<<s_typ()<<' '<<num<<" has ";
		if(arrow_cnt) cout<<"arrow("<<arrow_cnt<<')', ot=true;
		if(bomb_sta) ot?cout<<',':cout, cout<<"bomb",ot=true;
		if(sword_atk) ot?cout<<',':cout, cout<<"sword("<<sword_atk<<')',ot=true;
		if(!ot) cout<<"no weapon"; cout<<endl;
	}
}*red_map[22], *blu_map[22], *tem_map[22],*Rhome,*Bhome;
void warrior::_march(){
	pos+=dir;
	if(pos<0) Rhome=this, keep=true;
	if(pos>city_n+1) Bhome=this, keep=true;
}

void set_map(warrior **a){
	for(int i=0;i<=city_n+1;i++) tem_map[i]=NULL;
	for(int i=0;i<=city_n+1;i++){
		warrior *p=a[i];
		if(p==NULL||p->keep) continue;
		if(p->getpos()<0||p->gethp()<=0)delete p;
		else tem_map[p->getpos()]=p;
	}
	for(int i=0;i<=city_n+1;i++) a[i]=tem_map[i];
}

bool pr_arrow(warrior *p,warrior *q){
	if(p==NULL||q==NULL) return false;
	int res=p->shoot(q); if(res==-1) return false;
	pr_time(35), cout<<p->s_dir()<<' '<<p->s_typ()<<' '<<p->getnum()<<" shot";
	if(res) cout<<" and killed "<<q->s_dir()<<' '<<q->s_typ()<<' '<<q->getnum();
	return cout<<endl,true;
}
bool pr_bomb(warrior *p,warrior *q){ //p---attack--->q
	if(p==NULL||q==NULL) return false; int res=p->test_bomb(q);
	if(res>0) pr_time(38), cout<<q->s_dir()<<' '<<q->s_typ()<<' '<<q->getnum()<<" used a bomb and killed "<<p->s_dir()<<' '<<p->s_typ()<<' '<<p->getnum()<<endl;
	if(res<0) pr_time(38), cout<<p->s_dir()<<' '<<p->s_typ()<<' '<<p->getnum()<<" used a bomb and killed "<<q->s_dir()<<' '<<q->s_typ()<<' '<<q->getnum()<<endl;
	return res;
}
void all_shoot(){
	for(int i=0;i<=city_n+1;i++){
		if(i+1<city_n+1)pr_arrow(red_map[i],blu_map[i+1]);
		if(i-1>0)pr_arrow(blu_map[i],red_map[i-1]);
	}
}
void all_bomb(){
	for(int i=0;i<=city_n+1;i++){
		warrior *p=blu_map[i],*q=red_map[i];
		if(city[i].flag==1||(!city[i].flag&&i%2)) swap(p,q);
		pr_bomb(p,q);
	}
}
void all_fight(){
	map<int,int>pool;
	for(int i=1,res;i<=city_n;i++){
		warrior *p=blu_map[i],*q=red_map[i];
		if(p==NULL||q==NULL) continue;
		if(city[i].flag==1||(!city[i].flag&&i%2)) swap(p,q);
		if(p->hp>0&&q->hp>0){
			pr_time(40), cout<<p->s_dir()<<' '<<p->s_typ()<<' '<<p->getnum()<<" attacked "<<q->s_dir()<<' '<<q->s_typ()<<' '<<q->getnum()<<" in city "<<i<<" with "<<p->hp<<" elements and force "<<p->atk<<endl, p->attack(q);
			if(q->hp<=0) pr_time(40), cout<<q->s_dir()<<' '<<q->s_typ()<<' '<<q->getnum()<<" was killed in city "<<i<<endl;
			else if(q->typ!=2){
				pr_time(40), cout<<q->s_dir()<<' '<<q->s_typ()<<' '<<q->getnum()<<" fought back against "<<p->s_dir()<<' '<<p->s_typ()<<' '<<p->getnum()<<" in city "<<i<<endl, q->counter(p);
				if(p->hp<=0) pr_time(40), cout<<p->s_dir()<<' '<<p->s_typ()<<' '<<p->getnum()<<" was killed in city "<<i<<endl;
			}
		}
		if(p->hp>0) p->yell();
		if(p->hp>0&&q->hp>0) p->d_loyal(), q->d_loyal(), city[i].win_cnt=0;
		else if(p->hp<=0&&q->hp<=0) p->pos=-99, q->pos=-99;
		else{
			if(p->hp<=0) swap(p,q); p->rob(q), p->hp+=q->las_hp(), q->pos=-99;
			pr_time(40), cout<<p->s_dir()<<' '<<p->s_typ()<<' '<<p->num<<" earned "<<city[i].lif<<" elements for his headquarter"<<endl;
			pool[p->dir]+=city[i].lif, city[i].lif=0;
			if(p->dir==1) city[i].win_cnt=max(city[i].win_cnt,0)+p->dir;
			else city[i].win_cnt=min(city[i].win_cnt,0)+p->dir;
			if(abs(city[i].win_cnt)==2&&city[i].flag!=p->dir)
				city[i].flag=p->dir, pr_time(40), cout<<p->s_dir()<<" flag raised in city "<<i<<endl;
		}
	}
	for(int i=1;i<=city_n&&blu.lif>=8;i++){
		warrior *r=red_map[i], *b=blu_map[i];
		if(b&&r&&b->pos==i&&r->pos==-99) blu.lif-=8, b->hp+=8;
	}
	for(int i=city_n;i>0&&red.lif>=8;i--){
		warrior *r=red_map[i], *b=blu_map[i];
		if(b&&r&&r->pos==i&&b->pos==-99) red.lif-=8, r->hp+=8;
	}
	set_map(blu_map), set_map(red_map),
	blu.lif+=pool[-1], red.lif+=pool[1];
}

class dragon:public warrior{
public:
	static int start_hp,start_atk,typ_num;
	double morale;
	int getsthp() const{ return start_hp; }
	void init(int _dir,int cur_lif){
		_init(_dir,typ_num,start_hp,start_atk);
		weap_init(num%3), morale=1.0*cur_lif/start_hp;
	}
	void yell(){
		if(morale>0.8) pr_time(40), cout<<s_dir()<<' '<<s_typ()<<' '<<num<<" yelled in city "<<pos<<endl;
	}
};
class ninja:public warrior{
public:
	static int start_hp,start_atk,typ_num;
	int getsthp() const{ return start_hp; }
	void init(int _dir,int cur_lif){
		_init(_dir,typ_num,start_hp,start_atk);
		weap_init(num%3), weap_init((num+1)%3);
	}
};
class iceman:public warrior{
	int s_cnt;
public:
	static int start_hp,start_atk,typ_num;
	int getsthp() const{ return start_hp; }
	void init(int _dir,int cur_lif){
		_init(_dir,typ_num,start_hp,start_atk);
		s_cnt=0, weap_init(num%3);
	}
	void march(){ s_cnt?s_cnt=0,hp=max(1,hp-9),atk+=20:s_cnt=1, _march(); }
};
class lion:public warrior{
	int loyal;
public:
	static int start_hp,start_atk,typ_num,loyal_del;
	int getsthp() const{ return start_hp; }
	void init(int _dir,int cur_lif){
		_init(_dir,typ_num,start_hp,start_atk);
		loyal=cur_lif;
	}
	int las_hp(){ return t_hp; }
	void d_loyal(){ loyal-=loyal_del; }
	void check_loyal(){
		if(loyal<=0){
			pos=-99, pr_time(5);
			cout<<s_dir()<<" lion "<<num<<" ran away"<<endl;
		}
	}
};
class wolf:public warrior{
public:
	static int start_hp,start_atk,typ_num;
	int getsthp() const{ return start_hp; }
	void init(int _dir,int cur_lif){
		_init(_dir,typ_num,start_hp,start_atk);
	}
	void rob(warrior *p){
		if(!arrow_cnt)arrow_cnt=p->get_arrow_cnt();
		if(!sword_atk)sword_atk=p->get_sword_atk();
		if(!bomb_sta)bomb_sta=p->get_bomb_sta();
	}
};

int warrior::red_tot=0,warrior::blu_tot=0,warrior::arrow_atk=0,
	dragon::start_atk=0,dragon::start_hp=0,dragon::typ_num=1,
	ninja::start_atk=0,ninja::start_hp=0,ninja::typ_num=2,
	iceman::start_atk=0,iceman::start_hp=0,iceman::typ_num=3,
	lion::start_atk=0,lion::start_hp=0,lion::typ_num=4,lion::loyal_del=0,
	wolf::start_atk=0,wolf::start_hp=0,wolf::typ_num=5;

void init(){
	warrior::initot(), red.init(st_lif), blu.init(st_lif),
	Rhome=Bhome=NULL, hour=0;
	for(int i=0;i<=city_n+1;i++)
		city[i].init(0), red_map[i]=blu_map[i]=NULL;
}

bool summon(int dir,int &lif){
	warrior *p=NULL;
	int cur=warrior::gettot(dir);
	switch (summon_perm[dir==1?0:1][cur%5]){
		case 1: p=new dragon; break;
		case 2: p=new ninja; break;
		case 3: p=new iceman; break;
		case 4: p=new lion; break;
		case 5: p=new wolf; break;
		default: cerr<<"This sentence should not be printed."<<endl;
	}
	if(lif<p->getsthp()){ delete p; return false; }
	else {
		lif-=p->getsthp(), p->init(dir,lif);
		if(dir==1) red_map[0]=p; else blu_map[city_n+1]=p;
		pr_time(0), cout<<p->s_dir()<<' '<<p->s_typ()<<' '<<p->getnum()<<" born"<<endl;
		if(p->s_typ()=="dragon")cout<<"Its morale is "<<fixed<<setprecision(2)<<1.0*lif/p->getsthp()<<endl;
		if(p->s_typ()=="lion")cout<<"Its loyalty is "<<lif<<endl; return true;
	}
}

void check_lion(){
	for(int i=0;i<=city_n+1;i++){
		if(red_map[i]!=NULL) red_map[i]->check_loyal();
		if(blu_map[i]!=NULL) blu_map[i]->check_loyal();
	} set_map(red_map), set_map(blu_map);
}

void all_march(){
	for(int i=0;i<=city_n+1;i++){
		if(red_map[i]!=NULL) red_map[i]->march();
		if(blu_map[i]!=NULL) blu_map[i]->march();
	} set_map(red_map), set_map(blu_map);
	for(int i=0;i<=city_n+1;i++){
		if(red_map[i]!=NULL) red_map[i]->pr_march();
		if(blu_map[i]!=NULL) blu_map[i]->pr_march();
	}
}

bool check_win(){ return red.win_cnt==2||blu.win_cnt==2; }

void all_gain(){
	for(int i=1;i<=city_n;i++){
		city[i].lif+=10;
		warrior *r=red_map[i], *b=blu_map[i];
		if(r&&!b)
			pr_time(30), cout<<"red "<<r->s_typ()<<' '<<r->getnum()<<" earned "<<city[i].lif<<" elements for his headquarter"<<endl, red.lif+=city[i].lif, city[i].lif=0;
		if(b&&!r)
			pr_time(30), cout<<"blue "<<b->s_typ()<<' '<<b->getnum()<<" earned "<<city[i].lif<<" elements for his headquarter"<<endl, blu.lif+=city[i].lif, city[i].lif=0;
	}
}

void home_rep(){
	pr_time(50), cout<<red.lif<<" elements in red headquarter"<<endl;
	pr_time(50), cout<<blu.lif<<" elements in blue headquarter"<<endl;
}

void all_rep(){
	for(int i=0;i<=city_n+1;i++) if(red_map[i]!=NULL) red_map[i]->report(); if(Bhome) Bhome->report();
	if(Rhome) Rhome->report(); for(int i=0;i<=city_n+1;i++) if(blu_map[i]!=NULL) blu_map[i]->report();
}

int main(){
	cin>>T;
	for(int cas=1;cas<=T;cas++){
		cout<<"Case "<<cas<<':'<<endl;
		cin>>st_lif>>city_n>>warrior::arrow_atk>>lion::loyal_del>>tim;
		cin>>dragon::start_hp>>ninja::start_hp>>iceman::start_hp>>lion::start_hp>>wolf::start_hp;
		cin>>dragon::start_atk>>ninja::start_atk>>iceman::start_atk>>lion::start_atk>>wolf::start_atk;
		init();
		while(tim>=0){
			if(tim>=0) summon(1,red.lif),summon(-1,blu.lif); tim-=5;
			if(tim>=0) check_lion(); tim-=5;
			if(tim>=0) all_march(); if(check_win()) tim=-1; tim-=20;
			if(tim>=0) all_gain(); tim-=5;
			if(tim>=0) all_shoot(); tim-=3;
			if(tim>=0) all_bomb(); tim-=2;
			if(tim>=0) all_fight(); tim-=10;
			if(tim>=0) home_rep(); tim-=5;
			if(tim>=0) all_rep(); tim-=5, ++hour;
		}
	}
}
