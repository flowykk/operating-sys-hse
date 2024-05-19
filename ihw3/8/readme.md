# Отчёт для программы: 8 баллов. Клиент–серверное приложение, которое отображает всю комплексную информации о выполнении приложения и допускает возможность подключения множекства клиентов.  

## Код программы

Клиент №1 - [client.cpp](client.cpp)

Клиент №2 - [client2.cpp](client2.cpp)

Клиент №3 - [client2.cpp](client3.cpp)

Сервер - [server.cpp](server.cpp)

## Описание входных данных

В терминале `Клиента №1` пользователю предлагается ввести энергию Ций каждого из бойцов.

## Описание алгоритма работы программы

- `Клиент №1` предлагает клиенту ввести суммарное количество будущих клиентов, а далее - энергию Ций каждого из бойцов
- Далее алгоритм работы идентичен алгоритму работы приложения на оценку [6-7](../6-7), но теперь поддерживается подключение множества клиентов

## Описание кода программы

`Клиент №2`, `Клиент №1` - идентичны коду на оценку [4-5](../4-5). Единтвенное для `Клиента №1` добавлена возможность ввода будущего количества клиентов.
```cpp
for (int i = 0; i < n; i++) {
    if (i == 0)
        printf("Enter number of future clients: ");
    else
        printf("Enter Ci-energy of fighter %d: ", i);

    fgets(buffer, BUFFSIZE, stdin);
    recvMsgSize = strlen(buffer);

    if (send(sock, buffer, recvMsgSize, 0) != recvMsgSize) {
        DieWithError("Mismatch in number of sent bytes");
    }
}
```

`Клиент №3` - идентичен коду на оценку [6-7](../6-7) 

`Сервер` теперь содержит массив сокетов клиентов `clients`, куда записываются сокеты каждого из клиентов при помощи цикла
```cpp
for (int i = 0; i < numberOfClients - 1; i++) {
    clients[i] = AcceptTCPClient(serversock);
}
```

`AcceptTCPClient(..)` - функция, позволяющая подключить определённого клиента к серверу.
```cpp
int AcceptTCPClient(int servSock) {
    int clntSock;                    /* Socket descriptor for client */
    struct sockaddr_in echoClntAddr; /* Client address */
    unsigned int clntLen;            /* Length of client address data structure */

    clntLen = sizeof(echoClntAddr);

    if ((clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr, &clntLen)) < 0)
        DieWithError("accept() failed");

    printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));

    return clntSock;
}
```

Далее в цлике `while` полученные данные уже отправляются каждому из клиентов `clients[i]` при помощи цикла `for`.
```cpp
while (totalBytesRcvd > 0) {
    messegesReceived++;
    if ((totalBytesRcvd = recv(clientsock1, buffer, BUFFSIZE, 0)) < 0) {
        DieWithError("Failed to receive additional bytes from client");
    }
    buffer[totalBytesRcvd] = '\0';

    for (int i = 0; i < numberOfClients - 1; i++) {
        if (send(clients[i], buffer, totalBytesRcvd, 0) != totalBytesRcvd) {
            DieWithError("Failed to send bytes to client");
        } else {
            fprintf(stdout, "Handling server\n");
        }
    }

    // ...
}
```

## Скриншоты, демонстрирующие работу программы

### Действия клиента №1

<img width="729" alt="Снимок экрана 2024-05-19 в 16 20 43" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/11d58bc2-4ddc-4ae0-8eac-7ee38e2608a3">

### Действия сервера

<img width="728" alt="Снимок экрана 2024-05-19 в 16 20 52" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/4a2b0937-926b-4f70-aab9-30452e7736d1">

### Действия клиента №2.1

<img width="729" alt="Снимок экрана 2024-05-19 в 16 21 02" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/caa1e577-03e2-43b1-9391-f6528293d8de">

### Действия клиента №2.2

<img width="730" alt="Снимок экрана 2024-05-19 в 16 21 12" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/2d32fcc7-824c-4d50-a64a-6d9fb171d569">

### Действия клиента №3

<img width="730" alt="Снимок экрана 2024-05-19 в 16 21 23" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/5b6a1f7b-3a72-41e0-8028-c43491b0bce5">
