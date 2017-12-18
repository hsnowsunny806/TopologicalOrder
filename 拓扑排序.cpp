#include"utility.h"
#include "adj_list_dir_graph.h"	
#include "lk_queue.h"			
#include "string.h" 
struct courseType
{
	char No[5];
	char Name[100];
	int shijian;
	int term;
};
template<int Num>
class Arrange
{
	private:
		AdjListDirGraph<courseType>* pGraph;
		int* deg;
		char table[Num+1][11][6][100];
		int Termcourse[Num + 1];
		ifstream* psource;  ofstream * ptarget;
		LinkQueue<int>q[Num+1];
		int Locate(char No[]);
		void Readln();
		void Range(int num, int term, char Name[]);
		bool RangeOne(int &weekDay, int term, char Name[]);
		bool RangeTwo(int &weekDay, int term, char Name[]);
		bool RangeThree(int &weekDay, int term, char Name[]);
		int FindOne(int weekDay, int term);
		int FindTwo(int weekDay, int term);
		int FindThree(int weekDay, int term);
		void Write(char s[],int l);
		char *Chinese(int n, char* s);		
	public:
		Arrange(ifstream*pIFile, ofstream* pOFile);
		virtual~Arrange();
		void Read();
		void TopologicalOrder();
		void Write();
};
template<int Num>  
int Arrange<Num>::Locate(char No[]) 
{ 
	for (int v = 0; v < pGraph->GetVexNum(); v++) 
 	{	 
		courseType course;			
 		pGraph->GetElem(v, course);	 
 		if (strcmp(course.No, No) == 0) 
 		{
			return v;						
 		} 
 	}  
 	return -1;								
} 

template<int Num>  
void Arrange<Num>::Readln()  
{	 
	char ch;								
 	while ((ch = (*psource).peek()) != EOF&&(ch = (*psource).get()) != '\n'); 
} 
template<int Num>  
Arrange<Num>::Arrange(ifstream *psFile, ofstream *ptFile) 
{ 
 	psource = psFile;						
 	ptarget = ptFile;					
 	pGraph = NULL; deg = NULL;						 
 	for (int i = 1; i <= Num; i++) 
 	{	 
 		for (int j = 1; j <= 10; j++) 
		{	
 			for (int k = 1; k <= 5; k++) 
 			{	   
				strcpy(table[i][j][k], "");	
 			} 
 		} 
 	} 
 } 
template<int Num>  
Arrange<Num>::~Arrange() 
{ 
	if (pGraph != NULL) delete pGraph;		
 	if (deg != NULL) delete []deg; 
} 

template<int Num>  
void Arrange<Num>::Range(int num, int term, char Name[]) 
{ 
	int i;
 	static int weekDay = 1;													
 	if (num == 1) 
 	{	
 		if (!RangeOne(weekDay, term, Name)) 
 		{	
 			throw Error("排课失败!");		
 		} 
	} 
 	else if (num % 3 == 0) 
 	{	
 		for ( i = 0; i < num / 3; i++) 
 		{ 
 			if (!RangeThree(weekDay, term, Name)) 
 			{
				throw Error("排课失败!");	
 			} 
		} 
 	} 
 	else if (num % 3 == 1) 
 	{	
		for (i = 0; i < num / 3 - 1; i++) 
 		{ 
			if (!RangeThree(weekDay, term, Name)) 
 			{	
 				throw Error("排课失败!");	
 			} 
 		} 
 		for (i = 0; i < 2; i++) 
		{ 
 			if (!RangeTwo(weekDay, term, Name)) 
 			{	
 				throw Error("排课失败!");
 			} 
 		} 
	} 
 	else 
 	{	
 		for (i = 0; i < num / 3; i++) 
 		{ 
 			if (!RangeThree(weekDay, term, Name)) 
 			{	 
 				throw Error("排课失败!");	
 			} 
 		} 
 		if (!RangeTwo(weekDay, term, Name)) 
 		{	
 			throw Error("排课失败!");		
 		} 
 	} 
} 
 
