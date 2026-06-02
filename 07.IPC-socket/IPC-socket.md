# Embedded Linux - IPC Socket

> Tài liệu học về Linux Socket Programming: Khái niệm Socket, Domains, Types, Protocols, Workflow (TCP/UDP), Address Structures, Non-blocking I/O, và I/O Multiplexing (select/poll/epoll).

---

## 📑 Mục Lục

### [1. Introduction](#1-introduction)
- [1.1. What is Socket?](#11-what-is-socket)
- [1.2. Domain (Address Family)](#12-domain-address-family)
- [1.3. Type (Socket Type)](#13-type-socket-type)
- [1.4. Protocol](#14-protocol)
- [1.5. So sánh TCP vs UDP](#15-so-sánh-tcp-vs-udp)

### [2. TCP Socket Workflow](#2-tcp-socket-workflow)
- [2.1. Three-Way Handshake](#21-three-way-handshake)
- [2.2. TCP Server Workflow](#22-tcp-server-workflow)
- [2.3. TCP Client Workflow](#23-tcp-client-workflow)
- [2.4. Code Example: TCP Echo Server](#24-code-example-tcp-echo-server)
- [2.5. Code Example: TCP Echo Client](#25-code-example-tcp-echo-client)
- [2.6. TCP Connection Termination](#26-tcp-connection-termination)

### [3. UDP Socket Workflow](#3-udp-socket-workflow)
- [3.1. UDP Server Workflow](#31-udp-server-workflow)
- [3.2. UDP Client Workflow](#32-udp-client-workflow)
- [3.3. Code Example: UDP Echo Server](#33-code-example-udp-echo-server)
- [3.4. Code Example: UDP Echo Client](#34-code-example-udp-echo-client)

### [4. Socket Address Structures](#4-socket-address-structures)
- [4.1. Generic Socket Address](#41-generic-socket-address)
- [4.2. IPv4 Socket Address (sockaddr_in)](#42-ipv4-socket-address-sockaddr_in)
- [4.3. IPv6 Socket Address (sockaddr_in6)](#43-ipv6-socket-address-sockaddr_in6)
- [4.4. Convert Functions (inet_pton, inet_ntop)](#44-convert-functions-inet_pton-inet_ntop)
- [4.5. Byte Ordering (htonl, htons, ntohl, ntohs)](#45-byte-ordering-htonl-htons-ntohl-ntohs)

### [5. Unix Domain Socket](#5-unix-domain-socket)
- [5.1. Unix Socket Address](#51-unix-socket-address)
- [5.2. Code Example: Unix Domain Stream Server](#52-code-example-unix-domain-stream-server)
- [5.3. Code Example: Unix Domain Stream Client](#53-code-example-unix-domain-stream-client)

### [6. Socket Options & Advanced Features](#6-socket-options--advanced-features)
- [6.1. Common Socket Options (setsockopt)](#61-common-socket-options-setsockopt)
- [6.2. Non-blocking Sockets](#62-non-blocking-sockets)
- [6.3. Handling SIGPIPE](#63-handling-sigpipe)
- [6.4. TCP_NODELAY & Nagle Algorithm](#64-tcp_nodelay--nagle-algorithm)

### [7. I/O Multiplexing](#7-io-multiplexing)
- [7.1. select() - Cross-platform](#71-select---cross-platform)
- [7.2. poll() - POSIX Standard](#72-poll---posix-standard)
- [7.3. epoll() - Linux High-Performance](#73-epoll---linux-high-performance)
- [7.4. So sánh select vs poll vs epoll](#74-so-sánh-select-vs-poll-vs-epoll)

### [8. Multi-Client Server Models](#8-multi-client-server-models)
- [8.1. Fork-based Server](#81-fork-based-server)
- [8.2. Thread-based Server](#82-thread-based-server)
- [8.3. Event-driven Server (epoll)](#83-event-driven-server-epoll)

### [9. Error Handling & Debugging](#9-error-handling--debugging)
- [9.1. Common Socket Errors](#91-common-socket-errors)
- [9.2. Debugging với strace & netstat](#92-debugging-với-strace--netstat)

---

# 1. Introduction

## 1.1. What is Socket?

**Socket** là một điểm cuối (endpoint) trong liên kết giao tiếp (communication link) hai chiều giữa hai chương trình đang chạy trên mạng.

- Socket cung cấp cơ chế để trao đổi dữ liệu giữa các process chạy trên cùng một máy hoặc trên các máy khác nhau qua mạng.
- Trong Linux, socket cũng được coi là một file (everything is a file). Bạn có thể read/write socket bằng file descriptor.
- Socket là nền tảng cho hầu hết các ứng dụng mạng: web server, database, chat, streaming...

```
Socket Abstraction:
┌─────────────────────────────────────────────────────────────┐
│                         Application                         │
├─────────────────────────────────────────────────────────────┤
│                        Socket API                           │
├──────────────────────┬──────────────────────┬───────────────┤
│    Transport Layer  │    Transport Layer   │    Unix Domain │
│    (TCP/IP)         │    (UDP/IP)          │    (Local IPC) │
├──────────────────────┴──────────────────────┴───────────────┤
│                    Network / Kernel Stack                    │
└─────────────────────────────────────────────────────────────┘
```

## 1.2. Domain (Address Family)

Domain (họ giao thức) xác định phương thức địa chỉ (addressing structure) mà socket sẽ sử dụng.

| Domain         | Mô tả                                                     |
| -------------- | --------------------------------------------------------- |
| **AF_UNIX**    | Giao tiếp giữa các process trên **cùng một máy** (Local). |
| **AF_INET**    | Giao tiếp qua mạng IPv4.                                  |
| **AF_INET6**   | Giao tiếp qua mạng IPv6.                                  |
| **AF_NETLINK** | Giao tiếp giữa User-space và Kernel-space.                |
| **AF_PACKET**  | Giao tiếp ở tầng raw packet (Link layer).                 |

> **Lưu ý**: AF = Address Family. Đôi khi bạn sẽ thấy PF (Protocol Family) như PF_INET, nhưng hiện nay AF và PF thường được dùng thay thế cho nhau.

## 1.3. Type (Socket Type)

Loại socket xác định ngữ nghĩa của giao tiếp (communication semantics).

| Type            | Mô tả                                                                         | Giao thức tương ứng (IP) |
| --------------- | ----------------------------------------------------------------------------- | ------------------------ |
| **SOCK_STREAM** | Cung cấp luồng dữ liệu hai chiều, tin cậy, theo thứ tự (connection-oriented). | **TCP**                  |
| **SOCK_DGRAM**  | Cung cấp giao tiếp dạng gói tin (packet), không tin cậy (connectionless).     | **UDP**                  |
| **SOCK_RAW**    | Cung cấp quyền truy cập trực tiếp vào giao thức tầng dưới (IP layer).         | IP, ICMP                 |
| **SOCK_SEQPACKET** | Gói tin có tuần tự, tin cậy (giống Stream nhưng giữ boundary message).     | SCTP                     |

## 1.4. Protocol

Thường thì với một cặp (Domain, Type) nhất định, chỉ có một Protocol mặc định phù hợp.
- Ví dụ: `AF_INET` + `SOCK_STREAM` → Mặc định là **TCP**.
- Ví dụ: `AF_INET` + `SOCK_DGRAM` → Mặc định là **UDP**.

Khi tạo socket bằng hàm `socket()`, ta thường để tham số protocol là `0` để hệ thống tự chọn protocol mặc định.

```c
int sock_fd = socket(AF_INET, SOCK_STREAM, 0);  // TCP socket
int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);   // UDP socket
int sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);   // Unix domain stream socket
```

## 1.5. So sánh TCP vs UDP

| Tiêu chí              | TCP                                   | UDP                            |
| --------------------- | ------------------------------------- | ------------------------------ |
| **Kết nối**           | Connection-oriented (có kết nối)      | Connectionless (không kết nối) |
| **Độ tin cậy**        | Tin cậy (ACK, retransmission)         | Không tin cậy (best-effort)    |
| **Thứ tự gói tin**    | Đảm bảo thứ tự (sequencing)           | Không đảm bảo thứ tự           |
| **Kiểm soát luồng**   | Có (flow control - sliding window)    | Không                          |
| **Kiểm soát tắc nghẽn**| Có (congestion control)              | Không                          |
| **Toàn vẹn dữ liệu**  | Checksum bắt buộc                     | Checksum (tùy chọn trong IPv4) |
| **Tốc độ**            | Chậm hơn (do overhead)                | Nhanh hơn                      |
| **Ứng dụng**          | Web, Email, File Transfer, SSH        | Streaming, DNS, VoIP, Game     |
| **Header size**        | 20-60 bytes                           | 8 bytes                        |

---

# 2. TCP Socket Workflow

## 2.1. Three-Way Handshake

Trước khi dữ liệu được truyền, TCP thiết lập kết nối bằng **Three-Way Handshake**:

```
CLIENT                              SERVER
   │                                   │
   │  1. SYN (SEQ=100)                 │
   │ ───────────────────────────────►  │  (Client gửi SYN)
   │                                   │
   │  2. SYN+ACK (SEQ=300, ACK=101)    │
   │ ◄───────────────────────────────  │  (Server đồng ý)
   │                                   │
   │  3. ACK (SEQ=101, ACK=301)        │
   │ ───────────────────────────────►  │  (Client xác nhận)
   │                                   │
   │  === Kết nối đã được thiết lập === │
   │                                   │
   │  Data Transfer...                 │
   │ ◄═══════════════════════════════► │
```

**Chi tiết các bước:**
1. **SYN**: Client gửi gói SYN với sequence number ngẫu nhiên (SEQ=100)
2. **SYN-ACK**: Server nhận SYN, gửi lại SYN+ACK (SEQ=300, ACK=101)
3. **ACK**: Client gửi ACK xác nhận (SEQ=101, ACK=301)

Sau bước 3, kết nối được thiết lập và dữ liệu có thể truyền.

## 2.2. TCP Server Workflow

```
SERVER WORKFLOW:
┌──────────────┐
│  socket()    │  ← Tạo socket file descriptor
└──────┬───────┘
       │
┌──────▼───────┐
│   bind()     │  ← Gán địa chỉ (IP + Port) cho socket
└──────┬───────┘
       │
┌──────▼───────┐
│  listen()    │  ← Chuyển socket sang trạng thái lắng nghe
└──────┬───────┘
       │
┌──────▼───────┐  ──┐
│  accept()    │    │ Block cho đến khi có client kết nối
└──────┬───────┘  ──┘
       │
       ▼
┌──────────────────────────────────────────┐
│  recv() / send()  ← Giao tiếp dữ liệu   │
└──────────────────────────────────────────┘
       │
┌──────▼───────┐
│  close()     │  ← Đóng kết nối
└──────────────┘
```

### Chi tiết các hàm:

**1. socket() - Tạo Socket**
```c
#include <sys/socket.h>
int socket(int domain, int type, int protocol);
```
- `domain`: `AF_INET`, `AF_INET6`, `AF_UNIX`
- `type`: `SOCK_STREAM`, `SOCK_DGRAM`
- `protocol`: 0 (mặc định)
- Returns: file descriptor (> 0) hoặc -1 (lỗi)

**2. bind() - Gán địa chỉ**
```c
#include <sys/socket.h>
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```
- `sockfd`: Socket descriptor
- `addr`: Con trỏ đến struct sockaddr (cần cast từ sockaddr_in)
- `addrlen`: Kích thước struct addr

> **Lưu ý**: `INADDR_ANY` (0.0.0.0) cho phép server lắng nghe trên tất cả network interfaces.

**3. listen() - Chuyển sang chế độ lắng nghe**
```c
#include <sys/socket.h>
int listen(int sockfd, int backlog);
```
- `backlog`: Số lượng kết nối tối đa có thể xếp hàng đợi

> **backlog**: Giới hạn số connection đang chờ `accept()`. Nếu quá nhiều client kết nối cùng lúc, các kết nối dư sẽ bị từ chối. Giá trị thường dùng: 5-128.

**4. accept() - Chấp nhận kết nối**
```c
#include <sys/socket.h>
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```
- Returns: **New socket descriptor** cho client này (khác với listening socket)
- `addr`: Nhận thông tin địa chỉ của client (có thể NULL)
- **Blocking**: Hàm block cho đến khi có client kết nối

**5. recv() / send() - Giao tiếp dữ liệu**
```c
#include <sys/socket.h>
ssize_t recv(int sockfd, void *buf, size_t len, int flags);
ssize_t send(int sockfd, const void *buf, size_t len, int flags);
```
- `flags`: 0 (thông thường) hoặc `MSG_WAITALL`, `MSG_NOSIGNAL`,...
- Returns: Số bytes thực sự đọc/ghi, 0 (EOF - peer đóng kết nối), -1 (lỗi)
- `recv()` return 0 nghĩa là client đã đóng kết nối

## 2.3. TCP Client Workflow

```
CLIENT WORKFLOW:
┌──────────────┐
│  socket()    │  ← Tạo socket (không cần bind)
└──────┬───────┘
       │
┌──────▼───────┐
│  connect()   │  ← Kết nối đến server (Three-way Handshake)
└──────┬───────┘
       │
       ▼
┌──────────────────────────────────────────┐
│  send() / recv()  ← Giao tiếp dữ liệu   │
└──────────────────────────────────────────┘
       │
┌──────▼───────┐
│  close()     │  ← Đóng kết nối
└──────────────┘
```

**connect() - Kết nối đến Server**
```c
#include <sys/socket.h>
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```
- Thực hiện Three-way Handshake với server
- **Blocking**: Block cho đến khi kết nối thành công hoặc thất bại
- Return: 0 (thành công), -1 (lỗi)

## 2.4. Code Example: TCP Echo Server

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // 1. Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 2. Set socket option (reuse address)
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 3. Configure address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;  // Listen on all interfaces
    address.sin_port = htons(PORT);

    // 4. Bind
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 5. Listen
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("TCP Server listening on port %d...\n", PORT);

    // 6. Accept loop
    while (1) {
        printf("\nWaiting for client...\n");
        client_fd = accept(server_fd, (struct sockaddr *)&address,
                          (socklen_t *)&addrlen);
        if (client_fd < 0) {
            perror("accept");
            continue;
        }

        // Print client info
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &address.sin_addr, client_ip, INET_ADDRSTRLEN);
        printf("Client connected: %s:%d\n", client_ip, ntohs(address.sin_port));

        // 7. Echo loop: read and send back
        ssize_t bytes_read;
        while ((bytes_read = recv(client_fd, buffer, BUFFER_SIZE, 0)) > 0) {
            printf("Received: %s", buffer);
            send(client_fd, buffer, bytes_read, 0);
            memset(buffer, 0, BUFFER_SIZE);
        }

        if (bytes_read == 0) {
            printf("Client disconnected\n");
        } else {
            perror("recv error");
        }

        close(client_fd);
    }

    close(server_fd);
    return 0;
}
```

**Compile & Run:**
```bash
gcc tcp_server.c -o tcp_server
./tcp_server
```

## 2.5. Code Example: TCP Echo Client

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE] = {0};

    // 1. Create socket
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 2. Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("Invalid address");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    // 3. Connect to server
    if (connect(sock_fd, (struct sockaddr *)&server_addr,
                sizeof(server_addr)) < 0) {
        perror("connect failed");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
    printf("Connected to server.\n");

    // 4. Communication loop
    while (1) {
        printf("\nEnter message (or 'exit' to quit): ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';  // Remove newline

        // Send to server
        send(sock_fd, buffer, strlen(buffer), 0);

        if (strcmp(buffer, "exit") == 0) {
            printf("Closing connection.\n");
            break;
        }

        // Receive echo from server
        memset(buffer, 0, BUFFER_SIZE);
        recv(sock_fd, buffer, BUFFER_SIZE, 0);
        printf("Server echo: %s\n", buffer);
    }

    close(sock_fd);
    return 0;
}
```

**Compile & Run:**
```bash
gcc tcp_client.c -o tcp_client
./tcp_client
```

**Output mẫu:**
```
=== Server ===
TCP Server listening on port 8080...
Waiting for client...
Client connected: 127.0.0.1:54321
Received: Hello Server!
Received: How are you?

=== Client ===
Connected to server.
Enter message: Hello Server!
Server echo: Hello Server!
Enter message: How are you?
Server echo: How are you?
Enter message: exit
Closing connection.
```

## 2.6. TCP Connection Termination

TCP kết thúc kết nối bằng **Four-Way Wavehand**:

```
CLIENT                              SERVER
   │                                   │
   │  1. FIN (SEQ=101)                 │
   │ ───────────────────────────────►  │  (Client gửi FIN)
   │                                   │
   │  2. ACK (SEQ=X, ACK=102)          │
   │ ◄───────────────────────────────  │  (Server ACK)
   │          (Half-close state)       │
   │                                   │
   │  3. FIN (SEQ=X, ACK=102)          │
   │ ◄───────────────────────────────  │  (Server gửi FIN)
   │                                   │
   │  4. ACK (SEQ=102, ACK=X+1)        │
   │ ───────────────────────────────►  │  (Client ACK)
   │                                   │
   │  === Kết nối đã đóng ===          │
```

> **Lưu ý**: Sau `close()`, socket vào trạng thái **TIME_WAIT** (2MSL - thường 60 giây) để đảm bảo các gói tin còn lại không gây nhầm lẫn.

---

# 3. UDP Socket Workflow

## 3.1. UDP Server Workflow

```
UDP SERVER:
┌──────────────┐
│  socket()    │
└──────┬───────┘
       │
┌──────▼───────┐
│   bind()     │  ← Cần bind để client biết port
└──────┬───────┘
       │
┌──────▼───────┐
│ recvfrom()   │  ← Nhận dữ liệu + địa chỉ client
└──────┬───────┘
       │
┌──────▼───────┐
│  sendto()    │  ← Gửi dữ liệu đến client cụ thể
└──────┬───────┘
       │
┌──────▼───────┐
│  close()     │
└──────────────┘
```

## 3.2. UDP Client Workflow

```
UDP CLIENT:
┌──────────────┐
│  socket()    │  ← Không cần bind (OS tự cấp port)
└──────┬───────┘
       │
┌──────▼───────┐
│  sendto()    │  ← Gửi đến server
└──────┬───────┘
       │
┌──────▼───────┐
│ recvfrom()   │  ← Nhận phản hồi
└──────┬───────┘
       │
┌──────▼───────┐
│  close()     │
└──────────────┘
```

## 3.3. Code Example: UDP Echo Server

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // 1. Create UDP socket
    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 2. Bind
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(sock_fd, (struct sockaddr *)&server_addr,
             sizeof(server_addr)) < 0) {
        perror("bind failed");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    printf("UDP Server listening on port %d...\n", PORT);

    // 3. Receive and echo
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t n = recvfrom(sock_fd, buffer, BUFFER_SIZE, 0,
                            (struct sockaddr *)&client_addr, &client_len);
        if (n < 0) {
            perror("recvfrom error");
            continue;
        }

        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        printf("Received from %s:%d: %s\n",
               client_ip, ntohs(client_addr.sin_port), buffer);

        // Echo back
        sendto(sock_fd, buffer, n, 0,
               (struct sockaddr *)&client_addr, client_len);
    }

    close(sock_fd);
    return 0;
}
```

**Compile & Run:**
```bash
gcc udp_server.c -o udp_server
./udp_server
```

## 3.4. Code Example: UDP Echo Client

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock_fd;
    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(server_addr);
    char buffer[BUFFER_SIZE];

    // 1. Create UDP socket
    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 2. Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    printf("UDP Client ready. Type messages:\n");

    // 3. Communication (no connect - we use sendto/recvfrom directly)
    while (1) {
        printf("> ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        if (strcmp(buffer, "exit") == 0) break;

        // Send to server
        sendto(sock_fd, buffer, strlen(buffer), 0,
               (struct sockaddr *)&server_addr, addr_len);

        // Receive echo
        memset(buffer, 0, BUFFER_SIZE);
        recvfrom(sock_fd, buffer, BUFFER_SIZE, 0, NULL, NULL);
        printf("Server echo: %s\n", buffer);
    }

    close(sock_fd);
    return 0;
}
```

**Compile & Run:**
```bash
gcc udp_client.c -o udp_client
./udp_client
```

---

# 4. Socket Address Structures

## 4.1. Generic Socket Address

Đây là cấu trúc địa chỉ tổng quát (`struct sockaddr`). Các hàm socket (bind, connect...) thường nhận con trỏ tới struct này.

```c
struct sockaddr {
    sa_family_t sa_family;  // Address family (AF_INET, AF_INET6, ...)
    char        sa_data[14]; // Protocol address
};
```

Tuy nhiên, khi lập trình với IPv4/IPv6, ta dùng các struct cụ thể (`sockaddr_in`, `sockaddr_in6`) rồi ép kiểu (cast) về `struct sockaddr *`.

## 4.2. IPv4 Socket Address (sockaddr_in)

Dùng cho `AF_INET`.

```c
#include <netinet/in.h>

struct sockaddr_in {
    sa_family_t    sin_family; // Phải là AF_INET
    in_port_t      sin_port;   // Port number (Network Byte Order - Big Endian)
    struct in_addr sin_addr;   // IPv4 address
    unsigned char  sin_zero[8];// Padding (phải zero)
};

struct in_addr {
    uint32_t       s_addr;     // Address in Network Byte Order
};
```

## 4.3. IPv6 Socket Address (sockaddr_in6)

Dùng cho `AF_INET6`.

```c
struct sockaddr_in6 {
    sa_family_t     sin6_family;   // AF_INET6
    in_port_t       sin6_port;     // Port number
    uint32_t        sin6_flowinfo; // IPv6 flow information
    struct in6_addr sin6_addr;     // IPv6 address (16 bytes)
    uint32_t        sin6_scope_id; // Scope ID (interface index)
};
```

## 4.4. Convert Functions (inet_pton, inet_ntop)

**inet_pton** (Presentation to Network) - Chuyển từ chuỗi "192.168.1.1" sang binary:

```c
#include <arpa/inet.h>

int inet_pton(int af, const char *src, void *dst);
// Returns: 1 (OK), 0 (invalid input), -1 (error)
```

```c
struct sockaddr_in addr;
addr.sin_family = AF_INET;
if (inet_pton(AF_INET, "192.168.1.100", &addr.sin_addr) != 1) {
    perror("Invalid IP address");
}
```

**inet_ntop** (Network to Presentation) - Chuyển từ binary sang chuỗi:

```c
const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);
// Returns: pointer to dst (success) or NULL (error)
```

```c
char ip_str[INET_ADDRSTRLEN];  // 16 bytes for IPv4
inet_ntop(AF_INET, &addr.sin_addr, ip_str, sizeof(ip_str));
printf("Client IP: %s\n", ip_str);
```

## 4.5. Byte Ordering (htonl, htons, ntohl, ntohs)

### Endianness là gì?

- **Little Endian** (Intel x86): Số 0x1234 được lưu là [0x34, 0x12] - byte thấp trước
- **Big Endian** (Network): Số 0x1234 được lưu là [0x12, 0x34] - byte cao trước

Mạng Internet dùng **Big Endian** (Network Byte Order). Mọi dữ liệu gửi qua mạng phải ở Network Byte Order.

```c
#include <arpa/inet.h>

uint32_t htonl(uint32_t hostlong);  // Host to Network Long (32-bit)
uint16_t htons(uint16_t hostshort); // Host to Network Short (16-bit)
uint32_t ntohl(uint32_t netlong);   // Network to Host Long
uint16_t ntohs(uint16_t netshort);  // Network to Host Short
```

```c
// Khi bind:
server_addr.sin_port = htons(8080);  // Port 8080 → Network Byte Order

// Khi nhận port từ client:
printf("Client port: %d\n", ntohs(client_addr.sin_port));
```

---

# 5. Unix Domain Socket

## 5.1. Unix Socket Address

Dùng cho giao tiếp Local IPC (`AF_UNIX` hoặc `AF_LOCAL`). Nhanh hơn Internet socket vì không qua stack mạng TCP/IP. Địa chỉ là một đường dẫn file (file path).

```c
#include <sys/un.h>

struct sockaddr_un {
    sa_family_t sun_family;               // AF_UNIX
    char        sun_path[108];            // Path name
};
```

**So sánh Unix Socket vs Internet Socket (localhost):**

| Tiêu chí          | Unix Domain Socket          | TCP Localhost (127.0.0.1)     |
| ----------------- | --------------------------- | ----------------------------- |
| **Tốc độ**        | Nhanh hơn (2x-5x)           | Chậm hơn (qua TCP/IP stack)   |
| **Bảo mật**       | File permission kiểm soát   | Port-based                    |
| **Định danh**     | File path (/tmp/mysocket)   | IP:Port (127.0.0.1:8080)      |
| **Tồn tại**       | File trên filesystem        | Port ảo                       |
| **Namespace**     | Filesystem namespace        | Network namespace             |

## 5.2. Code Example: Unix Domain Stream Server

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>

#define SOCKET_PATH "/tmp/unix_socket"

int main() {
    int server_fd, client_fd;
    struct sockaddr_un addr;
    char buffer[1024];

    // 1. Create socket (AF_UNIX)
    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 2. Remove old socket file if exists
    unlink(SOCKET_PATH);

    // 3. Bind
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // 4. Listen
    listen(server_fd, 5);
    printf("Unix Domain Server listening at %s\n", SOCKET_PATH);

    // 5. Accept
    client_fd = accept(server_fd, NULL, NULL);
    if (client_fd < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // 6. Read and echo
    read(client_fd, buffer, sizeof(buffer));
    printf("Received: %s\n", buffer);
    write(client_fd, "Hello from Unix Server!", 23);

    close(client_fd);
    close(server_fd);
    unlink(SOCKET_PATH);
    return 0;
}
```

## 5.3. Code Example: Unix Domain Stream Client

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>

#define SOCKET_PATH "/tmp/unix_socket"

int main() {
    int sock_fd;
    struct sockaddr_un addr;
    char buffer[1024];

    // 1. Create socket
    sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 2. Connect
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    if (connect(sock_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    // 3. Communicate
    write(sock_fd, "Hello from Unix Client!", 23);
    read(sock_fd, buffer, sizeof(buffer));
    printf("Server reply: %s\n", buffer);

    close(sock_fd);
    return 0;
}
```

---

# 6. Socket Options & Advanced Features

## 6.1. Common Socket Options (setsockopt)

```c
#include <sys/socket.h>

int setsockopt(int sockfd, int level, int optname,
               const void *optval, socklen_t optlen);
```

### Các option thường dùng:

| Level        | Option            | Mô tả                                        |
| ------------ | ----------------- | -------------------------------------------- |
| SOL_SOCKET   | SO_REUSEADDR      | Cho phép bind lại port đang TIME_WAIT        |
| SOL_SOCKET   | SO_KEEPALIVE      | Gửi keep-alive packet để kiểm tra kết nối    |
| SOL_SOCKET   | SO_RCVBUF         | Set kích thước receive buffer                |
| SOL_SOCKET   | SO_SNDBUF         | Set kích thước send buffer                   |
| SOL_SOCKET   | SO_RCVTIMEO       | Set timeout cho recv()                       |
| SOL_SOCKET   | SO_SNDTIMEO       | Set timeout cho send()                       |
| IPPROTO_TCP  | TCP_NODELAY       | Tắt Nagle algorithm                          |
| IPPROTO_TCP  | TCP_CORK          | Tối ưu gửi nhiều dữ liệu nhỏ (Linux specific) |

### Ví dụ: SO_REUSEADDR - Tránh lỗi "Address already in use"

```c
int opt = 1;
if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    perror("setsockopt");
}
```

> **Tại sao cần SO_REUSEADDR?** Sau khi server crash/restart, port vẫn trong trạng thái TIME_WAIT. Nếu không có option này, bind() sẽ thất bại với lỗi "Address already in use".

### Ví dụ: SO_RCVTIMEO - Set timeout cho recv()

```c
struct timeval timeout;
timeout.tv_sec = 5;   // 5 seconds
timeout.tv_usec = 0;
setsockopt(client_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

// Nếu không nhận được dữ liệu trong 5s, recv() return -1, errno = EAGAIN
```

## 6.2. Non-blocking Sockets

Mặc định, socket là **blocking**: các hàm như `accept()`, `recv()`, `send()`, `connect()` sẽ block cho đến khi hoàn thành.

### Cách set non-blocking:

**Cách 1: Khi tạo socket**
```c
int sock_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
```

**Cách 2: Dùng fcntl()**
```c
int flags = fcntl(sock_fd, F_GETFL, 0);
fcntl(sock_fd, F_SETFL, flags | O_NONBLOCK);
```

### Hành vi của non-blocking socket:

| Hàm        | Không có dữ liệu/sự kiện              |
| ---------- | ------------------------------------- |
| `recv()`   | Return -1, errno = **EAGAIN/EWOULDBLOCK** |
| `send()`   | Return -1, errno = **EAGAIN/EWOULDBLOCK** |
| `accept()` | Return -1, errno = **EAGAIN**         |
| `connect()`| Return -1, errno = **EINPROGRESS**    |

### Ví dụ: Non-blocking recv()

```c
#include <fcntl.h>
#include <errno.h>

int set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

// Trong hàm xử lý:
set_nonblocking(client_fd);

ssize_t n = recv(client_fd, buffer, sizeof(buffer), 0);
if (n == -1) {
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
        printf("No data available yet, try again later\n");
        // OK - không có dữ liệu, không phải lỗi
    } else {
        perror("recv error");
        // Lỗi thật sự
    }
}
```

## 6.3. Handling SIGPIPE

Khi bạn `send()` vào một socket đã bị đóng bởi peer, hệ thống gửi **SIGPIPE** đến process. Mặc định, SIGPIPE kill process.

```c
// Cách 1: Ignore SIGPIPE
signal(SIGPIPE, SIG_IGN);

// Cách 2: Dùng MSG_NOSIGNAL flag
ssize_t n = send(sock_fd, data, len, MSG_NOSIGNAL);
// Nếu peer đã đóng, send() return -1, errno = EPIPE (không có SIGPIPE)
```

## 6.4. TCP_NODELAY & Nagle Algorithm

### Nagle Algorithm:
- Gom nhiều gói tin nhỏ thành một gói lớn trước khi gửi
- Giảm số lượng gói tin trên mạng
- Nhưng gây **delay** cho các ứng dụng real-time (game, SSH)

```c
// Tắt Nagle (cho real-time apps)
int flag = 1;
setsockopt(sock_fd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag));
```

---

# 7. I/O Multiplexing

I/O Multiplexing cho phép một process **giám sát nhiều file descriptors** cùng lúc và chỉ xử lý khi có dữ liệu/sự kiện.

## 7.1. select() - Cross-platform

### Prototype:
```c
#include <sys/select.h>

int select(int nfds, fd_set *readfds, fd_set *writefds,
           fd_set *exceptfds, struct timeval *timeout);
```

### Các macro thao tác với fd_set:
```c
void FD_ZERO(fd_set *set);       // Xóa tất cả FDs
void FD_SET(int fd, fd_set *set); // Thêm FD vào set
void FD_CLR(int fd, fd_set *set); // Xóa FD khỏi set
int  FD_ISSET(int fd, fd_set *set); // Kiểm tra FD có trong set?
```

### Parameters:
- `nfds`: Số FD lớn nhất + 1
- `readfds`: Tập FD cần kiểm tra đọc được
- `writefds`: Tập FD cần kiểm tra ghi được
- `exceptfds`: Tập FD cần kiểm tra exception
- `timeout`: NULL (block forever), 0 (poll, return ngay), hoặc struct timeval

### Return:
- `> 0`: Số FD ready
- `0`: Timeout
- `-1`: Error

### Ví dụ: select() với TCP Server

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_fds[MAX_CLIENTS], max_fd, activity;
    struct sockaddr_in address;
    fd_set readfds;
    char buffer[BUFFER_SIZE];

    // Initialize client FDs
    for (int i = 0; i < MAX_CLIENTS; i++) client_fds[i] = 0;

    // Create server socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    printf("select() TCP Server listening on port %d...\n", PORT);

    while (1) {
        // Clear FD set
        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);
        max_fd = server_fd;

        // Add client sockets to set
        for (int i = 0; i < MAX_CLIENTS; i++) {
            int sd = client_fds[i];
            if (sd > 0) FD_SET(sd, &readfds);
            if (sd > max_fd) max_fd = sd;
        }

        // Wait for activity (blocking)
        activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);

        if (activity < 0) {
            perror("select error");
            continue;
        }

        // Check if new connection
        if (FD_ISSET(server_fd, &readfds)) {
            int addrlen = sizeof(address);
            int new_socket = accept(server_fd, (struct sockaddr *)&address,
                                   (socklen_t *)&addrlen);
            printf("New client connected: fd %d\n", new_socket);

            // Add to array
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (client_fds[i] == 0) {
                    client_fds[i] = new_socket;
                    break;
                }
            }
        }

        // Check each client
        for (int i = 0; i < MAX_CLIENTS; i++) {
            int sd = client_fds[i];
            if (FD_ISSET(sd, &readfds)) {
                int n = recv(sd, buffer, BUFFER_SIZE, 0);
                if (n <= 0) {
                    printf("Client disconnected: fd %d\n", sd);
                    close(sd);
                    client_fds[i] = 0;
                } else {
                    buffer[n] = '\0';
                    printf("Received: %s\n", buffer);
                    send(sd, buffer, n, 0);
                }
            }
        }
    }

    return 0;
}
```

## 7.2. poll() - POSIX Standard

### Prototype:
```c
#include <poll.h>

struct pollfd {
    int   fd;         // File descriptor
    short events;     // Events to monitor (input)
    short revents;    // Events returned (output)
};

int poll(struct pollfd *fds, nfds_t nfds, int timeout);
```

### Events flags:
| Flag         | Mô tả                          |
| ------------ | ------------------------------ |
| POLLIN       | Có dữ liệu để đọc              |
| POLLOUT      | Có thể ghi dữ liệu             |
| POLLERR      | Lỗi (output only)              |
| POLLHUP      | Peer đóng kết nối (output only)|
| POLLRDHUP    | Peer đóng 1 đầu kết nối        |

### Ví dụ: poll() với TCP Server

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int main() {
    int server_fd;
    struct sockaddr_in address;
    struct pollfd fds[MAX_CLIENTS + 1];  // +1 for server
    char buffer[BUFFER_SIZE];

    // Create server socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    // Initialize poll structure
    fds[0].fd = server_fd;
    fds[0].events = POLLIN;
    for (int i = 1; i <= MAX_CLIENTS; i++) fds[i].fd = -1;

    printf("poll() TCP Server listening on port %d...\n", PORT);

    while (1) {
        int ret = poll(fds, MAX_CLIENTS + 1, -1);  // -1 = infinite timeout
        if (ret < 0) { perror("poll error"); continue; }

        // New connection?
        if (fds[0].revents & POLLIN) {
            int addrlen = sizeof(address);
            int new_fd = accept(server_fd, (struct sockaddr *)&address,
                               (socklen_t *)&addrlen);
            printf("New client: fd %d\n", new_fd);

            // Add to pollfd array
            for (int i = 1; i <= MAX_CLIENTS; i++) {
                if (fds[i].fd == -1) {
                    fds[i].fd = new_fd;
                    fds[i].events = POLLIN;
                    break;
                }
            }
        }

        // Check clients
        for (int i = 1; i <= MAX_CLIENTS; i++) {
            int fd = fds[i].fd;
            if (fd == -1) continue;

            if (fds[i].revents & POLLIN) {
                int n = recv(fd, buffer, BUFFER_SIZE, 0);
                if (n <= 0) {
                    printf("Client %d disconnected\n", fd);
                    close(fd);
                    fds[i].fd = -1;
                } else {
                    buffer[n] = '\0';
                    printf("From fd %d: %s\n", fd, buffer);
                    send(fd, buffer, n, 0);
                }
            }

            if (fds[i].revents & POLLHUP) {
                printf("Client %d hung up\n", fd);
                close(fd);
                fds[i].fd = -1;
            }
        }
    }

    return 0;
}
```

## 7.3. epoll() - Linux High-Performance

**epoll** là I/O event notification facility của Linux, được thiết kế cho các ứng dụng cần xử lý hàng ngàn kết nối cùng lúc (C10K problem).

### Ưu điểm của epoll so với select/poll:
- **Không cần duyệt toàn bộ FD set** mỗi lần gọi
- **Hiệu quả với số lượng FD lớn** (O(1) vs O(n))
- **Edge-triggered** (ET) và **Level-triggered** (LT) modes
- Có thể modify FD set mà không cần rebuild

### Các hàm epoll:

```c
#include <sys/epoll.h>

// Tạo epoll instance
int epoll_create1(int flags);  // flags: 0 hoặc EPOLL_CLOEXEC

// Thêm/sửa/xóa FD
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);

// Đợi sự kiện
int epoll_wait(int epfd, struct epoll_event *events,
               int maxevents, int timeout);

struct epoll_event {
    uint32_t     events;    // Bitmask of events
    epoll_data_t data;      // User data
};

typedef union epoll_data {
    void    *ptr;
    int      fd;
    uint32_t u32;
    uint64_t u64;
} epoll_data_t;
```

### Ví dụ: epoll TCP Echo Server

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

#define PORT 8080
#define MAX_EVENTS 100
#define BUFFER_SIZE 1024

int main() {
    int server_fd, epoll_fd;
    struct sockaddr_in address;
    struct epoll_event event, events[MAX_EVENTS];
    char buffer[BUFFER_SIZE];

    // Create server socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    // Create epoll instance
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }

    // Add server_fd to epoll
    event.events = EPOLLIN;  // Level-triggered
    event.data.fd = server_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) == -1) {
        perror("epoll_ctl: server_fd");
        exit(EXIT_FAILURE);
    }

    printf("epoll TCP Server listening on port %d...\n", PORT);

    while (1) {
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            continue;
        }

        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == server_fd) {
                // New connection
                int addrlen = sizeof(address);
                int client_fd = accept(server_fd,
                    (struct sockaddr *)&address, (socklen_t *)&addrlen);
                printf("New client: fd %d\n", client_fd);

                // Add to epoll
                event.events = EPOLLIN;
                event.data.fd = client_fd;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event);
            } else {
                // Client data
                int client_fd = events[i].data.fd;
                int n = recv(client_fd, buffer, BUFFER_SIZE, 0);

                if (n <= 0) {
                    printf("Client %d disconnected\n", client_fd);
                    close(client_fd);
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, NULL);
                } else {
                    buffer[n] = '\0';
                    printf("From fd %d: %s\n", client_fd, buffer);
                    send(client_fd, buffer, n, 0);
                }
            }
        }
    }

    close(server_fd);
    return 0;
}
```

## 7.4. So sánh select vs poll vs epoll

| Tiêu chí              | select()           | poll()             | epoll()                    |
| --------------------- | ------------------ | ------------------ | -------------------------- |
| **Hỗ trợ**           | POSIX, cross-platform | POSIX, cross-platform | Linux only              |
| **Số FD tối đa**     | FD_SETSIZE (1024)  | Không giới hạn     | Không giới hạn             |
| **Hiệu năng**        | O(n)               | O(n)               | O(1) - chỉ FD active       |
| **FD set**           | fd_set (bitmask)   | pollfd array       | RB-tree + ready list       |
| **Thay đổi FD set**  | Rebuild mỗi lần    | Rebuild mỗi lần    | Có thể thay đổi động       |
| **Edge-triggered**   | Không              | Không              | Có (EPOLLET)               |
| **Sử dụng phù hợp**  | < 1024 clients     | < 1000 clients     | Hàng ngàn+ clients         |

---

# 8. Multi-Client Server Models

## 8.1. Fork-based Server

Mỗi client được phục vụ bởi một child process riêng.

```
┌──────────────────────────────────────┐
│          Main Process                │
│      (listening on socket)           │
│         accept()                     │
└─────┬──────────────────┬─────────────┘
      │                  │
┌─────▼─────┐    ┌──────▼──────┐
│ Child 1   │    │  Child 2    │
│ fork()    │    │  fork()     │
│ recv()/send()  │  recv()/send()  │
└───────────┘    └─────────────┘
```

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_client(int client_fd) {
    char buffer[BUFFER_SIZE];
    ssize_t n;

    while ((n = recv(client_fd, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[n] = '\0';
        printf("[Child %d] Received: %s", getpid(), buffer);
        send(client_fd, buffer, n, 0);
    }

    printf("[Child %d] Client disconnected\n", getpid());
    close(client_fd);
    exit(0);
}

// Reap zombie children
void sigchld_handler(int sig) {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    signal(SIGCHLD, sigchld_handler);  // Prevent zombies

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 5);

    printf("Fork-based server listening on port %d...\n", PORT);

    while (1) {
        client_fd = accept(server_fd, (struct sockaddr *)&address,
                          (socklen_t *)&addrlen);
        if (client_fd < 0) continue;

        pid_t pid = fork();
        if (pid == 0) {
            // Child process
            close(server_fd);  // Child doesn't need listening socket
            handle_client(client_fd);
        } else if (pid > 0) {
            // Parent process
            close(client_fd);  // Parent doesn't need client socket
            printf("Forked child %d for client\n", pid);
        }
    }

    close(server_fd);
    return 0;
}
```

### Nhược điểm của fork-based:
- Tốn tài nguyên (mỗi client = 1 process)
- Không scale tốt với hàng ngàn clients
- Chậm do overhead của fork()

## 8.2. Thread-based Server

Mỗi client được phục vụ bởi một thread riêng.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void *handle_client(void *arg) {
    int client_fd = *(int *)arg;
    free(arg);  // Free allocated fd
    pthread_detach(pthread_self());  // Auto cleanup on exit

    char buffer[BUFFER_SIZE];
    ssize_t n;

    while ((n = recv(client_fd, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[n] = '\0';
        printf("[Thread %lu] Received: %s", pthread_self(), buffer);
        send(client_fd, buffer, n, 0);
    }

    printf("[Thread %lu] Client disconnected\n", pthread_self());
    close(client_fd);
    return NULL;
}

int main() {
    int server_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 5);

    printf("Thread-based server listening on port %d...\n", PORT);

    while (1) {
        int *client_fd = malloc(sizeof(int));
        *client_fd = accept(server_fd, (struct sockaddr *)&address,
                           (socklen_t *)&addrlen);
        if (*client_fd < 0) {
            free(client_fd);
            continue;
        }

        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, client_fd);
        // Thread will detach itself
    }

    close(server_fd);
    return 0;
}
```

## 8.3. Event-driven Server (epoll)

Phần này đã được trình bày ở mục 7.3 (epoll example). Đây là mô hình hiệu quả nhất cho hàng ngàn kết nối.

---

# 9. Error Handling & Debugging

## 9.1. Common Socket Errors

| Error                 | errno        | Nguyên nhân                                         |
| --------------------- | ------------ | --------------------------------------------------- |
| **Address in use**    | EADDRINUSE   | Port đang được dùng (chờ SO_REUSEADDR hoặc TIME_WAIT) |
| **Connection refused**| ECONNREFUSED | Không có server lắng nghe tại port đó               |
| **Connection reset**  | ECONNRESET   | Peer bất ngờ đóng kết nối (crash, network down)     |
| **Broken pipe**       | EPIPE        | Ghi vào socket đã đóng                              |
| **Resource unavailable| EAGAIN       | Non-blocking socket chưa có dữ liệu                 |
| **Operation in progress| EINPROGRESS | Non-blocking connect đang thực hiện                 |
| **Timed out**         | ETIMEDOUT    | Mất kết nối mạng                                    |
| **Too many open files**| EMFILE      | Hết file descriptors (cần tăng ulimit -n)            |

### Pattern xử lý lỗi chung:

```c
int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
if (sock_fd < 0) {
    perror("socket creation failed");
    // Có thể thử lại hoặc exit
    exit(EXIT_FAILURE);
}

// Khi gặp lỗi non-critical (EAGAIN với non-blocking):
if (errno == EAGAIN || errno == EWOULDBLOCK) {
    // OK - thử lại sau
    continue;
}

// Khi gặp lỗi connection reset:
if (errno == ECONNRESET) {
    printf("Connection reset by peer\n");
    close(fd);
    // Remove fd from tracking
}
```

## 9.2. Debugging với strace & netstat

### netstat / ss - Xem trạng thái socket:

```bash
# Xem tất cả socket đang lắng nghe
netstat -tlnp
# Proto Recv-Q Send-Q Local Address  Foreign Address  State   PID/Program
# tcp   0      0      0.0.0.0:8080    0.0.0.0:*       LISTEN  1234/./server

# Xem kết nối TCP
netstat -tnp
# Proto Recv-Q Send-Q Local Address   Foreign Address    State
# tcp   0      0      127.0.0.1:8080   127.0.0.1:54321   ESTABLISHED

# ss (modern replacement for netstat)
ss -tlnp   # Listening sockets
ss -tnp    # Established connections
ss -tupan  # All TCP/UDP sockets

# Xem các socket Unix
ss -xlnp
```

### strace - Theo dõi system calls:

```bash
# Theo dõi tất cả socket-related syscalls
strace -e trace=network ./server

# Output mẫu:
# socket(AF_INET, SOCK_STREAM, 0) = 3
# bind(3, {sa_family=AF_INET, sin_port=htons(8080), ...}, 16) = 0
# listen(3, 3) = 0
# accept(3, ...) = 4
# recvfrom(4, "Hello", 1024, 0, ...) = 5
# sendto(4, "Hello", 5, 0, ...) = 5

# Theo dõi riêng read/write (data)
strace -e trace=read,write ./server
```

### lsof - Xem file descriptors:

```bash
# Xem socket của một process
lsof -i :8080
# COMMAND   PID  USER   FD   TYPE  DEVICE SIZE NODE NAME
# server   1234  user   3u   IPv4  12345   0t0  TCP *:8080 (LISTEN)

# Xem tất cả socket của process
lsof -p 1234 | grep -i sock

# Xem số FD đang mở
lsof -p 1234 | wc -l
```

### Công cụ hữu ích khác:

```bash
# Kiểm tra port đã được sử dụng chưa
nc -zv 127.0.0.1 8080
# Connection to 127.0.0.1 port 8080 succeeded!

# Gửi request HTTP thủ công
echo -e "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n" | nc 127.0.0.1 8080

# Ping port (kiểm tra kết nối)
telnet 127.0.0.1 8080   # Ctrl+] rồi gõ quit để thoát

# Tăng giới hạn file descriptors cho server
ulimit -n 100000
```

---

## 📚 Tài liệu tham khảo

- *The Linux Programming Interface* - Michael Kerrisk (Chapters 56-63)
- *UNIX Network Programming* - W. Richard Stevens
- *TCP/IP Illustrated* - W. Richard Stevens
- Linux man pages: `man 7 socket`, `man 7 tcp`, `man 7 udp`, `man 7 ip`
- [epoll man page](https://man7.org/linux/man-pages/man7/epoll.7.html)
- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/)
