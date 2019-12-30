#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void serv_client(int fd, struct sockaddr_in *sin);
int path_check(const char *path); // check if the path is avalible
void makeResponse( char *buf);
void getGET_line(char *buf); // get the request GET

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
    {"htm", "text/html" },
    {"html","text/html" },
    {"exe","text/plain" },
    {0,0} };


char *now = "/Users/dragonfly/Desktop/testcase";
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
    if(chdir( now ) == -1){ 
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
    printf("root now: %s" , now);
    char root[1024];
    realpath(now, root);
    char trg_path[1024];
    realpath(path, trg_path);
    if (strncmp(trg_path, root, strlen(root)) != 0){
        fprintf( stderr,"ls: cannot open directory '%s'\n", path);
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

void getGET_line(char *buf){
    char *target;
    target = strtok(buf, "\n");
    const char *del = " ";
    char *arr[3];
    split(arr, target, del);
    printf("target = %s\n", arr[1]);

}

void makeResponse( char *buf){
 //   char *host = "example.com"; // 目標 URI
 //   char *PORT_NUM = "80"; // HTTP port

    char request[0xfff], response[0xfff]; // 請求 與 回應訊息
    char *requestLine = "HTTP/1.1 200 OK\r\n"; // 請求行
 //   char *headerFmt = "Host: %s\r\n"; // Host 表頭欄位
    char *CRLF = "\r\n";  // 表頭後的 CRLF

    int cfd; // Socket 檔案描述符 (File Descriptor)
    int gaiStatus; // getaddrinfo 狀態碼
    
 //   size_t bufferLen = strlen(headerFmt) + strlen(host) + 1;
 //   char *buffer = (char *) malloc(bufferLen);

    strcpy(request, requestLine);
 //   snprintf(buffer, bufferLen, headerFmt, host);
 //   strcat(request, buffer);
    strcat(request, CRLF);

    strcpy( buf, request);
    printf("buf: \n %s\n", buf);

}


void serv_client(int fd, struct sockaddr_in *sin) {
    int len;
    char buf[2048];
    printf("connected from %s:%d\n",
        inet_ntoa(sin->sin_addr), ntohs(sin->sin_port));
    while((len = recv(fd, buf, sizeof(buf), 0)) > 0) { // recieve request

        getGET_line( buf );
        puts(buf);
        makeResponse( buf );

        if(send(fd, buf, len, 0) < 0) {
            perror("send");
            exit(-1);
        }
    }
    printf("disconnected from %s:%d\n",
        inet_ntoa(sin->sin_addr), ntohs(sin->sin_port));
    return;
}
