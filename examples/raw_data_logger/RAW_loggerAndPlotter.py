# Tool for read RAW data from serial port and plot
# ailr16.github.io
# Jan 2024

import matplotlib.pyplot as plt
import serial
import numpy
import csv

final_data = [
        [],
        [],
        []
    ]

def plot():
    fig = plt.figure(1)
    plt.plot(final_data[2], final_data[1])
    plt.xlabel('Sample')
    plt.ylabel('Raw value')
    plt.grid()
    plt.show()

def readHeadset( samples_to_read : int = 10000):
    board = serial.Serial( port="/dev/ttyACM0", baudrate=115200, timeout=1 )

    i = 0
    lines = []
    skip_lines = 4
    processed_data1 = []

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
        for i in range( 2 ):
            final_data[i].append( int(actual_processed_line[i]) )
        final_data[2].append( sample )
        sample += 1

    # Print in CLI
    print( 32*"-" + " RESULTS " + 32*"-" )
    print("TOTAL samples=",len(final_data[0]))
    print( "Sample\tQuality\tRAW" )
    for i in range(len(final_data[0])):
        print( "{0}\t{1}\t{2}".format(
            final_data[2][i],
            final_data[0][i],
            final_data[1][i],
            ) )
        
    # Save CSV
    numpy.savetxt(
        "/home/ailr16/Documents/raw.csv",
        numpy.c_[final_data[2],
                 final_data[0],
                 final_data[1]],
                 delimiter=",",
                 header="Sample,Quality,RAW"
    )

    print("Finished headset logging!!")
    plot()

def readCSV():
    with open("/home/ailr16/Documents/raw.csv") as csv_log:
        csv_read = csv.reader( csv_log, delimiter=',' )
        actual_line_count = 0
        for line in csv_read:
            if actual_line_count == 0:
                actual_line_count += 1
            else:
                final_data[2].append( float(line[0]) )
                final_data[1].append( float(line[2]) )
                actual_line_count += 1
    print("Finished csv reading!!")
    plot()

class MenuOptions():
    EXIT = 0
    READ_HEADSET = 1
    READ_CSV = 2
    DEFAULT = 99

selection = MenuOptions.DEFAULT

while( 1 ):
    print( "Enter the option number:" )
    print( "\t{0} to Exit".format( MenuOptions.EXIT ) )
    print( "\t{0} to log from headset".format( MenuOptions.READ_HEADSET ) )
    print( "\t{0} to read a CSV".format( MenuOptions.READ_CSV ) )

    selection = int( input("Your selection: ") )
    if( selection < MenuOptions.EXIT or selection > MenuOptions.READ_CSV ):
        print("INVALID OPTION!")

    if(selection == MenuOptions.EXIT):
        print("BYE!")
        exit()
    
    if(selection == MenuOptions.READ_HEADSET):
        samples_to_read = int(input("Samples to read: "))
        print("Logging from headset!")
        readHeadset(samples_to_read)

    if(selection == MenuOptions.READ_CSV):
        print("Reading csv!")
        readCSV()
    
    print("")