template<int Num>  
bool Arrange<Num>::RangeOne(int &weekDay, int term, char Name[]) 
{	 
 	for(int k = 1; k <= 5; k++) 
 	{	
 		int i = FindOne(weekDay,term);			 
 		if (i == -1) 
 		{
 			weekDay = (weekDay +2 > 5) ? (weekDay + 2 - 5) : (weekDay + 2);	  			
 		} 
 		else 
 		{	
 			strcpy(table[term][i][weekDay], Name);	 			
 			return true;					
 		} 
	} 
 	return false;								
} 
template<int Num>  
bool Arrange<Num>::RangeTwo(int &weekDay, int term, char Name[]) 
{ 
 	for (int k = 1; k <= 5; k++) 
 	{	
 		int i = FindTwo(weekDay, term);			
		if (i != -1) 
		{	
 			weekDay = (weekDay +2 > 5) ? (weekDay + 2 - 5) : (weekDay + 2);	 
 			strcpy(table[term][i][weekDay], Name); 
 			strcpy(table[term][i + 1][weekDay], Name); 		
			return true;						
		} 
 	}  	 
 	if (!RangeOne(weekDay, term, Name)) return false; 
 	if (!RangeOne(weekDay, term, Name)) return false; 
 	return true;								
} 
  
template<int Num>  
bool Arrange<Num>::RangeThree(int &weekDay, int term, char Name[]) 
{ 
	for (int k = 0; k <= 5; k++) 
	{	
 		int i = FindThree(weekDay, term);
		if (i != -1) 
 		{	 
 			weekDay = (weekDay +2 > 5) ? (weekDay + 2 - 5) : (weekDay + 2);	 
 			strcpy(table[term][i][weekDay], Name); 
 			strcpy(table[term][i + 1][weekDay], Name); 
 			strcpy(table[term][i + 2][weekDay], Name); 
 			return true;				
 		} 
 	} 
 	if (!RangeTwo(weekDay,term,Name)) return false; 
	if (!RangeOne(weekDay,term,Name)) return false; 
 	return true;						
}   
template<int Num>  
int Arrange<Num>::FindOne(int weekDay, int term) 
{ 
 	for(int i = 1; i <= 8; i++) 
 	{
 		if (strlen(table[term][i][weekDay]) == 0) 
 		{	
 			return i;						
		} 
	} 
 	return -1;							
}  
 
template<int Num>  
int Arrange<Num>::FindTwo(int weekDay,int term) 
{ 
 	if (strlen(table[term][1][weekDay]) ==0 && strlen(table[term][2][weekDay]) ==0 ) 
 	{	
		return 1;						
 	} 
 	if (strlen(table[term][3][weekDay]) == 0 && strlen(table[term][4][weekDay]) == 0) 
 	{		 
 		return 3;							
 	} 
 	if (strlen(table[term][6][weekDay]) == 0 && strlen(table[term][7][weekDay]) == 0) 
 	{	
 		return 6;						 
 	} 
 	if (strlen(table[term][8][weekDay]) == 0 && strlen(table[term][9][weekDay]) == 0) 
 	{	
		return 8;							
 	} 
 	return -1;								 
 } 
 
template<int Num>  
int Arrange<Num>::FindThree(int weekDay,int term) 
{ 
 	if (strlen(table[term][3][weekDay]) == 0 && strlen(table[term][4][weekDay]) == 0 && strlen(table[term][5][weekDay]) == 0) 
 	{	
		return 3;						
	} 
	if (strlen(table[term][8][weekDay]) == 0 && strlen(table[term][9][weekDay]) == 0 && strlen(table[term][10][weekDay]) == 0) 
 	{	
 		return 8;						
 	} 
 	return -1;								
} 

