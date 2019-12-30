#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define BUFSIZE 2097152
void serv_client(int fd, struct sockaddr_in *sin);
int path_check(const char *path); // check if the path is avalible
void makeResponse( char *buf, int fd);
void getGET_line(char *buf, char *line); // get the request GET

struct {
    char *ext;
    char *filetype;
} extensions [] = {
    {"gif", "image/gif" },
    {"jpg", "image/jpeg"},
    {"jpeg","image/jpeg"},
    {"png", "image/png" },
    {"zip", "image/zip" },
    {"gz",  "image/gz"  },
    {"tar", "image/tar" },
    {"html","text/html" },
    {"htm", "text/html" },
    {"ogg", "audio/ogg"},
    {"mp4","video/mp4" },
    {"txt", "text/plain"},
    {0,0} };


char *now = ".";
int main(int argc, char *argv[]) {
	pid_t pid;
	int fd, pfd;
	unsigned val;
	struct sockaddr_in sin, psin;
	if(argc < 2) {
		fprintf(stderr, "usage: %s port\n", argv[0]);
		return(-1);
	} else if (argc == 3){
        now = argv[2]; // set the new root
    }
    
    char root[1024];
    realpath(now, root);
    printf("%s\n", root);
    if(chdir( root ) == -1){ 
        perror("chdir");
        return(-1);
    }
    printf("root_PATH = %s\n", now);

    signal(SIGCHLD, SIG_IGN); // parent no need to wait until child finish
	
    if((fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		perror("socket");
		return(-1);
	}
	val = 1;
	if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) < 0) {
		perror("setsockopt");
		return(-1);
	}

	bzero(&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(atoi(argv[1]));
	if(bind(fd, (struct sockaddr*) &sin, sizeof(sin)) < 0) {
		perror("bind");
		return(-1);
	}
	if(listen(fd, SOMAXCONN) < 0) {
		perror("listen");
		return(-1);
	}
	while(1) {
		val = sizeof(psin);
		bzero(&psin, sizeof(psin));
        // wait for client to connect
		if((pfd = accept(fd, (struct sockaddr*) &psin, &val))<0) {
			perror("accept");
			return(-1);
		}
		if((pid = fork()) < 0) {
			perror("fork");
			return(-1);
		} else if(pid == 0) {	/* child */
			close(fd);
			serv_client(pfd, &psin);
			exit(0);
		}
		/* parent */
		close(pfd);
	}
}

int path_check(const char *path){
//    char *now = getenv("path_now");
//    printf("root now: %s\n" , now);
    char root[1024];
    realpath(now, root);
    char trg_path[1024];
    realpath(path, trg_path);
    printf("trg = %s\nroot = %s\n", trg_path, root);
    if (strncmp(trg_path, root, strlen(root)) != 0){
        fprintf( stderr,"cannot open directory '%s'\n", path);
        return 0;
    }
    return 1;
}

void split(char **arr, char *str, const char *del) {
    char *s = strtok(str, del);  
    while(s != NULL) {
        *arr++ = s;
        s = strtok(NULL, del);
   }
}

void getGET_line(char *buf, char *line){
    char *target;
    target = strtok(buf, "\n");
    const char *del = " ";
    char *arr[3];
    split(arr, target, del);
//    printf("target arr= %s\n", arr[1]);
    strcpy( line, arr[1]);
    if ( strstr( line, "?") != NULL ){
        const char *del2 = "?";
        char *splt_q[2];
        split(splt_q, line, del2);
        //printf("split[0] = %s\nsplit[1] = %s\n", splt_q[0], splt_q[1] );
    }
    printf("target line= %s\n", line);
}

void serv_client(int fd, struct sockaddr_in *sin) {
    int len;
    static char buf[BUFSIZE+1];
    printf("connected from %s:%d  >>>>>>>>>>>>>>>>\n",
        inet_ntoa(sin->sin_addr), ntohs(sin->sin_port));
    while((len = recv(fd, buf, sizeof(buf), 0)) > 0) { // recieve request
        
//        printf("request:\n%s\n", buf);
        makeResponse( buf, fd );
        printf("response:\n%s\n", buf);
        
//        if(send(fd, buf, strlen(buf), 0) < 0) {
//            perror("send");
//            exit(-1);
//        }

        close(fd);
    }
        printf("disconnected from %s:%d <<<<<<<<<<<<<<\n",
        inet_ntoa(sin->sin_addr), ntohs(sin->sin_port));
    return;
}

void makeResponse( char *buf, int fd){
    int stat = 0; // 200 = 0, 403 = 3, 404 = 4
    char line[BUFSIZE];
    getGET_line( buf, line );

    // 檢查客戶端所要求的檔案格式 
    char * fstr;
    fstr = (char *)0;
    
    int i, len, file_fd;
    for(i=0;extensions[i].ext!=0;i++) {
        if(strstr( line, extensions[i].ext) != NULL) {
            printf("\n %s --- %s\n", line ,extensions[i].ext); // check
            fstr = extensions[i].filetype;
            break;
        }
    }
    // 檔案格式不支援 
    if(fstr == 0) {
        printf("not support\n"); //testing
        fstr = extensions[i-1].filetype;
    }
    
    if( !path_check( line )){
        stat = 4;
        printf("404 NOT OKK PATH\n");
    }
    else if((file_fd=open(line, O_RDONLY))==-1){// 開啟檔案

        stat = 3;
        printf("403 Not Such File\n");
    }
    
    char response_msg[BUFSIZE];
    if ( stat == 3 ){ // if going to inaccessable path
        sprintf(buf,"HTTP/1.1 403 FORBIDDEN\r\nContent-Type: %s\r\n\r\n", "text/html");
        char msg403[] = "<!doctype html>\n<html lang='en'>\n<head>\n<title>403 FORBIDDEN</title>\n</head>\n</html>\n";
        write(fd, msg403 , strlen(msg403) );
        strcpy(response_msg, msg403);
    }
    else if ( stat == 4 ){ // if going to inaccessable path
        sprintf(buf,"HTTP/1.1 404 NOT FOUND\r\nContent-Type: %s\r\n\r\n", "text/html");
        char msg404[] = "<!doctype html>\n<html lang='en'>\n<head>\n<title>404 Not Exist</title>\n</head>\n</html>\n";
        write(fd, msg404 , strlen(msg404) );
        strcpy(response_msg, msg404);
    }
    else{ // 傳回瀏覽器成功碼 200 和內容的格式
        sprintf(buf,"HTTP/1.1 200 OK\r\nContent-Type: %s\r\n\r\n", fstr);
        printf("Show buffer1: \n%s\n", buf);
        write(fd,buf,strlen(buf));

        long ret;
        while ((ret=read(file_fd, response_msg, BUFSIZE))>0) {
            write(fd, response_msg ,ret);
        }
    //printf("Show message: \n%s\n", response_msg);
    }
//    close(fd);    
    char *CRLF = "\r\n";  // 表頭後的 CRLF
    char response[0xfff]; //回應訊息

    strcat(response, buf);
    strcat(response, response_msg);
    strcpy(buf, response);
}


