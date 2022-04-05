#include <fcntl.h>
#include <sys/epoll.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void) {
    int fd, epfd;
    struct epoll_event event, events[1];
    char buf[1024];

    fd = open("/dev/tty", O_RDONLY);
    epfd = epoll_create(1);

    event.events = EPOLLIN;
    event.data.fd = fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event);

    while (1) {
        epoll_wait(epfd, events, 1, -1);
        read(fd, buf, sizeof(buf));
    }

    close(epfd);
    close(fd);
}
