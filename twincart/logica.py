import pyads as ps
import time

amse = "10.234.195.54.1.1"
porta = 851
ip = "10.234.197.74"

pos = [0, 1, 2, 1, 3, 4, 3, 0]
n = 0 
plc = ps.Connection(amse, porta, ip)
plc.open()

while True:

    status = plc.read_by_name("GVL.status", ps.PLCTYPE_INT)
    
    while  status != 0 :
        status = plc.read_by_name("GVL.status", ps.PLCTYPE_INT)
        
        proxima_pos = pos[n]
        plc.write_by_name("GVL.posicao", proxima_pos, ps.PLCTYPE_INT)
        

        garra_val = 1 if proxima_pos in [1, 3] else 0
        plc.write_by_name("GVL.garra", garra_val, ps.PLCTYPE_INT)
        
        print(f"Enviado: Posição {proxima_pos} (Passo {n})")
        
        n += 1
        time.sleep(2.0) 

        if n == 8 :
            break

