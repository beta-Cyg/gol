#include <cstdio>
#include <vector>
#include <span>
#include <cstring>
#include <graphics.h>

class gol{
public:
	class cell{
	public:
		int x,y;
		bool state;
	};
private:
	std::vector<std::vector<bool>> field;
	int len,wid;
	static char null[2];
public:
	gol()=default;

	int count_around(int x,int y)const{
		int cnt=0;
		if(x<0 or y<0 or x>=wid or y>=len)return cnt;
		if(x>0){
			if(y>0 and field[x-1][y-1])cnt++;
			if(field[x-1][y])cnt++;
			if(y<len-1 and field[x-1][y+1])cnt++;
		}
		if(y>0 and field[x][y-1])cnt++;
		if(y<len-1 and field[x][y+1])cnt++;
		if(x<wid-1){
			if(y>0 and field[x+1][y-1])cnt++;
			if(field[x+1][y])cnt++;
			if(y<len-1 and field[x+1][y+1])cnt++;
		}
		return cnt;
	}

	cell next(int x,int y){
		int count=count_around(x,y);
		if(field[x][y]==false and count==3)return cell{x,y,true};
		if(field[x][y]==true){
			if(count<2)return cell{x,y,false};
			if(count==2 or count==3)return cell{x,y,field[x][y]};
			if(count>3)return cell{x,y,false};
		}
		return cell{};
	}

	void step(){
		std::vector<cell>changes;
		for(int i=0;i<wid;i++)
			for(int j=0;j<len;j++){
				cell change=next(i,j);
				if(change.state!=field[i][j])
					changes.push_back(change);
			}
		for(auto i:changes)
			field[i.x][i.y]=i.state;
	}

	void set(int x,int y,bool state){
		field[x][y]=state;
	}

	void set(const cell& c){
		field[c.x][c.y]=c.state;
	}

	void set(std::span<cell> cells){
		for(auto i:cells)
			field[i.x][i.y]=i.state;
	}

	void clear(){
		for(int i=0;i<wid;i++)
			for(int j=0;j<len;j++)
				field[i][j]=false;
	}

	void print(const char* before=null,const char* after=null)const{
		std::printf(before);
		for(int i=0;i<wid;i++){
			for(int j=0;j<len;j++)
				std::putchar(field[i][j]?'#':'.');
			std::putchar('\n');
		}
		std::printf(after);
	}

	void display()const{
		for(int i=0;i<wid;i++)
			for(int j=0;j<len;j++)
				if(field[i][j])
					ege::bar(j*10,i*10,(j+1)*10,(i+1)*10);
	}

	int import_seed(const char* file_name){
		FILE* ptr=std::fopen(file_name,"r");
		int l,w;
		int tmp;
		if((tmp=std::fscanf(ptr,"%d %d",&l,&w))==EOF)return tmp;
		len=l;
		wid=w;
		field.resize(w);
		for(auto& i:field)
			i.resize(l);
		clear();
		int x,y;
		while(std::fscanf(ptr,"%d %d",&x,&y)!=EOF)
			if(x>=wid or y>=len or x<0 or y<0){
				std::fprintf(stderr,"wrong seed\n");
				return 1;
			}
			else
				field[y][x]=true;
		return std::fclose(ptr);
	}

	int export_seed(const char* file_name){
		FILE* ptr=std::fopen(file_name,"w");
		std::fprintf(ptr,"%d %d\n",len,wid);
		for(int i=0;i<wid;i++)
			for(int j=0;j<len;j++)
				if(field[i][j])
					std::fprintf(ptr,"%d %d\n",j,i);
		return std::fclose(ptr);
	}

	int length()const{
		return len;
	}

	int width()const{
		return wid;
	}
};

char gol::null[2]{""};
char input[64];
gol test;

int main(int argc,char** argv){
	if(argc!=2)return 1;
	int import_result;
	if(import_result=test.import_seed(argv[1]))
		return import_result;
	
	ege::initgraph(test.length()*10,test.width()*10);
	ege::setfillcolor(EGERGB(0xFF,0xFF,0xFF));
	
	bool isbeg=true;
	bool isrun=false;
	int fps;

	for(;ege::is_run();ege::delay_fps(10)){
		bool isplay=false;
		if(isbeg){
			test.display();
			isbeg=false;
		}
		std::scanf("%s",input);
		if(std::strcmp(input,"step")==0){
			test.step();
		}
		else if(std::strcmp(input,"play")==0){
			int n,v;
			std::scanf("%d %d",&n,&v);
			for(int i=0;i<n and ege::is_run();i++,ege::delay_fps(v)){
				test.step();
				ege::cleardevice();
				test.display();
			}
			isplay=true;
		}
		else if(std::strcmp(input,"run")==0){
			std::scanf("%d",&fps);
			isrun=true;
			break;
		}
		else if(std::strcmp(input,"exit")==0)
			break;
		else if(std::strcmp(input,"set")==0){
			int x,y,s;
			std::scanf("%d %d %d",&x,&y,&s);
			if(x>=test.width() or y>=test.length() or x<0 or y<0)
				std::fprintf(stderr,"not in the range\n");
			else
				test.set(y,x,s);
		}
		else if(std::strcmp(input,"export")==0){
			char fn[32];
			std::scanf("%s",fn);
			int export_result=test.export_seed(fn);
			if(export_result)
				std::fprintf(stderr,"failed to save\n");
			else
				std::printf("successfully saved\n");
		}
		else
			std::printf("undefined order\n");
		if(isplay){
			isplay=false;
			continue;
		}
		ege::cleardevice();
		test.display();
	}
	if(isrun)
		for(;ege::is_run();ege::delay_fps(fps)){
			ege::cleardevice();
			test.step();
			test.display();
		}

	ege::closegraph();

	return 0;
}
