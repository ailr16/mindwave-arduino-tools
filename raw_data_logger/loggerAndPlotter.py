# Tool for read RAW data from serial port and plot
# ailr16.github.io
# Dec 2023

import serial
import numpy

board = serial.Serial( port="/dev/ttyACM0", baudrate=115200, timeout=1 )

i = 0
lines = []
skip_lines = 4
processed_data1 = []
final_data = [
    [],
    [],
    []
]

while i < 32:
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
    final_data[0].append( int(actual_processed_line[0]) )
    final_data[9].append( sample )
    sample += 1
    for i in range(8):
        j = 0
        for value in actual_processed_line.pop().split(','):
            final_data[i + 1][j].append(int(value))
            j += 1