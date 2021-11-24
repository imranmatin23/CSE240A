NBITCOUNTER = 2
GHISTORYBITS = 13
PCINDEXBITS = 11
LHISTORYBITS = 11

def global_predictor_cost():
    return (NBITCOUNTER * (2**GHISTORYBITS)) + GHISTORYBITS

def local_predictor_cost():
    return ((2**PCINDEXBITS) * LHISTORYBITS) + ((2**LHISTORYBITS) * NBITCOUNTER)

def choice_predictor_cost():
    return (NBITCOUNTER * (2**GHISTORYBITS)) + GHISTORYBITS

def total_cost():
    return global_predictor_cost() + local_predictor_cost() + choice_predictor_cost() - GHISTORYBITS

if __name__ == "__main__":
    total_bit_cost = total_cost()
    print("Total Bit cost: " + str(total_bit_cost) + " bits, " + str(total_bit_cost / 1024) + " kilobits")