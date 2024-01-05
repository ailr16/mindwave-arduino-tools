# Tool for read RAW data from serial port and plot
# ailr16.github.io
# Dec 2023

import serial
import numpy
import tkinter as tk

board = serial.Serial( port="/dev/ttyACM0", baudrate=115200, timeout=1 )

samples_to_read = 2000

i = 0
lines = []
skip_lines = 4
processed_data1 = []
final_data = [
    [],
    [],
    [],
    [],
    []
]

while i < samples_to_read:
    actual_line = str(board.readline())
    if( i > skip_lines ):
        if len(actual_line) > 9 and len(actual_line) < 17:
            lines.append( actual_line )
            print( actual_line )
    i += 1

for line in lines:
    processed_data1.append( line[2:-3] )

sample = 0
for processed_line in processed_data1:
    processed_line:str
    actual_processed_line = processed_line.split(':')
    for i in range( 3 ):
        final_data[i].append( int(actual_processed_line[i]) )
    final_data[4].append( sample )
    sample += 1

for i in range( len(final_data[0]) ):
    if( final_data[1][i] & 0x80 == 128 ):
        final_data[3].append( (( final_data[1][i] ^ 0x80) * 256 + final_data[2][i]) * -1 )
    else:
        final_data[3].append( final_data[1][i] * 256  + final_data[2][i] )

def cli_print():
    print( 32*"-" + " RESULTS " + 32*"-" )
    print("TOTAL samples=",len(final_data[0]))
    print( "Sample\tQuality\tH_byte\tL_byte\tReal" )
    for i in range(len(final_data[0])):
        print( "{0}\t{1}\t{2}\t{3}\t{4}".format(
            final_data[4][i],
            final_data[0][i],
            final_data[1][i],
            final_data[2][i],
            final_data[3][i]
            ) )
        

def savelog_csv():
    numpy.savetxt(
        "/home/ailr16/Documents/raw.csv",
        numpy.c_[final_data[4],
                 final_data[0],
                 final_data[1],
                 final_data[2],
                 final_data[3]],
                 delimiter=",",
                 header="Sample,Quality,H_byte,L_byte,Real"
    )

cli_print()
savelog_csv()
