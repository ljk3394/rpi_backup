//uartinterface.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>

int open_serial_port(const char *device, int rate) {
    int fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1) {
        perror("open_serial_port: Unable to open device");
        return -1;
    }

    struct termios options;
    tcgetattr(fd, &options);

    cfsetispeed(&options, rate);
    cfsetospeed(&options, rate);

    options.c_cflag &= ~PARENB; // Clear parity bit, disabling parity
    options.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication
    options.c_cflag &= ~CSIZE; // Clear all bits that set the data size
    options.c_cflag |= CS8; // 8 bits per byte
    options.c_cflag |= (CLOCAL | CREAD); // Ignore modem controls, enable reading
    options.c_iflag &= ~(IXON | IXOFF | IXANY); // Shut off xon/xoff ctrl
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // Disable canonical mode, echo, and signal chars
    options.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    options.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed

    // Set the new attributes
    tcsetattr(fd, TCSANOW, &options);

    // // Make read operations non-blocking
    // fcntl(fd, F_SETFL,0);
    // printf("Serial port opened and configured.\n");
    // return fd;
        int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0) {
        perror("fcntl get");
        close(fd);
        return -1;
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0) {
        perror("fcntl set non-blocking");
        close(fd);
        return -1;
    }

    printf("Serial port opened and configured.\n");
    return fd;
}

void close_serial_port(int fd) {
    close(fd);
}

int read_from_serial_port(int fd, char *buffer, size_t size) {
    memset(buffer, '\0', size);
    int num_bytes = read(fd, buffer, size - 1);

    if (num_bytes < 0) {
        // 데이터가 현재 사용 가능하지 않은 경우, 에러 메시지를 출력하지 않음
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            // Non-blocking 모드에서의 정상적인 동작, 데이터가 없음
            return 0; // 읽은 바이트 수가 0이라고 가정할 수 있음
        } else {
            // 읽기 에러 처리
            perror("read_from_serial_port: Error reading");
            return num_bytes; // 에러를 반환
        }
    }

    return num_bytes; // 읽은 바이트 수 반환
}



int send_to_serial_port(int fd, const char *data, size_t length) {
    int num_bytes_written = write(fd, data, length);

    if (num_bytes_written < 0) {
        // 에러 처리
        perror("send_to_serial_port: Error writing");
    }

    return num_bytes_written; // 쓴 바이트 수 혹은 에러
}