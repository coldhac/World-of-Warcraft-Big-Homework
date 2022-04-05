#include<bits/stdc++.h>
using namespace std;

int T,st_lif,red_lif,blu_lif,city_n,tim,hour,
	summon_perm[2][5]={{3,4,5,2,1},{4,1,2,3,5}};
string typ_nam[9]={"","dragon","ninja","iceman","lion","wolf"},
	weapon_nam[9]={"sword","bomb","arrow"};
string dir_nam(int dir){ return dir==1?"red":"blue"; }
void pr_time(int min){ cout<<setw(3)<<setfill('0')<<hour<<':'<<setw(2)<<setfill('0')<<min<<' '; }

class warrior{
protected:
	int num,hp,atk,pos,dir,typ,
		weapon[4];//0=sword, 1=bomb, 2=arrow_new, 3=arrow_used
	queue<int>weapon_q;
	static int red_tot,blu_tot;
public:
	static void initot(){ red_tot=blu_tot=0; }
	void setnum(){ num=dir==1?++red_tot:++blu_tot; }
	void setpos(){ pos=dir==1?0:city_n+1; }
	void _init(int _dir,int typ_num,int _hp,int _atk){
		memset(weapon,0,sizeof weapon),
		dir=_dir, setnum(), setpos(), typ=typ_num, hp=_hp, atk=_atk; }
	virtual void init(int _dir,int cur_lif){ cerr<<"This sentence should not be printed."<<endl; }
	
	static int gettot(int _dir){ return _dir==1?red_tot:blu_tot; }
	virtual int getsthp() const{ cerr<<"This sentence should not be printed."<<endl; return 0; }
	int gettyp() const{ return typ; } int getnum() const{ return num; } int getpos() const{ return pos; }
	int sum_weapon(){ return weapon[0]+weapon[1]+weapon[2]+weapon[3]; }

	void q_init(){
		static int p[4]={0,1,3,2};
		while(!weapon_q.empty()) weapon_q.pop();
		for(int i=0;i<4;i++)
			for(int j=0;j<weapon[p[i]];j++)
				weapon_q.push(p[i]); }
	virtual void hurt_sword(warrior *user){ hp-=user->atk/5; }
	virtual void hurt_bomb(warrior *user){ hp-=user->atk*2/5; }
	virtual void hurt_arrow(warrior *user){ hp-=user->atk*3/10; }
	virtual void use_sword(){ weapon_q.push(0); }
	virtual void use_bomb(){ hp-=atk*2/5/2, --weapon[1]; }
	virtual void use_arrow_new(){ --weapon[2], ++weapon[3], weapon_q.push(3); }
	virtual void use_arrow_used(){ --weapon[3]; }
	friend bool attack(warrior *,warrior *);
	friend bool check_win(warrior *,warrior *);
	friend bool check_draw(warrior *,warrior *);
	friend void capture(warrior *,warrior *);
	friend void fight(warrior *,warrior *);

	virtual void yell(){  }
	virtual void check_loyal(){ ; }
	friend void rob(warrior *p,warrior *q);
	friend void pr_rob(warrior *p,warrior *q,int w_typ,int cnt);
	virtual void march(){ pos+=dir; }
	void pr_march(){
		pr_time(10);
		if(pos==0||pos==city_n+1)
			cout<<dir_nam(dir)<<' '<<typ_nam[typ]<<' '<<num<<" reached "<<dir_nam(-dir)<<" headquarter with "<<hp
			<<" elements and force "<<atk<<endl, pr_time(10), cout<<dir_nam(-dir)<<" headquarter was taken"<<endl;
		else cout<<dir_nam(dir)<<' '<<typ_nam[typ]<<' '<<num<<" marched to city "<<pos<<" with "<<hp
			<<" elements and force "<<atk<<endl; 
	}
	void report(){
		pr_time(55), cout<<dir_nam(dir)<<' '<<typ_nam[typ]<<' '<<num<<" has "<<weapon[0]<<" sword "
			<<weapon[1]<<" bomb "<<weapon[2]+weapon[3]<<" arrow and "<<hp<<" elements"<<endl;
	}
};

