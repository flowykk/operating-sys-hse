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

`Клиент №2`, `Клиент №1` - идентичны коду на оценку [6-7](../6-7). Единтвенное отличие заключается в том, что для `Клиента №1` добавлена возможность ввода будущего количества клиентов.
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

Для получения сообщений от каждого из существующих клиентов используется цикл `for`.
```cpp
clients.push_back(echoClient);
for (int i = 1; i < numberOfClients; i++) {
    struct sockaddr_in newClient;
    clientLen = sizeof(newClient);
    if ((totalBytesRcvd = recvfrom(serversock, buffer, BUFFSIZE, 0, (struct sockaddr *) &newClient, &clientLen)) < 0) {
        DieWithError("Failed to receive initial bytes from client");
    }
    clients.push_back(newClient);
}
```

Далее в цлике `while` полученные данные уже отправляются каждому из клиентов `clients[i]` при помощи цикла `for`.
```cpp
while (true) {
    clientLen = sizeof(echoClient);
    if ((totalBytesRcvd = recvfrom(serversock, buffer, BUFFSIZE, 0, (struct sockaddr *) &echoClient, &clientLen)) < 0) {
        DieWithError("Failed to receive additional bytes from client");
    }
    buffer[totalBytesRcvd] = '\0';
    messagesReceived++;

    for (int i = 0; i < numberOfClients - 1; i++) {
        if (sendto(serversock, buffer, totalBytesRcvd, 0, (struct sockaddr *) &clients[i], sizeof(clients[i])) != totalBytesRcvd) {
            DieWithError("Failed to send bytes to client");
        } else {
            fprintf(stdout, "Handling server\n");
        }
    }

    if (messagesReceived >= 6) {
        break;
    }
}
```

В конце своей работы каждая из программ закрывает выделенный для нее сокет при помощи `close`.

## Скриншоты, демонстрирующие работу программы

### Действия клиента №1



### Действия сервера



### Действия клиента №2.1



### Действия клиента №2.2



### Действия клиента №3


