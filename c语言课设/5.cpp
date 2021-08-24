#include<bits/stdc++.h> 
#define MAX 256
//Node�� Book���ڼ�¼ ͼ����е��飬Book�������ƣ�Node����ID�Ͷ�����Ϣ 
typedef struct Node {
    int data;			//��� 
    char readerName[MAX];//��ǰ���Ķ��ߵ����� 
    struct Node* next;
}Node;
typedef struct Book {
    int inNum;      //�ݴ�����
    int outNum;		//������� 
    char name[MAX]; //����
    Node* id;  		//���
    struct Book* next;
}Book;
//Readers: name����������֣�
//allBooks��������ÿ��Book�ڵ㣬inNum,outNumΪ������Ϣ��
//ֻ��Ҫ�����͸��������һϵ��ID 
typedef struct Readers{
	Book* allBooks;
	char name[MAX];
	struct Readers* next;
}Readers;

char BUF[1024];
Book* libary = NULL;
Readers* reader = NULL;
char s[25];//��¼��ǰģʽ 

int inintReaders();	//�����߼���ȡ�������� 
int inintLibrary();	//��ͼ����ļ���ȡ��������
void showBook(Book* bk);	//���bk�е�������Ϣ 
void showAdmin();		//չʾ����Ա����̨ 
void showReader();	//չʾ���߲���̨ 
Readers* findReader(char* name);	//��reader������Ѱ�� name �Ľڵ� 
int searchBookByName(char* name); 	//ͨ��nameѰ���鼮����ӡ��Ϣ 
Book* searchBookByNameHelper(Book* allBooks, char* name); //��allBook������ͨ��nameѰ�����鼮�ڵ� 
int searchBookByID(int id); //ͨ�� idѰ���鼮����ӡ��Ϣ 
int deleteBookByName(char *name); // ��libary��ɾ��name���鼮 
int deleteBookByID(int id);	//��libary��ɾ��id���鼮 
void addBook(Book* allBooks,char* name , int id);	// ��allBooks������������� 

int changeBook(char* name,int id,char* newname, int newid);	// ��libary�иı��鼮 
/*���ı������������һ�����鼮������ı�ID*/

int borrowBook(char* bkname , char* rdname);	// ���߽������ 
int returnBook(char* bkname , char* rdname);	//���߻������ 
void ForAdmin(); 					//����Աģʽ 
void ForReader(char* rdname); 			//����ģʽ 

/*�������򣬲�����ǰ״̬д�뵽�ļ��б�������*/ 
void endReader();
void endLib();
void end();

