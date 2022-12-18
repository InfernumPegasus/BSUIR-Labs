import serial
from time import time
from threading import Thread
from time import sleep
from os import system
from random import randint


number_of_stations = 5
delay = 2
choice_delay = 10
station_number = [i for i in range(1, number_of_stations + 1)]


def send(sender, receiver):
    station_status[randint(0, number_of_stations - 1)] = 1
    while station_status[sender] != 1:
        show()
    print("Отправитель получает маркер и делает кадр")
    station_status[sender] = 2
    while station_status[receiver] != 2:
        show()
    print("Станция получила свой кадр")
    sleep(delay)


def stat(x):
    if x == 1:
        return "М"
    elif x == 2:
        return "К"
    else:
        return " "

def shift(number, pos):
    global station_status
    if pos == len(station_number) - 1:
        station_status[pos], station_status[0] = 0, number
    else:
        station_status[pos], station_status[pos + 1] = 0, number


def show():
    print("Номер станции ", *station_number)
    print("Статус        ", *(list(map(stat, station_status))))
    sleep(delay)
    if 1 in station_status:
        shift(1, station_status.index(1))
    else:
        shift(2, station_status.index(2))
    system("clear")

def work():
    begin = time()
    station_status[randint(0, number_of_stations - 1)] = 1
    while True:
        show()
        end = time()
        if (end - begin) > choice_delay:
            return


while True:
    system("clear")
    station_status = [0 for i in range(number_of_stations)]
    choice = input("Хотите слать что-то?(да или нет) ")
    if choice == "да":
        sender = int(input("Введите номер отправителя "))
        while sender not in range(1, number_of_stations + 1):
            sender = int(input("Введите номер отправителя (от 1 до " + str(number_of_stations + 1)))
        receiver = int(input("Введите номер получателя "))
        while receiver not in range(1, number_of_stations + 1):
            receiver = int(input("Введите номер получателя (от 1 до " + str(number_of_stations + 1)))
        system("clear")
        thread = Thread(target=send(sender=sender-1, receiver=receiver-1))
        thread.start()
    if choice == "q":
    	exit(0)
    else:
        system("clear")
        print("Тогда смотрите на движение маркера")
        thread = Thread(target=work())
        thread.start()
