# Отчёт для программы: 4-5 баллов. Клиент–серверное приложение, в котором сервер (или серверы) и клиенты независимо друг от друга отображают только ту информацию, которая поступает им во время обмена. 

## Код программы

Клиент - [client.cpp](client.cpp)

Сервер - [server.cpp](server.cpp)

## Описание входных данных

В терминале `Клиента` пользователю предлагается ввести энергию Ций каждого из бойцов.

## Описание алгоритма работы программы

- `Клиент` предлагает клиенту ввести энергию Ций каждого из бойцов
- `Сервер` получает введённые данные и выводит их на экран
- Когда пользователь вводит последнее число, означающее энергию Ций, `Клиент` и `Сервер` завершают свою работу

## Описание кода программы

`Клиент`:

Самое важная логика программы происходит в цикле `for`, где пользователь вводит значение энергии Ций для каждого бойца. При помощи команды `sendto` информация отправляется `Серверу`.
```cpp
int n = 6;
for (int i = 0; i < n; i++) {
    printf("Enter Ci-energy of fighter %d: ", i + 1);
    fgets(buffer, BUFFSIZE, stdin);
    echolen = strlen(buffer);

    if (sendto(sock, buffer, echolen, 0, (struct sockaddr *)&echoServAddr, sizeof(echoServAddr)) != echolen) {
        DieWithError("Mismatch in number of sent bytes");
    }
}
```

`Сервер`:

Сначала сокет сервера биндится, чтобы к нему могли подключиться клиенты.
```cpp
if (bind(serversock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0) {
    DieWithError("Failed to bind the server socket");
} else {
    fprintf(stdout, "Server socket binded\n");
}
```

Далее в бесконечнои цикле `Cервер` получает информацию от `Клиента` и выводит сообщение об этом на экран.
```cpp
while (true) {
    if ((recvMsgSize = recvfrom(serversock, buffer, BUFFSIZE - 1, 0, (struct sockaddr *)&echoClntAddr, &clntAddrLen)) < 0) {
        DieWithError("Failed to receive bytes from client");
    }

    buffer[recvMsgSize] = '\0'; // Null-terminate the received data
    printf("Received: %s", buffer);
    messagesReceived++;

    if (messagesReceived >= 6) {
        break;
    }
}
```

В конце программы сокет `Сервера` закрывается.
```cpp
close(serversock);
```

## Скриншоты, демонстрирующие работу программы

### Действия клиента

<img width="730" alt="Снимок экрана 2024-06-01 в 14 10 01" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/a3add50b-6250-48e2-adc4-1197718f0863">

### Действия сервера

<img width="730" alt="Снимок экрана 2024-06-01 в 14 10 13" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/5ff45dc3-74b8-4163-88b0-099156aa42bd">
