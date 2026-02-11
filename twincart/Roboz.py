import pyads as ps
import time

amse = "10.234.195.54.1.1"
porta = 851
ip = "10.234.197.74"

plc = ps.Connection(amse, porta, ip)
plc.open()


while True:
    a = int(input("Valor: "))
    plc.write_by_name("GVL.status", a, ps.PLCTYPE_INT)
    time.sleep(0.3)
    plc.write_by_name("GVL.status", 0, ps.PLCTYPE_INT)