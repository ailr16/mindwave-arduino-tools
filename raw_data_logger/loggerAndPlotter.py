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
    [ [], [], [] ],
    [ [], [], [] ],
    [ [], [], [] ],
    [ [], [], [] ],
    [ [], [], [] ],
    [ [], [], [] ],
    [ [], [], [] ],
    [ [], [], [] ],
    []
]

names = ["mid_gamma",
         "low_gamma",
         "high_beta",
         "low_beta",
         "high_alpha",
         "low_alpha",
         "theta",
         "delta",]

while i < 32:
    actual_line = str(board.readline())
    if( i > skip_lines ):
        if len(actual_line) > 54:
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

def cli_print():
    print( 32*"-" + " RESULTS " + 32*"-" )
    print("TOTAL samples=",len(final_data[0]))
    print( "Sample\tQuality\tM_gamma\tL_gamma\tH_beta\tL_beta\tH_alpha\tL_alpha\ttheta\tdelta" )
    for i in range(len(final_data[0])):
        print( "{0}\t{1}\t{2}\t{3}\t{4}\t{5}\t{6}\t{7}\t{8}".format(
            final_data[9][i],
            final_data[0][i],
            final_data[1][0][i],
            final_data[2][0][i],
            final_data[3][0][i],
            final_data[4][0][i],
            final_data[5][0][i],
            final_data[6][0][i],
            final_data[7][0][i],
            final_data[8][0][i]
            ) )
    
def savelog_csv():
    numpy.savetxt(
        "/home/ailr16/Documents/test.csv",
        numpy.c_[final_data[9],
                 final_data[0],
                 final_data[1][0],
                 final_data[2][0],
                 final_data[3][0],
                 final_data[4][0],
                 final_data[5][0],
                 final_data[6][0],
                 final_data[7][0],
                 final_data[8][0]],
                 delimiter=",",
                 header="Sample,Quality,M_gamma,L_gamma,H_beta,L_beta,H_alpha,L_alpha,theta,delta"
    )

cli_print()
savelog_csv()