void pr_rob(warrior *p,warrior *q,int w_typ,int cnt){
	pr_time(35), cout<<dir_nam(p->dir)<<' '<<typ_nam[p->typ]<<' '<<p->num<<" took "<<cnt<<' '
		<<weapon_nam[w_typ]<<" from "<<dir_nam(q->dir)<<' '<<typ_nam[q->typ]<<' '<<q->num
		<<" in city "<<p->pos<<endl;
}
void rob(warrior *p,warrior *q){
	if(p==NULL||q==NULL) return;
	if((p->typ==5)^(q->typ==5)==false) return;
	if(q->typ==5) swap(p,q); //p ---rob--> q
	int slot=10-p->sum_weapon(), d=0, e=0;
	if(q->weapon[0])
		d=min(slot,q->weapon[0]), p->weapon[0]+=d, q->weapon[0]-=d, pr_rob(p,q,0,d);
	else if(q->weapon[1])
		d=min(slot,q->weapon[1]), p->weapon[1]+=d, q->weapon[1]-=d, pr_rob(p,q,1,d);
	else if(q->weapon[2]+q->weapon[3])
		d=min(slot,q->weapon[2]), p->weapon[2]+=d, q->weapon[2]-=d, slot-=d,
		e=min(slot,q->weapon[3]), p->weapon[3]+=e, q->weapon[3]-=e, pr_rob(p,q,2,d+e);
}

bool attack(warrior *p,warrior *q){ //p ---attack--> q
	if(p->weapon_q.empty()) return false;
	int w=p->weapon_q.front(); p->weapon_q.pop();
	switch (w){
		case 0: p->use_sword(),q->hurt_sword(p); break;
		case 1: p->use_bomb(),q->hurt_bomb(p); break;
		case 2: p->use_arrow_new(),q->hurt_arrow(p); break;
		case 3: p->use_arrow_used(),q->hurt_arrow(p); break;
		default: cerr<<"This sentence should not be printed."<<endl;
	}
	return true;
}
bool check_win(warrior *p,warrior *q){ return p->hp<=0||q->hp<=0; }
bool check_draw(warrior *p,warrior *q){
	if(p->weapon_q.empty()&&q->weapon_q.empty()) return true;
	if(p->sum_weapon()==p->weapon[0]&&p->atk/5==0&&q->sum_weapon()==q->weapon[0]&&q->atk/5==0) return true;
	return false;
}
void capture(warrior *p,warrior *q){
	int slot=10-p->sum_weapon(), d=0;
	d=min(slot,q->weapon[0]), p->weapon[0]+=d, q->weapon[0]-=d, slot-=d,
	d=min(slot,q->weapon[1]), p->weapon[1]+=d, q->weapon[1]-=d, slot-=d,
	d=min(slot,q->weapon[2]), p->weapon[2]+=d, q->weapon[2]-=d, slot-=d,
	d=min(slot,q->weapon[3]), p->weapon[3]+=d, q->weapon[3]-=d, slot-=d;
}
void fight(warrior *red,warrior *blu){
	if(red==NULL||blu==NULL) return;
	int cur=red->pos%2==0;
	red->q_init(), blu->q_init();
	while(!check_win(red,blu)){
		if(cur) attack(blu,red);
		else attack(red,blu); cur^=1;
		if(check_draw(red,blu))break;
	}
	pr_time(40);
	if(red->hp>0){
		if(blu->hp>0) cout<<"both red "<<typ_nam[red->typ]<<' '<<red->num<<" and blue "<<typ_nam[blu->typ]
		<<' '<<blu->num<<" were alive in city "<<red->pos<<endl;
		else cout<<"red "<<typ_nam[red->typ]<<' '<<red->num<<" killed blue "<<typ_nam[blu->typ]
		<<' '<<blu->num<<" in city "<<red->pos<<" remaining "<<red->hp<<" elements"<<endl, capture(red,blu);
	}
	else{
		if(blu->hp>0) cout<<"blue "<<typ_nam[blu->typ]<<' '<<blu->num<<" killed red "<<typ_nam[red->typ]
		<<' '<<red->num<<" in city "<<red->pos<<" remaining "<<blu->hp<<" elements"<<endl, capture(blu,red);
		else cout<<"both red "<<typ_nam[red->typ]<<' '<<red->num<<" and blue "<<typ_nam[blu->typ]
		<<' '<<blu->num<<" died in city "<<red->pos<<endl;
	}
	if(red->hp<=0) red->pos=-99; else red->yell();
	if(blu->hp<=0) blu->pos=-99; else blu->yell();
}

