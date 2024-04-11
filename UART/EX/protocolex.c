#include <stdio.h>
#include <unistd.h>
#include <string.h>

/**
 * UART 포트로 8바이트 데이터에 '<', '>'를 추가하여 10바이트 패킷을 전송하는 함수.
 * 
 * @param uart_filestream UART 파일 디스크립터.
 * @param data 8바이트 데이터 문자열.
 * @return 성공 시 0, 실패 시 -1을 반환.
 */
int uart_send_packet(int uart_filestream, const char* data) {
    if (strlen(data) != 8) {
        printf("Data must be exactly 8 bytes long.\n");
        return -1;
    }

    char packet[11]; // 10바이트 데이터 + 널 문자('\0')를 위한 공간
    int count;

    // 패킷 생성: '<' + 데이터 + '>'
    snprintf(packet, sizeof(packet), "<%s>", data);

    // UART 포트로 패킷 전송
    count = write(uart_filestream, packet, strlen(packet));
    if (count < 0) {
        printf("UART TX error\n");
        return -1;
    }

    return 0; //sucess
}

/**
 * UART 포트로부터 데이터 패킷을 읽고, '<', '>'로 둘러싸인 데이터를 추출하는 함수.
 * 
 * @param uart_filestream UART 파일 디스크립터.
 * @param buffer 데이터를 저장할 버퍼. 이 버퍼에는 '<', '>'를 제외한 순수 데이터만 저장됩니다.
 * @param buf_size 버퍼의 크기. 이 함수는 최대 8바이트의 데이터를 읽을 것으로 예상하지만, 종료 문자까지 고려하여 여유 있는 크기를 제공해야 합니다.
 * @return 실제로 읽은 데이터의 바이트 수. 실패 시 -1을 반환.
 */
int uart_read_packet(int uart_filestream, char* buffer, size_t buf_size) {
    char read_buf[256]; // 임시로 데이터를 읽어올 큰 버퍼
    memset(read_buf, 0, sizeof(read_buf));

    int bytes_read = read(uart_filestream, read_buf, sizeof(read_buf) - 1);
    if (bytes_read <= 0) {
        // 데이터 읽기 실패 또는 데이터가 없음
        return -1;
    }

    // '<' 시작 문자 찾기
    char* start = strchr(read_buf, '<');
    char* end = strchr(read_buf, '>');
    if (start == NULL || end == NULL || end <= start) {
        // 유효하지 않은 패킷
        return -1;
    }

    // 데이터 길이 계산 (시작과 끝 문자 제외)
    int data_length = end - start - 1;
    if (data_length <= 0 || data_length >= buf_size) {
        // 데이터 길이가 부적절하거나 버퍼 크기 초과
        return -1;
    }

    // 데이터 복사
    memcpy(buffer, start + 1, data_length);
    buffer[data_length] = '\0'; // NULL 문자로 종료

    return data_length;
}