template<int Num>  
void Arrange<Num>::Write(char s[], int l) 
{ 
	(*ptarget) << s;						
 	for(unsigned int i = 0; i < l - strlen(s); i++) (*ptarget) << " "; 
}  
template<int Num>  
char *Arrange<Num>::Chinese(int n, char *s) 
{	 
 	char digital[11][3] = {"零", "一", "二", "三", "四", "五", "六", "七", "八", "九", "十"};					 
 	strcpy(s, "");						
 	if (n < 0) 
 	{	
 		strcat(s, "负");				
 		n = -n;							
 	} 
	n = n % 100;							
 	if (n / 10 > 1) 
 	{
 		strcat(s, digital[n / 10]);			
 		strcat(s, digital[10]);				
 		if (n %10 > 0) 
 			strcat(s, digital[n % 10]);		 
 	} 
 	else if (n /10 == 1) 
 	{	
 		strcat(s, digital[10]);				
 		if (n %10 > 0) 	
 			strcat(s, digital[n % 10]);		 
 	} 
 	else 	
 		strcpy(s, digital[n % 10]);			
	return s; 
} 
 
 template<int Num>  
 void Arrange<Num>::Read() 
 {  
 	char ch;								
 	int i, n, v, u, courseTotalNum = 0;						
 	courseType course;							
 	char prioNo[5];						
 	(*psource).seekg(0);					
 	ch = GetChar(*psource);						
 	while (ch < '0' || ch > '9') 
 	{	
 		if (ch != '\n') Readln();			
 		ch = GetChar(*psource);					
 	} 
 	(*psource).putback(ch);					
 	for (i = 1; i <= Num; i++) 
 	{	
 		(*psource) >> n;						
 		Termcourse[i] = n;				
 		courseTotalNum += n;				 
 	} 
 	courseType *vex = new courseType[courseTotalNum];		 
 	deg = new int[courseTotalNum];						 
 	for (i = 0; i < courseTotalNum; i++) deg[i] = 0;		 
 	(*psource).seekg(0);					 
 	ch = GetChar(*psource);					
 	int count = 0;								
 	while (ch != EOF) 
 	{
 		while (ch != EOF && ch != 'c') 
 		{	
 			if(ch != '\n') Readln();			
 			ch = GetChar(*psource);				
 		} 
 		if (ch != EOF) 
 		{	
 			(*psource).putback(ch);			
 			(*psource) >> course.No;		
 			(*psource) >> course.Name;	 
 			(*psource) >> course.shijian;	
 			(*psource) >> course.term;		
 			vex[count++] = course;				
 			Readln();						
 		} 
 		ch = GetChar(*psource);				
 	} 
 	if (count != courseTotalNum) throw Error("课程数与设置的课程总数不等!");
 	pGraph = new AdjListDirGraph<courseType>(vex, courseTotalNum);	
	(*psource).seekg(0); 
 	ch = GetChar(*psource); 
 	while (ch != EOF) 
 	{	
 		while (ch != EOF && ch != 'c') 
 		{	
 			if (ch != '\n') Readln();		
 			ch = GetChar(*psource);				
 		} 
 		if (ch != EOF) 
 		{	
 			(*psource).putback(ch);				
 			(*psource) >> course.No;		
 			(*psource) >> course.Name;	
 			(*psource) >> course.shijian;		
 			(*psource) >> course.term;			
 			v = Locate(course.No);	
 			if (v == -1) throw Error("非法");	
 			ch = GetChar(*psource);				
 			while (ch == 'c') 
 			{	
 				(*psource).putback(ch);			
 				(*psource) >> prioNo;		
 				u = Locate(prioNo);
 				if (u == -1)  throw Error("非法");	
 				pGraph->InsertEdge(u, v);		
 				deg[v]++;					 
 				ch = GetChar(*psource); 
 			} 
 		} 
 		if (ch != '\n') Readln();				
 		ch = GetChar(*psource);				
 	} 
 } 

