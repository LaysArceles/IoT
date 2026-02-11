import pyads as ps
from ctypes import sizeof
amse = "10.234.195.54.1.1"
porta = 851
ip = "10.234.197.74"


plc = ps.Connection(amse, porta, ip)

plc.open()

lido = plc.read_by_name("GVL.tempo", ps.PLCTYPE_INT)
print(f"Leitura antes: {lido}")

num = int(input("Digite o valor para alterar: "))

plc.write_by_name("GVL.tempo", num , ps.PLCTYPE_INT)


lido = plc.read_by_name("GVL.tempo", ps.PLCTYPE_INT)
print(f"\nLeitura Depois: {lido}")