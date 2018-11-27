/*
	Want to simulate ps -ef | grep | sort | uniq
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(){

	int p2g[2], g2s[2], s2u[2];
	
	pipe(p2g);
	pipe(g2s);
	pipe(s2u);


	if(fork() == 0){
		close(p2g[0]);
		close(g2s[1]);
		close(g2s[0]);
		close(s2u[1]);
		close(s2u[0]);
		dup2(p2g[1], 1);
		execlp("ps", "ps", "-ef", NULL);
		close(p2g[1]);
		exit(1);
	}

	if(fork() == 0){
		close(g2s[0]);
		close(p2g[1]);
		close(s2u[0]);
		close(s2u[1]);		
		dup2(p2g[0], 0);
		dup2(g2s[1], 1);
		execlp("grep", "grep", "geo", NULL);
		close(g2s[1]);
		close(p2g[0]);
		exit(1);
	}

	if(fork() == 0){
		close(p2g[0]);
		close(p2g[1]);
		close(g2s[1]);
		close(s2u[0]);
		dup2(g2s[0], 0);
		dup2(s2u[1], 1);
		execlp("sort", "sort", NULL);
		close(g2s[0]);
		close(s2u[1]);
		exit(1);	
	}

	if(fork() == 0){
		close(p2g[0]);
		close(p2g[1]);
		close(g2s[1]);
		close(g2s[0]);
		close(s2u[1]);
		dup2(s2u[0], 0);
		execlp("uniq", "uniq", NULL);
		close(s2u[0]);
		exit(1);
	}
	wait(0); wait(0); wait(0); wait(0);
	close(p2g[1]); close(p2g[0]);
	close(g2s[1]); close(g2s[0]);
	close(s2u[1]); close(s2u[0]);	
	
	return 0;
}