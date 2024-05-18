# Отчёт для программы: 4-5 баллов. Клиент–серверное приложение, в котором сервер (или серверы) и клиенты независимо друг от друга отображают только ту информацию, которая поступает им во время обмена. 

## Код программы

Клиент №1 - [client.cpp](client.cpp)
Клиент №2 - [client2.cpp](client2.cpp)
Сервер - [server.cpp](server.cpp)

## Описание входных данных

В терминале `Клиента №1` пользователю предлагается ввести энергию Ций каждого из бойцов.

## Описание алгоритма работы программы

- `Клиент №1` предлагает клиенту ввести энергию Ций каждого из бойцов
- `Сервер` получает введённые данные и перенаправляет эти данные `Клиенту №2`, а также выводит сообщение об этом
- `Клиент №2` получает данные от `Сервера` и выводит данные на экран
- Когда пользователь вводит последнее число, означающее энергию Ций, `Клиент №1`, `Клиент №2` и `Сервер` завершают свою работу

## Описание кода программы

`Клиент №1`

Сначала `Клиент №1` подключается к серверу.
```cpp
if (connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0) {
      DieWithError("Failed to connect with server");
  }
```

Далее идет цикл по `n = 6` элементов. Каждую итерацию цикла пользователь вводит значени, а введенное число отправляется `Серверу`.
```cpp
int n = 6;
for (int i = 0; i < n; i++) {
    printf("Enter Ci-energy of fighter %d: ", i + 1);
    fgets(buffer, BUFFSIZE, stdin);
    recvMsgSize = strlen(buffer);

    if (send(sock, buffer, recvMsgSize, 0) != recvMsgSize) {
        DieWithError("Mismatch in number of sent bytes");
    }
}
```

`Клиент` завершает работу.
```cpp
close(sock);
return 0;
```

`Сервер`
В бесконечном цикле `Сервер` ожидает, пока два клиента подключатся к нему, и выводит об этом сообщение
```cpp
if ((clientsock1 = accept(serversock, (struct sockaddr *) &echoClient, &recvMsgSize)) < 0) {
      DieWithError("Failed to accept client connection");
  } else {
      fprintf(stdout, "Client connected: %s\n", inet_ntoa(echoClient.sin_addr));
  }

  if ((clientsock2 = accept(serversock, (struct sockaddr *) &echoClient, &recvMsgSize)) < 0) {
      DieWithError("Failed to accept client connection");
  } else {
      fprintf(stdout, "Client2 connected: %s\n", inet_ntoa(echoClient.sin_addr));
  }
```

Далее `Сервер` получает сообщение от `Клиента №1` и выводит об этом сообщение
```cpp
if ((totalBytesRcvd = recv(clientsock1, buffer, BUFFSIZE, 0)) < 0) {
      DieWithError("Failed to receive initial bytes from client");
  }
  buffer[totalBytesRcvd] = '\0';
```

В цикле `while` полученние сообщение отправляется `Клиенту №2`. Переменная `messegesReceived` отражает количество полученных на данный момент времени сообщений, при достижении нужного значения программа завершается.
```cpp
while (totalBytesRcvd > 0) {
      messegesReceived++;
      if (send(clientsock2, buffer, totalBytesRcvd, 0) != totalBytesRcvd) {
          DieWithError("Failed to send bytes to client");
      } else {
          fprintf(stdout, "Handling server\n");
          printf("%d", messegesReceived);
      }
      
      if (strcmp(buffer, "clear\n") == 0 || messegesReceived >= 6) {
          flag = true;
          break;
      }
      
      if ((totalBytesRcvd = recv(clientsock1, buffer, BUFFSIZE, 0)) < 0) {
          DieWithError("Failed to receive additional bytes from client");
      }
      buffer[totalBytesRcvd] = '\0';
}
```

В  конце сокеты для обоих клиентов закрыавются, а программа завершается.
```cpp
close(clientsock1);
close(clientsock2);
return 0;
```

`Клиент №2`

Сначала `Клиент №2` подключается к серверу, после чего выводится сообщение об ожидании ввода данных.
```cpp
if (connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0) {
  DieWithError("Failed to connect with server");
}
printf("Waiting for the server to send data...\n");
```

Далее идет бесконечный цикл, внутри которого происходит обработка данных от `Клиента №1`. 
`totalBytesRcvd` - байты данных, которые данные клиент получает от `Cервера`, после чего на экран выводится соответствующее сообщение, а `messegesReceived` - количество полученных на данный момент сообщений, при достижении определенного значения программа завершается, а сокет закрывается.
```cpp
for(;;) {
  if ((totalBytesRcvd = recv(sock, buffer, BUFFSIZE - 1, 0)) <= 0) {
      DieWithError("Failed to receive initial bytes from server");
  }

  buffer[totalBytesRcvd] = '\0';

  printf("Received: %s", buffer);

  messegesReceived++;
  if (messegesReceived >= 6) {
      close(sock);
      break;
  }
}
```

## Скриншоты, демонстрирующие работу программы

### Действия клиента №1

<img width="730" alt="Снимок экрана 2024-05-18 в 19 54 27" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/14defe63-f137-4d19-bfa2-338312677ac7">

### Действия сервера

<img width="730" alt="Снимок экрана 2024-05-18 в 20 00 56" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/085337e0-06b8-40f0-bb1b-10e83e12a980">

### Действия клиента №2

<img width="730" alt="Снимок экрана 2024-05-18 в 19 54 45" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/9b4f2443-1e2f-415f-8b6d-a68c1c937e9d">