//----------------�ɰ��ķָ��� --------------------------------- 
int main(int argc, char *argv[]) {
    if(inintLibrary() == 0)
    	return 0;
	if(inintReaders() == 0)
    	return 0;
    if(strcmp(argv[1],"-u")==0){
    	ForReader(argv[2]);
	}else{
		ForAdmin(); 
	}
	end();
    return 0;
}
//----------------����ķָ��� --------------------------------- 
void endReader(){
	Readers* rd = reader->next;
	FILE* fp = fopen("myreaders.readers","w");
	while(rd != NULL){
		fprintf(fp,"%s#",rd->name);
		Book* bk = rd->allBooks->next;
		while(bk != NULL){
			fprintf(fp,"%s",bk->name);
			Node* p = bk->id->next;
			while(p != NULL){
				fprintf(fp,",%d",p->data);
				p = p->next;
			}
			fprintf(fp,"#");
			bk = bk->next;
		}
		fprintf(fp,"\n");
		rd = rd->next;
	}
}
void endLib(){
	FILE* fp = fopen("mylibary.libary","w");
	Book* bk = libary->next;
	while(bk != NULL){
		printf("sa\n");
		fprintf(fp,"%s#",bk->name);
		fprintf(fp,"%d#",bk->inNum);
		fprintf(fp,"%d#",bk->outNum);
		Node* p = bk->id->next;
		while(p != NULL){
			fprintf(fp,"%d#%s#",p->data,strlen(p->readerName)>0?p->readerName:".");
			p = p->next;
		}
		fprintf(fp,"\n");
		bk = bk->next;
	}
}
void end(){
	endReader();
	endLib();
	printf("��ӭʹ�ã��ټ���\n");
}
void ForReader(char* rdname){
	//�����ڵ�ǰ���ߵĻ�
	Readers* r = findReader(rdname);
	if(r == NULL){
		Readers* rp = reader;
		r = (Readers*)malloc(sizeof(Readers));
		strcpy(r->name,rdname);
		r->allBooks = (Book*)malloc(sizeof(Book));
		r->allBooks->id = (Node*)malloc(sizeof(Node));
		r->allBooks->id->next = NULL;
		r->allBooks->next = NULL;
		
		r->next = rp->next;
		rp->next = r;
	} 
	while(1){
		showReader();
		int choice = 0;
		scanf("%d",&choice);
		if(choice == 0){
			return ;
		}else if(choice == 1){
			char bkname[MAX]="";
			printf("����������鼮:\n");
			scanf("%s",bkname);getchar();
			borrowBook(bkname , rdname);
		}else if(choice == 2){
			char bkname[MAX]="";
			printf("������黹�鼮:\n");
			scanf("%s",bkname);getchar();
			returnBook(bkname , rdname);
		}else{
			printf("Error!\n");
		}
	}
}
int inintReaders(){
	FILE* fp = fopen("myreaders.readers", "r");
    if (fp == NULL) {
        printf("ERROR!");
        return 0;
    }
    reader = (Readers*)malloc(sizeof(Readers));
	reader->next = NULL;
	reader->allBooks = NULL;
	Readers* cur = reader;
    while (!feof(fp)) {
    	//����BUF������ȡһ�е�BUF�в�����һ���� 
    	memset(BUF,'\0',sizeof(BUF));
        fgets(BUF, 1024, fp);
        if(BUF[0]=='\0'||BUF[0]=='\n'||BUF[0]=='\r')
        	break;
        int n = strlen(BUF);
        //�����û� 
        Readers* p = (Readers*)malloc(sizeof(Readers));
        p->next = NULL;
        p->allBooks = (Book*)malloc(sizeof(Book));
        p->allBooks->next = NULL;
        //�鼮���ƽڵ� 
        Book* curbk = p->allBooks;
        int index = 0;
        char name[MAX] = "";
        int t = 0;
        //�������� 
        while(index<n && BUF[index]!='#'){
        	name[t] = BUF[index];
        	t++;
        	index ++;
		}
		strcpy(p->name,name);
		//ʣ�µ���Ϣȫ�����鼮�� 
		while(index<n&& BUF[index]!='\n'&&BUF[index]!='\0'&&BUF[index]!='\r'){
			if(BUF[index] == '#'){
				if(index == n-1 || BUF[index]=='\n'||BUF[index]=='\0'||BUF[index]=='\r')
					break;
				index ++;
				if(BUF[index]=='\n'||BUF[index]=='\0'||BUF[index]=='\r')
					break;
				memset(name,'\0',sizeof(name));
				t = 0;
				//����һ������ 
				Book* bk = (Book*)malloc(sizeof(Book));
				bk->id = (Node*)malloc(sizeof(Node));
				bk->id->next = NULL;
				bk->id->data = -1;
				bk->next = NULL;
				Node* sd = bk->id;
				//���� 
				while(index<n && BUF[index]!=','){
					name[t] = BUF[index];
					t++;
					index++;
				}
				strcpy(bk->name,name);
				index ++;
				//��¼�����е�ID 
				while(index<n && BUF[index]!='\n'&&BUF[index]!='\0'&&BUF[index]!='\r' && BUF[index]!='#'){
					if(BUF[index]==','){
						index ++;
					}else{
						int id = 0;
						Node* s = (Node*)malloc(sizeof(Node));
						s->next = NULL;
						while(index<n && BUF[index]!='\n'&&BUF[index]!='\0'&&BUF[index]!='\r' && BUF[index]!='#' && BUF[index]!=','){
							id = id*10 + BUF[index]-'0';
							index ++;
						} 
						s->data = id;
						sd->next = s;
						sd = sd->next;
					}
				} 
				//��һ�� 
				curbk->next = bk;
				curbk = curbk->next;	
			}else{
				index ++;
			}
		}
       	//��һ������ 
		cur->next = p;
		cur = cur->next;
    }
   	return 1;
}
int inintLibrary() {
    FILE* fp = fopen("mylibary.libary", "r+");
    if (fp == NULL) {
        printf("ERROR!");
        return 0;
    }

    libary = (Book*)malloc(sizeof(Book));//ͷ�ڵ� 
    libary->next = NULL;
    Book* cur = libary;
	
    while (!feof(fp)) {
    	//����BUF������ȡһ�е�BUF�в�����һ���� 
    	memset(BUF,0,sizeof(BUF));
        fgets(BUF, 1024, fp);
        if(BUF[0]=='\0'||BUF[0]=='\n'||BUF[0]=='\r')
        	break; 
        Book* p = (Book*)malloc(sizeof(Book));
        p->next = NULL;
        int index = 0;
        int n = strlen(BUF);
        
        //��ȡ���� 
        while (index < n && BUF[index] != '#') {
            p->name[index] = BUF[index];
            index++;
        }
        p->name[index] = '\0';
        index++;
        
        //��ȡ���� 
        p->inNum = 0;
        p->outNum = 0;
        int num = 0;
        //��ǰͼ��ݴ��ڵ����� 
        while (index < n && BUF[index] != '#') {
            num = num * 10 + BUF[index] - '0';
            index++;
        }
        p->inNum = num;
        index++;
        //��ǰͼ�����������
		num = 0;
		while (index < n && BUF[index] != '#') {
            num = num * 10 + BUF[index] - '0';
            index++;
        }
        p->outNum = num;
        index++;
        //��ȡ��Ų��Ƴ�Node���� 
        p->id = (Node*)malloc(sizeof(Node));
        p->id->data = -1;
        strcpy(p->id->readerName ,"");
        Node* tmp = p->id;
        while (index<n && BUF[index]!='\0' && BUF[index]!='\n') {
            if (BUF[index] == '#') {
                index++;
            }
            else {
                int d = 0;
                //��ȡID 
                while (index < n && isdigit(BUF[index])) {
                    d = d * 10 + (BUF[index] - '0');
                    index++;
                }
                if(BUF[index] == '#'){
                	index ++;
				}
				char nameBuf[MAX]="";
				int t = 0;
				while(index<n && BUF[index]!='\n'&&BUF[index]!='#'&&BUF[index]!='\0'&&BUF[index]!='\r'){
					nameBuf[t] = BUF[index];
					index++;
					t++;
				}
                Node* s = (Node*)malloc(sizeof(Node));
                s->data = d;
                if(strcmp(nameBuf,".") == 0)
                	strcpy(s->readerName,"\0");
                else
                	strcpy(s->readerName,nameBuf);
                s->next = NULL;
                tmp->next = s;
                tmp = tmp->next;
            }
        }
  		//��һ�� 
        cur->next = p;
        cur = cur->next;
    }
    return 1;
}
void addBook(Book* allBooks,char* name , int id) {
	Book* cur = searchBookByNameHelper(allBooks,name);
	if(cur == NULL){
		Book* bk = (Book*)malloc(sizeof(Book));
		strcpy(bk->name,name);
		bk->outNum = 0;
		bk->inNum = 1;
		bk->id = (Node*)malloc(sizeof(Node));
		Node* tmp = bk->id;
		tmp->data = -1;
		Node* s = (Node*)malloc(sizeof(Node));
		s->data = id;
		s->next = NULL;
		tmp->next = s;
		
		Book* cur = allBooks;
		bk->next = cur->next;
		cur->next = bk;
		printf("OK!\n");
	}else{
		cur->inNum++;
		Node* tmp = (Node*)malloc(sizeof(Node));
		tmp->data = id;
		strcpy(tmp->readerName,"");
		Node* p = cur->id;
		tmp->next = p->next;
		p->next = tmp;
	}
	printf("��ӳɹ���\n");
}
Book* searchBookByNameHelper(Book* allBooks, char* name){
	Book* cur = allBooks->next;
	while(cur != NULL){
		if(strcmp(cur->name , name)==0){
			return cur;
		}
		cur = cur->next;
	}
	return NULL;
}
int searchBookByName(char* name) {
	Book* bk = searchBookByNameHelper(libary,name);
	if(bk == NULL){
		printf("��Ǹ���������Ȿ�飡\n");
		return 0;
	}
	printf("����Ѱ�ҵ��������Ϣ:\n");
	showBook(bk);
	return 1;
}
int searchBookByID(int id) {
	Book* bk = libary->next;
	while(bk != NULL){
		Node* p = bk->id->next;
		while(p != NULL){
			if(p->data == id){
				printf("����Ѱ�ҵ��������Ϣ:\n");
				showBook(bk);
				return 1;
			}
		}
	}
	printf("��Ǹ���������Ȿ�飡\n");
	return 0;
}
void showAdmin() {
    printf("\n--------ͼ��ݹ���ϵ(administrators)-----------\n");
    printf("| �����ѡ�����²���(������Ӧ���ּ�):         |\n");
    printf("| 0.�˳�����                                  |\n");
    printf("| 1.¼��ͼ����Ϣ                              |\n");
    printf("| 2.ɾ��ͼ����Ϣ                              |\n");
    printf("| 3.�޸�ͼ����Ϣ                              |\n");
    printf("| 4.��ѯͼ����Ϣ                              |\n");
    printf("| 5.¼�������Ϣ                              |\n");
    printf("| 6.ɾ��������Ϣ                              |\n");
    printf("| 7.�޸Ķ�����Ϣ                              |\n");
    printf("| 8.��ѯ������Ϣ                              |\n");
    printf("----------------------------------------------\n");
}
void showReader() {
    printf("\n--------ͼ��ݹ���ϵ(reader)------------------\n");
    printf("| �����ѡ�����²���(������Ӧ���ּ�):         |\n");
    printf("| 0.�˳�����                                  |\n");
    printf("| 1.����                                      |\n");
    printf("| 2.����                                      |\n");
    printf("----------------------------------------------\n");
}
int deleteBookByName(char *name) {
	Book* bk = libary;
	while(bk->next!=NULL){
		if(strcmp(bk->next->name,name) == 0){
			Book* p = bk->next;
			bk->next = p->next;
			free(p);
			printf("ɾ���ɹ���\n");
			return 1;
		}
		bk = bk->next;
	}
	printf("ɾ��ʧ�ܣ������ڣ�\n"); 
	return 0;	
}
int deleteBookByID(int id){
	Book* bk = libary->next;
	while(bk != NULL){
		Node* p = bk->id;
		while(p->next != NULL){
			if(p->next->data == id){
				Node* s = p->next;
				p->next = s->next;
				free(s);
				bk->inNum--;
				printf("ɾ���ɹ���\n");
				return 1;
			}
			p = p->next;
		}
	}
	printf("ɾ��ʧ�ܣ������ڣ�\n"); 
	return 0;
}
void showBook(Book* bk){
	if(bk==NULL)
		return ;
	printf("����: %s\n",bk->name);
	printf("�ݴ�����: %d\n",bk->inNum);
	printf("�������: %d\n",bk->outNum);
	Node* cur = bk->id->next;
	printf("����б�: \n"); 
	while(cur!=NULL){
		printf("ID:%d ������:%s\n",cur->data,strlen(cur->readerName)>0?cur->readerName:"δ���");
		cur = cur->next;
	}
	printf("\n");
}
int changeBook(char* name,int id,char* newname, int newid){
	Book* bk = searchBookByNameHelper(libary,name);
	if(bk == NULL){
		printf("�޸�ʧ�ܣ������ڣ�\n");
		return 0;
	}
	Node* tmp = bk->id;
	while(tmp->next != NULL){
		if(tmp->next->data == id){
			if(name == newname){	//����δ�� 
				tmp->next->data = newid;
				printf("�޸ĳɹ���\n");
				return 1;
			}else{
				Node* s = tmp->next;
				tmp->next = s->next;
				bk->inNum--;
				free(s);
				addBook(libary,newname , newid);//�����ѱ䣬�½��鱾 
				return 1;	
			}
		}
		tmp = tmp->next;
	}
	printf("�޸�ʧ�ܣ������ڣ�\n");
	return 0;	
}
Readers* findReader(char* name){
	Readers* cur = reader->next;
	while(cur != NULL){
		if(strcmp(cur->name,name)==0){
			return cur;
		}
		cur = cur->next;
	}
	return NULL;
}
int  borrowBook(char* bkname , char* rdname) {
	Book* bk = searchBookByNameHelper(libary,bkname);
	if(bk==NULL){
		printf("����ʧ�ܣ������ڣ�\n"); 
		return 0;
	}else if(bk->inNum == 0){
		printf("����ʧ�ܣ������ڣ�\n");
		return 0;
	}
	Node* p = bk->id->next;
	while(p != NULL && strlen(p->readerName)>0){
		p = p->next;
	}
	if(p == NULL){
		printf("����ʧ�ܣ������ڣ�\n");
		return 0;
	}
	int id = p->data;
	Readers* rd = findReader(rdname);
	if(rd == NULL){
		printf("δ��ѯ�����ߣ�\n");
		return 0;
	}else{
		
		strcpy(p->readerName , rdname);
		//�ڶ������е��鼮��Ѱ�� 
		Book* rk = searchBookByNameHelper(rd->allBooks,bkname);
		//������ 
		if(rk == NULL){
			Book* book = rd->allBooks;
			Book* tmp = (Book*)malloc(sizeof(Book));
			strcpy(tmp->name,bkname);
			tmp->next = NULL;
			//ID�ļ�¼ 
			tmp->id = (Node*)malloc(sizeof(Node));
			tmp->id->data = -1;
			//��ID���� 
			Node* t = (Node*)malloc(sizeof(Node));
			t->data = id;
			t->next = NULL;
			tmp->id->next = t;
			//���鼮����
			tmp->next = book->next;
			book->next = tmp; 
		}else{
			Node* tp = rk->id;
			Node* sp = (Node*)malloc(sizeof(Node));	
			sp->data = id;
			sp->next = tp->next;
			tp->next = sp;
		}
	}
	bk->inNum--;
	bk->outNum++;
	printf("���ĳɹ���\n");
	return 1;
}
int returnBook(char* bkname , char* rdname) {
	Readers* rd = findReader(rdname);
	if(rd == NULL){
		printf("�黹ʧ��,δ��ѯ�����ߣ�\n");
		return 0;
	}
	Book* bk = searchBookByNameHelper(rd->allBooks,bkname);
	if(bk == NULL){
		printf("�黹ʧ��,����δ���Ĵ��飡\n");
		return 0;
	}
	Node* t = bk->id;
	if(t->next == NULL){
		printf("�黹ʧ��,����δ���Ĵ��飡\n");
		return 0;
	}
	Node* st = t->next;
	t->next = st->next;
	bk = searchBookByNameHelper(libary,bkname);
	if(bk == NULL)	//������ 
		addBook(libary,bkname,st->data);
	else{
		Node* p = bk->id->next;
		while(p != NULL){
			if(p->data == st->data){
				strcpy(p->readerName,"");
				break;
			}
			p = p->next;
		}
		//�����ڴ�ID 
		if(p == NULL){
			p = bk->id;
			Node* tp = (Node*)malloc(sizeof(Node));
			tp->data = st->data;
			strcpy(tp->readerName , "");
			tp->next = p->next;
			p->next = tp; 
		}
	}
	free(st);
	bk->inNum++;
	bk->outNum--;
	printf("�黹�ɹ���\n");
	return 1;
}
void ForAdmin(){
	while(1){
		showAdmin();
		int choice = 0;
		scanf("%d",&choice);
		if(choice == 0){
			return;
		}
		else if(choice == 1){
			char name[MAX] = "";
			printf("�������������: \n");
			scanf("%s",name);getchar();
			int id = 0;
			printf("���������ID: \n");
			scanf("%d",&id);
			addBook(libary, name , id);
		}
		else if(choice == 2){
			int ch = 0;
			printf("������ɾ���밴0����IDɾ���밴1: \n");
			scanf("%d",&ch);
			if(ch == 0){
				printf("����������:\n");
				char name[MAX] = "";
				scanf("%s",name);getchar();
				deleteBookByName(name);
			}else if(ch == 1){
				int id = 0;
				scanf("%d",&id);
				deleteBookByID(id);
			}else{
				printf("Error!\n");
			}
		}
		else if(choice == 3){
			char name[MAX] = "";
			char newname[MAX] = "";
			int id = 0;
			int newid = 0;
			printf("�����뱻�޸��鼮������: \n");
			scanf("%s",name);getchar();
			printf("�����뱻�޸��鼮��ID: \n");
			scanf("%d",&id);
			printf("���������鼮������: \n");
			scanf("%s",newname);getchar();
			printf("���������鼮��ID: \n");
			scanf("%d",&newid);
			changeBook(name,id,newname,newid);
		}
		else if(choice == 4){
			int ch = 0;
			printf("��������ѯ�밴0����ID��ѯ�밴1: \n");
			scanf("%d",&ch);
			if(ch == 0){
				printf("����������:\n");
				char name[MAX] = "";
				scanf("%s",name);getchar();
				searchBookByName(name);
			}else if(ch == 1){
				int id = 0;
				scanf("%d",&id);
				searchBookByID(id);
			}else{
				printf("Error!\n");
			}
		}
		else if(choice == 5){
			printf("��������ߵ�����\n");
			char name[MAX] = "";
			scanf("%s",name);getchar();
			Readers* rd = reader;
			Readers* p = (Readers*)malloc(sizeof(Readers));	
			strcpy(p->name,name);
			p->allBooks = (Book*)malloc(sizeof(Book));
			p->allBooks->id = (Node*)malloc(sizeof(Node));
			p->allBooks->id->next = NULL;
			p->allBooks->next = NULL;

			p->next = rd->next;
			rd->next = p;
			printf("��ӳɹ���\n");
		}
		else if(choice == 6){
			printf("����ɾ���Ķ��ߵ�����: \n");
			char name[MAX] = "";
			scanf("%s",name);getchar();
			Readers* rd = reader;
			int flag = 1;
			while(rd->next != NULL){
				if(strcmp(rd->next->name,name) == 0){
					Readers* tp = rd->next;
					rd->next = tp->next;
					free(tp);
					flag = 0;
					printf("ɾ���ɹ���\n");
					break;
				}
				rd = rd->next;
			}
			if(flag == 1){
				printf("�����ڸö���!\n");
			}
		}
		else if(choice == 7){
			char name[MAX] = "";
			char newname[MAX] = "";
			printf("���뱻�޸ĵĶ��ߵ�����: \n");
			printf("���뱻�޸ĵĶ��ߵ�������: \n");
			Readers* rd = findReader(name);
			if(rd == NULL){
				printf("�����ڸö��ߣ�\n");
			}else{
				strcpy(rd->name,newname);
				printf("�޸ĳɹ���\n");
			}
		}
		else if(choice == 8){
			printf("�������������: \n");
			char name[MAX]= "";
			scanf("%s",name);getchar();
			Readers* rd = findReader(name);
			if(rd == NULL){
				printf("δ��ѯ����");
			}else{
				printf("��������: %s",name);
				Book* bk = rd->allBooks->next;
				printf("�����鼮: \n");
				while(bk != NULL){
					printf("%s: ",bk->name);
					Node* tp = bk->id->next;
					while(tp != NULL){
						printf("%d",tp->data);
						if(tp->next!=NULL)
							printf(",");
						tp = tp->next;
					}
					bk = bk->next;
				}
			}
		}
		else {
			printf("Error!\n");
		}
	}
}