class dragon:public warrior{
public:
	static int start_hp,start_atk,typ_num;
	int getsthp() const{ return start_hp; }
	void init(int _dir,int cur_lif){
		_init(_dir,typ_num,start_hp,start_atk);
		++weapon[num%3];
	}
	void yell(){
		pr_time(40), cout<<dir_nam(dir)<<' '<<typ_nam[typ]<<' '<<num<<" yelled in city "<<pos<<endl;
	}
};
class ninja:public warrior{
public:
	static int start_hp,start_atk,typ_num;
	int getsthp() const{ return start_hp; }
	void init(int _dir,int cur_lif){
		_init(_dir,typ_num,start_hp,start_atk);
		++weapon[num%3],++weapon[(num+1)%3];
	}
	void use_bomb(){ --weapon[1]; }
};
class iceman:public warrior{
public:
	static int start_hp,start_atk,typ_num;
	int getsthp() const{ return start_hp; }
	void init(int _dir,int cur_lif){
		_init(_dir,typ_num,start_hp,start_atk);
		++weapon[num%3];
	}
	void march(){ hp-=hp/10, pos+=dir; }
};
class lion:public warrior{
	int loyal;
public:
	static int start_hp,start_atk,typ_num,loyal_del;
	int getsthp() const{ return start_hp; }
	void init(int _dir,int cur_lif){
		_init(_dir,typ_num,start_hp,start_atk);
		++weapon[num%3],loyal=cur_lif;
	}
	void check_loyal(){
		if(loyal<=0){
			pos=-99, pr_time(5);
			cout<<dir_nam(dir)<<" lion "<<num<<" ran away"<<endl;
		}
	}
	void march(){ loyal-=loyal_del, pos+=dir; }
};
class wolf:public warrior{
public:
	static int start_hp,start_atk,typ_num;
	int getsthp() const{ return start_hp; }
	void init(int _dir,int cur_lif){
		_init(_dir,typ_num,start_hp,start_atk);
	}
};

int warrior::red_tot=0,warrior::blu_tot=0,
	dragon::start_atk=0,dragon::start_hp=0,dragon::typ_num=1,
	ninja::start_atk=0,ninja::start_hp=0,ninja::typ_num=2,
	iceman::start_atk=0,iceman::start_hp=0,iceman::typ_num=3,
	lion::start_atk=0,lion::start_hp=0,lion::typ_num=4,lion::loyal_del=0,
	wolf::start_atk=0,wolf::start_hp=0,wolf::typ_num=5;

warrior *red_map[22], *blu_map[22], *tem_map[22];

void init(){
	warrior::initot(),red_lif=blu_lif=st_lif,hour=0;
	for(int i=0;i<=city_n+1;i++)
		red_map[i]=blu_map[i]=NULL;
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
		pr_time(0), cout<<dir_nam(dir)<<' '<<typ_nam[p->gettyp()]<<' '<<p->getnum()<<" born"<<endl;
		if(p->gettyp()==4)cout<<"Its loyalty is "<<lif<<endl; return true;
	}
}

void set_map(warrior **a){
	for(int i=0;i<=city_n+1;i++) tem_map[i]=NULL;
	for(int i=0;i<=city_n+1;i++){
		warrior *p=a[i];
		if(p==NULL) continue;
		if(p->getpos()<0)delete p;
		else tem_map[p->getpos()]=p;
	}
	for(int i=0;i<=city_n+1;i++) a[i]=tem_map[i];
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

void wolf_rob(){ for(int i=0;i<=city_n+1;i++) rob(red_map[i],blu_map[i]); }

void all_fight(){
	for(int i=0;i<=city_n+1;i++)
		fight(red_map[i],blu_map[i]);
	set_map(red_map), set_map(blu_map);
}

bool check_home(){ return red_map[city_n+1]||blu_map[0]; }

void home_rep(){
	pr_time(50), cout<<red_lif<<" elements in red headquarter"<<endl;
	pr_time(50), cout<<blu_lif<<" elements in blue headquarter"<<endl;
}

void all_rep(){
	for(int i=0;i<=city_n+1;i++){
		if(red_map[i]!=NULL) red_map[i]->report();
		if(blu_map[i]!=NULL) blu_map[i]->report();
	}
}

int main(){
	//freopen("wow3.in","r",stdin),
	//freopen("wow3.out","w",stdout);
	cin>>T;
	for(int cas=1;cas<=T;cas++){
		cout<<"Case "<<cas<<':'<<endl;
		cin>>st_lif>>city_n>>lion::loyal_del>>tim;
		cin>>dragon::start_hp>>ninja::start_hp>>iceman::start_hp>>lion::start_hp>>wolf::start_hp;
		cin>>dragon::start_atk>>ninja::start_atk>>iceman::start_atk>>lion::start_atk>>wolf::start_atk;
		init();
		while(tim>=0){
			if(tim>=0) summon(1,red_lif),summon(-1,blu_lif); tim-=5;
			if(tim>=0) check_lion(); tim-=5;
			if(tim>=0) all_march(); if(check_home()) tim=-1; tim-=25;
			if(tim>=0) wolf_rob(); tim-=5;
			if(tim>=0) all_fight(); tim-=10;
			if(tim>=0) home_rep(); tim-=5;
			if(tim>=0) all_rep(); tim-=5, ++hour;
		}
	}
}