template<int Num>
void Arrange<Num>::TopologicalOrder()
{
	courseType cv,cw;
	int i,size0,sizeTerm,term,v,w;
	for(v = 0;v<pGraph->GetVexNum();v++)
	{
		if(deg[v] == 0)
		{
			pGraph->GetElem(v, cv);
			q[cv.term].InQueue(v);
		}
	}
	for(term = 1; term <= Num;term++)
	{
		size0 = q[0].Length();
		sizeTerm = q[term].Length();
		if(size0+sizeTerm<Termcourse[term]||sizeTerm>Termcourse[term])
			throw Error("排课冲突!");
		for(i = 1;i <=sizeTerm;i++)
		{
			q[term].OutQueue(v);
			pGraph->GetElem(v,cv);
			Range(cv.shijian,term,cv.Name);
			for(w = pGraph->FirstAdjVex(v);w != -1;w = pGraph->NextAdjVex(v,w))
			{
				deg[w]--;
				pGraph->GetElem(w,cw);
				if(deg[w] == 0&&(cw.term == 0||cw.term>term))
				{
					q[cw.term].InQueue(w);
				}
				else if(deg[w]==0&&(cw.term>0&&cw.term<=term))
					throw Error("排课冲突!"); 
			} 
		}
		for(i = 1;i<= Termcourse[term]-sizeTerm;i++)
		{
			q[0].OutQueue(v);
			pGraph->GetElem(v,cv);
			Range(cv.shijian,term,cv.Name);
			for(w=pGraph->FirstAdjVex(v);w!=-1;w=pGraph->NextAdjVex(v,w)) 
			{
				deg[w]--;
				pGraph->GetElem(w,cw);
				if(deg[w] == 0&&(cw.term == 0||cw.term>term))
				{
					q[cw.term].InQueue(w);
				}
				else if(deg[w]==0&&(cw.term>0&&cw.term<=term))
					throw Error("排课冲突！"); 
			}
		} 
	}
} 
template<int Num>  
void Arrange<Num>::Write() 
 { 
 	int term, i, weekDay;				
 	char s[20], strTem[20];					
 	for (term = 1; term <= Num; term++) 
 	{	
 		(*ptarget) << "第" << Chinese(term, strTem) << "学期课表" << endl;	
 		(*ptarget) << endl;					 
 		Write("节次", 8); 
 		for (i = 1; i <= 5; i++) 
 		{	 
 			Chinese(i, strTem);				
 			Write(strcat(strcpy(s, "星期"), strTem), 16);	
 		} 
 		(*ptarget) << endl << endl;				 
 		for (i = 1; i <= 10; i++) 
 		{	 
 			Chinese(i, strTem);			
 			Write(strcat(strcat(strcpy(s, "第"), strTem), "节"), 8);	
 			for (weekDay = 1; weekDay <= 5; weekDay++) 
 			{	
 				Write(table[term][i][weekDay], 16);
 			} 
 			(*ptarget)<<endl;				
 
 			if (i==2 || i == 7) 
 			{
 				(*ptarget) << endl << "课间休息" << endl << endl; 
 			} 
 			if (i == 5) 
 			{	
 				(*ptarget) << endl << endl << "午间休息" << endl << endl << endl; 
 			} 
 			if (i == 10) 
 			{	
 				(*ptarget) << endl << "晚自习" << endl << endl; 
 			} 
 		} 
 		(*ptarget) << endl << endl << endl; 
 	} 
 }  
 int main() 
 { 
 		char *sourcefile = "course_inf.txt", *targetfile = "curriculum.txt"; 		 		
 		ifstream source(sourcefile);						   			
 		ofstream target(targetfile);					
 		cout << "课程信息文件为: " << sourcefile << endl;  
 		Arrange<8> a(&source, &target);					
 		a.Read();								
 		a.TopologicalOrder();				
 		a.Write();								 
 		cout<<"课表文件为: "<<targetfile<<endl; 			
 	return 0;						
 } 

 

