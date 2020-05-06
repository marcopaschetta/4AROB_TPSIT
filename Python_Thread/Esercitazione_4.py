# esempio ping pong tra due thread sincronizzati
import threading 
import time
import logging

MAXITER = 10 

def ping():
    i = 0
    while(i<MAXITER):
        i = i + 1
        s1.acquire()
        print("ping")
        time.sleep(1)
        s2.release()

def pong():
    i = 0
    while(i<MAXITER):
        i = i + 1
        s2.acquire()
        print("pong")
        time.sleep(1)
        s1.release()


if __name__ == "__main__":
    format = "%(asctime)s: %(message)s"
    logging.basicConfig(format = format, level = logging.INFO, datefmt="%H:%M:%S")

    t1 = threading.Thread(target = ping)
    t2 = threading.Thread(target = pong)

    s1 = threading.Lock()
    s2 = threading.Lock()
    s2.acquire()

    t1.start()
    t2.start()

    t1.join()
    t2.join()

    logging.info("PADRE: procedura terminata")