import threading
import logging 
import time 

def fn_thread(val):
    logging.info("Thread %s: inizio", val)
    time.sleep(2)
    logging.info("Thread %s: fine", val)

if __name__ == "__main__":
    format = "%(asctime)s: %(message)s"
    logging.basicConfig(format = format, level = logging.INFO, datefmt="%H:%M:%S")

    logging.info("PADRE: creo un thread")

    x = threading.Thread(target = fn_thread, args=(1, ))

    logging.info("PADRE: avvio thread creato")
    x.start()

    logging.info("PADRE: aspetto la terminazione del thread creato")
    x.join()

    logging.info("PADRE: fine")
